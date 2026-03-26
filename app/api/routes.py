"""
REST API endpoints for simulation control.

Provides HTTP endpoints for initializing, starting, stopping, and stepping
the simulation, as well as querying the current state. These endpoints
are used by the frontend when WebSocket is not available or for one-shot
operations like initialization.
"""

from fastapi import APIRouter
from pydantic import BaseModel

router = APIRouter(prefix="/api/simulation", tags=["simulation"])


class SimulationConfig(BaseModel):
    """Configuration payload for simulation initialization.

    Attributes:
        width: Environment width.
        height: Environment height.
        num_cells: Number of DFC cells.
        cell_radius: Cell base radius.
        num_filopodia: Filopodia per cell.
        num_contour_points: Membrane discretization points.
        velocity_scale: Velocity estimation scaling factor.
        evl_enabled: Enable EVL boundary.
        evl_velocity: EVL migration speed.
        deb_enabled: Enable DEB boundary.
        proliferation_enabled: Enable cell proliferation.
        speed_ms: Milliseconds between simulation steps.
    """

    width: int = 400
    height: int = 350
    num_cells: int = 24
    cell_radius: float = 10.0
    num_filopodia: int = 4
    num_contour_points: int = 100
    velocity_scale: float = 0.01
    evl_enabled: bool = True
    evl_velocity: float = 1.5
    deb_enabled: bool = True
    proliferation_enabled: bool = False
    speed_ms: int = 50
