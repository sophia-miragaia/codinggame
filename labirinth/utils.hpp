#include <fstream> 
#include <iostream> 
#include <string> 
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
#include <tuple>
#include <set>

using namespace std; 

typedef tuple<int,int> Pos;
typedef deque<Pos> Path;

struct Vertex {
	vector<Vertex*> adj; // destination vertecies
	Pos v_pos;
	Vertex(Pos pos) : v_pos(pos) {}
};

Pos add_position(Pos p1, Pos p2);
string pos2string(Pos p);

typedef map<Pos, Vertex *> vmap;
typedef map<Pos, char> cmap;

class Graph {
	private:
		vmap graph;
	public:
		Graph();
		~Graph();
		void add_Vertex(Pos pos);
		void add_edge(Pos pos_from, Pos pos_to);
		Path BFS(Pos pos_from, Pos pos_to, int max_dist);
		Path BFS(Pos pos_from, Pos pos_to);
};

void test_graph01();

struct Config {
	string scenario;
	string map_file;
	int SIGHT_RANGE;
	int MAX_TURNS;
	int MAX_TURNS_RETURN;
};

enum Status{
	OOB = -4,
	Out_of_turns_return = -3,
	Out_of_turns = -2,
	Crashed = -1,
	Stopped = 0,
	Running = 1,
	Sucess = 2
};

struct Map_Features {
	cmap game_map, revealed_map;
	Pos control_pos, tp_pos;
	int rows, cols;
};

Map_Features load_map_file(string filename);
void read_map(string filename);
Config load_config(string config_filename);
void test_read_map01();