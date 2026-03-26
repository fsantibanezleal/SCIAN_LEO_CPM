"""
Agent-based system managing populations of DFC, DEB, and EVL cells.

Models collective cell behavior during zebrafish embryogenesis including:
- DFC (Dorsal Forerunner Cells): migratory cells with filopodia
- DEB (Dorsal Epiblast Boundary): lower tissue margin
- EVL (Enveloping Layer): upper tissue boundary

The system handles cell initialization in grid layouts, collision detection
and resolution, cell proliferation, and tissue boundary dynamics.

References:
    - Oteiza et al. (2015), Cell collectivity regulation within migrating
      cell cluster during Kupffer's vesicle formation
    - Ablooglu et al. (eLife 2021), Apical contacts stemming from incomplete
      delamination guide progenitor cell allocation
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

        # Step 1: Update each cell
        for cell in self.cells:
            if cell.active:
                cell.simulation_step()

        # Step 2: Resolve collisions (two-pass)
        self._resolve_collisions()

        # Step 3: Apply boundary constraints
        self._apply_boundaries(env)

        # Step 4: Check proliferation
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

    def _fix_overlap(self, cell_a, cell_b, factor=0.0):
        """Resolve overlap between two cells using radial push-apart.

        When two cells overlap (center distance < sum of radii), they are
        pushed apart along the axis connecting their centers. The push
        magnitude depends on the overlap distance and the factor parameter.

        Args:
            cell_a: First CellWM instance.
            cell_b: Second CellWM instance.
            factor: Softness factor (0.0 = hard separation, 1.0 = soft).
        """
        dist = np.linalg.norm(cell_a.position - cell_b.position)
        min_dist = cell_a.base_radius + cell_b.base_radius

        if dist >= min_dist or dist < 1e-10:
            return

        # Push apart along connecting axis
        direction = (cell_a.position - cell_b.position) / dist
        overlap = min_dist - dist
        push = overlap * (1.0 - factor * 0.5)

        cell_a.position += direction * push * 0.5
        cell_b.position -= direction * push * 0.5

        # Rebuild contours
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

    def _check_proliferation(self, env):
        """Add new cells according to proliferation schedule.

        Creates new cells at random valid positions within the DFC region.
        A position is valid if it does not overlap with any existing cell.
        The proliferation rate depends on the current environment stage.

        Args:
            env: EnvironmentSystem instance with proliferation parameters.
        """
        if env.proliferation_stage >= len(env.proliferation_rates):
            return

        target = int(
            len(self.cells) * (1 + env.proliferation_rates[env.proliferation_stage])
        )

        while len(self.cells) < target and len(self.cells) < 1000:
            # Find valid position
            for _ in range(100):
                x = np.random.uniform(env.dfc_region[0], env.dfc_region[2])
                y = np.random.uniform(env.dfc_region[1], env.dfc_region[3])

                valid = all(
                    np.linalg.norm(np.array([x, y]) - c.position) > 2 * c.base_radius
                    for c in self.cells
                    if c.active
                )

                if valid:
                    new_cell = CellWM([x, y], radius=self.cells[0].base_radius)
                    new_cell.is_offspring = True
                    self.cells.append(new_cell)
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
