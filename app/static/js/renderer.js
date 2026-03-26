/**
 * Canvas 2D renderer for CPM simulation.
 *
 * Renders cell contours, centers, environment boundaries, and grid
 * on an HTML5 Canvas element. Automatically scales the simulation
 * coordinate system to fit the canvas dimensions.
 *
 * Color scheme:
 *   - Cells: hue-rotated contours for visual distinction between cells
 *   - Cell centers: yellow dots
 *   - EVL boundary: red dashed line
 *   - DEB boundary: green dashed line
 *   - DFC region: blue semi-transparent rectangle
 */
class SimRenderer {
    /**
     * @param {string} canvasId - DOM id of the canvas element.
     */
    constructor(canvasId) {
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d');
        this.scale = 1;
        this.offsetX = 0;
        this.offsetY = 0;

        // Display options (toggled by controls)
        this.showContours = true;
        this.showCenters = true;
        this.showBoundaries = true;
        this.showGrid = false;

        // Color palette
        this.colors = {
            background: '#0a0a1a',
            cell: 'rgba(78, 204, 163, 0.8)',
            cellFill: 'rgba(78, 204, 163, 0.08)',
            center: '#ffd700',
            evl: '#e94560',
            deb: '#4ecca3',
            dfcRegion: 'rgba(15, 52, 96, 0.4)',
            grid: 'rgba(255, 255, 255, 0.05)',
        };

        this._setupResize();
    }

    /**
     * Set up automatic canvas resizing on window resize events.
     * The canvas fills its parent container minus the info bar height.
     */
    _setupResize() {
        const resize = () => {
            const rect = this.canvas.parentElement.getBoundingClientRect();
            this.canvas.width = rect.width;
            this.canvas.height = rect.height - 30;
        };
        window.addEventListener('resize', resize);
        resize();
    }

    /**
     * Render a complete simulation state frame.
     * @param {object} state - State object with 'agents' and 'environment' keys.
     */
    render(state) {
        if (!state || !state.agents) return;

        const { agents, environment } = state;
        const ctx = this.ctx;
        const env = environment;

        // Calculate scale to fit environment in canvas with padding
        const scaleX = (this.canvas.width - 40) / env.width;
        const scaleY = (this.canvas.height - 40) / env.height;
        this.scale = Math.min(scaleX, scaleY);
        this.offsetX = (this.canvas.width - env.width * this.scale) / 2;
        this.offsetY = (this.canvas.height - env.height * this.scale) / 2;

        // Clear canvas
        ctx.fillStyle = this.colors.background;
        ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

        // Draw layers back-to-front
        if (this.showGrid) this._drawGrid(env);

        if (this.showBoundaries) {
            this._drawDFCRegion(env);
            this._drawBoundaries(env);
        }

        // Draw cells
        if (agents.cells) {
            agents.cells.forEach((cell, i) => {
                if (this.showContours) this._drawCell(cell, i);
                if (this.showCenters) this._drawCenter(cell);
            });
        }
    }

    /**
     * Convert simulation coordinates to screen (canvas) coordinates.
     * @param {number} x - Simulation x coordinate.
     * @param {number} y - Simulation y coordinate.
     * @returns {number[]} Screen [x, y] coordinates.
     */
    _toScreen(x, y) {
        return [
            this.offsetX + x * this.scale,
            this.offsetY + y * this.scale
        ];
    }

    /**
     * Draw a single cell contour as a filled and stroked polygon.
     * Each cell gets a unique hue based on its index for visual distinction.
     * @param {object} cell - Cell state with contour array.
     * @param {number} index - Cell index for color assignment.
     */
    _drawCell(cell, index) {
        const ctx = this.ctx;
        const contour = cell.contour;
        if (!contour || contour.length < 2) return;

        // Hue varies by cell index for visual distinction
        const hue = (index * 37) % 360;
        ctx.strokeStyle = `hsla(${hue}, 70%, 60%, 0.8)`;
        ctx.fillStyle = `hsla(${hue}, 70%, 60%, 0.06)`;
        ctx.lineWidth = 1.5;

        ctx.beginPath();
        const [sx, sy] = this._toScreen(contour[0][0], contour[0][1]);
        ctx.moveTo(sx, sy);

        for (let i = 1; i < contour.length; i++) {
            const [px, py] = this._toScreen(contour[i][0], contour[i][1]);
            ctx.lineTo(px, py);
        }
        ctx.closePath();
        ctx.fill();
        ctx.stroke();
    }

