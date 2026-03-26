"""Tests for AgentsSystem.

Validates agent initialization in grid layout, simulation stepping,
state serialization, and pairwise collision resolution.
"""

import numpy as np
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from app.simulation.agents import AgentsSystem
from app.simulation.environment import EnvironmentSystem


def test_initialization():
    """Verify agents are created at the correct count and all are active."""
    env = EnvironmentSystem({"width": 400, "height": 350})
    agents = AgentsSystem({"num_filopodia": 4, "velocity_scale": 0.01})
    agents.initialize(env, num_dfcs=12, radius=10.0)

    assert len(agents.cells) == 12
    assert all(c.active for c in agents.cells)
    print("PASS: test_initialization")


def test_simulation_step():
    """Verify a single update increments step_count."""
    env = EnvironmentSystem(
        {"width": 400, "height": 350, "evl_enabled": False, "deb_enabled": False}
    )
    agents = AgentsSystem({"num_filopodia": 2, "velocity_scale": 0.01})
    agents.initialize(env, num_dfcs=8, radius=10.0)

    agents.update(env)
    assert agents.step_count == 1
    print("PASS: test_simulation_step")


def test_get_state():
    """Verify get_state returns correct cell count and structure."""
    env = EnvironmentSystem()
    agents = AgentsSystem({"num_filopodia": 2})
    agents.initialize(env, num_dfcs=6)

    state = agents.get_state()
    assert state["num_cells"] == 6
    assert len(state["cells"]) == 6
    assert state["step"] == 0
    print("PASS: test_get_state")


def test_collision_resolution():
    """Verify overlapping cells are pushed apart to at least the sum of their radii."""
    env = EnvironmentSystem(
        {"width": 400, "height": 350, "evl_enabled": False, "deb_enabled": False}
    )
    agents = AgentsSystem({"num_filopodia": 2, "velocity_scale": 0.0})
    agents.initialize(env, num_dfcs=2, radius=10.0)

    # Place two cells very close (distance 5, but min_dist = 20)
    agents.cells[0].position = np.array([100.0, 100.0])
    agents.cells[1].position = np.array([105.0, 100.0])

    agents._resolve_collisions()

    dist = np.linalg.norm(agents.cells[0].position - agents.cells[1].position)
    assert dist >= 19.0, f"Cells should be separated, got distance {dist}"
    print("PASS: test_collision_resolution")


if __name__ == "__main__":
    test_initialization()
    test_simulation_step()
    test_get_state()
    test_collision_resolution()
    print("\nAll agent tests passed!")
