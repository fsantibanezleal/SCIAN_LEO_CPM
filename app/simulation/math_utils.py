"""
Vector math and utility functions for the CPM simulation.

Provides helper functions for 2D vector operations, angle normalization,
and distance calculations used throughout the simulation engine.
"""

import numpy as np


def normalize_angle(angle):
    """Normalize an angle to the range [-pi, pi].

    Args:
        angle: Angle in radians (any value).

    Returns:
        Equivalent angle in [-pi, pi].
    """
    return ((angle + np.pi) % (2 * np.pi)) - np.pi


def vec2_length(v):
    """Compute the Euclidean length of a 2D vector.

    Args:
        v: Array-like of length 2 representing [x, y].

    Returns:
        Scalar length sqrt(x^2 + y^2).
    """
    return np.sqrt(v[0] ** 2 + v[1] ** 2)


def vec2_normalize(v):
    """Return a unit vector in the direction of v.

    Args:
        v: Array-like of length 2 representing [x, y].

    Returns:
        Unit vector [x/|v|, y/|v|], or [0, 0] if |v| < 1e-10.
    """
    length = vec2_length(v)
    if length < 1e-10:
        return np.zeros(2, dtype=np.float64)
    return np.array(v, dtype=np.float64) / length


def vec2_distance(a, b):
    """Compute the Euclidean distance between two 2D points.

    Args:
        a: First point [x1, y1].
        b: Second point [x2, y2].

    Returns:
        Scalar distance between a and b.
    """
    return np.sqrt((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2)


def gaussian(x, mu, sigma):
    """Evaluate a normalized Gaussian function.

    Args:
        x: Evaluation point(s).
        mu: Mean (center) of the Gaussian.
        sigma: Standard deviation (width).

    Returns:
        Gaussian value(s) exp(-(x - mu)^2 / (2 * sigma^2)).
    """
    return np.exp(-((x - mu) ** 2) / (2 * sigma**2 + 1e-10))


def polar_to_cartesian(r, theta, center=None):
    """Convert polar coordinates to Cartesian.

    Args:
        r: Radial distance.
        theta: Angle in radians.
        center: Optional [x, y] center offset. Defaults to origin.

    Returns:
        Array [x, y] in Cartesian coordinates.
    """
    if center is None:
        center = np.zeros(2)
    return np.array(
        [center[0] + r * np.cos(theta), center[1] + r * np.sin(theta)]
    )
