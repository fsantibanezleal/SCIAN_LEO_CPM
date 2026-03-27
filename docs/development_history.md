# Development History

## v2.2.0 (2026-03-26) -- Physics Rewrite

### Critical Fix: Velocity Model

**Problem**: The v2.0 velocity model used centroid offset:
```
v = V_0 * (centroid_contour - center)
```
For a cell with N filopodia roughly distributed around the boundary, the contour centroid is nearly identical to the center, producing near-zero velocity regardless of filopodial activity.

**Solution**: Filopodia-weighted propulsion model:
```
v = V₀ · Σⱼ Aⱼ · (cos θⱼ, sin θⱼ)
```
Each filopodium contributes a traction force proportional to its amplitude, directed radially outward. This correctly produces directed motion when filopodia cluster on one side.

**Physical basis**: Actin polymerization at filopodial tips generates protrusive forces. The net force is the vector sum of all protrusions, weighted by their extension length.

### Critical Fix: Durotaxis (Mechanotaxis)

**Problem**: v2.0 added a constant force vector to cell velocity:
```
v_final = v_cell + F_constant
```
This is NOT durotaxis. Real mechanotaxis involves cells sensing substrate stiffness through their filopodia and preferentially extending in the stiffer direction.

**Solution**: Sinusoidal durotactic torque on filopodium angles:
```
Δθⱼ = α · |∇E| · sin(θ_gradient - θⱼ)
```
where ∇E is the stiffness gradient direction. The sin() creates a restoring torque that rotates filopodia toward stiffer substrate. Filopodia already aligned with the gradient feel no torque; perpendicular ones feel maximum torque.

**Physical basis**: Focal adhesions on stiffer substrates mature faster (Rens & Merks, iScience 2020), reinforcing filopodia that point toward stiff regions and causing retraction of filopodia pointing toward soft regions.

### Fix: Viscoelastic Membrane Relaxation

**Problem**: v2.0 applied per-filopodium damping coefficients gamma_j to individual filopodial contributions, then took the maximum. This mixed damping rates across the contour in a physically inconsistent way.

**Solution**: Single effective damping rate applied to the full envelope:
```
γ_eff = mean(γⱼ)
R(θ,t+Δt) = R(θ,t) + γ_eff · (R_ideal(θ) - R(θ,t))
```
The ideal shape is the Gaussian envelope; the membrane relaxes toward it as a single viscoelastic material.

### Fix: Cell Division (Hertwig's Rule)

**Problem**: v2.0 spawned new cells at random positions with no relation to parent cells.

