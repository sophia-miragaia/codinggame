#include "Simulator.hpp"

using namespace std;

Player::Player() {
}

Player::Player(int mode) {
	Player::mode = mode;
}

Player::~Player() {
}

void Player::set_mode(int mode) {
	Player::mode = mode;
}

int Player::get_mode() {
	return mode;
}

Pos Player::get_move() {
	Pos p_result;
	if(mode == 0) {
		p_result = Player::get_move_from_input();
	}
	return p_result;
}

Pos Player::get_move_from_input() {
	
	Pos p;
	char key_move;
	cout << "Move: ";
	cin >> key_move;

	switch (key_move)
	{
	case 'w':
		p = tuple(-1,0);
		break;
	case 's':
		p = tuple(1,0);
		break;
	case 'a':
		p = tuple(0,-1);
		break;
	case 'd':
		p = tuple(0,1);
		break;
	
	default:
		cerr << "Unkwown input..." << endl;
		break;
	}
	return p;
}

Simulator::Simulator()
{	
}

Simulator::Simulator(string config_filename)
{	
	Simulator::config = load_config(config_filename);
	Simulator::smap = load_map_file(Simulator::config.map_file);
}

Simulator::~Simulator()
{
}

bool Simulator::pos_OOB(Pos pos) {

	cmap::iterator itr = smap.game_map.find(pos);
	bool x = itr == smap.game_map.end();
	return x;

}

void Simulator::render(bool fog) {

	if(player.get_mode() == 0) {
		cmap* render_map;
		if (fog) render_map = &smap.revealed_map;
		else render_map = &smap.game_map;

		for (int r = 0; r < smap.rows; r++) {
			for (int c = 0; c < smap.cols; c++) {
				Pos p = tuple(r,c);
				char out;
				if (p == player_pos) out = 'P';
				else out = (*render_map)[p];
				cout << out;
			}
			cout << endl;
		}
	}
}

void Simulator::print_status() {

	switch (status)
	{
	case OOB:
		cout << "Player moved out of the map." << endl;
		break;
	case Out_of_turns_return:
		cout << "Player ran out of turns returning to the teleport." << endl;
		break;
	case Out_of_turns:
		cout << "Player ran out of turns." << endl;
		break;
	case Crashed:
		cout << "Player crashed into a wall." << endl;
		break;
	case Stopped:
		cout << "Game paused." << endl;
		break;
	case Running:
		cout << "Game running normally." << endl;
		break;
	case Sucess:
		cout << "Player WINS!" << endl;
		break;
	
	default:
		break;
	}

}

void Simulator::update_revealed_map() {

	Pos p = player_pos;
	int px = get<0>(p);
	int py = get<1>(p);

	for (int x = px - config.SIGHT_RANGE; x < px + config.SIGHT_RANGE + 1; x++)
		for (int y = py - config.SIGHT_RANGE; y < py + config.SIGHT_RANGE + 1; y++) {
			p = tuple(x,y);
			if (!pos_OOB(p)) 
				smap.revealed_map[p] = smap.game_map[p];
		}
	
}

void Simulator::init(Player player) {

	string filename = config.scenario + ".map";
	load_map_file(filename);

	Simulator::player = player;

	player_pos = smap.tp_pos;
	turns = 0;
	turns_return = 0;
	status = Running;
	update_revealed_map();

	render(true);

}

void Simulator::run() {

	while(status == 1) {
		
		cout << "Turn: " << turns << " | Pos: " << pos2string(player_pos) << endl;

		Pos move = player.get_move();
		cout << "Moving: " << pos2string(move) << endl;

		status = step(move);
		print_status();
	}

}

Status Simulator::step(Pos move) {

	player_pos = add_position(player_pos, move);
	turns++;
	if (control_reached) turns_return++;
	Status status {Running};

	if (turns > config.MAX_TURNS) 
		status = Out_of_turns;
	else if (control_reached && (turns_return > config.MAX_TURNS_RETURN)) 
		status = Out_of_turns_return;
	else if (pos_OOB(player_pos)) 
		status = OOB;
	else if (smap.game_map[player_pos] == '#') 
		status = Crashed;
	else if (player_pos == smap.control_pos) 
		control_reached = true;
	else if (control_reached && (player_pos == smap.tp_pos)) 
		status = Sucess;
	update_revealed_map();
	render(true);

	return status;

}

void test_simulator01() {

	cout << "Test Simulator #01" << endl;

	string config_filename = "scenario02.config";
	Simulator sim(config_filename);
	Player player(0);
	
	sim.init(player);
	sim.run();
	
}
