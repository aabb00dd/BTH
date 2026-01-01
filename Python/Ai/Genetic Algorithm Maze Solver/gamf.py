# import necessary libraries
import numpy as np
import random
import matplotlib.pyplot as plt


# representing the mazes layout 1000: wall, 1: path
maze = [
    [1,1,1,1,1,1,1,1,1,1,1,1,1,1000,1,1,1,1,1,1,1,1,1,1,1000,1,1,1,1,1,1,1,1000,1,1,1,1,1],
    [1000,1000,1000,1000,1000,1000,1,1000,1000,1000,1000,1000,1,1000,1000,1000,1000,1000,1000,1,1000,1000,1000,1,1000,1000,1000,1000,1000,1000,1000,1,1000,1,1000,1,1000,1000],
    [1,1,1,1,1,1,1,1000,1,1,1,1000,1,1,1,1000,1,1,1,1,1000,1,1000,1,1,1,1,1000,1,1,1,1,1000,1,1000,1,1,1],
    [1,1000,1000,1000,1000,1000,1000,1000,1000,1000,1,1000,1000,1000,1,1000,1,1000,1000,1000,1000,1,1000,1000,1000,1000,1,1000,1,1000,1000,1000,1000,1,1000,1000,1000,1],
    [1,1,1,1,1,1000,1,1,1,1,1,1,1,1000,1,1,1,1000,1,1,1,1,1,1,1,1,1,1000,1,1,1,1,1,1,1,1,1000,1],
    [1000,1000,1000,1000,1,1000,1000,1000,1,1000,1000,1000,1000,1000,1000,1000,1000,1000,1,1000,1000,1000,1000,1000,1000,1000,1,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1],
    [1,1,1,1000,1,1,1,1000,1,1,1,1,1,1,1,1,1,1000,1,1000,1,1,1,1,1,1000,1,1,1,1,1,1,1,1,1,1,1,1],
    [1,1000,1,1000,1000,1000,1,1000,1000,1000,1000,1000,1000,1000,1000,1000,1,1000,1,1000,1,1000,1000,1000,1,1000,1000,1000,1000,1000,1000,1000,1000,1,1000,1000,1000,1000],
    [1,1000,1,1,1,1,1,1000,1,1,1,1000,1,1,1,1000,1,1000,1,1000,1,1000,1,1,1,1000,1,1,1,1000,1,1,1,1,1000,1,1,1],
    [1,1000,1000,1000,1000,1000,1000,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1,1000,1000,1,1000,1,1000,1],
    [1,1000,1,1,1,1,1,1000,1,1000,1,1,1,1000,1,1000,1,1,1,1000,1,1000,1,1000,1,1,1,1000,1,1000,1,1,1000,1,1,1,1000,1],
    [1,1000,1,1000,1000,1000,1,1000,1,1000,1000,1000,1000,1000,1,1000,1000,1000,1000,1000,1,1000,1,1000,1000,1000,1000,1000,1,1000,1000,1000,1000,1,1000,1000,1000,1],
    [1,1000,1,1,1,1000,1,1,1,1000,1,1,1,1,1,1,1,1,1,1,1,1000,1,1,1,1,1,1000,1,1,1,1,1,1,1000,1,1,1]
]

# define start and end positions
start = (0, 0)
end = (12, 35)

# define moves
moves = ['U', 'D', 'L', 'R']

# genetic algorithm parameters
elitism_rate = 0.2
population_size = 500
mutation_rate = 0.6
generations = 1000
path_length = 80


# helper function to calculate manhattan distance
def manhattan_distance(x1, y1, x2, y2):
    return abs(x1 - x2) + abs(y1 - y2)

# fitness function to evaluate a path
def fitness(path):
    x, y = start
    visited = set()
    score = 0
    for move in path:
        # adjust the coordinates based on the current move
        if move == 'U': x -= 1
        elif move == 'D': x += 1
        elif move == 'L': y -= 1
        elif move == 'R': y += 1
        
        # check if the position has been visited before
        if (x, y) in visited:
            score -= 150 # penalize revisiting the same position
        visited.add((x, y))
        
        # check for walls or out of bounds moves
        if x < 0 or y < 0 or x >= len(maze) or y >= len(maze[0]) or maze[x][y] == 1000:
            score -= 100 # penalty for hitting walls or going out of bounds
            break
        
        # reward getting closer to the goal using manhattan distance
        distance_to_goal = manhattan_distance(x, y, end[0], end[1])
        score += 50 / (distance_to_goal + 1)
        
        # if the goal is reached, give a large reward
        if (x, y) == end:
            score += 10000  # large reward for reaching the goal
            return score, True # return fitness and a flag indicating the goal was reached

    return score, False # return fitness and a flag indicating the goal was not reached


