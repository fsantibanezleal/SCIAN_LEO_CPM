"""
System health endpoints for liveness and deployment probes.

Provides a cheap GET /api/health suitable for cPanel, Docker, and reverse-
proxy health checks. Does not touch the simulation lock nor advance state,
so it is safe to hit at high frequency.
"""

from fastapi import APIRouter

from .. import __version__

router = APIRouter(tags=["System"])


@router.get("/api/health")
async def health() -> dict:
    """Liveness probe for load balancers and orchestrators.

    Returns a small JSON payload indicating that the ASGI process is
    responsive and whether a simulation has been initialized. Never
    modifies simulation state.

    Returns:
        Dictionary with keys:
            status: always ``"ok"`` when the handler executes.
            version: application version from ``app.__version__``.
            sim_initialized: ``True`` if ``/api/simulation/init`` has run.
    """
    # Import lazily to avoid circular import with app.main at startup
    from ..main import simulation_state

    return {
        "status": "ok",
        "version": __version__,
        "sim_initialized": simulation_state.get("agents") is not None,
    }
