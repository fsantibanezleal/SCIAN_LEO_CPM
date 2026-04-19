"""Unit tests for the /api/health endpoint.

The health route must be cheap, safe to hit at any frequency (it is used
by cPanel, Docker, and reverse-proxy probes), and must never touch the
simulation lock. The tests also verify the ``sim_initialized`` flag
flips correctly after ``/api/simulation/init``.
"""

import os
import sys

from fastapi.testclient import TestClient

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from app import __version__
from app.main import app, simulation_state


def _reset_simulation_state() -> None:
    """Tests run against the global ``simulation_state``; reset between cases."""
    simulation_state["env"] = None
    simulation_state["agents"] = None
    simulation_state["running"] = False


def test_health_ok_before_init():
    """Health endpoint returns 200 and sim_initialized=False on a fresh process."""
    _reset_simulation_state()
    client = TestClient(app)

    response = client.get("/api/health")

    assert response.status_code == 200
    payload = response.json()
    assert payload["status"] == "ok"
    assert payload["version"] == __version__
    assert payload["sim_initialized"] is False
    print("PASS: test_health_ok_before_init")


def test_health_sim_initialized_flag_flips():
    """After /api/simulation/init, health reports sim_initialized=True."""
    _reset_simulation_state()
    client = TestClient(app)

    # Initialize with a tiny simulation to keep the test fast.
    init_payload = {"num_cells": 3, "cell_radius": 8.0}
    init_resp = client.post("/api/simulation/init", json=init_payload)
    assert init_resp.status_code == 200

    response = client.get("/api/health")

    assert response.status_code == 200
    payload = response.json()
    assert payload["status"] == "ok"
    assert payload["sim_initialized"] is True
    print("PASS: test_health_sim_initialized_flag_flips")


if __name__ == "__main__":
    test_health_ok_before_init()
    test_health_sim_initialized_flag_flips()
    print("\nAll health tests passed!")
