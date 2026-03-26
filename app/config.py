"""
Application configuration for SCIAN_LEO_CPM.

Centralizes default values and environment variable overrides for the
FastAPI application and simulation parameters.
"""

from pydantic import BaseModel
from typing import Optional


class AppConfig(BaseModel):
    """Global application configuration.

    Attributes:
        host: Server bind address.
        port: Server bind port.
        reload: Whether to enable auto-reload in development.
        log_level: Logging level (debug, info, warning, error).
    """

    host: str = "0.0.0.0"
    port: int = 8001
    reload: bool = True
    log_level: str = "info"


class SimulationDefaults(BaseModel):
    """Default simulation parameters.

    These values are used when no explicit configuration is provided
    through the API.

    Attributes:
        width: Environment width in spatial units.
        height: Environment height in spatial units.
        num_cells: Default number of DFC cells.
        cell_radius: Default cell base radius.
        num_filopodia: Default number of filopodia per cell.
        num_contour_points: Default membrane discretization.
        velocity_scale: Default velocity scaling factor.
        evl_enabled: Whether EVL boundary is active by default.
        evl_velocity: Default EVL migration speed.
        deb_enabled: Whether DEB boundary is active by default.
        proliferation_enabled: Whether proliferation is active by default.
        speed_ms: Default milliseconds between simulation steps.
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


# Singleton instances
app_config = AppConfig()
sim_defaults = SimulationDefaults()
