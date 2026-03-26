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

        # Membrane contour
        self.contour = np.zeros((num_contour, 2), dtype=np.float64)
        self._create_contour()

    def fun_p(self, theta, filo_index):
        """Gaussian filopodium radial function.

        Computes the radial distance at angle theta for filopodium filo_index.
        Uses cyclic angle wrapping for proper behavior near +/- pi.

        The function evaluates:
            R(theta) = R0 + A_j * exp(-(theta - theta0_j)^2 / (2 * W_j^2))

        Args:
            theta: Angle in radians [-pi, pi].
            filo_index: Index of the filopodium (0-based).

        Returns:
            Radial distance R(theta) for this filopodium.
        """
        d_angle = theta - self.angles[filo_index]
        # Normalize to [-pi, pi]
        if d_angle > np.pi:
            d_angle -= 2 * np.pi
        if d_angle < -np.pi:
            d_angle += 2 * np.pi

        return self.base_radius + self.amplitudes[filo_index] * np.exp(
            -d_angle**2 / (2 * self.widths[filo_index] ** 2 + 1e-10)
        )

    def _create_contour(self):
        """Generate cell membrane contour as envelope of all filopodia.

        For each discrete angle theta_i, the contour radius is the maximum
        of R(theta_i) across all filopodia. This produces the envelope shape
        where the cell boundary bulges outward at filopodium locations.
        """
        thetas = np.linspace(-np.pi, np.pi, self.num_contour, endpoint=False)

        for i, theta in enumerate(thetas):
            # Take maximum radius across all filopodia
            max_r = max(self.fun_p(theta, j) for j in range(self.num_filo))
            self.contour[i, 0] = self.position[0] + max_r * np.cos(theta)
            self.contour[i, 1] = self.position[1] + max_r * np.sin(theta)

    def update_angles(self):
        """Random walk update of filopodium angles (Brownian motion).

        Each filopodium angle performs a bounded random step proportional
        to its sigma parameter. The step size is scaled by pi/4 and the
        result is wrapped to [-pi, pi].
        """
        for i in range(self.num_filo):
            step = (np.pi / 4) * self.sigmas[i] * (np.random.random() - 0.5)
            self.angles[i] += step
            # Wrap to [-pi, pi]
            self.angles[i] = ((self.angles[i] + np.pi) % (2 * np.pi)) - np.pi

    def update_shape(self):
        """Update membrane shape with elastic damping.

        For each contour point, the current radial distance is relaxed toward
        the ideal distance defined by the nearest filopodium. The damping
        coefficient gamma controls how fast the membrane reaches equilibrium.
        The final radius is the maximum candidate across all filopodia,
        bounded below by the base radius.
        """
        thetas = np.linspace(-np.pi, np.pi, self.num_contour, endpoint=False)

        for i, theta in enumerate(thetas):
            current_dx = self.contour[i, 0] - self.position[0]
            current_dy = self.contour[i, 1] - self.position[1]
            current_r = np.sqrt(current_dx**2 + current_dy**2)

            best_r = self.base_radius
            for j in range(self.num_filo):
                ideal_r = self.fun_p(theta, j)
                candidate = current_r - self.gammas[j] * (current_r - ideal_r)
                if candidate > best_r:
                    best_r = candidate

            self.contour[i, 0] = self.position[0] + best_r * np.cos(theta)
            self.contour[i, 1] = self.position[1] + best_r * np.sin(theta)

    def estimate_velocity(self):
        """Estimate cell velocity from membrane shape asymmetry.

        The velocity is computed as the weighted sum of radial vectors from
        the center to each contour point. When the cell shape is symmetric,
        these vectors cancel out and the velocity is near zero. Asymmetric
        protrusions (filopodia) create a net velocity in the protrusion
        direction, modeling filopodia-driven motility.
        """
        thetas = np.linspace(-np.pi, np.pi, self.num_contour, endpoint=False)
        vx, vy = 0.0, 0.0

        for i, theta in enumerate(thetas):
            dx = self.contour[i, 0] - self.position[0]
            dy = self.contour[i, 1] - self.position[1]
            r = np.sqrt(dx**2 + dy**2)
            vx += r * np.cos(theta)
            vy += r * np.sin(theta)

        self.velocity[0] = self.velocity_scale * vx
        self.velocity[1] = self.velocity_scale * vy

    def update_position(self):
        """Euler integration of position.

        Advances the cell center position by adding the velocity vector.
        This is the simplest forward-Euler time integration scheme.
        """
        self.position += self.velocity

    def simulation_step(self):
        """Execute one complete simulation step.

        The step consists of four sequential operations:
        1. Update filopodium angles (random walk / Brownian motion)
        2. Update membrane shape (elastic relaxation toward filopodia)
        3. Estimate velocity from shape asymmetry
        4. Advance position via Euler integration
        """
        self.update_angles()
        self.update_shape()
        self.estimate_velocity()
        self.update_position()

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
        }
