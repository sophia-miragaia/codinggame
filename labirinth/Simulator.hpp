#include "utils.cpp"

using namespace std;

class Player
{
private:
	int rows, cols, turns, mode;
	bool control_found, control_reached = false;
	Pos player_pos, player_last_pos, control_pos, tp_pos = tuple(-1, -1);
	Path exploration_path, control_path, return_path;
	set<Pos> nodes, explored_nodes, unexplored_nodes;
	Graph graph;
	Pos get_move_from_input();
	Pos get_move_from_BFS();
public:
	Player();
	Player(int mode);
	~Player();
	void set_mode(int mode);
	int get_mode();
	Pos get_move();
	void update_graph(cmap sight_map);
};

class Simulator
{
private:
	int turns, turns_return;
	bool sim_running, control_reached = false;
	Pos player_pos = tuple(-1, -1);
	Map_Features smap;
	Player player;
	Config config;
	Status status {Stopped};
public:
	Simulator();
	Simulator(string config_filename);
	~Simulator();
	void init(Player player);
	void run();
	void render(bool fog);
	void print_status();
	void update_revealed_map();
	Status step(Pos move);
};

void test_simulator01();
void test_simulator02();