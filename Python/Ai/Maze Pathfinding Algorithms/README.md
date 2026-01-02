# Maze Pathfinding Algorithms in Python

This Python project implements three classic pathfinding algorithms — **Breadth-First Search (BFS)**, **Depth-First Search (DFS)**, and **A\* (A-Star)** to find a path through a 2D maze. It also measures the **peak memory usage** of each algorithm using Python’s `tracemalloc` module.

---

## Features

- Solves a hardcoded maze using:
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - A* (A-Star) Search with Manhattan distance
- Tracks and prints:
  - Path length
  - Peak memory usage (in bytes)
- Visually prints the maze with the path marked using `*`

---

## What I Learned

- Implementing **graph traversal algorithms**: BFS, DFS, and A*
- Using **priority queues** with `heapq` for efficient A* search
- Calculating **Manhattan distance** as a heuristic function in A*
- Tracking **memory usage** with `tracemalloc` for performance analysis

- Visualizing paths in a **2D grid-based maze** using nested lists and coordinate systems

