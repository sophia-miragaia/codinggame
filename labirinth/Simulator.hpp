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
	Graph g;
	Pos get_move_from_input();
public:
	Player();
	Player(int mode);
	~Player();
	void set_mode(int mode);
	int get_mode();
	Pos get_move();
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
	bool pos_OOB(Pos pos);
	void update_revealed_map();
	void render(bool fog);
	void print_status();
	Status step(Pos move);
};

void test_simulator01();