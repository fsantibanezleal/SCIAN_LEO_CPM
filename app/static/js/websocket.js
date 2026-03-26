/**
 * WebSocket client for real-time simulation communication.
 *
 * Handles connection management, message routing, and auto-reconnection.
 * The client connects to the server's /ws/simulation endpoint and receives
 * simulation state updates as JSON messages. It can also send control
 * commands (start, stop, speed, init) to the server.
 */
class SimWebSocket {
    constructor() {
        this.ws = null;
        this.onStateUpdate = null;
        this.connected = false;
        this.reconnectAttempts = 0;
        this.maxReconnectAttempts = 10;
    }

    /**
     * Establish WebSocket connection to the server.
     * Automatically determines ws:// or wss:// based on page protocol.
     * Sets up event handlers for open, message, close, and error events.
     * On disconnection, attempts exponential-backoff reconnection.
     */
    connect() {
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const url = `${protocol}//${window.location.host}/ws/simulation`;

        this.ws = new WebSocket(url);

        this.ws.onopen = () => {
            console.log('WebSocket connected');
            this.connected = true;
            this.reconnectAttempts = 0;
        };

        this.ws.onmessage = (event) => {
            const data = JSON.parse(event.data);
            if (this.onStateUpdate) {
                this.onStateUpdate(data);
            }
        };

        this.ws.onclose = () => {
            this.connected = false;
            if (this.reconnectAttempts < this.maxReconnectAttempts) {
                this.reconnectAttempts++;
                setTimeout(() => this.connect(), 1000 * this.reconnectAttempts);
            }
        };

        this.ws.onerror = (err) => {
            console.error('WebSocket error:', err);
        };
    }

    /**
     * Send a command to the server.
     * @param {string} action - The action name (start, stop, speed, init).
     * @param {object} data - Additional data to include in the message.
     */
    send(action, data = {}) {
        if (this.connected && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(JSON.stringify({ action, ...data }));
        }
    }
}
