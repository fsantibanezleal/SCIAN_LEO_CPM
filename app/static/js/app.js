/**
 * Main application entry point.
 *
 * Orchestrates the WebSocket connection, Canvas 2D renderer, and UI controls.
 * Handles initialization, start/stop, single-step, and display option toggling.
 * Uses an IIFE to avoid polluting the global namespace.
 */
(function() {
    const ws = new SimWebSocket();
    const renderer = new SimRenderer('simCanvas');
    const controls = new SimControls();

    let initialized = false;
    let running = false;
    let lastFrameTime = performance.now();
    let frameCount = 0;
    let fps = 0;

    // Connect WebSocket
    ws.connect();

    // Handle state updates streamed from the server via WebSocket
    ws.onStateUpdate = (state) => {
        renderer.render(state);

        // Update info displays
        document.getElementById('stepCount').textContent =
            `Step: ${state.agents?.step || 0}`;
        document.getElementById('cellCount').textContent =
            `Cells: ${state.agents?.num_cells || 0}`;

        // FPS counter (updates once per second)
        frameCount++;
        const now = performance.now();
        if (now - lastFrameTime >= 1000) {
            fps = frameCount;
            frameCount = 0;
            lastFrameTime = now;
            document.getElementById('fpsCount').textContent = `FPS: ${fps}`;
        }
    };

    // ── Button handlers ─────────────────────────────────────────────────────

    document.getElementById('btnInit').addEventListener('click', async () => {
        const config = controls.getConfig();
        const resp = await fetch('/api/simulation/init', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(config),
        });
        const result = await resp.json();
        if (result.status === 'initialized') {
            initialized = true;
            running = false;
            controls.setButtonStates(initialized, running);

            // Fetch and render initial state
            const stateResp = await fetch('/api/simulation/state');
            const state = await stateResp.json();
            renderer.render(state);

            document.getElementById('stepCount').textContent = 'Step: 0';
            document.getElementById('cellCount').textContent =
                `Cells: ${result.num_cells}`;
        }
    });

    document.getElementById('btnStart').addEventListener('click', () => {
        ws.send('start');
        running = true;
        controls.setButtonStates(initialized, running);
    });

    document.getElementById('btnStop').addEventListener('click', () => {
        ws.send('stop');
        running = false;
        controls.setButtonStates(initialized, running);
    });

    document.getElementById('btnStep').addEventListener('click', async () => {
        const resp = await fetch('/api/simulation/step', { method: 'POST' });
        const state = await resp.json();
        renderer.render(state);
        document.getElementById('stepCount').textContent =
            `Step: ${state.agents?.step || 0}`;
    });

    // ── Display option changes ──────────────────────────────────────────────

    controls.onParamChange((id, value) => {
        if (id === 'showContours') renderer.showContours = value;
        if (id === 'showCenters') renderer.showCenters = value;
        if (id === 'showBoundaries') renderer.showBoundaries = value;
        if (id === 'showGrid') renderer.showGrid = value;
        if (id === 'simSpeed') ws.send('speed', { value: value });
    });

    // Set initial button states (nothing initialized yet)
    controls.setButtonStates(false, false);
})();
