"""
SCIAN_LEO_CPM - Cellular Potts Model Web Application.

FastAPI-based web application for interactive simulation of the Cellular
Potts Model applied to zebrafish embryonic cell migration (DFC/EVL/DEB).

The application serves a browser-based frontend with real-time visualization
via Canvas 2D and WebSocket streaming. Users can configure simulation
parameters, start/stop the simulation, and observe cell behavior including
filopodia-driven motility, collision resolution, and tissue boundary dynamics.

Run with:
    uvicorn app.main:app --reload --port 8001

Or from the project root:
    python -m uvicorn app.main:app --reload --port 8001
"""

import asyncio
import json
from pathlib import Path

from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.staticfiles import StaticFiles
from fastapi.responses import FileResponse
from pydantic import BaseModel
from typing import Optional

from .simulation.agents import AgentsSystem
from .simulation.environment import EnvironmentSystem

app = FastAPI(
    title="SCIAN LEO CPM",
    description="Cellular Potts Model simulator for zebrafish embryonic development",
    version="2.0.0",
)

# Serve static files
static_dir = Path(__file__).parent / "static"
app.mount("/static", StaticFiles(directory=str(static_dir)), name="static")


# ── Simulation state (global, single-user) ──────────────────────────────────

simulation_state = {
    "env": None,
    "agents": None,
    "running": False,
    "speed": 50,  # ms between steps
}


# ── Pydantic models ─────────────────────────────────────────────────────────


class SimulationConfig(BaseModel):
    """Configuration payload for simulation initialization.

    All fields have sensible defaults and can be overridden by the client.
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


# ── REST endpoints ───────────────────────────────────────────────────────────


@app.get("/")
async def root():
    """Serve the main HTML page."""
    return FileResponse(str(static_dir / "index.html"))


@app.post("/api/simulation/init")
async def init_simulation(config: SimulationConfig):
    """Initialize (or re-initialize) the simulation with given parameters.

    Creates a new EnvironmentSystem and AgentsSystem, places DFC cells in
    a grid layout, and resets the step counter.

    Args:
        config: SimulationConfig with all parameters.

    Returns:
        JSON with status and number of cells created.
    """
    env_config = {
        "width": config.width,
        "height": config.height,
        "evl_enabled": config.evl_enabled,
        "evl_velocity": config.evl_velocity,
        "deb_enabled": config.deb_enabled,
        "proliferation_enabled": config.proliferation_enabled,
    }
    agent_config = {
        "num_filopodia": config.num_filopodia,
        "num_contour_points": config.num_contour_points,
        "velocity_scale": config.velocity_scale,
        "proliferation_enabled": config.proliferation_enabled,
    }

    simulation_state["env"] = EnvironmentSystem(env_config)
    simulation_state["agents"] = AgentsSystem(agent_config)
    simulation_state["agents"].initialize(
        simulation_state["env"],
        num_dfcs=config.num_cells,
        radius=config.cell_radius,
    )
    simulation_state["speed"] = config.speed_ms
    simulation_state["running"] = False

    return {
        "status": "initialized",
        "num_cells": len(simulation_state["agents"].cells),
    }


@app.post("/api/simulation/start")
async def start_simulation():
    """Start the simulation loop (WebSocket will begin streaming)."""
    simulation_state["running"] = True
    return {"status": "running"}


@app.post("/api/simulation/stop")
async def stop_simulation():
    """Pause the simulation loop."""
    simulation_state["running"] = False
    return {"status": "stopped"}


@app.post("/api/simulation/step")
async def step_simulation():
    """Advance the simulation by exactly one time step.

    Returns the full simulation state (agents + environment) after the step.
    """
    if simulation_state["agents"] is None:
        return {"error": "Simulation not initialized"}

    simulation_state["env"].update()
    simulation_state["agents"].update(simulation_state["env"])

    return {
        "agents": simulation_state["agents"].get_state(),
        "environment": simulation_state["env"].get_state(),
    }


@app.get("/api/simulation/state")
async def get_state():
    """Query the current simulation state without advancing it."""
    if simulation_state["agents"] is None:
        return {"error": "Simulation not initialized"}
    return {
        "agents": simulation_state["agents"].get_state(),
        "environment": simulation_state["env"].get_state(),
    }


# ── WebSocket endpoint ──────────────────────────────────────────────────────


@app.websocket("/ws/simulation")
async def websocket_simulation(websocket: WebSocket):
    """Real-time simulation streaming over WebSocket.

    Accepts control commands from the client and pushes simulation state
    at the configured rate when the simulation is running.

    Client commands (JSON):
        {"action": "start"}       - Start simulation loop
        {"action": "stop"}        - Pause simulation loop
        {"action": "speed", "value": <int>}  - Set step interval in ms
        {"action": "init", "config": {...}}  - Re-initialize simulation

    Server pushes (JSON):
        {"agents": {...}, "environment": {...}}  - Full state each step
    """
    await websocket.accept()
    try:
        while True:
            # Check for client messages (non-blocking with short timeout)
            try:
                data = await asyncio.wait_for(
                    websocket.receive_text(), timeout=0.01
                )
                msg = json.loads(data)
                if msg.get("action") == "start":
                    simulation_state["running"] = True
                elif msg.get("action") == "stop":
                    simulation_state["running"] = False
                elif msg.get("action") == "speed":
                    simulation_state["speed"] = msg.get("value", 50)
                elif msg.get("action") == "init":
                    config = SimulationConfig(**msg.get("config", {}))
                    await init_simulation(config)
            except asyncio.TimeoutError:
                pass

            # Run simulation step if running
            if simulation_state["running"] and simulation_state["agents"]:
                simulation_state["env"].update()
                simulation_state["agents"].update(simulation_state["env"])

                state = {
                    "agents": simulation_state["agents"].get_state(),
                    "environment": simulation_state["env"].get_state(),
                }
                await websocket.send_json(state)

            await asyncio.sleep(simulation_state["speed"] / 1000.0)
    except WebSocketDisconnect:
        pass
