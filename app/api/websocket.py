"""
WebSocket handler for real-time simulation streaming.

Manages bidirectional communication between the browser client and the
simulation engine. The server pushes simulation state updates at the
configured rate, and accepts control commands (start, stop, speed) from
the client.

Protocol:
    Client -> Server (JSON):
        {"action": "start"}
        {"action": "stop"}
        {"action": "speed", "value": <int>}
        {"action": "init", "config": {...}}

    Server -> Client (JSON):
        {"agents": {...}, "environment": {...}}
"""

# WebSocket handler is implemented directly in main.py for simplicity.
# This module documents the protocol and can be extended for more complex
# message routing if needed.
