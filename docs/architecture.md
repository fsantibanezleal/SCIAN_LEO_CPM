# Architecture

## System Overview

SCIAN_LEO_CPM is a web-based simulation platform for modeling collective cell migration during zebrafish embryonic development. The system is split into two main layers: a **Python backend** that runs the numerical simulation and exposes it through a network API, and a **browser frontend** that visualizes the results in real time using HTML5 Canvas.

The architecture diagram below shows how these layers connect:

![System Architecture](svg/architecture.svg)

The design follows a strict separation between computation and presentation. The backend is the single source of truth for simulation state: it owns every cell position, every membrane contour, and every boundary location. The frontend never performs simulation math; it only receives pre-computed state snapshots and draws them.

---

## Technology Stack Justification

### Why FastAPI?

FastAPI was chosen because it supports both REST endpoints and WebSocket connections natively within the same application. Traditional frameworks like Flask or Django require extra middleware or separate servers for WebSocket support. FastAPI also generates interactive API documentation automatically (Swagger UI at `/docs` and ReDoc at `/redoc`), which makes it easy for developers to inspect and test the API without writing client code. Its Pydantic integration provides request validation at no extra effort.

### Why HTML5 Canvas (not WebGL, not a charting library)?

Canvas 2D provides the right balance of simplicity and performance for this application. The cells are drawn as closed polygons with up to 100 vertices each, and the total cell count rarely exceeds 100. Canvas handles this comfortably at 20+ FPS. WebGL would add shader complexity without a meaningful performance gain at this scale. Charting libraries (D3, Chart.js, Plotly) are designed for data visualization, not for real-time polygon rendering of deformable shapes. Using plain Canvas keeps the frontend dependency-free (no npm, no bundler).

### Why WebSocket instead of polling?

The simulation produces state updates at 10-50 Hz depending on the speed setting. HTTP polling at this rate would introduce per-request overhead (connection setup, headers, parsing) and make it difficult to maintain smooth animation. A persistent WebSocket connection eliminates this overhead: the server pushes a JSON frame whenever a new step completes, and the client renders it immediately. The WebSocket also carries control commands from the client back to the server (start, stop, speed adjustment, re-initialization), giving true bidirectional communication over a single connection.

### Why NumPy/SciPy?

The cell model involves repeated trigonometric evaluations (sine, cosine, exponential) over arrays of contour points. NumPy vectorizes these operations efficiently. SciPy is available for any future extensions that need interpolation, optimization, or spatial data structures (e.g., KD-trees for collision detection at higher cell counts).

---

## Directory Structure

```
SCIAN_LEO_CPM/
├── app/                           # All application code
│   ├── __init__.py                # Package marker
│   ├── main.py                    # FastAPI app, REST routes, WebSocket endpoint
│   ├── config.py                  # Default configuration (AppConfig, SimulationDefaults)
│   ├── simulation/                # Core simulation engine (pure Python + NumPy)
│   │   ├── __init__.py
│   │   ├── cell.py                # CellWM class: deformable cell with Gaussian filopodia
│   │   ├── agents.py              # AgentsSystem: population manager, collisions, proliferation
│   │   ├── environment.py         # EnvironmentSystem: domain, EVL/DEB boundaries
│   │   └── math_utils.py          # Helper functions: vector math, angle normalization
│   ├── api/                       # API documentation layer
│   │   ├── __init__.py
│   │   ├── routes.py              # REST endpoint definitions (if split from main)
│   │   └── websocket.py           # WebSocket protocol documentation
│   └── static/                    # Browser frontend (served by FastAPI StaticFiles)
│       ├── index.html             # Single-page application shell
│       ├── css/
│       │   └── style.css          # Dark-theme stylesheet
│       └── js/
│           ├── app.js             # Main orchestrator: wires controls, WS, and renderer
│           ├── renderer.js        # SimRenderer: Canvas 2D drawing (cells, boundaries, grid)
│           ├── controls.js        # UI control bindings (sliders, buttons, checkboxes)
│           └── websocket.js       # WebSocket client with auto-reconnect
├── tests/                         # Automated test suite
│   ├── test_cell.py               # Unit tests for CellWM
│   ├── test_agents.py             # Unit tests for AgentsSystem
│   └── test_simulation.py         # Integration tests (init + step + state)
├── docs/                          # Project documentation
│   ├── architecture.md            # This file
│   ├── biological_model.md        # Cell model math and biological context
│   ├── development_history.md     # Project changelog and migration notes
│   ├── references.md              # Key publications and software references
│   ├── user_guide.md              # Installation, usage, troubleshooting
│   └── svg/                       # Hand-crafted SVG diagrams
│       ├── architecture.svg       # System architecture diagram
│       ├── cell_model.svg         # Gaussian filopodia cell concept
│       ├── simulation_flow.svg    # Simulation step pipeline
│       ├── biological_context.svg # Zebrafish embryo cross-section
│       └── app_screenshot.svg     # Application screenshot representation
├── legacy/                        # Previous C++/CLI codebase (2014-2024)
│   ├── Documents/                 # Original documentation and papers
│   ├── Version Base/              # Original C++ source code
│   └── Version Dummy/             # Simplified test version
├── requirements.txt               # Python dependencies (pip freeze output)
└── README.md                      # Project overview and quick start
```

