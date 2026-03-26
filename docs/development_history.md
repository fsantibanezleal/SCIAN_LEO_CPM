# Development History

## Project Origins

The SCIAN_LEO_CPM project originated in 2014 at the SCIAN-Lab (Scientific Image Analysis Laboratory), University of Chile. The initial goal was to build a computational tool for simulating how Dorsal Forerunner Cells migrate collectively during zebrafish embryonic development. Understanding this migration is important because these cells go on to form Kupffer's vesicle, the organ that establishes left-right asymmetry in the developing fish.

The original implementation was written in **C++/CLI** using **Visual Studio 2012** on Windows. It used **OpenGL** for rendering the cell shapes in real time and **Windows Forms** for the graphical user interface (parameter sliders, start/stop buttons, etc.). Geometric operations such as polygon intersection and union -- needed for collision detection and cell-cell contact area calculations -- were handled by the **Boost.Geometry** library.

This legacy codebase served the lab's research needs for nearly a decade, but it was tightly coupled to the Windows platform, required a specific Visual Studio version to compile, and was difficult for new team members to set up and modify.

## Migration to Python/FastAPI (2026)

In March 2026, the project was completely rewritten as a modern web application. The motivations for the rewrite were:

1. **Cross-platform access**: A web application runs on any operating system with a browser. No compilation, no IDE dependency, no platform lock-in.
2. **Easier onboarding**: Python is the dominant language in scientific computing. New lab members can read and modify the simulation code without learning C++/CLI or Windows Forms.
3. **Live collaboration**: Multiple people can open the same URL and observe the simulation in real time. The server can be run on a lab machine and accessed remotely.
4. **Modern tooling**: FastAPI provides automatic API documentation, type validation through Pydantic, and native WebSocket support. The frontend uses vanilla JavaScript with no build step.
5. **Testability**: The Python codebase includes unit tests for every major component, runnable with a single command.

The rewrite preserved the core mathematical model (Gaussian filopodia with elastic membrane relaxation) while simplifying the architecture and making it more accessible.

---

## Changelog

### v2.0.0 (2026-03-26)

**Complete rewrite from C++/CLI to Python/FastAPI web application.**

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

### v1.x (2014-2024) [Legacy]

The legacy codebase is preserved in the `legacy/` directory for reference.

- **v1.0 (2014)**: Initial C++/CLI application with OpenGL rendering and Windows Forms UI
  - Gaussian filopodia cell model with Boost.Geometry polygon operations
  - Basic collision detection using polygon intersection area
  - Single-threaded simulation loop with OpenGL draw calls
  - Hardcoded parameters, limited configurability

- **v1.1 (~2016)**: Added EVL boundary progression model
  - Stage-dependent adhesion rates from experimental data
  - EVL front moves across the simulation domain
  - DFC cells coupled to EVL through adhesion parameters

- **v1.2 (~2018)**: Improved collision handling
  - Replaced polygon-based overlap detection with center-distance-based check
  - Added proliferation mechanism with stage schedule
  - DEB boundary convergence from below

- **v1.3 (~2020)**: Usability improvements
  - More configurable parameters exposed through the Windows Forms UI
  - Adjusted default values based on experimental measurements
  - Minor performance optimizations in the rendering loop

- **v1.4 (~2022-2024)**: Final legacy updates
  - Bug fixes for boundary edge cases
  - Updated adhesion rate tables with new experimental data
  - Documentation of model parameters and equations

---

## Technology Migration Summary

| Aspect               | Legacy (v1.x)              | Current (v2.0)                    |
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
