import sys
import math
import numpy as np
from copy import copy
from collections import deque

INT_NODES = {'empty':0, 'surv':1, 'block':2}
STR_NODES = {'empty':'.', 'surv':'@', 'block':'#'}
INVERSE_STR_NODES = {'.':'empty', '@':'surv', '#':'block'}
DIRECTION = [(0,1),(1,0),(0,-1),(-1,0)]

MAX_BOMBS = 10
BOMB_RANGE = 3

class State():

	def __init__(self, rounds, n_bombs_left, bombs_info, surv_pos, block_pos):

		self.rounds = rounds
		self.n_bombs_left = n_bombs_left
		self.bombs_info = bombs_info
		self.surv_pos = surv_pos
		self.block_pos = block_pos
		self.id = self._get_id()

	def _get_id(self):
		bombs_str = '_'.join([str(f'{pos[0]:>02d}.{pos[1]:>02d}|{self.bombs_info[pos]}') for pos in self.bombs_info])
		surv_str = '_'.join([str(f'{pos[0]:>02d}.{pos[1]:>02d}') for pos in self.surv_pos])
		block_str = '_'.join([str(f'{pos[0]:>02d}.{pos[1]:>02d}') for pos in self.block_pos])
		return str(f'r{self.rounds:>02d}_n{self.n_bombs_left:>02d}_') + 'b' + bombs_str + 's' + surv_str + 'w' + block_str

	def get_allowed_action(self):

		allowed_action = dict()
		if self.n_bombs_left > 0:
			# for i in range(H):
			# 	for j in range(W):
			# 		if ((i,j) not in self.bombs_info) and ((i,j) not in self.surv_pos) and ((i,j) not in self.block_pos):
			# 			allowed_action.add((i,j))
			for pos in self.surv_pos:
				for d in DIRECTION:
					for i in range(BOMB_RANGE):
						this_pos = (pos[0] + d[0]*(i+1),pos[1] + d[1]*(i+1))
						if (this_pos[0] >= 0) and (this_pos[1] >= 0) and (this_pos[0] < H) and (this_pos[1] < W):
							if this_pos in self.block_pos:
								break
							elif (this_pos not in self.surv_pos) and (this_pos not in self.bombs_info):
								if this_pos in allowed_action:
									allowed_action[this_pos] += 1
								else:
									allowed_action[this_pos] = 1

		allowed_action[None] = 0
		# print(f"allowed_action: {allowed_action}", file=sys.stderr)
		return allowed_action

	def update_info(self, rounds, n_bombs_left):
		self.rounds = rounds
		self.n_bombs_left = n_bombs_left
		self.id = self._get_id()


	def step(self, action):
		# action is either None for no action os a tuple for the position

		this_rounds = self.rounds
		this_n_bombs_left = self.n_bombs_left
		this_bombs_info = copy(self.bombs_info)
		this_surv_pos = copy(self.surv_pos)
		this_block_pos = copy(self.block_pos)

		# print(f"step id: {self.id}", file=sys.stderr)
		# print(f"step action: {action}", file=sys.stderr)
		# print(f"step this_bombs_info: {this_bombs_info}", file=sys.stderr)

		if this_rounds == 0:
			return None
		else:
			this_rounds -= 1

		if (action != None) and (action not in this_bombs_info):
			#Put bomb in
			this_bombs_info[action] = 3
			this_n_bombs_left -= 1

		to_explode = list()
		for bomb_pos in this_bombs_info:
			if this_bombs_info[bomb_pos] == 0:
				to_explode.append(bomb_pos)
				this_bombs_info, to_explode, this_surv_pos = self.explode(bomb_pos, this_bombs_info, to_explode, this_surv_pos, this_block_pos)
			else:
				this_bombs_info[bomb_pos] -= 1

		for bomb_pos in to_explode:
			this_bombs_info.pop(bomb_pos)

		next_state = State(this_rounds, this_n_bombs_left, this_bombs_info, this_surv_pos, this_block_pos)

		return next_state

	def explode(self, bomb_pos, bombs_info, to_explode, surv_pos, block_pos):
		
		# print(f"explode bomb_pos: {bomb_pos}", file=sys.stderr)
		for d in DIRECTION:
			for i in range(BOMB_RANGE):
				this_pos = (bomb_pos[0] + d[0]*(i+1),bomb_pos[1] + d[1]*(i+1))
				if (this_pos in bombs_info) and (this_pos not in to_explode):
					to_explode.append(this_pos)
					bombs_info, to_explode, surv_pos = self.explode(this_pos, bombs_info, to_explode, surv_pos, block_pos)
				elif this_pos in surv_pos:
					surv_pos.remove(this_pos)
				elif this_pos in block_pos:
					break

		return bombs_info, to_explode, surv_pos