---

## API Endpoints

### REST Endpoints

All REST endpoints accept and return JSON. The base URL is `http://localhost:8001`.

| Method | Path                       | Request Body           | Response                                   | Description                                |
|--------|----------------------------|------------------------|--------------------------------------------|--------------------------------------------|
| GET    | `/`                        | --                     | HTML page                                  | Serves the single-page application         |
| POST   | `/api/simulation/init`     | `SimulationConfig` JSON | `{status, num_cells}`                      | Creates or resets the simulation            |
| POST   | `/api/simulation/start`    | --                     | `{status: "running"}`                      | Sets the running flag to true              |
| POST   | `/api/simulation/stop`     | --                     | `{status: "stopped"}`                      | Pauses the simulation loop                 |
| POST   | `/api/simulation/step`     | --                     | `{agents: {...}, environment: {...}}`       | Advances one step, returns full state      |
| GET    | `/api/simulation/state`    | --                     | `{agents: {...}, environment: {...}}`       | Returns current state without advancing    |

#### SimulationConfig Schema

```json
{
  "width": 400,
  "height": 350,
  "num_cells": 24,
  "cell_radius": 10.0,
  "num_filopodia": 4,
  "num_contour_points": 100,
  "velocity_scale": 0.01,
  "evl_enabled": true,
  "evl_velocity": 1.5,
  "deb_enabled": true,
  "proliferation_enabled": false,
  "speed_ms": 50
}
```

All fields have defaults. You can send a partial object and only override what you need.

### WebSocket Protocol

**Endpoint**: `ws://localhost:8001/ws/simulation`

After connecting, the server enters a polling loop. Each iteration, it checks for a client message (with a 10 ms timeout), then runs one simulation step if the simulation is in the running state, and sends the resulting state as JSON.

**Client-to-server messages** (JSON):

| Message                                  | Effect                                    |
|------------------------------------------|-------------------------------------------|
| `{"action": "start"}`                    | Sets running flag to true                 |
| `{"action": "stop"}`                     | Sets running flag to false                |
| `{"action": "speed", "value": 100}`      | Changes inter-step delay to 100 ms        |
| `{"action": "init", "config": {...}}`    | Re-initializes with provided config       |

**Server-to-client messages** (JSON):

Each frame is a complete state snapshot:

```json
{
  "agents": {
    "step": 142,
    "num_cells": 24,
    "cells": [
      {
        "position": [120.5, 200.3],
        "contour": [[130.1, 195.2], [128.4, 192.1], ...],
        "velocity": [0.15, -0.08],
        "active": true,
        "radius": 10.0
      }
    ],
    "running": true
  },
  "environment": {
    "width": 400,
    "height": 350,
    "evl_position": 85.5,
    "evl_stage": 1,
    "evl_adhesion": 0.8,
    "deb_position": 320.0,
    "dfc_region": [10, 140, 260, 210]
  }
}
```

---

## Data Flow

### Continuous Mode (WebSocket)

