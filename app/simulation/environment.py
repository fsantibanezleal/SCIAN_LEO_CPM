"""
Simulation environment for the CPM model.

Defines the spatial domain and tissue boundaries (EVL, DEB) that constrain
cell behavior. The EVL (Enveloping Layer) migrates progressively through
defined stages with varying adhesion properties.

The environment supports:
- Configurable workspace dimensions
- EVL boundary with multi-stage progression
- DEB margin with attraction dynamics
- Proliferation scheduling (8-stage)

References:
    - Keller et al. (bioRxiv 2025), Modeling Epithelial Morphogenesis
      during Zebrafish Epiboly
    - Rho et al. (2009), Zebrafish epiboly: mechanics and mechanisms
"""

import numpy as np


class EnvironmentSystem:
    """Spatial domain and tissue boundary manager.

    Manages the rectangular simulation domain and the two main tissue
    boundaries that constrain DFC cell migration:

    - EVL (Enveloping Layer): Migrates from top to bottom across the domain,
      representing the epiboly front. Cells above the EVL are pushed downward.
      The EVL progresses through discrete stages with different adhesion
      properties derived from experimental data.

    - DEB (Dorsal Epiblast Boundary): The lower boundary of the dorsal
      epiblast tissue. Moves upward, compressing the DFC region from below.

    Attributes:
        width: Domain width in spatial units.
        height: Domain height in spatial units.
        dfc_region: [x_min, y_min, x_max, y_max] bounds for DFC initialization.
        evl_enabled: Whether the EVL boundary is active.
        evl_position: Current y-position of the EVL front.
        evl_velocity: Speed of EVL migration (units/step).
        evl_stage: Current EVL stage index.
        evl_stage_positions: Y-positions triggering stage transitions.
        evl_stage_adhesion: Adhesion strength per stage.
        deb_enabled: Whether the DEB boundary is active.
        deb_position: Current y-position of the DEB margin.
        deb_velocity: Speed of DEB movement (units/step).
        proliferation_enabled: Whether cell proliferation is scheduled.
        proliferation_rates: Per-stage proliferation rate multipliers.
        proliferation_stage: Current proliferation stage index.
        proliferation_interval: Steps between proliferation checks.
        steps_since_proliferation: Counter for proliferation timing.
    """

    def __init__(self, config=None):
        """Initialize the environment.

        Args:
            config: Optional dictionary with configuration overrides. Keys:
                'width', 'height', 'dfc_region_width', 'dfc_region_height',
                'margin', 'evl_enabled', 'evl_initial_position', 'evl_velocity',
                'deb_enabled', 'deb_initial_position', 'deb_velocity',
                'proliferation_enabled', 'proliferation_interval'.
        """
        config = config or {}

        self.width = config.get("width", 400)
        self.height = config.get("height", 350)

        # DFC initialization region [x_min, y_min, x_max, y_max]
        dfc_w = config.get("dfc_region_width", 250)
        dfc_h = config.get("dfc_region_height", 70)
        margin = config.get("margin", 10)
        self.dfc_region = [
            margin,
            self.height / 2 - dfc_h / 2,
            margin + dfc_w,
            self.height / 2 + dfc_h / 2,
        ]

        # EVL (Enveloping Layer)
        self.evl_enabled = config.get("evl_enabled", True)
        self.evl_position = config.get("evl_initial_position", 0.0)
        self.evl_velocity = config.get("evl_velocity", 1.5)
        self.evl_stage = 0
        self.evl_stage_positions = [0, 70, 140, 210, 280, 350]
        self.evl_stage_adhesion = [0.8, 0.8, 0.45, 0.2, 0.0, 0.0]

        # DEB (Dorsal Epiblast Boundary)
        self.deb_enabled = config.get("deb_enabled", True)
        self.deb_position = config.get("deb_initial_position", self.height)
        self.deb_velocity = config.get("deb_velocity", 0.5)

        # Mechanotaxis: substrate stiffness gradient
        self.mechanotaxis_enabled = config.get('mechanotaxis_enabled', False)
        self.stiffness_gradient_direction = np.array(
            config.get('stiffness_gradient', [0.0, -1.0]),  # default: stiffer toward bottom
            dtype=np.float64
        )
        self.stiffness_gradient_strength = config.get('stiffness_gradient_strength', 0.002)

        # Proliferation
        self.proliferation_enabled = config.get("proliferation_enabled", False)
        self.proliferation_rates = [0.0, 0.0, 0.1, 0.0, 0.2, 0.0, 0.0, 0.0]
        self.proliferation_stage = 0
        self.proliferation_interval = config.get("proliferation_interval", 100)
        self.steps_since_proliferation = 0

    def update(self):
        """Advance environment by one time step.

        Updates:
        1. EVL position and stage transitions.
        2. DEB position with lower bound clamping.
        3. Proliferation stage timing.
        """
        # EVL migration
        if self.evl_enabled:
            self.evl_position += self.evl_velocity
            # Check stage transition
            if (
                self.evl_stage < len(self.evl_stage_positions) - 1
                and self.evl_position >= self.evl_stage_positions[self.evl_stage + 1]
            ):
                self.evl_stage += 1

        # DEB movement
        if self.deb_enabled:
            self.deb_position -= self.deb_velocity
            self.deb_position = max(self.deb_position, self.height * 0.3)

        # Proliferation timing
        if self.proliferation_enabled:
            self.steps_since_proliferation += 1
            if self.steps_since_proliferation >= self.proliferation_interval:
                self.steps_since_proliferation = 0
                if self.proliferation_stage < len(self.proliferation_rates) - 1:
                    self.proliferation_stage += 1

    def get_mechanotaxis_force(self, position):
        """Compute mechanotactic bias force at a given position.

        Cells in a stiffness gradient experience durotaxis: migration
        toward stiffer substrate regions. The force is modeled as a
        constant directional bias proportional to the gradient strength.

        F_mechano = strength * gradient_direction

        This simplified model captures the key phenomenology without
        requiring a full finite-element substrate model.

        Args:
            position: Cell center [x, y] coordinates.

        Returns:
            Force vector [fx, fy] for mechanotactic bias.
        """
        if not self.mechanotaxis_enabled:
            return np.zeros(2)
        return self.stiffness_gradient_strength * self.stiffness_gradient_direction

    def get_state(self):
        """Return serializable environment state.

        Returns:
            Dictionary with width, height, EVL/DEB positions and stages,
            adhesion strength, DFC region bounds, and mechanotaxis settings.
        """
        return {
            "width": self.width,
            "height": self.height,
            "evl_position": self.evl_position,
            "evl_stage": self.evl_stage,
            "evl_adhesion": self.evl_stage_adhesion[
                min(self.evl_stage, len(self.evl_stage_adhesion) - 1)
            ],
            "deb_position": self.deb_position,
            "dfc_region": self.dfc_region,
            "mechanotaxis_enabled": self.mechanotaxis_enabled,
            "stiffness_gradient": self.stiffness_gradient_direction.tolist(),
            "stiffness_gradient_strength": self.stiffness_gradient_strength,
        }
