import sys
import math
from collections import deque

MOVEMENT = ["RIGHT","DOWN","LEFT","UP"]
DIRECTION = {"RIGHT": (0,1),"DOWN": (1,0),"LEFT": (0,-1),"UP": (-1,0)}
INVERSE_DIRECTION = {(0,1):"RIGHT",(1,0):"DOWN",(0,-1):"LEFT",(-1,0):"UP"}
SIGHT_DIST = 2 # How many cell can be seen

class Graph():

	def __init__(self):
		# Initializes a graph object 
		self.graph_dict = {}

	def vertices(self):
		# Returns the vertices of the graph
		return list(self.graph_dict.keys())

	def edges(self):
		# Returns the edges of a graph
		return self.generate_edges()

	def add_vertex(self, vertex):
		# If the vertex is not in the graph, adds a ney key to dict
		if vertex not in self.graph_dict:
			self.graph_dict[vertex] = []

	def add_edge(self, v1, v2):
		# Adds an edge between verticies v1 and v2

		# If v1 is in the graph and v2 is not connected to v1, add v2 to the v1 list
		if v1 in self.graph_dict:
			if v2 not in self.graph_dict[v1]:
				self.graph_dict[v1].append(v2)
		# Else, create a node v1 with v2 in its list
		else:
			self.graph_dict[v1] = [v2]

		# If v2 is in the graph and v1 is not connected to v2, add v1 to the v2 list
		if v2 in self.graph_dict:
			if v1 not in self.graph_dict[v2]:
				self.graph_dict[v2].append(v1)
		# Else, create a node v2 with v1 in its list
		else:
			self.graph_dict[v2] = [v1]


	def generate_edges(self):
		#A method for getting the edges of the graph. 
		# Edges are represented as tuples of two nodes 
		
		edges = []
		for vertex in self.graph_dict:
			for neighbour in self.graph_dict[vertex]:
				if (neighbour, vertex) not in edges:
					edges.append((vertex, neighbour))
		return edges

	def __str__(self):
		res = "vertices: "
		for k in self.graph_dict:
			res += str(k) + " "
		res += "\nedges: "
		for edge in self.generate_edges():
			res += str(edge) + " "
		return res

	def BFS(self, start_vertex, end_vertex, max_dist=float('Inf')):
		# Finds a path between start and end vertex via Breadth First Search. 
		# If the path is longer than max_dist, return None
		
		visited, queue, parent = set(), deque([start_vertex]), {}
		visited.add(start_vertex)

		# BFS
		while queue:
			vertex = queue.popleft()
			# If reached the end vertex, quit the loop
			if vertex == end_vertex:
				break
			for neighbour in self.graph_dict[vertex]:
				if neighbour not in visited:
					visited.add(neighbour)
					queue.append(neighbour)
					parent[neighbour] = vertex

		if end_vertex not in visited:
			return None

		# Builds the path, using the parent dict created in the BFS algo
		path = deque([end_vertex])
		w = end_vertex
		while True:
			v = parent[w]
			if v == start_vertex:
				break
			path.append(v)
			w = v

		# If the path is too long, return None
		if len(path) > max_dist:
			return None

		return path

def getExplorationPath(graph, unexplored_nodes, mapGame, pos, C_pos):
	# Analyzes the map and return a path to explore
	# Maximizing 

	max_avg_discovery = 0
	for node in unexplored_nodes:
		this_path = graph.BFS(pos, node)
		if this_path != None:
			# Checks if the Control room is not in the path to avoid triggering the alarm countdown before ready
			if C_pos not in this_path:
				path_discovery = get_path_discovery(mapGame, pos, this_path)
				if path_discovery/len(this_path) >= max_avg_discovery:
					path = this_path
					max_avg_discovery = path_discovery/len(this_path)
	return path

def get_path_discovery(mapGame, pos, path):
	# Measure the total discovery (?) in a path

	discoveryCells = 0
	current_pos = pos
	for next_pos in path:
		discoveryCells += get_discovery(mapGame, current_pos, next_pos)
		current_pos = next_pos
	return discoveryCells

def get_discovery(mapGame, pos, next_pos):
	# Measure the total discovery (?) in a move

	d = [next_pos[0] - pos[0],next_pos[1] - pos[1]]
	if d[0] == 0: 	#Horizontal movement
		dCells = [(next_pos[0] + x, next_pos[1] + SIGHT_DIST*d[1]) for x in range(-SIGHT_DIST,SIGHT_DIST+1)]
	else: 			#Vertical movement
		dCells = [(next_pos[0] + SIGHT_DIST*d[0], next_pos[1] + y) for y in range(-SIGHT_DIST,SIGHT_DIST+1)]

	discoveryCells = 0
	for cellPos in dCells:
		if cellPos[0]>=0 and cellPos[0]<R and cellPos[1]>=0 and cellPos[1]<C:
			cell = mapGame[cellPos[0]][cellPos[1]]
			if cell == "?":
				discoveryCells += 1

	return discoveryCells

def printMap(map2print):

	for row in range(R):
		print(''.join(map2print[row][:]), file=sys.stderr)