# mutation function to introduce random changes in a path
def mutate(path):
    # apply mutation based on the mutation rate
    if random.random() < mutation_rate:
        num_mutations = random.randint(1, 3)  # number of moves to mutate
        
         # mutate random moves in the path
        for _ in range(num_mutations):
            i = random.randint(0, len(path) - 1)
            path[i] = random.choice(moves)
        
         # occasionally replace a subpath with a random sequence of moves
        if random.random() < 0.1:
            start_idx = random.randint(0, len(path) - 5)
            subpath_length = random.randint(3, 5)
            new_subpath = [random.choice(moves) for _ in range(subpath_length)]
            path[start_idx:start_idx + subpath_length] = new_subpath
            
    return path


# crossover function to combine two parent paths and generate offspring
def crossover(parent1, parent2):
    child1, child2 = [], []
    for p1_move, p2_move in zip(parent1, parent2):
        
        # randomly swap moves between parents to create two children
        if random.random() < 0.5:
            child1.append(p1_move)
            child2.append(p2_move)
        else:
            child1.append(p2_move)
            child2.append(p1_move)
            
    return child1, child2


# selection function to select a parent based on fitness
def selection(population, fitnesses, k=5):
    selected = random.sample(list(zip(population, fitnesses)), k)
    selected.sort(key=lambda x: x[1][0], reverse=True)
    return selected[0][0]


# plot maze with path
def plot_maze_with_path(maze, path):
    fig, ax = plt.subplots(figsize=(10, 10))

    # create a grid to visualize the maze
    grid = np.zeros((len(maze), len(maze[0])))

    for i in range(len(maze)):
        for j in range(len(maze[0])):
            if maze[i][j] == 1000:
                grid[i][j] = 1
            else:
                grid[i][j] = 0

    ax.imshow(grid, cmap="gray")

    x, y = start
    path_coords = [(x, y)] # start tracking the path from the initial position

    for move in path:
        # update coordinates based on the move
        if move == 'U': x -= 1
        elif move == 'D': x += 1
        elif move == 'L': y -= 1
        elif move == 'R': y += 1
        
        # stop plotting the path if it hits a wall or goes out of bounds
        if x < 0 or y < 0 or x >= len(maze) or y >= len(maze[0]) or maze[x][y] == 1000:
            break
        path_coords.append((x, y))

    # plot the path taken
    for coord in path_coords:
        ax.plot(coord[1], coord[0], 'bo')

    ax.plot(start[1], start[0], 'go', label='start')
    ax.plot(end[1], end[0], 'ro', label='end')
    
    ax.legend()
    plt.title("maze with path")
    plt.show()


# main loop of the genetic algorithm
if __name__ == "__main__":
    # initialize population
    population = [[random.choice(moves) for _ in range(path_length)] for _ in range(population_size)]

    best_score = float('-inf')
    best_path = None

    for generation in range(generations):
        # evaluate fitness
        evaluated_population = [(path, fitness(path)) for path in population]
        evaluated_population.sort(key=lambda x: x[1][0], reverse=True)

        # check for new best solution
        top_path, (top_score, reached_goal) = evaluated_population[0]
        if top_score > best_score:
            best_score = top_score
            best_path = top_path
            print(f"generation {generation}: best score: {best_score}")
            if reached_goal:
                print("goal reached!")

        # apply elitism to select the top paths
        elitism_rate = min(elitism_rate + 0.001, 0.2)
        num_elites = int(elitism_rate * population_size)
        next_gen = [path for path, _ in evaluated_population[:num_elites]]

        # crossover and mutation with tournament selection
        while len(next_gen) < population_size:
            parent1 = selection([path for path, _ in evaluated_population], [fitness for _, fitness in evaluated_population])
            parent2 = selection([path for path, _ in evaluated_population], [fitness for _, fitness in evaluated_population])
            child1, child2 = crossover(parent1, parent2)
            next_gen.extend([mutate(child1), mutate(child2)])

        population = next_gen[:population_size] # update population for the next generation

    # final results
    if best_path:
        print("best fitness:", best_score)
    else:
        print("no path to the end was found.")

    # plot the final maze with the best path
    plot_maze_with_path(maze, best_path)
