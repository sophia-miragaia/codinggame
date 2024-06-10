#include "Graph.cpp"
#include "utils.cpp"

using namespace std;

#define SIGHT_RANGE 2

class Simulator
{
private:
	bool control_found, control_reached = false;
	Pos current_pos, last_pos, control_pos, tp_pos = tuple(-1, -1);
	Path exploration_path, control_path, return_path;
	set<Pos> nodes, explored_nodes, unexplored_nodes;
	Graph g;
	cmap game_map, revealed_map;
public:
	Simulator();
	~Simulator();
	void load_map_file(string filename);
	void init();
	void step(Pos move);
};

Simulator::Simulator()
{	
}

Simulator::~Simulator()
{
}

void Simulator::load_map_file(string filename) {
		
	ifstream file(filename); 
	string line; 
	int r = 0;
	Pos p;

	if (file.is_open()) { 
		while (getline(file, line)) { 
			for (int c = 0; c < line.size(); c++) {
				char a = line[c];
				p = tuple(r, c);
				Simulator::game_map[p] = a;
				Simulator::revealed_map[p] = '?';
				if(a == 'T') Simulator::tp_pos = p;
				if(a == 'C') Simulator::control_pos = p;
			}
		} 
		file.close(); 
	} 
	else { 
		cerr << "Unable to open file!" << endl; 
	} 
}

void Simulator::init() {
	
}

void Simulator::step(Pos move) {

}


void test_02() {

	cout << "Test #02" << endl;

}