**Solution**: Division along the longest axis (Hertwig's rule):
1. Select a cell probabilistically (rate depends on proliferation stage)
2. Division axis = direction of the strongest filopodium
3. Two daughters placed at +/-0.6*R_0 along the division axis
4. Daughter radius = 0.85*R_0 (conserves approximate area)
5. Mother cell deactivated

### New: Cell-Cell Adhesion

Added pairwise differential adhesion (Steinberg, 1963):
```
F_adh = s · (d - d_contact) / (d_max - d_contact) · d_hat
```
for d_contact < d < d_max. Creates cohesive cell clusters matching biological DFC behavior.

### Nomenclature Clarification

Added explicit documentation that this project is NOT a classical Cellular Potts Model (lattice-based, Monte Carlo, Hamiltonian-driven). It is a geometric agent-based model with prescribed Gaussian filopodia shapes. The CPM name is retained as the project identifier following the original research group's convention.

### Architecture Diagram

> See `docs/diagrams/simulation_flow.svg` for visual reference.

```
+-----------------------------------------------------+
|  For each active cell:                              |
|    1. update_angles (random walk + durotaxis)       |
|    2. update_shape  (viscoelastic relaxation)       |
|    3. estimate_velocity (filopodial propulsion)     |
|    4. update_position (Euler integration)           |
|    5. compute_energy (Hamiltonian monitoring)       |
+-----------------------------------------------------+
+-----------------------------------------------------+
|  Collective interactions:                           |
|    6. Resolve collisions (two-pass: soft + hard)    |
|    7. Apply adhesion (differential adhesion)        |
|    8. Enforce boundaries (walls, EVL, DEB)          |
|    9. Check division (Hertwig's rule)               |
+-----------------------------------------------------+
```

---

## v2.1.0 (2026-03-26)

### Algorithmic Improvements

#### Vectorized Computation Engine
All cell operations use NumPy broadcasting instead of Python loops.
Contour generation computes all filopodia at all angles simultaneously:

```
Δθ_{ij} = wrap(θ_i - θ_{0,j})
R_{ij} = R_0 + A_j * exp(-Δθ_{ij}² / (2 * W_j²))
R(θ_i) = max_j(R_{ij})
```

#### Hamiltonian Energy Model
Shape energy functional for each cell:

```
H = λ_A(A-A₀)² + λ_P(P-P₀)²
```

Area via shoelace formula, perimeter via segment summation.

#### Gaussian Filopodial Envelope (Core Model)
Each cell's radial contour is defined by the maximum of Gaussian bumps:
```
R(θ) = max_j{R₀ + Aⱼ·exp(-(θ-θⱼ)²/(2Wⱼ²))}
```
where Aⱼ is the amplitude, θⱼ the angular position, and Wⱼ the width of filopodium j.

#### Mechanotaxis (Durotaxis)
Directional bias from substrate stiffness gradient:

```
F_mechano = s * d_hat
```

#### Persistent Random Walk
Filopodia maintain direction for τ ∈ [5, 20] steps:

```
Δθ_j = (π/4) * σ_j * ξ + 0.05 * wrap(θ_pref - θ_j)
```

#### Improved Collision Detection
Uses actual contour extent (max radial reach) instead of base radius.

#### Centroid-Based Velocity
```
v = V₀ · (centroid(contour) - center)
```

### Frontend Improvements
- Help modal with usage guide
- Tooltips on all controls
- Energy display in status bar
- Mechanotaxis controls and direction indicator
- Section-level expandable help text

---

## v2.0.0 (2026-03-26)

**Complete rewrite from C++/CLI to Python/FastAPI web application.**

### Core Gaussian Filopodial Model

The cell shape is defined by a radial envelope of Gaussian filopodia:
```
R(θ) = max_j{R₀ + Aⱼ·exp(-(θ-θⱼ)²/(2Wⱼ²))}
```

> See `docs/diagrams/cell_model.svg` and `docs/diagrams/architecture.svg` for visual reference.

### Features

- Rebuilt the simulation engine in Python with NumPy for numerical operations
- Implemented FastAPI backend with REST endpoints and WebSocket streaming
- Created browser-based frontend using HTML5 Canvas 2D rendering
- Designed modern dark-themed UI with interactive sliders, buttons, and checkboxes
- Added bidirectional WebSocket communication for real-time simulation control
- Implemented two-pass collision resolution algorithm (forward soft + backward hard)
- Added configurable EVL boundary with 6-stage progression and adhesion model
- Added DEB boundary with upward convergence dynamics
- Implemented optional cell proliferation with 8-stage schedule
- Built comprehensive test suite (cell, agents, simulation integration tests)
- Added automatic API documentation via Swagger UI and ReDoc
- Created SVG diagrams for architecture, cell model, simulation flow, and biological context
- Wrote exhaustive documentation covering architecture, biological model, user guide, and references

---

## v1.x (2014-2024) [Legacy]

The legacy codebase is preserved in the `legacy/` directory for reference.

- **v1.4 (~2022-2024)**: Final legacy updates
  - Bug fixes for boundary edge cases
  - Updated adhesion rate tables with new experimental data
  - Documentation of model parameters and equations

- **v1.3 (~2020)**: Usability improvements
  - More configurable parameters exposed through the Windows Forms UI
  - Adjusted default values based on experimental measurements
  - Minor performance optimizations in the rendering loop

- **v1.2 (~2018)**: Improved collision handling
  - Replaced polygon-based overlap detection with center-distance-based check
  - Added proliferation mechanism with stage schedule
  - DEB boundary convergence from below

- **v1.1 (~2016)**: Added EVL boundary progression model
  - Stage-dependent adhesion rates from experimental data
  - EVL front moves across the simulation domain
  - DFC cells coupled to EVL through adhesion parameters

- **v1.0 (2014)**: Initial C++/CLI application with OpenGL rendering and Windows Forms UI
  - Gaussian filopodia cell model with Boost.Geometry polygon operations
  - Basic collision detection using polygon intersection area
  - Single-threaded simulation loop with OpenGL draw calls
  - Hardcoded parameters, limited configurability

---

## Project Origins

The SCIAN_LEO_CPM project originated in 2014 at the SCIAN-Lab (Scientific Image Analysis Laboratory), University of Chile. The initial goal was to build a computational tool for simulating how Dorsal Forerunner Cells migrate collectively during zebrafish embryonic development. Understanding this migration is important because these cells go on to form Kupffer's vesicle, the organ that establishes left-right asymmetry in the developing fish.

The original implementation was written in **C++/CLI** using **Visual Studio 2012** on Windows. It used **OpenGL** for rendering the cell shapes in real time and **Windows Forms** for the graphical user interface (parameter sliders, start/stop buttons, etc.). Geometric operations such as polygon intersection and union -- needed for collision detection and cell-cell contact area calculations -- were handled by the **Boost.Geometry** library.

This legacy codebase served the lab's research needs for nearly a decade, but it was tightly coupled to the Windows platform, required a specific Visual Studio version to compile, and was difficult for new team members to set up and modify.

---

## Technology Migration Summary

| Aspect               | Legacy (v1.x)              | Current (v2.x)                    |
|----------------------|----------------------------|-----------------------------------|
| Language             | C++/CLI                    | Python 3.10+                      |
| Build system         | Visual Studio 2012         | pip + requirements.txt            |
| UI framework         | Windows Forms              | HTML5 + vanilla JavaScript        |
| Rendering            | OpenGL                     | HTML5 Canvas 2D                   |
| Geometry library     | Boost.Geometry             | NumPy (direct computation)        |
| Network              | None (desktop only)        | FastAPI + WebSocket               |
| API documentation    | None                       | Swagger UI + ReDoc (automatic)    |
| Platform             | Windows only               | Any OS with Python and a browser  |
| Tests                | None                       | pytest-compatible test suite      |
| Configuration        | Hardcoded + Windows Forms  | Pydantic models + REST API        |
