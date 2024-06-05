#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 * ---
 * Hint: You can use the debug stream to print initialTX and initialTY, if Thor seems not follow your orders.
 **/

class Position {
	private:
		int _x;
		int _y;
		Position _position_direction(Position pos);
		void _update_position(Position position_direction);

	public:
		Position(int x, int y);
		void print();
		int get_x();
		int get_y();
		string get_direction(Position pos);
};

Position::Position(int x, int y) {
	Position::_x = x;
	Position::_y = y;
};

void Position::print() {
	cout << "(" << Position::_x << "," << Position::_y << ")";
};

int Position::get_x() {
	return Position::_x;
};

int Position::get_y() {
	return Position::_y;
};

Position Position::_position_direction(Position pos) {
	return Position(pos.get_x() - Position::_x, pos.get_y() - Position::_y);
};

void Position::_update_position(Position position_direction) {

	if(position_direction.get_y()<0) {
		Position::_y--;
	} else if (position_direction.get_y()>0)
	{
		Position::_y++;
	}

	if(position_direction.get_x()<0) {
		Position::_x--;
	} else if (position_direction.get_x()>0)
	{
		Position::_x++;
	}
};

string Position::get_direction(Position pos) {
	Position d = Position::_position_direction(pos);
	Position::_update_position(d);

	string direction;
	if(d.get_y()<0) {
		direction.append("N");
	} else if (d.get_y()>0)
	{
		direction.append("S");
	}
	if(d.get_x()<0) {
		direction.append("W");
	} else if (d.get_x()>0)
	{
		direction.append("E");
	}

	return direction;
}

int main()
{
	int light_x; // the X position of the light of power
	int light_y; // the Y position of the light of power
	int initial_tx; // Thor's starting X position
	int initial_ty; // Thor's starting Y position
	std::cin >> light_x >> light_y >> initial_tx >> initial_ty; std::cin.ignore();

	Position light = Position(light_x, light_y);
	Position thor = Position(initial_tx, initial_ty);

	// game loop
	while (1) {
		int remaining_turns; // The remaining amount of turns Thor can move. Do not remove this line.
		std::cin >> remaining_turns; std::cin.ignore();

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;

		// Simple solution, quadrants and lines

		string direction = thor.get_direction(light);
		// cerr << direction << endl;

		// A single line providing the move to be made: N NE E SE S SW W or NW
		std::cout << direction << endl;
	}
}