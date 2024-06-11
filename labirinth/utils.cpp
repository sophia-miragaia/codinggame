#include "utils.hpp"

using namespace std; 

Pos add_position(Pos p1, Pos p2) {
	
	int x = get<0>(p1) + get<0>(p2);
	int y = get<1>(p1) + get<1>(p2);
	Pos p_result = tuple(x,y);
	return p_result;

};

string pos2string(Pos p) {
	ostringstream  s;
	s << "( " << get<0>(p) << " , " << get<1>(p) << " )";
	return s.str();
};

Graph::Graph() {
};

Graph::~Graph() {
};

bool Graph::add_Vertex(Pos pos) {

	vmap::iterator itr = graph.find(pos);
	bool new_vertex = false;

	if (itr == graph.end())
	{
		Vertex *v;
		v = new Vertex(pos);
		graph[pos] = v;
		new_vertex = true;
	}

	return new_vertex;
}

void Graph::add_edge(Pos pos_from, Pos pos_to) {

	Vertex *f = (graph.find(pos_from)->second);
	Vertex *t = (graph.find(pos_to)->second);
	f->adj.push_back(t);
	t->adj.push_back(f);

}

void Graph::update_edges(Pos pos) {

	vmap::iterator itr;
	Pos dir = tuple(0,1);
	Pos new_pos = add_position(pos, dir);
	if(graph.count(new_pos) == 1) add_edge(pos, new_pos);

}

Path Graph::BFS(Pos pos_from, Pos pos_to, int max_dist) {

	Path path = BFS(pos_from, pos_to);
	if (path.size() > max_dist) {
		Path path;
		path.push_back(tuple(-1,-1));
		return path;
	} 
	return path;

}

Path Graph::BFS(Pos pos_from, Pos pos_to) {

	map<Pos, bool> visited;
	queue<Pos> q;
	vmap parent;
	
	visited[pos_from] = true;
	q.push(pos_from);

	while(!q.empty()) {

		Pos pos = q.front();
		q.pop();

		if(pos == pos_to) break;
		Vertex *v = graph[pos];
		for (auto& it : v->adj) {
			Pos pos = it->v_pos;
			if (!visited[pos]) {
				visited[pos] = true;
				q.push(pos);
				parent[pos] = v;
			}
		}
	}

	Path path;
	if(!visited[pos_to]) {
		path.push_back(tuple(-1,-1));
	}
	else {
		path.push_back(pos_to);
		Pos w = pos_to;
		while(true) {
			Vertex *v = parent[w];
			if(v->v_pos == pos_from) break;
			path.push_back(v->v_pos);
			w = v->v_pos;
		}
	}

	return path;
}

void test_graph01() {

	cout << "Test #01" << endl;

	Graph g;

	Pos p1 = tuple(0,0);
	Pos p2 = tuple(0,1);
	Pos p3 = tuple(0,2);
	Pos p4 = tuple(0,3);
	Pos p5 = tuple(0,4);

	g.add_Vertex(p1);
	g.add_Vertex(p2);
	g.add_Vertex(p3);
	g.add_Vertex(p4);
	g.add_Vertex(p5);

	g.add_edge(p1, p2);
	g.add_edge(p2, p3);
	g.add_edge(p3, p4);
	g.add_edge(p4, p5);
	g.add_edge(p2, p4);

	Path path = g.BFS(p1,p5);
	while(!path.empty()) {

		Pos pos = path.back();
		path.pop_back();
		cout << "\t(" << get<0>(pos) << "," << get<1>(pos) << ")" << endl;
	}
}

Map_Features load_map_file(string filename) {
		
	ifstream file(filename); 
	string line; 
	int r = 0;
	int c;
	Pos p;

	Map_Features smap;

	if (file.is_open()) { 
		while (getline(file, line)) { 
			for (c = 0; c < line.size(); c++) {
				char a = line[c];
				p = tuple(r, c);
				smap.game_map[p] = a;
				smap.revealed_map[p] = '?';
				if(a == 'T') smap.tp_pos = p;
				if(a == 'C') smap.control_pos = p;
			}
			r++;
		} 
		file.close(); 
	} 
	else { 
		cerr << "Unable to open file!" << endl; 
	} 
	smap.rows = r; smap.cols = c;

	return smap;
}

void read_map(string filename) {

	ifstream file(filename); 
	string line; 

	if (file.is_open()) { 
		// Read each line from the file and store it in the 
		// 'line' variable. 
		while (getline(file, line)) { 
			cout << "\t" << line << endl; 
		} 
  
		// Close the file stream once all lines have been 
		// read. 
		file.close(); 
	} 
	else { 
		// Print an error message to the standard error 
		// stream if the file cannot be opened. 
		cerr << "Unable to open file!" << endl; 
	} 

}

Config load_config(string config_filename) {

	Config config;
	ifstream file(config_filename); 
	string line;

	if (file.is_open()) { 
		getline(file, line);
		config.scenario = line;
		getline(file, line);
		config.map_file = line;
		getline(file, line);
		config.SIGHT_RANGE = stoi(line);
		getline(file, line);
		config.MAX_TURNS = stoi(line);
		getline(file, line);
		config.MAX_TURNS_RETURN = stoi(line);
		
		file.close(); 
	}

	return config;
}

bool pos_OOB(Pos pos, cmap game_map) {

	cmap::iterator itr = game_map.find(pos);
	bool x = itr == game_map.end();
	return x;

}

cmap sight_map(Pos player_pos, cmap game_map, int range) {

	Pos p = player_pos;
	int px = get<0>(p);
	int py = get<1>(p);
	cmap revealed_map;

	for (int x = px - range; x < px + range + 1; x++)
		for (int y = py - range; y < py + range + 1; y++) {
			p = tuple(x,y);
			if (!pos_OOB(p, game_map)) 
				revealed_map[p] = game_map[p];
		}
	
	return revealed_map;
	
}


void test_read_map01() {

	cout << "Test read_map" << endl;
	read_map("map_test.map");

}