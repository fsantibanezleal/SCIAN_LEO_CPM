"""
Cellular Potts Model - Cell with Membrane (CellWM).

Implements a deformable cell model where the cell boundary is defined by
Gaussian-shaped filopodia protruding from a base radius. The cell contour
is computed as the envelope of multiple Gaussian peaks in polar coordinates.

Mathematical model:
    R(theta) = max_j(R0 + A_j * exp(-(theta - theta0_j)^2 / (2 * W_j^2)))

Where:
    R0: base radius
    A_j: amplitude of filopodium j (protrusion height)
    theta0_j: angular position of filopodium j
    W_j: Gaussian width of filopodium j

References:
    - Graner & Glazier (1992), Simulation of biological cell sorting using a
      two-dimensional extended Potts model
    - Wortel & Textor (2021), Artistoo, eLife 10:e61288
"""

import numpy as np
from dataclasses import dataclass, field
from typing import Optional


@dataclass
class CellParams:
    """Parameters for a single cell.

    Attributes:
        num_filopodia: Number of Gaussian filopodial protrusions.
        base_radius: Base radius of the cell (R0) in spatial units.
        num_contour_points: Number of discrete points defining the membrane.
        velocity_scale: Scaling factor for velocity estimation from shape asymmetry.
        stochastic_motion: Whether filopodium angles undergo Brownian random walk.
        amplitudes: Per-filopodium peak amplitudes (A_j).
        angles: Per-filopodium angular positions (theta0_j).
        widths: Per-filopodium Gaussian widths (W_j).
        gammas: Per-filopodium elastic damping coefficients.
        sigmas: Per-filopodium stochastic step sizes for angle random walk.
    """

    num_filopodia: int = 4
    base_radius: float = 10.0
    num_contour_points: int = 100
    velocity_scale: float = 0.01
    stochastic_motion: bool = True

    # Per-filopodium parameters (arrays)
    amplitudes: Optional[np.ndarray] = None
    angles: Optional[np.ndarray] = None
    widths: Optional[np.ndarray] = None
    gammas: Optional[np.ndarray] = None
    sigmas: Optional[np.ndarray] = None


