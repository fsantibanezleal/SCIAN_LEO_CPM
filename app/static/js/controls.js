/**
 * UI controls manager.
 *
 * Binds HTML input elements (sliders, checkboxes, buttons) to simulation
 * parameters and handles user interaction events. Provides a getConfig()
 * method that returns the current parameter values as a configuration
 * object suitable for the /api/simulation/init endpoint.
 */
class SimControls {
    constructor() {
        /** @type {Object<string, HTMLInputElement>} Slider elements by id */
        this.sliders = {};
        /** @type {Object<string, HTMLInputElement>} Checkbox elements by id */
        this.checkboxes = {};
        /** @type {Object<string, Function>} Event callbacks */
        this.callbacks = {};
        this._bindSliders();
        this._bindCheckboxes();
    }

    /**
     * Bind all range slider inputs to their display labels.
     * Updates the label text in real-time as the slider moves.
     */
    _bindSliders() {
        const sliderDefs = [
            ['numCells', 'valNumCells'],
            ['cellRadius', 'valRadius'],
            ['numFilo', 'valFilo'],
            ['numContour', 'valContour'],
            ['velocityScale', 'valVelocity'],
            ['envWidth', 'valWidth'],
            ['envHeight', 'valHeight'],
            ['evlVelocity', 'valEVL'],
            ['simSpeed', 'valSpeed'],
        ];

        sliderDefs.forEach(([sliderId, labelId]) => {
            const slider = document.getElementById(sliderId);
            const label = document.getElementById(labelId);
            if (slider && label) {
                this.sliders[sliderId] = slider;
                slider.addEventListener('input', () => {
                    label.textContent = slider.value;
                    if (this.callbacks.paramChange) {
                        this.callbacks.paramChange(sliderId, parseFloat(slider.value));
                    }
                });
            }
        });
    }

    /**
     * Bind all checkbox inputs and fire change events.
     */
    _bindCheckboxes() {
        ['evlEnabled', 'debEnabled', 'prolifEnabled',
         'showContours', 'showCenters', 'showBoundaries', 'showGrid'].forEach(id => {
            const cb = document.getElementById(id);
            if (cb) {
                this.checkboxes[id] = cb;
                cb.addEventListener('change', () => {
                    if (this.callbacks.paramChange) {
                        this.callbacks.paramChange(id, cb.checked);
                    }
                });
            }
        });
    }

    /**
     * Collect current values from all controls into a configuration object.
     * @returns {object} Configuration suitable for SimulationConfig on the server.
     */
    getConfig() {
        return {
            width: parseInt(this.sliders.envWidth?.value || 400),
            height: parseInt(this.sliders.envHeight?.value || 350),
            num_cells: parseInt(this.sliders.numCells?.value || 24),
            cell_radius: parseFloat(this.sliders.cellRadius?.value || 10),
            num_filopodia: parseInt(this.sliders.numFilo?.value || 4),
            num_contour_points: parseInt(this.sliders.numContour?.value || 100),
            velocity_scale: parseFloat(this.sliders.velocityScale?.value || 0.01),
            evl_enabled: this.checkboxes.evlEnabled?.checked ?? true,
            evl_velocity: parseFloat(this.sliders.evlVelocity?.value || 1.5),
            deb_enabled: this.checkboxes.debEnabled?.checked ?? true,
            proliferation_enabled: this.checkboxes.prolifEnabled?.checked ?? false,
            speed_ms: parseInt(this.sliders.simSpeed?.value || 50),
        };
    }

    /**
     * Register a callback for parameter changes.
     * @param {Function} cb - Callback(id, value) called on any control change.
     */
    onParamChange(cb) { this.callbacks.paramChange = cb; }

    /**
     * Update button enabled/disabled states based on simulation status.
     * @param {boolean} initialized - Whether the simulation has been initialized.
     * @param {boolean} running - Whether the simulation is currently running.
     */
    setButtonStates(initialized, running) {
        document.getElementById('btnInit').disabled = running;
        document.getElementById('btnStart').disabled = !initialized || running;
        document.getElementById('btnStop').disabled = !running;
        document.getElementById('btnStep').disabled = !initialized || running;
    }
}
