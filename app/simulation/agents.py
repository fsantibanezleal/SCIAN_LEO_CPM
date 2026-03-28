"""
Agent-based system managing populations of DFC, DEB, and EVL cells.

===== SYSTEM ARCHITECTURE =====

The AgentsSystem manages the full simulation pipeline per time step:

    +-----------------------------------------------------+
    |  For each active cell:                              |
    |    1. update_angles (random walk + durotaxis)       |
    |    2. update_shape  (viscoelastic relaxation)       |
    |    3. estimate_velocity (filopodial propulsion)     |
    |    4. update_position (Euler integration)           |
    |    5. compute_energy (Hamiltonian monitoring)       |
    +-----------------------------------------------------+
    +-----------------------------------------------------+
    |  Collective interactions:                           |
    |    6. Resolve collisions (two-pass: soft + hard)    |
    |    7. Apply adhesion (differential adhesion forces) |
    |    8. Enforce boundaries (walls, EVL, DEB)          |
    |    9. Check proliferation (Hertwig's rule division) |
    +-----------------------------------------------------+

===== COLLISION ALGORITHM =====

Two-pass collision resolution prevents oscillations:
  Forward pass (i < j):  Soft push (factor=1.0) -- allows partial overlap
  Backward pass (i > j): Hard push (factor=0.0) -- ensures full separation

This is equivalent to Gauss-Seidel relaxation with forward-backward sweep.

===== BIOLOGICAL CONTEXT =====

Models DFC (Dorsal Forerunner Cell) migration during zebrafish epiboly:
- ~20-30 DFCs form a cohesive cluster
- Cluster migrates collectively toward the vegetal pole
- EVL boundary drives migration via apical attachments
- DEB boundary constrains the cluster from below
- DFCs eventually form Kupffer's vesicle (left-right organizer)

References:
    - Oteiza et al. (2015), Cell collectivity regulation during KV formation
    - Ablooglu et al. (eLife 2021), DFC apical contacts and dragging mechanism
"""

import numpy as np
from .cell import CellWM