class CellWM:
    """Cell With Membrane - deformable cell with Gaussian filopodia.

    This class represents a single cell whose boundary (membrane) is described
    as the radial envelope of multiple Gaussian peaks centered at different
    angular positions. Each Gaussian peak models a filopodium that extends
    the cell boundary outward. The cell moves by estimating a velocity vector
    from its shape asymmetry — protruding regions create net force toward
    the protrusion direction.

    Attributes:
        position: 2D position of the cell center [x, y].
        velocity: 2D velocity vector [vx, vy].
        active: Whether the cell is alive and participating in simulation.
        is_offspring: Whether this cell was created by proliferation.
        base_radius: Base radius R0 of the cell.
        num_filo: Number of filopodia.
        num_contour: Number of discrete contour points.
        velocity_scale: Scaling factor for velocity estimation.
        angles: Angular positions of filopodia (radians, in [-pi, pi]).
        amplitudes: Peak amplitudes of filopodia.
        widths: Gaussian widths of filopodia.
        gammas: Elastic damping coefficients for membrane relaxation.
        sigmas: Step size parameters for angle random walk.
        contour: Nx2 array of membrane contour coordinates.
    """

    def __init__(self, position, radius=10.0, num_filo=4, num_contour=100):
        """Initialize a CellWM instance.

        Args:
            position: Initial [x, y] position of the cell center.
            radius: Base radius R0 of the cell.
            num_filo: Number of Gaussian filopodia.
            num_contour: Number of points discretizing the membrane contour.
        """
        self.position = np.array(position, dtype=np.float64)
        self.velocity = np.zeros(2, dtype=np.float64)
        self.active = True
        self.is_offspring = False

        # Parameters
        self.base_radius = radius
        self.num_filo = num_filo
        self.num_contour = num_contour
        self.velocity_scale = 0.01

        # Per-filopodium arrays (randomized initialization)
        self.angles = np.random.uniform(-np.pi, np.pi, num_filo)
        self.amplitudes = 0.3 * radius * np.random.random(num_filo)
        self.widths = 0.8 * np.random.random(num_filo)
        self.gammas = 1.5 * np.random.random(num_filo)
        self.sigmas = 1.8 * np.random.random(num_filo)

        # Persistent random walk parameters
        self.persistence_time = np.random.randint(5, 20)
        self.steps_since_change = 0
        self.preferred_direction = np.random.uniform(-np.pi, np.pi)

        # Energy tracking (populated by compute_energy)
        self._current_energy = 0.0
        self._current_area = 0.0
        self._current_perimeter = 0.0

        # Membrane contour
        self.contour = np.zeros((num_contour, 2), dtype=np.float64)
        self._create_contour()

    def fun_p(self, theta, filo_index):
        """Evaluate single filopodium radial function at a given angle.

        R(theta, j) = R0 + A_j * exp(-(theta - theta0_j)^2 / (2 * W_j^2))

        This is a convenience method for single-point queries.
        The vectorized contour generation uses batch computation instead.

        Args:
            theta: Query angle in radians [-pi, pi].
            filo_index: Index of the filopodium (0 to num_filo-1).

        Returns:
            Radial distance at the given angle for the specified filopodium.
        """
        d_angle = theta - self.angles[filo_index]
        d_angle = (d_angle + np.pi) % (2 * np.pi) - np.pi
        return self.base_radius + self.amplitudes[filo_index] * np.exp(
            -d_angle**2 / (2 * self.widths[filo_index]**2 + 1e-10)
        )

    def _create_contour(self):
        """Generate cell membrane contour as envelope of all filopodia.

        For each discretized angle theta_i, computes:
            R(theta_i) = max_j{ R0 + A_j * exp(-(d_theta_ij)^2 / (2*W_j^2)) }
        where d_theta_ij is the wrapped angular distance between theta_i and theta0_j.

        The contour is the polar envelope of all Gaussian peaks,
        converted to Cartesian coordinates centered at the cell position.
        """
        thetas = np.linspace(-np.pi, np.pi, self.num_contour, endpoint=False)

        # Compute all filopodia radii at all angles simultaneously
        # Shape: (num_contour, num_filo) via broadcasting
        d_angles = thetas[:, np.newaxis] - self.angles[np.newaxis, :]  # (N, F)
        # Cyclic wrapping to [-pi, pi]
        d_angles = (d_angles + np.pi) % (2 * np.pi) - np.pi

        # Gaussian radial function for each filopodium at each angle
        radii = self.base_radius + self.amplitudes * np.exp(
            -d_angles**2 / (2 * self.widths**2 + 1e-10)
        )  # (N, F)

        # Envelope: take maximum radius across filopodia at each angle
        max_radii = np.max(radii, axis=1)  # (N,)

        # Convert polar to Cartesian
        self.contour[:, 0] = self.position[0] + max_radii * np.cos(thetas)
        self.contour[:, 1] = self.position[1] + max_radii * np.sin(thetas)

    def update_angles(self):
        """Apply persistent random walk to filopodium angles.

        Filopodia exhibit directional persistence: they maintain their
        orientation for several time steps before reorienting. This
        models the biological observation that cell protrusions have
        a characteristic lifetime before retraction and re-extension.

        The persistence is controlled by a timer. When the timer expires,
        a new preferred direction is chosen randomly, and the timer resets.
        """
        self.steps_since_change += 1

        if self.steps_since_change >= self.persistence_time:
            # Major reorientation event
            self.steps_since_change = 0
            self.persistence_time = np.random.randint(5, 20)
            self.preferred_direction = np.random.uniform(-np.pi, np.pi)

        # Small stochastic steps biased toward preferred direction
        bias = 0.1 * (self.preferred_direction - self.angles)
        bias = (bias + np.pi) % (2 * np.pi) - np.pi  # wrap

        steps = (np.pi / 4) * self.sigmas * (np.random.random(self.num_filo) - 0.5) + 0.05 * bias
        self.angles += steps
        self.angles = (self.angles + np.pi) % (2 * np.pi) - np.pi

    def update_shape(self):
        """Update membrane shape with viscoelastic damping toward ideal form.

        Each membrane point relaxes toward its ideal radius with a rate
        controlled by the damping coefficient gamma:
            r_new = r_current - gamma * (r_current - r_ideal)

        When gamma=1, the membrane instantly adopts the ideal shape.
        When gamma<1, the membrane gradually relaxes (viscoelastic).
        When gamma>1, the response overshoots (overdamped oscillation).
        """
        thetas = np.linspace(-np.pi, np.pi, self.num_contour, endpoint=False)

        # Current radii from center
        dx = self.contour[:, 0] - self.position[0]
        dy = self.contour[:, 1] - self.position[1]
        current_r = np.sqrt(dx**2 + dy**2)

        # Ideal radii for each filopodium (vectorized)
        d_angles = thetas[:, np.newaxis] - self.angles[np.newaxis, :]
        d_angles = (d_angles + np.pi) % (2 * np.pi) - np.pi
        ideal_radii = self.base_radius + self.amplitudes * np.exp(
            -d_angles**2 / (2 * self.widths**2 + 1e-10)
        )  # (N, F)

        # Apply damping per filopodium, then take maximum
        damped = current_r[:, np.newaxis] - self.gammas * (
            current_r[:, np.newaxis] - ideal_radii
        )  # (N, F)
        best_r = np.maximum(np.max(damped, axis=1), self.base_radius * 0.5)

        self.contour[:, 0] = self.position[0] + best_r * np.cos(thetas)
        self.contour[:, 1] = self.position[1] + best_r * np.sin(thetas)

    def estimate_velocity(self):
        """Estimate cell velocity from membrane shape asymmetry.

        The velocity is computed as the normalized displacement between
        the cell center and the centroid of the membrane contour:
            v = V0 * (centroid - center)

        This captures the idea that asymmetric filopodial protrusions
        generate a net force that propels the cell in that direction,
        consistent with the self-propelled particle model.
        """
        centroid = np.mean(self.contour, axis=0)
        displacement = centroid - self.position
        self.velocity = self.velocity_scale * displacement

    def update_position(self):
        """Euler integration of position.

        Advances the cell center position by adding the velocity vector.
        This is the simplest forward-Euler time integration scheme.
        """
        self.position += self.velocity

    def simulation_step(self, mechanotaxis_force=None):
        """Execute one complete simulation step with optional mechanotaxis.

        The step sequence:
            1. Brownian angle update (filopodium reorientation)
            2. Elastic shape relaxation
            3. Velocity from shape asymmetry
            4. Add mechanotactic bias if present
            5. Euler position integration
            6. Compute energy for monitoring

        Args:
            mechanotaxis_force: Optional [fx, fy] external force vector.
        """
        self.update_angles()
        self.update_shape()
        self.estimate_velocity()

        # Apply mechanotaxis bias
        if mechanotaxis_force is not None:
            self.velocity += mechanotaxis_force

        self.update_position()
        self.compute_energy()

    def compute_energy(self, target_area=None, target_perimeter=None,
                       lambda_area=0.1, lambda_perimeter=0.05):
        """Compute the cell's Hamiltonian energy.

        The energy functional penalizes deviations from target shape:
            H = lambda_A * (A - A_target)^2 + lambda_P * (P - P_target)^2

        where A is the current area, P the perimeter, and lambda values
        control the strength of each constraint.

        This energy drives the cell toward a preferred morphology
        while allowing deformations caused by filopodial activity.

        Args:
            target_area: Preferred area (defaults to pi*R0^2).
            target_perimeter: Preferred perimeter (defaults to 2*pi*R0).
            lambda_area: Area constraint strength.
            lambda_perimeter: Perimeter constraint strength.

        Returns:
            Total Hamiltonian energy value.
        """
        if target_area is None:
            target_area = np.pi * self.base_radius**2
        if target_perimeter is None:
            target_perimeter = 2 * np.pi * self.base_radius

        # Shoelace formula for polygon area
        x = self.contour[:, 0]
        y = self.contour[:, 1]
        area = 0.5 * np.abs(np.sum(x * np.roll(y, -1) - np.roll(x, -1) * y))

        # Perimeter as sum of segment lengths
        diffs = np.diff(self.contour, axis=0, append=self.contour[:1])
        perimeter = np.sum(np.sqrt(diffs[:, 0]**2 + diffs[:, 1]**2))

        energy = lambda_area * (area - target_area)**2 + \
                 lambda_perimeter * (perimeter - target_perimeter)**2

        self._current_area = area
        self._current_perimeter = perimeter
        self._current_energy = energy
        return energy

    def get_state(self):
        """Return serializable state for WebSocket transmission.

        Returns:
            Dictionary containing position, contour, velocity, active status,
            and base radius. All numpy arrays are converted to Python lists
            for JSON serialization.
        """
        return {
            "position": self.position.tolist(),
            "contour": self.contour.tolist(),
            "velocity": self.velocity.tolist(),
            "active": self.active,
            "radius": self.base_radius,
            "energy": getattr(self, '_current_energy', 0),
            "area": getattr(self, '_current_area', 0),
            "perimeter": getattr(self, '_current_perimeter', 0),
        }
