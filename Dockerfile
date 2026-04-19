# SCIAN_LEO_CPM - Cellular Potts Model simulator
#
# Multi-stage-free slim image: Python 3.12 + uvicorn serving FastAPI on :8001.
# The image embeds only the runtime surface (app/, requirements.txt); legacy/,
# tests/, notebooks/, and docs/ are excluded via .dockerignore.
#
# Build:
#   docker build -t scian-leo-cpm:latest .
# Run:
#   docker run --rm -p 8001:8001 scian-leo-cpm:latest
# Then open http://localhost:8001 in a browser.
#
# Health check:
#   curl -f http://localhost:8001/api/health

FROM python:3.12-slim

# Avoid interactive prompts, keep image small, disable pip cache.
ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PIP_NO_CACHE_DIR=1 \
    PIP_DISABLE_PIP_VERSION_CHECK=1

WORKDIR /app

# Install dependencies first so this layer is cached when source code changes.
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy application source. .dockerignore prunes legacy/, tests/, notebooks/.
COPY app ./app
COPY run_app.py ./

EXPOSE 8001

# Container-level health probe hits the /api/health route added in PR #43/#44.
HEALTHCHECK --interval=30s --timeout=5s --start-period=10s --retries=3 \
  CMD python -c "import urllib.request,sys; \
sys.exit(0 if urllib.request.urlopen('http://127.0.0.1:8001/api/health', timeout=3).status == 200 else 1)"

# Single worker because simulation_state is an in-process singleton.
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8001", "--workers", "1"]