1. User opens the browser. The `index.html` page loads JavaScript files.
2. `app.js` creates a `SimRenderer`, a `SimWebSocket` client, and wires up UI controls.
3. User clicks **Initialize**. The WebSocket sends `{"action": "init", "config": {...}}`.
4. The server creates new `EnvironmentSystem` and `AgentsSystem` instances and places cells in a grid.
5. User clicks **Start**. The WebSocket sends `{"action": "start"}`.
6. On the server, each loop iteration calls `env.update()` (move EVL/DEB) then `agents.update(env)` (step cells, resolve collisions, apply boundaries).
7. The resulting state is serialized to JSON and pushed to the client.
8. `renderer.js` clears the canvas, scales coordinates, and draws cell polygons, center dots, and boundary lines.
9. The loop repeats until the user sends `{"action": "stop"}` or disconnects.

### Step Mode (REST)

1. After initialization, the user clicks **Step**.
2. The frontend sends `POST /api/simulation/step`.
3. The server advances one step and returns the state in the HTTP response body.
4. The frontend renders that state on the canvas.

---

## Deployment Options

### Development (default)

```bash
python -m uvicorn app.main:app --reload --port 8001
```

The `--reload` flag watches for file changes and restarts the server automatically. Suitable for development only.

### Production (single worker)

```bash
uvicorn app.main:app --host 0.0.0.0 --port 8001 --workers 1
```

Because the simulation state is stored in a global Python dictionary, multi-worker deployment is not supported. Each worker would have its own independent simulation state, leading to inconsistent behavior. For production use, run a single worker behind a reverse proxy (e.g., Nginx) if TLS or load balancing is needed.

### Docker (optional)

A Dockerfile is not included but would be straightforward:

```dockerfile
FROM python:3.12-slim
WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY . .
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8001"]
```

### cPanel / Passenger (shared hosting)

The repository includes a `passenger_wsgi.py` entry point that exposes the FastAPI ASGI application under the name `application`, which is the symbol Passenger looks up by default:

```python
# passenger_wsgi.py
import sys, os
sys.path.insert(0, os.path.dirname(__file__))
from app.main import app as application
```

To deploy under cPanel with Passenger:

1. Create a Python application in cPanel pointing at the repository root.
2. Set the application startup file to `passenger_wsgi.py` and the application entry point to `application`.
3. In the application's virtual environment, run `pip install -r requirements.txt`.
4. Restart the Python application from cPanel.

Notes specific to Passenger:

- Passenger spawns a fresh worker per request pool. Because the simulation state lives in a process-local Python object, long-running WebSocket sessions require a single persistent worker. If cPanel is configured with multiple workers, only one of them will see a given simulation state.
- The `/ws/simulation` endpoint needs WebSocket support at the reverse proxy layer (LiteSpeed handles this transparently; Apache with mod_passenger needs the `Upgrade`/`Connection` headers forwarded).
- If the hosting provider restricts outbound ports or WebSocket upgrades, the application gracefully degrades to REST `step` mode, which uses plain HTTP POST requests.

---

## Performance Considerations

### Collision Detection: O(N^2)

The two-pass collision resolution checks every pair of active cells (forward and backward), making it O(N^2) in the number of cells. For the typical range of 20-50 cells, this is negligible. For larger simulations (100+ cells), a spatial indexing structure (grid hash or KD-tree) could reduce this to O(N log N).

### Contour Resolution vs. Speed

Each cell membrane is discretized into `num_contour_points` points (default: 100). The shape update, velocity estimation, and contour generation all iterate over these points. Reducing the count to 40-60 can roughly double the simulation throughput with minimal visual degradation.

### WebSocket Serialization

Every frame serializes the full contour arrays of all cells to JSON. With 24 cells and 100 contour points each, this is approximately 24 * 100 * 2 * 8 = ~38 KB per frame. At 20 FPS that is about 760 KB/s, well within the capacity of a local network. For remote deployment, a binary serialization format (e.g., MessagePack) or delta encoding could reduce bandwidth.

### Canvas Rendering

The frontend draws each cell as a single polygon path (moveTo + lineTo loop + closePath). Canvas 2D handles this efficiently for up to several hundred polygons. The main bottleneck is the stroke/fill overhead per cell. Batching all cells into a single path (using subpaths) could reduce draw calls if needed.
