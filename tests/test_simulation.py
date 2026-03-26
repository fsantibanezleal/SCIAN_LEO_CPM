"""Integration tests for the full simulation.

Validates end-to-end behavior: multiple steps with EVL/DEB boundaries
active, cell boundary enforcement, and EVL stage progression.
"""

import numpy as np
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from app.simulation.agents import AgentsSystem
from app.simulation.environment import EnvironmentSystem


def test_full_simulation():
    """Run 50 steps and verify cells remain within environment bounds."""
    env = EnvironmentSystem(
        {
            "width": 400,
            "height": 350,
            "evl_enabled": True,
            "evl_velocity": 1.5,
            "deb_enabled": True,
        }
    )
    agents = AgentsSystem(
        {
            "num_filopodia": 4,
            "velocity_scale": 0.01,
        }
    )
    agents.initialize(env, num_dfcs=20, radius=10.0)

    # Run 50 steps
    for _ in range(50):
        env.update()
        agents.update(env)

    assert agents.step_count == 50
    assert all(c.active for c in agents.cells)

    # Cells should still be within bounds
    for c in agents.cells:
        assert 0 <= c.position[0] <= env.width
        assert 0 <= c.position[1] <= env.height

    print("PASS: test_full_simulation")


def test_evl_progression():
    """Verify EVL boundary advances and triggers stage transitions."""
    env = EnvironmentSystem({"evl_enabled": True, "evl_velocity": 10.0})
    initial_pos = env.evl_position

    for _ in range(100):
        env.update()

    assert env.evl_position > initial_pos
    assert env.evl_stage > 0
    print("PASS: test_evl_progression")


def test_mechanotaxis_drift():
    """Verify cells drift in mechanotaxis direction over many steps."""
    env = EnvironmentSystem({
        'width': 400, 'height': 350,
        'evl_enabled': False, 'deb_enabled': False,
        'mechanotaxis_enabled': True,
        'stiffness_gradient': [0.0, -1.0],
        'stiffness_gradient_strength': 0.01,
    })
    agents = AgentsSystem({'num_filopodia': 2, 'velocity_scale': 0.001})
    agents.initialize(env, num_dfcs=5, radius=8.0)

    initial_y = np.mean([c.position[1] for c in agents.cells])

    for _ in range(100):
        env.update()
        agents.update(env)

    final_y = np.mean([c.position[1] for c in agents.cells])

    # Cells should have drifted downward (negative y direction from gradient)
    assert final_y < initial_y, f"Cells should drift down: initial={initial_y:.1f}, final={final_y:.1f}"
    print("PASS: test_mechanotaxis_drift")


if __name__ == "__main__":
    test_full_simulation()
    test_evl_progression()
    test_mechanotaxis_drift()
    print("\nAll integration tests passed!")