class AgentsSystem:
    """Multi-cell population manager for the CPM simulation.

    Manages a collection of CellWM instances, handling their initialization
    in a grid layout, collective update (stepping each cell), pairwise
    collision resolution, boundary enforcement, and optional cell
    proliferation.

    The collision resolution uses a two-pass algorithm:
    1. Forward pass: soft collision with partial overlap tolerance.
    2. Backward pass: hard separation ensuring no overlap remains.

    Attributes:
        config: Dictionary of simulation parameters.
        cells: List of CellWM instances.
        running: Whether the simulation is actively advancing.
        step_count: Number of simulation steps completed.
        rate_ant_attachment: Anterior adhesion rates per EVL stage (experimental).
        rate_post_attachment: Posterior adhesion rates per EVL stage (experimental).
        attachment_stage: Current adhesion stage index.
    """

    def __init__(self, config):
        """Initialize the agent system.

        Args:
            config: Dictionary with keys such as 'num_filopodia',
                'num_contour_points', 'velocity_scale', 'proliferation_enabled'.
        """
        self.config = config
        self.cells = []
        self.running = False
        self.step_count = 0

        # Adhesion rate tables (from experimental data)
        self.rate_ant_attachment = [
            7 / 21, 8 / 21, 10 / 22, 10 / 22, 7 / 24, 6 / 24, 1 / 32, 1 / 32, 0,
        ]
        self.rate_post_attachment = [
            10 / 21, 9 / 21, 10 / 22, 10 / 22, 7 / 24, 6 / 24, 1 / 32, 1 / 32, 0,
        ]
        self.attachment_stage = 0

    def initialize(self, env, num_dfcs=24, radius=10.0):
        """Initialize DFC cells in a grid pattern within the DFC region.

        Places cells on a regular grid with spacing proportional to the cell
        radius. The grid is fitted within the environment's DFC region bounds.

        Args:
            env: EnvironmentSystem instance defining the spatial domain.
            num_dfcs: Number of DFC cells to create.
            radius: Base radius for each cell.
        """
        self.cells = []

        # Available space within the DFC region (with radius margin)
        region_w = (env.dfc_region[2] - env.dfc_region[0]) - 2 * radius
        region_h = (env.dfc_region[3] - env.dfc_region[1]) - 2 * radius

        # Determine grid dimensions that fit all cells
        # Start with an aspect-ratio-aware estimate and widen until all fit
        cols = max(1, int(np.ceil(np.sqrt(num_dfcs * region_w / max(region_h, 1)))))
        rows = int(np.ceil(num_dfcs / cols))

        # Compute spacing to distribute cells evenly within the region
        spacing_x = region_w / max(cols - 1, 1) if cols > 1 else 0.0
        spacing_y = region_h / max(rows - 1, 1) if rows > 1 else 0.0

        # Ensure minimum spacing of 2 * radius to avoid immediate overlap
        min_spacing = 2.0 * radius
        if spacing_x < min_spacing and cols > 1:
            cols = max(1, int(region_w / min_spacing) + 1)
            rows = int(np.ceil(num_dfcs / cols))
            spacing_x = region_w / max(cols - 1, 1) if cols > 1 else 0.0
            spacing_y = region_h / max(rows - 1, 1) if rows > 1 else 0.0

        start_x = env.dfc_region[0] + radius
        start_y = env.dfc_region[1] + radius

        count = 0
        for row in range(rows):
            for col in range(cols):
                if count >= num_dfcs:
                    break
                x = start_x + col * spacing_x
                y = start_y + row * spacing_y

                cell = CellWM(
                    position=[x, y],
                    radius=radius,
                    num_filo=self.config.get("num_filopodia", 4),
                    num_contour=self.config.get("num_contour_points", 100),
                )
                cell.velocity_scale = self.config.get("velocity_scale", 0.01)
                self.cells.append(cell)
                count += 1

        self.running = True

    def update(self, env):
        """Advance simulation by one step.

        Performs the following operations in order:
        1. Step each active cell (angle update, shape update, velocity, position).
        2. Resolve pairwise collisions (two-pass: forward soft, backward hard).
        3. Enforce environment boundaries (walls, EVL, DEB).
        4. Optionally check and apply cell proliferation.

        Args:
            env: EnvironmentSystem instance for boundary constraints.
        """
        if not self.running:
            return

        # Step 1: Update each cell (with optional stiffness gradient for durotaxis)
        for cell in self.cells:
            if cell.active:
                gradient = env.get_stiffness_gradient(cell.position) if hasattr(env, 'get_stiffness_gradient') else None
                cell.simulation_step(mechanotaxis_gradient=gradient)

        # Step 2: Resolve collisions (two-pass)
        self._resolve_collisions()

        # Step 3: Apply cell-cell adhesion
        self._apply_adhesion()

        # Step 4: Apply boundary constraints
        self._apply_boundaries(env)

        # Step 5: Check proliferation
        if self.config.get("proliferation_enabled", False):
            self._check_proliferation(env)

        self.step_count += 1

    def _resolve_collisions(self):
        """Two-pass collision resolution between all cell pairs.

        Forward pass uses soft collision (factor=1.0) allowing partial overlap.
        Backward pass uses hard separation (factor=0.0) pushing cells fully apart.
        This two-pass approach prevents oscillations while ensuring separation.
        """
        active = [c for c in self.cells if c.active]
        n = len(active)

        # Forward pass (soft collision)
        for i in range(n):
            for j in range(i + 1, n):
                self._fix_overlap(active[i], active[j], factor=1.0)

        # Backward pass (hard separation)
        for i in range(n - 1, -1, -1):
            for j in range(i):
                self._fix_overlap(active[i], active[j], factor=0.0)

        # Apply Contact Inhibition of Locomotion (CIL) for colliding pairs
        for i in range(n):
            for j in range(i + 1, n):
                dist = np.linalg.norm(active[i].position - active[j].position)
                if dist < 3 * active[i].base_radius:
                    active[i].apply_contact_inhibition(active[j].position)
                    active[j].apply_contact_inhibition(active[i].position)

    def _fix_overlap(self, cell_a, cell_b, factor=0.0):
        """Resolve overlap between two cells using centroid-based repulsion.

        Applies a repulsive force proportional to the overlap depth.
        The factor parameter controls collision softness:
            factor=0.0 -> full separation (hard collision)
            factor=1.0 -> half-separation (soft, allows partial overlap)

        The push direction is along the line connecting cell centers,
        which approximates the minimum translation vector for convex shapes.
        Uses the actual contour extent (maximum radial reach) instead of
        the nominal base radius for more accurate overlap detection.

        Args:
            cell_a: First cell.
            cell_b: Second cell.
            factor: Softness factor in [0, 1].
        """
        dist = np.linalg.norm(cell_a.position - cell_b.position)
        # Use effective radius from actual contour extent (not just base_radius)
        r_a = np.max(np.sqrt(
            (cell_a.contour[:, 0] - cell_a.position[0])**2 +
            (cell_a.contour[:, 1] - cell_a.position[1])**2
        ))
        r_b = np.max(np.sqrt(
            (cell_b.contour[:, 0] - cell_b.position[0])**2 +
            (cell_b.contour[:, 1] - cell_b.position[1])**2
        ))
        min_dist = r_a + r_b

        if dist >= min_dist or dist < 1e-10:
            return

        direction = (cell_a.position - cell_b.position) / dist
        overlap = min_dist - dist
        push = overlap * (1.0 - factor * 0.5)

        cell_a.position += direction * push * 0.5
        cell_b.position -= direction * push * 0.5

        # Rebuild contours after position change
        cell_a._create_contour()
        cell_b._create_contour()

    def _apply_boundaries(self, env):
        """Constrain cells within environment boundaries.

        Enforces:
        - Hard wall boundaries at the environment edges.
        - EVL upper boundary (cells cannot go above EVL position).
        - DEB lower boundary (cells cannot go below DEB position).

        Args:
            env: EnvironmentSystem instance with boundary positions.
        """
        for cell in self.cells:
            if not cell.active:
                continue
            r = cell.base_radius
            cell.position[0] = np.clip(cell.position[0], r, env.width - r)
            cell.position[1] = np.clip(cell.position[1], r, env.height - r)

            # EVL boundary (upper)
            if env.evl_enabled and cell.position[1] < env.evl_position:
                cell.position[1] = env.evl_position + r

            # DEB boundary (lower)
            if env.deb_enabled and cell.position[1] > env.deb_position:
                cell.position[1] = env.deb_position - r

    def _apply_adhesion(self, strength=0.002, range_factor=3.0):
        """Vectorized differential adhesion using NumPy pairwise distances.

        ===== DIFFERENTIAL ADHESION MODEL =====

        Cells within range experience attractive forces mediated by
        cadherin-based adhesion:

            F_adh = s * (d - d_contact) / (d_max - d_contact) * d_hat

        for d_contact < d < d_max, where:
            s          = adhesion strength parameter
            d          = center-to-center distance
            d_contact  = sum of effective radii (touching distance)
            d_max      = range_factor * d_contact
            d_hat      = unit vector from cell i toward cell j

        This creates cohesive cell clusters, as observed in DFC collectives
        during zebrafish epiboly (Oteiza et al., 2015).
        """
        active = [c for c in self.cells if c.active]
        n = len(active)
        if n < 2:
            return

        # Build position matrix (n, 2)
        positions = np.array([c.position for c in active])
        radii = np.array([c.base_radius for c in active])

        # Pairwise distances (n, n) -- vectorized
        diff = positions[:, None, :] - positions[None, :, :]  # (n, n, 2)
        dists = np.linalg.norm(diff, axis=2)  # (n, n)

        # Contact and max range matrices
        r_contact = radii[:, None] + radii[None, :]  # (n, n)
        r_max = r_contact * range_factor

        # Adhesion mask: between contact and max range, upper triangle only
        mask = (dists > r_contact) & (dists < r_max) & (np.triu(np.ones((n, n), dtype=bool), k=1))

        if not np.any(mask):
            return

        # Force magnitude (n, n) -- only where mask is True
        force_mag = np.zeros((n, n))
        force_mag[mask] = strength * (dists[mask] - r_contact[mask]) / (r_max[mask] - r_contact[mask])

        # Direction unit vectors (n, n, 2)
        direction = np.zeros_like(diff)
        nonzero = dists > 1e-10
        direction[nonzero] = diff[nonzero] / dists[nonzero, None]

        # Force vectors (n, n, 2)
        forces = force_mag[:, :, None] * direction  # (n, n, 2)

        # Net force per cell: sum over interaction partners
        # Cell i is pulled toward j: +forces[i,j]
        # Cell j is pulled toward i: -forces[i,j] (Newton's 3rd law via upper triangle)
        net_force = forces.sum(axis=1) - forces.sum(axis=0)  # (n, 2)

        # Apply
        for i, cell in enumerate(active):
            cell.position += net_force[i]

    def _check_proliferation(self, env):
        """Cell division along the longest axis of the mother cell.

        ===== BIOLOGICAL MODEL =====

        Cell division in epithelial tissues follows Hertwig's rule: the
        mitotic spindle aligns with the cell's longest axis, producing
        two daughter cells side by side.

        The division process:
        1. Select a cell probabilistically (rate depends on environment stage)
        2. Compute the cell's principal axis (direction of maximum extent)
        3. Place two daughters at +/-offset along that axis
        4. Each daughter inherits half the mother's area (reduced radius)

        This replaces the original random-spawn model, which placed new
        cells at arbitrary positions unrelated to any parent cell.
        """
        if env.proliferation_stage >= len(env.proliferation_rates):
            return

        rate = env.proliferation_rates[env.proliferation_stage]
        if rate <= 0 or len(self.cells) >= 1000:
            return

        # Probabilistic division: each cell has a chance to divide
        for i in range(len(self.cells) - 1, -1, -1):  # iterate backwards for safe append
            cell = self.cells[i]
            if not cell.active:
                continue
            if np.random.random() > rate * 0.01:  # Small per-step probability
                continue

            # Find the cell's longest axis from its contour
            dx = cell.contour[:, 0] - cell.position[0]
            dy = cell.contour[:, 1] - cell.position[1]
            # Use the angle of the filopodium with maximum amplitude
            # as the division axis (approximation of longest axis)
            max_filo_idx = np.argmax(cell.amplitudes)
            div_angle = cell.angles[max_filo_idx]

            # Offset for daughter cells: half the base radius
            offset = cell.base_radius * 0.6
            daughter_radius = cell.base_radius * 0.85  # Slightly smaller

            pos1 = cell.position + offset * np.array([np.cos(div_angle), np.sin(div_angle)])
            pos2 = cell.position - offset * np.array([np.cos(div_angle), np.sin(div_angle)])

            # Create daughters
            d1 = CellWM(pos1, radius=daughter_radius,
                         num_filo=cell.num_filo, num_contour=cell.num_contour)
            d1.is_offspring = True
            d1.velocity_scale = cell.velocity_scale

            d2 = CellWM(pos2, radius=daughter_radius,
                         num_filo=cell.num_filo, num_contour=cell.num_contour)
            d2.is_offspring = True
            d2.velocity_scale = cell.velocity_scale

            # Deactivate mother, add daughters
            cell.active = False
            self.cells.append(d1)
            self.cells.append(d2)

            if len(self.cells) >= 1000:
                break

    def get_state(self):
        """Return full system state for WebSocket transmission.

        Returns:
            Dictionary with step count, number of active cells, per-cell
            states, and running flag. Suitable for JSON serialization.
        """
        return {
            "step": self.step_count,
            "num_cells": len([c for c in self.cells if c.active]),
            "cells": [c.get_state() for c in self.cells if c.active],
            "running": self.running,
        }
