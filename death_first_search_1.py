import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.

def bfs_shortest_path(graph, start, goal):
    # keep track of explored nodes
    explored = []
    # keep track of all the paths to be checked
    queue = [[start]]
    
    # return path if start is goal
    if start == goal:
        return None, False

    # keeps looping until all possible paths have been checked
    while queue:
        # pop the first path from the queue
        path = queue.pop(0)
        # get the last node from the path
        node = path[-1]
        if node not in explored:
            neighbours = graph[node]
            # go through all neighbour nodes, construct a new path and
            # push it into the queue
            for neighbour in neighbours:
                new_path = list(path)
                new_path.append(neighbour)
                queue.append(new_path)
                # return path if neighbour is goal
                if neighbour == goal:
                    return new_path, True
            # mark node as explored
            explored.append(node)

    # in case there's no path between the 2 nodes
    return None, False

# n: the total number of nodes in the level, including the gateways
# l: the number of links
# e: the number of exit gateways
n, l, e = [int(i) for i in input().split()]

graph = {i: [] for i in range(n)}
exits = [0 for i in range(e)]

for i in range(l):
    # n1: N1 and N2 defines a link between these nodes
    n1, n2 = [int(j) for j in input().split()]
    graph[n1].append(n2)
    graph[n2].append(n1)

for i in range(e):
    ei = int(input())  # the index of a gateway node
    exits[i] = ei
    
# game loop
while True:
    si = int(input())  # The index of the node on which the Skynet agent is positioned this turn

    shortest_path = None
    for E in exits:
        path, pathExists = bfs_shortest_path(graph, si, E)
        if pathExists:
            if not shortest_path or len(path) < len(shortest_path):
                shortest_path = path
            
    n1 = shortest_path[0]
    n2 = shortest_path[1]
    graph[n1].remove(n2)
    graph[n2].remove(n1)


    # Write an action using print
    # To debug: print("Debug messages...", file=sys.stderr)

    # Example: 0 1 are the indices of the nodes you wish to sever the link between
    print( str(n1) + " " + str(n2) )
