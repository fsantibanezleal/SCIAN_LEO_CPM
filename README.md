# SCIAN LEO CPM

**Cellular Potts Model simulator for zebrafish embryonic cell migration.**

An interactive web application that models the collective migration of Dorsal Forerunner Cells (DFCs) during Kupffer's vesicle formation in zebrafish embryos. Cells are represented as deformable bodies with Gaussian filopodia-driven motility, moving between two advancing tissue boundaries (EVL and DEB) in real time.

![Architecture](docs/svg/architecture.svg)

---

## Quick Start

```bash
# Clone and setup
cd SCIAN_LEO_CPM
python -m venv .venv
source .venv/Scripts/activate  # Windows Git Bash
pip install -r requirements.txt

# Run tests
python tests/test_cell.py
python tests/test_agents.py
python tests/test_simulation.py

# Start the application
python -m uvicorn app.main:app --reload --port 8001

# Open in browser
# http://localhost:8001
```

For macOS/Linux, use `source .venv/bin/activate` instead.

---

## Cell Model

Each cell is a 2D deformable body whose membrane is the envelope of multiple Gaussian peaks in polar coordinates. Filopodia extend the boundary, break symmetry, and drive cell motion.

![Cell Model](docs/svg/cell_model.svg)

---

## Application Screenshot

The application provides a dark-themed interface with a simulation canvas on the left and interactive controls on the right.

![App Screenshot](docs/svg/app_screenshot.svg)

---

## Features

- **Deformable cell model** -- Gaussian filopodia create realistic protrusions with elastic membrane dynamics
- **Real-time visualization** -- HTML5 Canvas 2D rendering at 10-50 FPS with per-cell color coding
- **WebSocket streaming** -- low-latency bidirectional communication between server and browser
- **Interactive controls** -- sliders and buttons for cell count, radius, filopodia, velocity, and more
- **Tissue boundary dynamics** -- EVL epiboly front with 6-stage adhesion model and DEB convergence margin
- **Two-pass collision resolution** -- forward soft pass + backward hard pass prevents overlap and oscillation
- **REST API** -- full simulation control via HTTP with automatic Swagger/ReDoc documentation
- **Cell proliferation** -- configurable 8-stage schedule for cell division
- **Cross-platform** -- runs on any OS with Python 3.10+ and a modern browser

---

## Project Structure

```
SCIAN_LEO_CPM/
├── app/                           # Application code
│   ├── main.py                    # FastAPI entry point, REST + WebSocket endpoints
│   ├── config.py                  # Default configuration (Pydantic models)
│   ├── simulation/                # Core simulation engine
│   │   ├── cell.py                # CellWM: deformable cell with Gaussian filopodia
│   │   ├── agents.py              # AgentsSystem: population, collisions, proliferation
│   │   ├── environment.py         # EnvironmentSystem: domain, EVL/DEB boundaries
│   │   └── math_utils.py          # Vector math, angle normalization helpers
│   ├── api/                       # API layer (routes, WebSocket docs)
│   └── static/                    # Browser frontend
│       ├── index.html             # Single-page application
│       ├── css/style.css          # Dark-theme stylesheet
│       └── js/                    # JavaScript modules
│           ├── app.js             # Main orchestrator
│           ├── renderer.js        # Canvas 2D rendering
│           ├── controls.js        # UI control bindings
│           └── websocket.js       # WebSocket client with auto-reconnect
├── tests/                         # Test suite
│   ├── test_cell.py               # CellWM unit tests
│   ├── test_agents.py             # AgentsSystem unit tests
│   └── test_simulation.py         # Integration tests
├── docs/                          # Documentation
│   ├── architecture.md            # System design, API protocol, data flow
│   ├── biological_model.md        # Cell model math, collision algorithm, boundaries
│   ├── development_history.md     # Changelog, C++ to Python migration history
│   ├── references.md              # Publications and software references
│   ├── user_guide.md              # Installation, usage, troubleshooting
│   └── svg/                       # Diagrams
│       ├── architecture.svg       # System architecture
│       ├── cell_model.svg         # Gaussian filopodia concept
│       ├── simulation_flow.svg    # Simulation step pipeline
│       ├── biological_context.svg # Zebrafish embryo cross-section
│       └── app_screenshot.svg     # Application interface
├── legacy/                        # Previous C++/CLI codebase (2014-2024)
├── requirements.txt               # Python dependencies
└── README.md                      # This file
```

---

## API Documentation

The server automatically generates interactive API documentation:

- **Swagger UI**: http://localhost:8001/docs
- **ReDoc**: http://localhost:8001/redoc

### REST Endpoints

| Method | Endpoint                   | Description                          |
|--------|----------------------------|--------------------------------------|
| GET    | `/`                        | Serve the web application            |
| POST   | `/api/simulation/init`     | Initialize simulation with config    |
| POST   | `/api/simulation/start`    | Start continuous simulation          |
| POST   | `/api/simulation/stop`     | Pause simulation                     |
| POST   | `/api/simulation/step`     | Advance one step, return state       |
| GET    | `/api/simulation/state`    | Query state without advancing        |

### WebSocket

Connect to `ws://localhost:8001/ws/simulation` for real-time bidirectional streaming. Send JSON commands (`start`, `stop`, `speed`, `init`) and receive full state snapshots each step.

### Example: initialize and step via curl

```bash
# Initialize with 30 cells
curl -X POST http://localhost:8001/api/simulation/init \
  -H "Content-Type: application/json" \
  -d '{"num_cells": 30, "cell_radius": 8}'

# Advance one step
curl -X POST http://localhost:8001/api/simulation/step
```

---

## Tests

```bash
# Run all tests with pytest
python -m pytest tests/ -v

# Or run individually
python tests/test_cell.py
python tests/test_agents.py
python tests/test_simulation.py
```

---

## Documentation

| Document | Description |
|----------|-------------|
| [Architecture](docs/architecture.md) | System design, technology choices, API protocol, data flow, deployment |
| [Biological Model](docs/biological_model.md) | Gaussian filopodia math, cell dynamics, collisions, boundary model |
| [Development History](docs/development_history.md) | Project origins (C++/CLI 2014), migration to Python, changelog |
| [References](docs/references.md) | Key publications and software dependencies |
| [User Guide](docs/user_guide.md) | Installation, UI walkthrough, API examples, troubleshooting |

---

## References

- Graner & Glazier (1992). Simulation of biological cell sorting using a two-dimensional extended Potts model. *Physical Review Letters*, 69(13).
- Wortel & Textor (2021). Artistoo. *eLife*, 10:e61288.
- Oteiza et al. (2008). Origin and shaping of the laterality organ in zebrafish. *Development*, 135(16).
- Ablooglu et al. (2021). Apical contacts stemming from incomplete delamination. *eLife*, 10:e66495.

See [docs/references.md](docs/references.md) for the full annotated reference list.

---

## License

This project is developed by the **SCIAN-Lab** (Scientific Image Analysis Laboratory) at the University of Chile. Contact the authors for licensing information.
