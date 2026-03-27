"""
Cell With Membrane (CellWM) — Deformable agent model with Gaussian filopodia.

===== IMPORTANT NOTE ON NOMENCLATURE =====

This project is named "CPM" (Cellular Potts Model) following the original
research group's convention, but the implementation is NOT a classical CPM.

A classical CPM (Graner & Glazier, 1992) is a lattice-based model where:
- Each pixel on a grid belongs to a cell (identified by spin sigma)
- Cell shapes EMERGE from Hamiltonian energy minimization
- Monte Carlo dynamics: stochastic pixel copy-attempts
- Energy: H = Sum J_adhesion + lambda(A-A0)^2 + lambda(P-P0)^2

This implementation is a GEOMETRIC AGENT-BASED model where:
- Each cell has a prescribed shape (Gaussian filopodia envelope)
- Cell contours are computed analytically, not from energy minimization
- Dynamics are deterministic + stochastic (not Monte Carlo)
- The Hamiltonian is computed for MONITORING, not for driving dynamics

The two approaches are complementary. The classical CPM excels at emergent
multicellular behaviors (cell sorting, tissue morphogenesis). This geometric
model excels at capturing individual cell morphology and filopodial dynamics.

===== MATHEMATICAL MODEL =====

Cell boundary in polar coordinates:
    R(theta) = max_j { R0 + Aj * exp(-(theta - theta0j)^2 / (2 * Wj^2)) }

Filopodium dynamics (persistent random walk + durotaxis):
    theta_j(t+dt) = theta_j(t) + sigma_j*xi + beta*sin(theta_pref - theta_j)
                    + alpha*sin(theta_stiff - theta_j)

Cell velocity (filopodia-weighted propulsion):
    v = V0 * Sum_j Aj * (cos theta_j, sin theta_j)

Membrane relaxation (viscoelastic):
    R(theta, t+dt) = R(theta, t) + gamma * (R_ideal(theta) - R(theta, t))

Hamiltonian energy (monitoring):
    H = lambda_A * (A - A0)^2 + lambda_P * (P - P0)^2

===== REFERENCES =====

- Graner & Glazier (1992), Simulation of biological cell sorting using a
  two-dimensional extended Potts model. Phys. Rev. Lett. 69(13), 2013.
- Wortel & Textor (2021), Artistoo. eLife 10:e61288.
- Rens & Merks (2020), Cell Shape and Durotaxis. iScience 23(9):101420.
- Selmeczi et al. (2005), Cell motility as persistent random motion.
  Biophys. J. 89(2), 912-931.
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

    def update_angles(self, stiffness_gradient=None):
        """Apply persistent random walk with optional durotactic bias.

        ===== MATHEMATICAL MODEL =====

        Each filopodium angle evolves as:

            theta_j(t+dt) = theta_j(t) + sigma_j*xi_j + beta_persist*d_pref
                            + beta_duro*d_stiff

        where:
            sigma_j*xi_j       = stochastic step (xi ~ Uniform(-0.5, 0.5))
            beta_persist*d_pref = bias toward preferred direction (persistence)
            beta_duro*d_stiff   = bias toward stiffness gradient (durotaxis)

        ===== DUROTAXIS MECHANISM =====

        When a stiffness gradient grad_E is present, filopodia experience a
        torque that rotates them toward the gradient direction:

            d_stiff = 0.1 * sin(theta_gradient - theta_j)

        The sin() function creates a restoring torque: filopodia aligned
        with the gradient feel no torque, while perpendicular ones feel
        maximum torque. This models focal adhesion reinforcement on
        stiffer substrates (Rens & Merks, 2020).

        Args:
            stiffness_gradient: Optional [gx, gy] gradient vector.
                Direction of increasing substrate stiffness.
        """
        self.steps_since_change += 1

        if self.steps_since_change >= self.persistence_time:
            self.steps_since_change = 0
            self.persistence_time = np.random.randint(5, 20)
            self.preferred_direction = np.random.uniform(-np.pi, np.pi)

        # Persistence bias: gentle pull toward preferred direction
        bias = 0.1 * (self.preferred_direction - self.angles)
        bias = (bias + np.pi) % (2 * np.pi) - np.pi

        # Stochastic step
        steps = (np.pi / 4) * self.sigmas * (np.random.random(self.num_filo) - 0.5) + 0.05 * bias

        # Durotactic bias: rotate filopodia toward stiffness gradient
        if stiffness_gradient is not None:
            grad = np.asarray(stiffness_gradient)
            grad_magnitude = np.linalg.norm(grad)
            if grad_magnitude > 1e-10:
                # Direction of maximum stiffness
                grad_angle = np.arctan2(grad[1], grad[0])
                # Sinusoidal torque: filopodia turn toward gradient
                # sin(theta_grad - theta_j) is positive when theta_j < theta_grad
                durotactic_torque = 0.1 * grad_magnitude * np.sin(grad_angle - self.angles)
                steps += durotactic_torque

        self.angles += steps
        self.angles = (self.angles + np.pi) % (2 * np.pi) - np.pi

    def update_shape(self):
        """Relax membrane shape toward the ideal Gaussian envelope.

        ===== VISCOELASTIC MEMBRANE MODEL =====

        The cell membrane behaves as a viscoelastic material that relaxes
        toward the shape dictated by the filopodial protrusions:

            R(theta, t+dt) = R(theta, t) + gamma_eff * (R_ideal(theta) - R(theta, t))

        where:
            R(theta, t)    = current radial distance at angle theta
            R_ideal(theta) = envelope of Gaussian filopodia at current angles
            gamma_eff      = effective damping rate (mean of per-filopodium gamma_j)

        When gamma_eff = 1: membrane instantly matches ideal shape (elastic limit)
        When gamma_eff < 1: membrane lags behind (viscoelastic, creates memory)
        When gamma_eff > 1: overshoots (can create oscillations, usually avoided)

        The ideal shape is the envelope (pointwise maximum) of all Gaussian
        filopodia, re-evaluated at the current filopodium angles:

            R_ideal(theta) = max_j { R0 + Aj * exp(-(theta - theta_j)^2 / (2*Wj^2)) }

        A minimum radius of 0.5*R0 is enforced to prevent cell collapse.
        """
        thetas = np.linspace(-np.pi, np.pi, self.num_contour, endpoint=False)

        # Current radii from center
        dx = self.contour[:, 0] - self.position[0]
        dy = self.contour[:, 1] - self.position[1]
        current_r = np.sqrt(dx**2 + dy**2)

        # Ideal envelope at current filopodium positions (vectorized)
        d_angles = thetas[:, np.newaxis] - self.angles[np.newaxis, :]
        d_angles = (d_angles + np.pi) % (2 * np.pi) - np.pi
        ideal_per_filo = self.base_radius + self.amplitudes * np.exp(
            -d_angles**2 / (2 * self.widths**2 + 1e-10)
        )
        ideal_r = np.max(ideal_per_filo, axis=1)

        # Effective damping rate: mean of per-filopodium damping coefficients
        gamma_eff = np.clip(np.mean(self.gammas), 0.01, 1.5)

        # Viscoelastic relaxation toward ideal shape
        new_r = current_r + gamma_eff * (ideal_r - current_r)

        # Enforce minimum radius to prevent collapse
        new_r = np.maximum(new_r, self.base_radius * 0.5)

        # Update contour in Cartesian coordinates
        self.contour[:, 0] = self.position[0] + new_r * np.cos(thetas)
        self.contour[:, 1] = self.position[1] + new_r * np.sin(thetas)

    def estimate_velocity(self):
        """Estimate cell velocity from active filopodial forces.

        ===== PHYSICAL MODEL =====

        Cell motility is driven by actin polymerization at filopodial tips
        generating protrusive forces. Each filopodium contributes a traction
        force proportional to its amplitude (protrusion length), directed
        radially outward at its angular position:

            v = V0 * Sum_j Aj * (cos theta_j, sin theta_j)

        This is a self-propelled particle model where shape asymmetry drives
        motion. When filopodia are clustered on one side, the cell migrates
        in that direction. When filopodia are uniformly distributed, forces
        cancel and the cell remains stationary.

        Unlike the centroid-offset model (v proportional to centroid - center),
        this formulation correctly produces directed motion even when the
        contour is nearly symmetric around the center.

        The velocity magnitude scales with the total protrusive activity
        (sum of amplitudes), matching the biological observation that more
        active cells move faster.

        ===== COMPARISON WITH ORIGINAL C++ =====

        The original C++ code (cellWM.cpp::EstimateV) computed velocity as:
            vx = Sum_i ri*cos(theta_i),  vy = Sum_i ri*sin(theta_i)
        summing over ALL contour points. This is equivalent to the centroid
        offset and suffers from the same cancellation problem.

        Our filopodia-weighted model sums over filopodia (not contour points),
        using amplitudes as weights, which gives physically meaningful motility.
        """
        # Each filopodium exerts a protrusive force proportional to its amplitude,
        # directed radially outward at its angular position
        force_x = np.sum(self.amplitudes * np.cos(self.angles))
        force_y = np.sum(self.amplitudes * np.sin(self.angles))

        # Scale by velocity factor to get displacement per step
        self.velocity = self.velocity_scale * np.array([force_x, force_y])

    def update_position(self):
        """Euler integration of position.

        Advances the cell center position by adding the velocity vector.
        This is the simplest forward-Euler time integration scheme.
        """
        self.position += self.velocity

    def simulation_step(self, mechanotaxis_gradient=None):
        """Execute one complete simulation step.

        ===== SIMULATION PIPELINE =====

        The step sequence implements a biologically-motivated update cycle:

        Step 1 -- Filopodium reorientation (Brownian + bias):
            theta_j(t+1) = theta_j(t) + sigma_j*xi + beta*bias_toward_gradient

        Step 2 -- Membrane shape relaxation (viscoelastic):
            R(theta, t+1) = R(theta, t) - gamma*(R(theta, t) - R_ideal(theta))

        Step 3 -- Velocity from filopodial forces (self-propelled):
            v = V0 * Sum_j Aj * (cos theta_j, sin theta_j)

        Step 4 -- Position integration (Euler):
            x(t+1) = x(t) + v*dt

        Step 5 -- Energy computation (monitoring):
            H = lambda_A*(A-A0)^2 + lambda_P*(P-P0)^2

        Args:
            mechanotaxis_gradient: Optional [gx, gy] substrate stiffness
                gradient vector. When present, filopodia are biased toward
                the gradient direction (durotaxis).
        """
        self.update_angles(stiffness_gradient=mechanotaxis_gradient)
        self.update_shape()
        self.estimate_velocity()
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
