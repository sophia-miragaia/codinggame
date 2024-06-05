#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

#define E_MAX 20

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// C++ code to print BFS traversal from a given
// source vertex

using namespace std;

// This class represents a directed graph using
// adjacency list representation
class Graph {

	// No. of vertices
	int V;
	// Pointer to an array containing adjacency lists
	vector<list<int>> adj;

public:
	// Constructor
	Graph(int V);
	// Function to add an edge to graph
	void addEdge(int v, int w);
    // Function to remove an edge
    void removeEdge(int v, int w);
	// BFS traversal from a given source s to e
	pair<bool, list<int>> BFS(int s, int e);
};

Graph::Graph(int V)
{
	this->V = V;
	adj.resize(V);
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w);
	adj[w].push_back(v);
}

void Graph::removeEdge(int v, int w)
{
    adj[v].remove(w);
    adj[w].remove(v);
}

pair<bool, list<int>> Graph::BFS(int s, int e)
{

	// Mark all the vertices as not visited
	vector<bool> visited;
	visited.resize(V, false);

	// Create a queue for BFS
	list<int> queue;
    
    vector<int> pred;
    pred.resize(V, -1);

    vector<int> dist;
    dist.resize(V, sizeof(int));

    list<int> path;
    bool found_path = false;
    pair<bool, list<int>> result;

	// Mark the current node as visited and enqueue it
	visited[s] = true;
	queue.push_back(s);
    dist[s] = 0;

    int node;

	while (!queue.empty() && !found_path) {
		
        if(s == e) break;
		// Dequeue a vertex from queue and print it
		node = queue.front();
		queue.pop_front();

		// Get all adjacent vertices of the dequeued
		// vertex s. If a adjacent has not been visited,
		// then mark it visited and enqueue it
		for (auto adjacent : adj[node]) {
			if (!visited[adjacent]) {
                dist[adjacent] = dist[node] + 1;
                pred[adjacent] = node;
				visited[adjacent] = true;
                if(adjacent == e) {
                    found_path = true;
                    break;
                }
				queue.push_back(adjacent);
			}
		}
	}

    if(found_path) {
        path.push_back(e);
        node = e;
        while (node != s) {
            node = pred[node];
            path.push_back(node);
        }
    }

    result = {found_path, path};

    return result;
}


int main()
{

    int n; // the total number of nodes in the level, including the gateways
    int l; // the number of links
    int e; // the number of exit gateways
    
    cin >> n >> l >> e; cin.ignore();

    Graph graph(n);
    int exit_nodes[E_MAX];

    for (int i = 0; i < l; i++) {
        int n1; // N1 and N2 defines a link between these nodes
        int n2;
        cin >> n1 >> n2; cin.ignore();
        graph.addEdge(n1, n2);
        // cerr << n1 << " - " << n2 << endl;
    }
    for (int i = 0; i < e; i++) {
        int ei; // the index of a gateway node
        cin >> ei; cin.ignore();
        exit_nodes[i] = ei;
    }

    // game loop
    while (1) {
        int si; // The index of the node on which the Bobnet agent is positioned this turn
        cin >> si; cin.ignore();

        pair<bool, list<int>> result;
        list<int> shortest_path;
        bool found_path = false;

        for (int i=0; i<e; i++) {
            result = graph.BFS(si, exit_nodes[i]);
            if(result.first) {
                if(!found_path || result.second.size() < shortest_path.size()) {
                    shortest_path = result.second;
                    found_path = true;
                }
            }
        }

        int n1 = shortest_path.back();
        shortest_path.pop_back();
        int n2 = shortest_path.back();

        std::cout << n1 << " " << n2 << endl;
    }
}