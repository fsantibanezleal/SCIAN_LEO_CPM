"""
CPM Parameter Exploration Tutorial
===================================

This script demonstrates how to explore the CPM simulation parameters
programmatically. Run it to see how different settings affect cell
behavior, cluster formation, and migration patterns.

Usage:
    python notebooks/parameter_exploration.py
"""
import sys
import os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np
from app.simulation.cell import CellWM
from app.simulation.agents import AgentsSystem
from app.simulation.environment import EnvironmentSystem


def experiment_filopodia_count():
    """How does the number of filopodia affect cell shape and velocity?"""
    print("=" * 60)
    print("Experiment 1: Filopodia Count vs Cell Behavior")
    print("=" * 60)

    for n_filo in [1, 2, 4, 8]:
        cell = CellWM([100, 100], radius=10.0, num_filo=n_filo)
        speeds = []
        for _ in range(100):
            cell.simulation_step()
            speeds.append(np.linalg.norm(cell.velocity))

        avg_speed = np.mean(speeds)
        area = cell._current_area
        print(f"  Filopodia={n_filo}: avg_speed={avg_speed:.4f}, area={area:.1f}, "
              f"perimeter={cell._current_perimeter:.1f}")


def experiment_stiffness_gradient():
    """How does durotaxis strength affect migration direction?"""
    print("\n" + "=" * 60)
    print("Experiment 2: Durotaxis -- Stiffness Gradient Effect")
    print("=" * 60)

    for strength in [0.0, 0.001, 0.005, 0.01]:
        cell = CellWM([200, 200], radius=10.0, num_filo=4)
        gradient = np.array([strength * 100, 0.0]) if strength > 0 else None

        for _ in range(200):
            cell.simulation_step(mechanotaxis_gradient=gradient)

        displacement = cell.position - np.array([200, 200])
        print(f"  Gradient strength={strength:.3f}: "
              f"final_pos=({cell.position[0]:.1f}, {cell.position[1]:.1f}), "
              f"x_displacement={displacement[0]:.1f}")


def experiment_cluster_formation():
    """How does cell count affect cluster compactness?"""
    print("\n" + "=" * 60)
    print("Experiment 3: Cell Count vs Cluster Compactness")
    print("=" * 60)

    for n_cells in [5, 10, 20, 40]:
        config = {"num_filopodia": 4, "velocity_scale": 0.01}
        env = EnvironmentSystem({"width": 300, "height": 200,
                                 "evl_enabled": False, "deb_enabled": False})
        agents = AgentsSystem(config)
        agents.initialize(env, num_dfcs=n_cells, radius=8.0)

        for _ in range(50):
            agents.update(env)

        positions = np.array([c.position for c in agents.cells if c.active])
        centroid = positions.mean(axis=0)
        spread = np.sqrt(np.mean(np.sum((positions - centroid)**2, axis=1)))

        print(f"  Cells={n_cells}: spread={spread:.1f}, "
              f"centroid=({centroid[0]:.1f}, {centroid[1]:.1f})")


def experiment_evl_migration():
    """How does EVL velocity affect cluster descent?"""
    print("\n" + "=" * 60)
    print("Experiment 4: EVL Velocity vs Cluster Descent")
    print("=" * 60)

    for evl_vel in [0.5, 1.0, 2.0, 4.0]:
        config = {"num_filopodia": 4, "velocity_scale": 0.01}
        env = EnvironmentSystem({"evl_velocity": evl_vel, "deb_enabled": True})
        agents = AgentsSystem(config)
        agents.initialize(env, num_dfcs=15, radius=8.0)

        for _ in range(100):
            env.update()
            agents.update(env)

        positions = np.array([c.position for c in agents.cells if c.active])
        mean_y = positions[:, 1].mean()

        print(f"  EVL_velocity={evl_vel}: mean_y={mean_y:.1f}, "
              f"EVL_pos={env.evl_position:.1f}, DEB_pos={env.deb_position:.1f}")


if __name__ == "__main__":
    print("CPM Parameter Exploration")
    print("=" * 60)
    experiment_filopodia_count()
    experiment_stiffness_gradient()
    experiment_cluster_formation()
    experiment_evl_migration()
    print("\n" + "=" * 60)
    print("All experiments complete!")
