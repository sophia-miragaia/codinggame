#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

#define EMPTY 0
#define SURV -1
#define BLOCK -2
#define MAX_WIDTH 20
#define MAX_HEIGHT 20
#define MAX_ROUNDS 20
#define MAX_BOMBS 10
#define BOMB_RANGE 3


using namespace std;

class Position
{
    public:
        int x;
        int y;
        Position(int x=-1, int y=-1)
        {
            Position::x = x;
            Position::y = y;
        }
        Position operator+(Position const& pos)
        {
            Position res;
            res.x = x + pos.x;
            res.y = y + pos.y;
            return res;
        }
        Position operator-(Position const& pos)
        {
            Position res;
            res.x = x - pos.x;
            res.y = y - pos.y;
            return res;
        }
        Position operator*(int const& m)
        {
            Position res;
            res.x = x * m;
            res.y = y * m;
            return res;
        }
};

class Action
{
    public:
        int x;
        int y;
        Position pos;
        bool is_wait;
        Action(int x, int y, bool is_wait);
        bool is_valid(State state);
};

Action::Action(int x, int y, bool is_wait)
{
    is_wait = is_wait;
    x = x;
    y = y;
    pos.x = x;
    pos.y = y;
}

class State
{
    private:
        string get_id();
    public:
        int rounds;
        int n_bombs_left;
        int bombs_placed = 0;
        Position pos_bombs[MAX_BOMBS];
        int rounds_bombs[MAX_BOMBS];
        int grid[MAX_HEIGHT][MAX_WIDTH];
        string id;
        State(int rounds, int n_bombs_left, int grid[MAX_HEIGHT][MAX_WIDTH]);
        State();
        void update_info(int rounds, int n_bombs_left);
        void explode_bombs();
        void explode(int bomb, bool to_explode[MAX_BOMBS]);
        State step(Action action);
};

State::State(int rounds, int n_bombs_left, int grid[MAX_HEIGHT][MAX_WIDTH])
{
    rounds = rounds;
    n_bombs_left = n_bombs_left;
    for(int i=0; i<MAX_HEIGHT; i++)
        for(int j=0; j<MAX_WIDTH; j++)
            grid[i][j] = grid[i][j];
}

State::State()
{
}

void State::update_info(int rounds, int n_bombs_left)
{
    rounds = rounds;
    n_bombs_left = n_bombs_left;
    id = get_id();
}

State State::step(Action action)
{
    State new_state(rounds, n_bombs_left, grid);

    if(rounds == 0) return State();
    else new_state.rounds--;

    if(~action.is_wait) 
    {
        new_state.grid[action.x][action.y] = new_state.bombs_placed + 1;

        new_state.pos_bombs[new_state.bombs_placed] = action.pos;
        new_state.rounds_bombs[new_state.bombs_placed] = 3;

        new_state.bombs_placed++;
        new_state.n_bombs_left--;
    }

    new_state.explode_bombs();

    return new_state;
}

void State::explode_bombs()
{
    bool to_explode[MAX_BOMBS];
    int k=0;
    for(int bomb=0; bomb<MAX_BOMBS; bomb++)
    {
        to_explode[bomb] = false;
        if(rounds_bombs[bomb] == 0)
        {
            to_explode[bomb] = true;
            explode(bomb, to_explode);
        }
        else rounds_bombs[bomb]--;
    }

    for(int bomb=0; bomb<MAX_BOMBS; bomb++)
    {
        if(to_explode[bomb])
        {
            grid[pos_bombs[bomb].x][pos_bombs[bomb].y] = EMPTY;
            rounds_bombs[bomb] = -1;
        }
    }

}

void State::explode(int bomb, bool to_explode[MAX_BOMBS])
{
    Position directios[4] = {Position(0,1), Position(1,0), Position(0,-1), Position(-1,0)};
    Position this_pos;
    for(int d=0; d<4; d++)
    {
        for(int i=1; i<=BOMB_RANGE; i++)
        {
            this_pos = pos_bombs[bomb] + directios[d] * i;
            if(grid[this_pos.x][this_pos.y] > 0) //outra bomba explode
            {
                int this_bomb = grid[this_pos.x][this_pos.y]-1;
                to_explode[this_bomb] = true;
                explode(this_bomb, to_explode);
            }
            else if(grid[this_pos.x][this_pos.y] == SURV)
            {
                grid[this_pos.x][this_pos.y] = EMPTY;
            }
            else if(grid[this_pos.x][this_pos.y] == BLOCK) break;
        }
    }
}

class Graph
{
    int V = MAX_ROUNDS;
    vector<list<int>> adj;
    public:
        Graph();
        void addEdge(int v, int w);
        void BFS(int s);
};

Graph::Graph()
{
    adj.resize(V);
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}

void Graph::BFS(int s)
{
    // Mark all the vertices as not visited
    vector<bool> visited;
    visited.resize(V, false);
 
    // Create a queue for BFS
    list<int> queue;
 
    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);
 
    while (!queue.empty()) {
         
        // Dequeue a vertex from queue and print it
        s = queue.front();
        cout << s << " ";
        queue.pop_front();
 
        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (auto adjacent : adj[s]) {
            if (!visited[adjacent]) {
                visited[adjacent] = true;
                queue.push_back(adjacent);
            }
        }
    }
}

int main()
{
    int width; // width of the firewall grid
    int height; // height of the firewall grid
    cin >> width >> height; cin.ignore();
    for (int i = 0; i < height; i++) {
        string mapRow;
        getline(cin, mapRow); // one line of the firewall grid
        cerr << mapRow << endl;
    }

    // game loop
    while (1) {
        int rounds; // number of rounds left before the end of the game
        int bombs; // number of bombs left
        cin >> rounds >> bombs; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << "0 1" << endl;
    }
}