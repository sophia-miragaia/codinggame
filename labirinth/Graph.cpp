#include <iostream>
#include <string>
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

struct vertex {
	vector<vertex*> adj; // destination vertecies
	Pos v_pos;
	vertex(Pos pos) : v_pos(pos) {}
};

typedef map<Pos, vertex *> vmap;
typedef map<Pos, char> cmap;

class Graph {
	private:
		vmap graph;
	public:
		void add_vertex(Pos pos);
		void add_edge(Pos pos_from, Pos pos_to);
		Path BFS(Pos pos_from, Pos pos_to, int max_dist);
		Path BFS(Pos pos_from, Pos pos_to);
};

void Graph::add_vertex(Pos pos) {

	vmap::iterator itr = graph.find(pos);

	if (itr == graph.end())
		{
			vertex *v;
			v = new vertex(pos);
			graph[pos] = v;
		}
}

void Graph::add_edge(Pos pos_from, Pos pos_to) {

	vertex *f = (graph.find(pos_from)->second);
	vertex *t = (graph.find(pos_to)->second);
	f->adj.push_back(t);
	t->adj.push_back(f);

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
		vertex *v = graph[pos];
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
			vertex *v = parent[w];
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

	g.add_vertex(p1);
	g.add_vertex(p2);
	g.add_vertex(p3);
	g.add_vertex(p4);
	g.add_vertex(p5);

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

