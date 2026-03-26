# User Guide

## Prerequisites

Before running the application, you need the following installed on your system:

- **Python 3.10 or later** -- download from [python.org](https://www.python.org/downloads/) or use your system package manager. Verify with `python --version`.
- **pip** -- the Python package installer. It is included with Python 3.10+. Verify with `pip --version`.
- **Git** -- for cloning the repository. Verify with `git --version`.
- **A modern web browser** -- Chrome, Firefox, Edge, or Safari. The application uses HTML5 Canvas 2D and WebSocket, both of which are supported by all current browsers.

No C/C++ compiler, Node.js, or other build tools are required. The entire application runs in Python with pure-Python and precompiled binary dependencies (NumPy, SciPy).

---

## Installation Step by Step

### 1. Clone the repository

```bash
git clone <repository-url>
cd SCIAN_LEO_CPM
```

### 2. Create a Python virtual environment

A virtual environment keeps this project's dependencies isolated from your system Python.

```bash
python -m venv .venv
```

### 3. Activate the virtual environment

On **Windows (Git Bash / MSYS2)**:
```bash
source .venv/Scripts/activate
```

On **Windows (PowerShell)**:
```powershell
.venv\Scripts\Activate.ps1
```

On **Windows (Command Prompt)**:
```cmd
.venv\Scripts\activate.bat
```

On **macOS / Linux**:
```bash
source .venv/bin/activate
```

You should see `(.venv)` appear in your terminal prompt, confirming the virtual environment is active.

### 4. Install dependencies

```bash
pip install -r requirements.txt
```

This installs FastAPI, Uvicorn, NumPy, SciPy, Pydantic, and their transitive dependencies. The full list is pinned in `requirements.txt` for reproducibility.

### 5. Verify the installation

Run the test suite to confirm everything works:

```bash
python tests/test_cell.py
python tests/test_agents.py
python tests/test_simulation.py
```

Or using pytest (if installed):

```bash
python -m pytest tests/ -v
```

All tests should pass. If any fail, check that you are using the correct Python version and that the virtual environment is active.

---

## Running the Application

### Start the server

```bash
python -m uvicorn app.main:app --reload --port 8001
```

You should see output like:

```
INFO:     Uvicorn running on http://127.0.0.1:8001 (Press CTRL+C to quit)
INFO:     Started reloader process [12345]
INFO:     Started server process [12346]
INFO:     Waiting for application startup.
INFO:     Application startup complete.
```

### Open the application

Navigate to **http://localhost:8001** in your browser. You will see the CPM Simulator interface with a dark canvas on the left and a controls panel on the right.

### Stop the server

Press **Ctrl+C** in the terminal where Uvicorn is running.

---

## Using the User Interface

### Workflow

1. Adjust parameters in the controls panel (right side).
2. Click **Initialize** to create the simulation with those parameters. Cells will appear on the canvas in a grid layout.
3. Click **Start** for continuous simulation, or **Step** for single-step advancement.
4. Click **Stop** to pause. You can adjust parameters and re-initialize at any time.

### Simulation Controls

| Button       | Behavior                                                              |
|-------------|-----------------------------------------------------------------------|
| Initialize  | Creates new cell population and environment. Resets step counter.      |
| Start       | Begins continuous simulation. State streams via WebSocket.            |
| Stop        | Pauses the simulation. Cells freeze in their current state.           |
| Step        | Advances exactly one simulation step. Useful for detailed inspection. |

### Cell Parameters

| Control          | Range        | Default | What it does                                                                |
|------------------|--------------|---------|-----------------------------------------------------------------------------|
| Number of Cells  | 4 -- 100     | 24      | Total DFC cells placed during initialization.                               |
| Cell Radius      | 3 -- 25      | 10      | Base radius R0. Larger cells are slower to move but fill more space.        |
| Filopodia        | 1 -- 10      | 4       | Gaussian protrusions per cell. More filopodia produce more irregular shapes.|
| Contour Points   | 20 -- 200    | 100     | Polygon resolution. Higher values produce smoother contours but cost more.  |
| Velocity Scale   | 0.001 -- 0.1 | 0.01    | Overall migration speed multiplier. Higher = faster but noisier motion.     |

### Environment Parameters

| Control        | Range      | Default | What it does                                                        |
|----------------|------------|---------|---------------------------------------------------------------------|
| Width          | 200 -- 800 | 400     | Horizontal extent of the simulation domain.                         |
| Height         | 200 -- 800 | 350     | Vertical extent of the simulation domain.                           |
| EVL Boundary   | on / off   | on      | Enables the enveloping layer boundary migrating from top to bottom. |
| EVL Velocity   | 0 -- 5     | 1.5     | Speed of EVL migration in spatial units per step.                   |
| DEB Boundary   | on / off   | on      | Enables the dorsal epiblast boundary converging from below.         |
| Proliferation  | on / off   | off     | Enables the 8-stage cell division schedule.                         |

### Display Options

| Control       | Default | What it does                                                            |
|---------------|---------|-------------------------------------------------------------------------|
| Speed (ms)    | 50      | Delay between simulation steps in milliseconds. Higher = slower.        |
| Show Contours | on      | Draws the cell membrane outlines as colored polygons.                   |
| Show Centers  | on      | Draws a yellow dot at each cell's center position.                      |
| Show Boundaries| on     | Draws the EVL (red) and DEB (green) boundary lines and DFC region box. |
| Show Grid     | off     | Draws a faint background grid at 50-unit intervals.                     |

### Visual Guide

- **Cell contours**: Each cell is drawn as a closed polygon. Colors are assigned by hue rotation so adjacent cells are visually distinct.
- **Cell centers**: Small gold dots indicating the mathematical center of each cell.
- **EVL boundary**: A red dashed horizontal line labeled with the current stage number. It moves downward as the simulation progresses.
- **DEB boundary**: A green dashed horizontal line labeled "DEB margin". It moves upward.
- **DFC region**: A blue semi-transparent rectangle showing where cells were initially placed.
- **Info bar**: Below the canvas, shows the current step number, active cell count, and rendering FPS.

---

## API Usage Examples

The REST API can be used from any HTTP client. Below are examples using `curl`.

### Initialize a simulation

```bash
curl -X POST http://localhost:8001/api/simulation/init \
  -H "Content-Type: application/json" \
  -d '{"num_cells": 30, "cell_radius": 8, "num_filopodia": 5}'
```

Response:
```json
{"status": "initialized", "num_cells": 30}
```

### Advance one step

```bash
curl -X POST http://localhost:8001/api/simulation/step
```

Response: full state JSON with agents and environment data.

### Query current state (without advancing)

```bash
curl http://localhost:8001/api/simulation/state
```

### Start continuous simulation

```bash
curl -X POST http://localhost:8001/api/simulation/start
```

Note: continuous mode streams state over WebSocket, not REST. The `/start` endpoint only sets a flag; the actual streaming requires a WebSocket connection.

### Connect via WebSocket (using websocat)

```bash
# Install websocat: https://github.com/vi/websocat
websocat ws://localhost:8001/ws/simulation
```

Then type JSON commands:
```json
{"action": "init", "config": {"num_cells": 20}}
{"action": "start"}
{"action": "speed", "value": 100}
{"action": "stop"}
```

### Interactive API documentation

FastAPI automatically generates interactive documentation:

- **Swagger UI**: http://localhost:8001/docs -- try out endpoints directly in the browser
- **ReDoc**: http://localhost:8001/redoc -- read-only formatted documentation

---

## Troubleshooting

### Port already in use

**Symptom**: `ERROR: [Errno 10048] error while attempting to bind on address ('0.0.0.0', 8001)`

**Fix**: Another process is using port 8001. Either stop that process or choose a different port:
```bash
python -m uvicorn app.main:app --reload --port 8002
```

### Module not found errors

**Symptom**: `ModuleNotFoundError: No module named 'fastapi'` (or numpy, etc.)

**Fix**: Make sure your virtual environment is active. You should see `(.venv)` in your prompt. If not, activate it:
```bash
source .venv/Scripts/activate   # Windows Git Bash
source .venv/bin/activate       # macOS/Linux
```
Then reinstall dependencies: `pip install -r requirements.txt`

### WebSocket disconnects repeatedly

**Symptom**: The browser console shows "WebSocket connection closed" messages.

**Explanation**: The WebSocket client includes auto-reconnect logic with exponential backoff (up to 10 retries). If the server is restarted (e.g., due to `--reload` detecting a file change), the WebSocket drops and the client reconnects automatically. If reconnection fails after all retries, refresh the browser page.

### Cells pile up at boundaries

**Symptom**: All cells cluster against the EVL or DEB boundary line.

**Explanation**: This is expected behavior when the boundaries compress the cell region faster than cells can spread. To reduce clustering:
- Increase the environment width/height to give cells more room.
- Decrease the EVL velocity.
- Disable one or both boundaries to observe free-space behavior.
- Increase the velocity scale so cells migrate faster than the boundaries advance.

### Slow simulation with many cells

**Symptom**: FPS drops below 5 with 50+ cells.

**Explanation**: The collision resolution is O(N^2) in the number of active cells, and each cell's shape/velocity calculation iterates over all contour points. To improve performance:
- Reduce contour points to 40-60. This has minimal visual impact.
- Increase the speed slider (step interval) to allow more computation time per frame.
- Reduce the number of cells.

### Canvas is blank after initialization

**Symptom**: You click Initialize but the canvas stays black.

**Fix**: Check the browser console (F12 > Console) for JavaScript errors. Common causes:
- The server is not running (check terminal for Uvicorn output).
- A browser extension is blocking WebSocket connections.
- The browser does not support Canvas 2D (extremely rare with modern browsers).

### Tests fail with import errors

**Symptom**: `ImportError: attempted relative import with no known parent package`

**Fix**: Run tests from the project root directory, not from inside the `tests/` folder:
```bash
cd SCIAN_LEO_CPM
python tests/test_cell.py
```

Or use pytest which handles imports correctly:
```bash
python -m pytest tests/ -v
```
