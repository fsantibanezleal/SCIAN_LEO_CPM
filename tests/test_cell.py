"""Tests for CellWM class.

Validates cell creation, contour generation, filopodium radial function,
simulation stepping, and state serialization.
"""

import numpy as np
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from app.simulation.cell import CellWM


def test_cell_creation():
    """Verify cell initializes with correct position, radius, and contour shape."""
    cell = CellWM([100, 100], radius=10.0, num_filo=4)
    assert cell.active
    assert cell.position[0] == 100.0
    assert cell.position[1] == 100.0
    assert cell.base_radius == 10.0
    assert cell.num_filo == 4
    assert cell.contour.shape == (100, 2)
    print("PASS: test_cell_creation")


def test_contour_bounds():
    """Verify contour points lie within a reasonable radial range of center."""
    cell = CellWM([50, 50], radius=10.0, num_filo=2)
    distances = np.sqrt(
        (cell.contour[:, 0] - 50) ** 2 + (cell.contour[:, 1] - 50) ** 2
    )
    assert np.all(distances > 0), "Contour points should be away from center"
    assert np.all(distances < 30), "Contour points should be near center"
    print("PASS: test_contour_bounds")


def test_simulation_step():
    """Verify a simulation step executes without error and preserves contour shape."""
    cell = CellWM([200, 200], radius=10.0)
    old_pos = cell.position.copy()
    cell.simulation_step()
    assert cell.contour.shape == (100, 2)
    print("PASS: test_simulation_step")


def test_fun_p():
    """Verify Gaussian filopodium radial function gives expected peak and far values."""
    cell = CellWM([0, 0], radius=10.0, num_filo=1)
    cell.angles[0] = 0.0
    cell.amplitudes[0] = 5.0
    cell.widths[0] = 0.5

    r_peak = cell.fun_p(0.0, 0)
    r_far = cell.fun_p(np.pi, 0)

    assert r_peak > r_far, "Radius at peak should be larger than at pi"
    assert abs(r_peak - 15.0) < 0.01, f"Peak radius should be ~15, got {r_peak}"
    assert abs(r_far - 10.0) < 1.0, f"Far radius should be ~10, got {r_far}"
    print("PASS: test_fun_p")


def test_get_state():
    """Verify get_state returns all expected keys with correct shapes."""
    cell = CellWM([100, 100], radius=10.0)
    state = cell.get_state()
    assert "position" in state
    assert "contour" in state
    assert "velocity" in state
    assert len(state["position"]) == 2
    assert len(state["contour"]) == 100
    print("PASS: test_get_state")


if __name__ == "__main__":
    test_cell_creation()
    test_contour_bounds()
    test_simulation_step()
    test_fun_p()
    test_get_state()
    print("\nAll cell tests passed!")
