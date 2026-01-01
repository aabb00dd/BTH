# Genetic Algorithm Maze Solver

This project implements a **genetic algorithm** to solve a complex maze by evolving a population of movement sequences. The objective is to guide an agent from a start point to a goal within a maze, avoiding walls and dead ends. The fitness of each path is based on how close it gets to the goal, penalizing collisions and revisits.

---

## Algorithm Overview

This project uses a **genetic algorithm** to evolve valid paths through the maze:

- **Moves:** `['U', 'D', 'L', 'R']`  
- **Chromosome:** A list of moves (e.g., `['D', 'D', 'R', 'U']`)
- **Population Size:** 500 individuals  
- **Mutation Rate:** 60%  
- **Generations:** 1000  
- **Fitness:** Based on proximity to the goal, with penalties for walls, backtracking, and rewards for reaching the goal.

---

## Features

- Fitness-based pathfinding
- Elitism and tournament selection
- Crossover and mutation for genetic diversity
- Manhattan distance as heuristic
- Real-time progress output in console
- Maze and best path visualization using `matplotlib`
