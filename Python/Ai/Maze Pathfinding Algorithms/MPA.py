from collections import deque
import heapq
import tracemalloc


maze = [
      [1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1],
      [0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1],
      [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1],
      [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1],
      [0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0],
      [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0],
      [0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0],
      [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0],
      [0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0],
      [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0]
]

# start and end points of the maze
start = (0, 1)
end = (9, 18)


# function to perform breadth first search in the maze
def bfs(maze, start, end):
    movs = [(0, 1), (1, 0), (0, -1), (-1, 0)]  # possible movements
    rows = len(maze)
    cols = len(maze[0])
    queue = deque([start])  # queue for bfs
    visited = set()  # set of visited nodes
    visited.add(start)
    parent = {start: None}  # dictionary to keep track of the path
    
    while queue:
        node = queue.popleft()
        if node == end:
            break
        
        for x, y in movs:
            next_node = (node[0] + x, node[1] + y)

            if 0 <= next_node[0] < rows and 0 <= next_node[1] < cols and maze[next_node[0]][next_node[1]] == 1:  # check if next_node is within the maze and is a 1
                if next_node not in visited:
                    queue.append(next_node)
                    visited.add(next_node)
                    parent[next_node] = node
    
    # reconstruct path from end to start
    path = []
    if end in parent:
        current = end
        while current:
            path.append(current)
            current = parent[current]
        path.reverse()
    return path


# function to perform depth first search in the maze
def dfs(maze, start, end):
    movs =  [(0, 1), (1, 0), (0, -1), (-1, 0)]  # possible movements
    rows = len(maze)
    cols = len(maze[0])
    stack = [start]  # stack for DFS
    visited = set()  # set of visited nodes
    visited.add(start)
    parent = {start: None}  # dictionary to keep track of the path
     
    while stack:
        node = stack.pop()
        if node == end:
            break
        
        for x, y in movs:
            next_node = (node[0] + x, node[1] + y)
            if 0 <= next_node[0] < rows and 0 <= next_node[1] < cols and maze[next_node[0]][next_node[1]] == 1:  # check if next_node is within the maze and is a 1
                if next_node not in visited:
                    stack.append(next_node)
                    visited.add(next_node)
                    parent[next_node] = node
                    
    # reconstruct path from end to start
    path = []
    if end in parent:
        current = end
        while current:
            path.append(current)
            current = parent[current]
        path.reverse()
    return path



# function to calculate manhattan distance between two points
def manhattan_distance(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])


# function to perform a* search in the maze
def a_star(maze, start, end):
    movs =  [(0, 1), (1, 0), (0, -1), (-1, 0)]  # possible movements
    rows = len(maze)
    cols = len(maze[0])
    priority_queue = []  # priority queue for a* search
    visited = set()  # set of visited nodes
    heapq.heappush(priority_queue, (0, start))
    g_value = {start: 0}  # dictionary to keep track of the cost from start to each node
    parent = {start: None}  # dictionary to keep track of the path
    
    while priority_queue:
        node = heapq.heappop(priority_queue)[1]
        if node == end:
            break
        
        visited.add(node)
        
        for x, y in movs:
            next_node = (node[0] + x, node[1] + y)
            if 0 <= next_node[0] < rows and 0 <= next_node[1] < cols and maze[next_node[0]][next_node[1]] == 1:  # check if next_node is within the maze and is a 1
                temp_g_value = g_value[node] + 1  # cost from start to next_node
                if next_node not in g_value or temp_g_value < g_value[next_node]:
                    g_value[next_node] = temp_g_value  # update g(n)
                    h_value = manhattan_distance(next_node, end) 
                    f_value = temp_g_value + h_value  # f(n) = g(n) + h(n)
                    heapq.heappush(priority_queue, (f_value, next_node))
                    parent[next_node] = node
    
    # reconstruct path from end to start
    path = []
    if end in parent:
        current = end
        while current:
            path.append(current)
            current = parent[current]
        path.reverse()
    return path


# function to print the maze with the given path marked
def print_maze(maze, path):
    maze_copy = [row[:] for row in maze]  # create a copy of the maze to modify
    for (x, y) in path:
        maze_copy[x][y] = '*'  # mark the path in the maze
    
    for row in maze_copy:
        print(' '.join(str(cell) for cell in row)) # print each row of the maze



# function to measure memory usage of a given algorithm
def memory_usage(func, maze, start, end):
    tracemalloc.start() # start tracking memory usage
    
    path = func(maze, start, end)
    peak = tracemalloc.get_traced_memory()[1] # get peak memory usage
    
    tracemalloc.stop() # stop tracking memory usage
    
    return path, peak


# main function to run the algorithms and print the results
def main():
    bfs_path, bfs_memory = memory_usage(bfs, maze, start, end)
    dfs_path, dfs_memory = memory_usage(dfs, maze, start, end)
    a_star_path, a_star_memory = memory_usage(a_star, maze, start, end)
    
    print("bfs path:")
    if bfs_path:
        print_maze(maze, bfs_path)
        print("bfs path length:", len(bfs_path))
        print("bfs peak memory usage (in bytes):", bfs_memory)
    else:
        print("no path found with bfs")
    
    print("\ndfs path:")
    if dfs_path:
        print_maze(maze, dfs_path)
        print("dfs path length:", len(dfs_path))
        print("dfs peak memory usage (in bytes):", dfs_memory)
    else:
        print("no path found with dfs")
    
    print("\na* path:")
    if a_star_path:
        print_maze(maze, a_star_path)
        print("a* path length:", len(a_star_path))
        print("a* peak memory usage (in bytes):", a_star_memory)
    else:
        print("no path found with a*")


main()