    /**
     * Draw a cell center marker (small yellow dot).
     * @param {object} cell - Cell state with position array.
     */
    _drawCenter(cell) {
        const ctx = this.ctx;
        const [cx, cy] = this._toScreen(cell.position[0], cell.position[1]);

        ctx.fillStyle = this.colors.center;
        ctx.beginPath();
        ctx.arc(cx, cy, 2.5, 0, Math.PI * 2);
        ctx.fill();
    }

    /**
     * Draw EVL and DEB boundary lines as colored dashed lines.
     * @param {object} env - Environment state.
     */
    _drawBoundaries(env) {
        const ctx = this.ctx;
        ctx.setLineDash([6, 4]);
        ctx.lineWidth = 2;

        // EVL boundary (red, from top moving down)
        if (env.evl_position > 0) {
            const [x1, y1] = this._toScreen(0, env.evl_position);
            const [x2] = this._toScreen(env.width, 0);
            ctx.strokeStyle = this.colors.evl;
            ctx.beginPath();
            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y1);
            ctx.stroke();

            ctx.fillStyle = this.colors.evl;
            ctx.font = '11px monospace';
            ctx.fillText(`EVL (stage ${env.evl_stage})`, x1 + 4, y1 - 5);
        }

        // DEB boundary (green, from bottom moving up)
        if (env.deb_position < env.height) {
            const [x1, y1] = this._toScreen(0, env.deb_position);
            const [x2] = this._toScreen(env.width, 0);
            ctx.strokeStyle = this.colors.deb;
            ctx.beginPath();
            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y1);
            ctx.stroke();

            ctx.fillStyle = this.colors.deb;
            ctx.fillText('DEB margin', x1 + 4, y1 + 14);
        }

        ctx.setLineDash([]);
    }

    /**
     * Draw the DFC initialization region as a semi-transparent rectangle.
     * @param {object} env - Environment state with dfc_region bounds.
     */
    _drawDFCRegion(env) {
        const ctx = this.ctx;
        const [x1, y1] = this._toScreen(env.dfc_region[0], env.dfc_region[1]);
        const [x2, y2] = this._toScreen(env.dfc_region[2], env.dfc_region[3]);

        ctx.fillStyle = this.colors.dfcRegion;
        ctx.fillRect(x1, y1, x2 - x1, y2 - y1);

        ctx.strokeStyle = 'rgba(15, 52, 96, 0.6)';
        ctx.lineWidth = 1;
        ctx.setLineDash([3, 3]);
        ctx.strokeRect(x1, y1, x2 - x1, y2 - y1);
        ctx.setLineDash([]);

        ctx.fillStyle = 'rgba(15, 52, 96, 0.8)';
        ctx.font = '10px monospace';
        ctx.fillText('DFC Region', x1 + 4, y1 + 12);
    }

    /**
     * Draw a background grid at 50-unit intervals.
     * @param {object} env - Environment state with width/height.
     */
    _drawGrid(env) {
        const ctx = this.ctx;
        ctx.strokeStyle = this.colors.grid;
        ctx.lineWidth = 0.5;

        const step = 50;
        for (let x = 0; x <= env.width; x += step) {
            const [sx, sy] = this._toScreen(x, 0);
            const [, ey] = this._toScreen(0, env.height);
            ctx.beginPath();
            ctx.moveTo(sx, sy);
            ctx.lineTo(sx, ey);
            ctx.stroke();
        }
        for (let y = 0; y <= env.height; y += step) {
            const [sx, sy] = this._toScreen(0, y);
            const [ex] = this._toScreen(env.width, 0);
            ctx.beginPath();
            ctx.moveTo(sx, sy);
            ctx.lineTo(ex, sy);
            ctx.stroke();
        }
    }
}