def updateGraph(graph, mapGame, pos, last_pos):

	# If it is the first step of the simulation, takes all info around Kirk
	if last_pos == (None,None):
		for i in range(-SIGHT_DIST,SIGHT_DIST+1):
			for j in range(-SIGHT_DIST,SIGHT_DIST+1):
				p = (pos[0] + i,pos[1] + j)
				# If p is valid (inside the map), add it to the graph as node
				if p[0]>=0 and p[0]<R and p[1]>=0 and p[1]<C:
					if mapGame[p[0]][p[1]] != "#":
						graph.add_vertex(p)
	else:
		last_step = (pos[0] - last_pos[0],pos[1] - last_pos[1])
		
		# Gets the newly seen cells
		if last_step[0] == 0: 	#Horizontal movement
			newCellPos = [(pos[0] + x, pos[1] + SIGHT_DIST*last_step[1]) for x in range(-SIGHT_DIST,SIGHT_DIST+1)]
		else: 			#Vertical movement
			newCellPos = [(pos[0] + SIGHT_DIST*last_step[0], pos[1] + y) for y in range(-SIGHT_DIST,SIGHT_DIST+1)]
		# Adds newly seen cells to the graph as nodes
		for p in newCellPos:
			if p[0]>=0 and p[0]<R and p[1]>=0 and p[1]<C:
				if mapGame[p[0]][p[1]] != "#":
					graph.add_vertex(p)

	# Adds the edges between cells
	nodes = graph.vertices()
	for n in nodes:
		for m in MOVEMENT:
			neighbour = (n[0] + DIRECTION[m][0],n[1] + DIRECTION[m][1])
			if neighbour in nodes:
				graph.add_edge(n,neighbour)
	return graph

def get_unexplored_nodes(graph, explored_nodes):
	# Gets a list of the unexplored nodes by calculating the difference between 
	# all visible nodes (graph.vertices) and explored nodes

	set_all_nodes = set(graph.vertices())

	unexplored_nodes = list(set_all_nodes - explored_nodes)

	return unexplored_nodes

def get_move_from_path(path,pos):
	# Pops the next item in the path and calculate the movement

	next_pos = path.pop()
	step = (next_pos[0] - pos[0],next_pos[1] - pos[1])
	move = INVERSE_DIRECTION[step]
	return move

if __name__ == "__main__":

	# R: number of rows.
	# C: number of columns.
	# A: number of rounds between the time the alarm countdown is activated and the time the alarm goes off.
	R, C, A = [int(i) for i in input().split()]
	# Map of the game
	mapGame = [["?" for _ in range(C)] for _ in range(R)]
	# Has Kirk reached the Control?
	hasReachControl = False
	# Kirk's last position
	last_pos = (None, None)
	# last_pos = (last_kr, last_kc)

	# Current exploration path
	exp_path = None

	# Control room position
	C_pos = (None,None)
	# Has the Control room been found?
	C_found = False
	# Has the Control room been reached?
	C_reached = False

	# Path to the Control room
	C_path = None
	# Path back to the Teleport
	Return_path = None

	# Graph of the possible paths
	graph = Graph()
	# Set of explored nodes
	explored_nodes = set()
	# List of unexplored nodes
	unexplored_nodes = []

	# game loop
	while True:
		# kr: row where Kirk is located.
		# kc: column where Kirk is located.
		kr, kc = [int(i) for i in input().split()]
		# Kirk's current position
		pos = (kr,kc)

		# Add the current position to the set of explored nodes
		explored_nodes.add(pos)
		# If the current position is the Control room, Control room has been reach
		if pos == C_pos:
			C_reached = True

		# If the last position is None, the current position is the Teleport
		if last_pos == (None, None):
			T_pos = (kr,kc)

		# For each row, reads the input from the simulator
		for i in range(R):
			row = input()  # C of the characters in '#.TC?' (i.e. one line of the ASCII maze).
			# Update teh game map
			mapGame[i][:] = row
			#If "C" is in the row, the Control room has been found.
			if "C" in row and not C_found:
				C_found = True
				C_pos = (i, row.index("C"))

		# Updates the graph
		graph = updateGraph(graph, mapGame, pos, last_pos)

		# If Control room has been found and there is no path to it
		if C_found and C_path == None:
			# Try to find a path between Control room and the teleport
			Return_path = graph.BFS(C_pos, T_pos, A)
			# If the return path has been found, try to find a path to the Control room
			if Return_path != None:
				C_path = graph.BFS(pos, C_pos)

		# If there is no defined path to the Control room, explore
		if C_path == None:
			if not exp_path:
				# Get the unexplored nodes
				unexplored_nodes = get_unexplored_nodes(graph, explored_nodes)
				# Get the exploration path
				exp_path = getExplorationPath(graph, unexplored_nodes, mapGame, pos, C_pos)
			move = get_move_from_path(exp_path,pos)
		# If there is a path to the Control room, but it hasn't been reached
		elif not C_reached:
			move = get_move_from_path(C_path,pos)
		# Else, Kirk is on the Return path
		else:
			move = get_move_from_path(Return_path,pos)

		# Update last_pos 
		last_pos = pos

		# Kirk's next move (UP DOWN LEFT or RIGHT).
		print(move)