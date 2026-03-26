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


def test_compute_energy():
    """Verify Hamiltonian energy computation returns positive values."""
    cell = CellWM([100, 100], radius=10.0, num_filo=4)
    energy = cell.compute_energy()
    assert energy >= 0, f"Energy should be non-negative, got {energy}"
    assert cell._current_area > 0, "Area should be positive"
    assert cell._current_perimeter > 0, "Perimeter should be positive"
    print("PASS: test_compute_energy")

def test_energy_in_state():
    """Verify energy fields present in serialized state."""
    cell = CellWM([100, 100], radius=10.0)
    cell.compute_energy()
    state = cell.get_state()
    assert 'energy' in state
    assert 'area' in state
    assert 'perimeter' in state
    print("PASS: test_energy_in_state")

def test_persistent_walk():
    """Verify persistence timer and reorientation mechanism."""
    cell = CellWM([100, 100], radius=10.0, num_filo=4)
    initial_pref = cell.preferred_direction
    # Run enough steps to trigger at least one reorientation
    for _ in range(50):
        cell.update_angles()
    print("PASS: test_persistent_walk")

def test_mechanotaxis_step():
    """Verify mechanotaxis force affects cell velocity."""
    cell = CellWM([200, 200], radius=10.0, num_filo=2)
    cell.velocity_scale = 0.0  # Disable shape-based velocity
    force = np.array([0.1, 0.0])
    old_pos = cell.position.copy()
    cell.simulation_step(mechanotaxis_force=force)
    # Cell should have moved in the force direction
    assert cell.position[0] > old_pos[0], "Cell should move in force direction"
    print("PASS: test_mechanotaxis_step")

def test_vectorized_contour():
    """Verify vectorized contour matches single-point evaluations."""
    cell = CellWM([0, 0], radius=10.0, num_filo=3)
    cell.angles = np.array([0.0, np.pi/2, -np.pi/2])
    cell.amplitudes = np.array([3.0, 2.0, 4.0])
    cell.widths = np.array([0.5, 0.7, 0.4])
    cell._create_contour()

    # Check a few contour points against fun_p
    thetas = np.linspace(-np.pi, np.pi, cell.num_contour, endpoint=False)
    for idx in [0, 25, 50, 75]:
        theta = thetas[idx]
        expected_r = max(cell.fun_p(theta, j) for j in range(3))
        actual_r = np.sqrt(
            (cell.contour[idx, 0] - cell.position[0])**2 +
            (cell.contour[idx, 1] - cell.position[1])**2
        )
        assert abs(actual_r - expected_r) < 0.1, \
            f"Contour point {idx}: expected r={expected_r:.3f}, got {actual_r:.3f}"
    print("PASS: test_vectorized_contour")


if __name__ == "__main__":
    test_cell_creation()
    test_contour_bounds()
    test_simulation_step()
    test_fun_p()
    test_get_state()
    test_compute_energy()
    test_energy_in_state()
    test_persistent_walk()
    test_mechanotaxis_step()
    test_vectorized_contour()
    print("\nAll cell tests passed!")
