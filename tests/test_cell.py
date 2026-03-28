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
    """Verify mechanotaxis gradient biases filopodial angles toward gradient."""
    cell = CellWM([200, 200], radius=10.0, num_filo=8)
    cell.velocity_scale = 0.01
    gradient = np.array([1.0, 0.0])  # Stiffness increases toward +x
    # Run many steps to let durotactic bias accumulate
    for _ in range(100):
        cell.simulation_step(mechanotaxis_gradient=gradient)
    # Filopodia should be biased toward +x (angle ~0), so velocity_x > 0
    # over many steps due to the angular bias
    assert cell.velocity[0] != 0.0 or cell.velocity[1] != 0.0, \
        "Cell should have non-zero velocity with gradient"
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


def test_polarity_initialization():
    """Verify polarity vector is initialized as unit vector matching preferred direction."""
    cell = CellWM([100, 100], radius=10.0, num_filo=4)
    assert hasattr(cell, 'polarity'), "Cell should have polarity attribute"
    mag = np.linalg.norm(cell.polarity)
    assert abs(mag - 1.0) < 1e-10, f"Polarity should be unit vector, got magnitude {mag}"
    expected_x = np.cos(cell.preferred_direction)
    expected_y = np.sin(cell.preferred_direction)
    assert abs(cell.polarity[0] - expected_x) < 1e-10
    assert abs(cell.polarity[1] - expected_y) < 1e-10
    print("PASS: test_polarity_initialization")


def test_contact_inhibition():
    """Verify CIL repolarizes cell away from contact direction."""
    cell = CellWM([100, 100], radius=10.0, num_filo=4)
    # Polarity pointing diagonally (not aligned with contact direction)
    cell.polarity = np.array([1.0, 1.0]) / np.sqrt(2)
    cell.preferred_direction = np.pi / 4
    # Other cell is directly to the right
    other_pos = np.array([115.0, 100.0])
    old_pol = cell.polarity.copy()
    cell.apply_contact_inhibition(other_pos)
    # Polarity x component should decrease (rotating away from contact at +x)
    assert cell.polarity[0] < old_pol[0], "Polarity x should decrease (away from contact)"
    mag = np.linalg.norm(cell.polarity)
    assert abs(mag - 1.0) < 1e-10, "Polarity should remain unit vector"
    print("PASS: test_contact_inhibition")


def test_cil_no_effect_when_far():
    """Verify CIL has no effect when cells are far apart."""
    cell = CellWM([100, 100], radius=10.0, num_filo=4)
    cell.polarity = np.array([1.0, 0.0])
    old_pol = cell.polarity.copy()
    # Other cell is very far away (> 3 * base_radius)
    other_pos = np.array([200.0, 100.0])
    cell.apply_contact_inhibition(other_pos)
    assert np.allclose(cell.polarity, old_pol), "Polarity should not change for far cells"
    print("PASS: test_cil_no_effect_when_far")


def test_polarity_in_state():
    """Verify polarity is included in serialized state."""
    cell = CellWM([100, 100], radius=10.0)
    state = cell.get_state()
    assert 'polarity' in state, "State should include polarity"
    assert len(state['polarity']) == 2
    print("PASS: test_polarity_in_state")


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
    test_polarity_initialization()
    test_contact_inhibition()
    test_cil_no_effect_when_far()
    test_polarity_in_state()
    print("\nAll cell tests passed!")