class TreeSearch():

	def __init__(self,root):
		self.root = root
		self.tree = dict()
		self.addNode(root)

	def __len__(self):
		return len(self.tree)

	def addNode(self, node):
		self.tree[node.id] = node

	def set_root(self, node):
		if node.id not in self.tree:
			self.tree[node.id] = node
		self.root = node

	def getAction(self):

		q = deque([self.root])
		parent = dict()
		while q:
			s = q.pop()
			# elif len(s.bombs_info) == 0 and s.n_bombs_left == 0 and len(s.surv_pos) > 0:
			# 	#fail
			# 	# print(f"sim fail 2", file=sys.stderr)
			# 	continue				
			if len(s.surv_pos) == 0:
				#solved!
				print(f"sim success", file=sys.stderr)
				break
			# print(f"sim id = {s.id}", file=sys.stderr)
			# print(f"sim q len = {len(q)}", file=sys.stderr)
			actions = s.get_allowed_action()
			# print(f"sim actions = {actions}", file=sys.stderr)
			for a, value in sorted(actions.items(), key=lambda item:item[1]):
				# print(f"sim action:{a}|value:{value}", file=sys.stderr)
				next_s = s.step(a)
				if next_s != None:
					q.append(next_s)
					parent[next_s] = (a, s)

		(a, p) = parent[s]
		action_seq = deque([a])
		while p != self.root:
			(a, p) = parent[p]
			action_seq.append(a)

		# print(f"sim action_seq: {action_seq}", file=sys.stderr)
		return action_seq

def get_print_action(action):

	if action == None:
		str_out = 'WAIT'
	else:
		str_out = str(action[1]) + ' ' + str(action[0])
	return str_out


def get_nodes(map_game, node_type):
	# Get the node_type pos
	pos = MAP_INDEX[map_game == INT_NODES[node_type]]
	pos_list = [tuple(p) for p in pos]
	return pos_list

def get_map_from_grid(grid):
	# Make the np map from str grid
	map_game = np.zeros([H,W],dtype=np.int8)
	for i in range(H):
		for j in range(W):
			cell = GRID_STR[i][j]
			map_game[i][j] = INT_NODES[INVERSE_STR_NODES[cell]]
	return map_game

if __name__ == "__main__":
	# W: width of the firewall grid
	# H: height of the firewall grid

	W, H = [int(i) for i in input().split()]
	MAP_INDEX = np.array([[[j,i] for i in range(W)] for j in range(H)])
	GRID_STR = [["." for _ in range(W)] for _ in range(H)]

	for i in range(H):
		grid_row = input()  # one line of the firewall grid
		GRID_STR[i][:] = grid_row

	map_game = get_map_from_grid(GRID_STR)

	bombs_info = dict()

	surv_pos = get_nodes(map_game, 'surv')
	block_pos = get_nodes(map_game, 'block')

	currentState = State(0, 0, bombs_info, surv_pos, block_pos)
	ts = TreeSearch(currentState)

	action_series = [(0,0),(2,0),None,None,None,None,None,None,None,None,None,None,(3,0),None,None]

	action_seq = None

	while True:

		rounds, n_bombs_left = [int(i) for i in input().split()]
		
		currentState.update_info(rounds, n_bombs_left)
		print(f"current id = {currentState.id}", file=sys.stderr)
		print(f"current bombs_info = {currentState.bombs_info}", file=sys.stderr)

		ts.set_root(currentState)

		if action_seq == None:
			action_seq = ts.getAction()

		action = action_seq.pop()
		print(f"current action = {action}", file=sys.stderr)
		print(get_print_action(action))

		# action = action_series.pop(0)
		# print(get_print_action(action))