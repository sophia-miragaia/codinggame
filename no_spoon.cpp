#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
	int get_x() const { return x; }
	int get_y() const { return y; }
	int x = -1;
	int y = -1;
};

class Grid {
	private:
		int _width;
		int _height;
		int _current_row = 0;
		char _grid[30][30];
		vector<Node> node_list;
		vector<Node>::iterator node_it;
	public:
		Grid(int, int);
		void add_line(string);
		void start_iterator();
		bool finished_iterator();
		void print();
		Node get_next_node();
		Node get_right_neighbor(Node);
		Node get_bottom_neighbor(Node);
};

Grid::Grid(int width, int height) {
	Grid::_width = width;
	Grid::_height = height;
}

void Grid::add_line(string line) {
	int column = 0;
	vector<Node>::iterator it = node_list.begin();

	for(auto i=line.begin(); i!=line.end(); ++i) {
		_grid[column][_current_row] = *i;

		if(*i == '0') {
			Node n; n.x = column; n.y=_current_row;
			node_list.insert(node_list.end(), n);
		}
		column++;
	}
	_current_row++;
}

void Grid::start_iterator() {
	node_it = node_list.begin();
}

bool Grid::finished_iterator() {
	return (node_it == node_list.end());
}

void Grid::print() {
	for (int i=0; i<_width; i++) {
		for (int j=0; j<_height; j++) {
			cerr << _grid[i][j];
		}
		cerr << endl;
	}
}

Node Grid::get_next_node() {
	Node this_node = *node_it;
	node_it++;
	return this_node;
}

Node Grid::get_right_neighbor(Node n) {

	Node closest_node;
	int min_dist = 99;

	for(auto i=node_list.begin(); i!=node_list.end(); ++i) {
		Node right_node = *i;
		if(right_node.get_y() == n.get_y()) {
			int dist = right_node.get_x() - n.get_x();
			if((dist > 0) && (dist < min_dist)) {
				closest_node = right_node;
				min_dist = dist;
			}
		}
	}
	return closest_node;
}

Node Grid::get_bottom_neighbor(Node n) {

	Node closest_node;
	int min_dist = 99;

	for(auto i=node_list.begin(); i!=node_list.end(); ++i) {
		Node right_node = *i;
		if(right_node.get_x() == n.get_x()) {
			int dist = right_node.get_y() - n.get_y();
			if((dist > 0) && (dist < min_dist)) {
				closest_node = right_node;
				min_dist = dist;
			}
		}
	}
	return closest_node;
}

int main()
{
	int width; // the number of cells on the X axis
	cin >> width; cin.ignore();
	int height; // the number of cells on the Y axis
	cin >> height; cin.ignore();

	Grid grid = Grid(width, height);

	for (int i = 0; i < height; i++) {
		string line;
		getline(cin, line); // width characters, each either 0 or .
		grid.add_line(line);
	}
	
	// grid.print();
	grid.start_iterator();
	Node n, nr, nb;
	while(not grid.finished_iterator()) {
		n = grid.get_next_node();
		nr = grid.get_right_neighbor(n);
		nb = grid.get_bottom_neighbor(n);
		cout << n.x << " " << n.y << " ";
		cout << nr.x << " " << nr.y << " ";
		cout << nb.x << " " << nb.y;
		cout << endl;
	}	
}