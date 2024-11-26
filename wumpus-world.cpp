#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

const int WORLD_SIZE = 4;
const int NUM_PITS = 3;
const int NUM_WUMPUS = 1;
const char EMPTY = ' ';
const char PIT = 'P';
const char WUMPUS = 'W';
const char AGENT = 'A';
const char BREEZE = 'B';
const char STENCH = 'S';

// Directions
const int UP = -1, DOWN = 1, LEFT = -1, RIGHT = 1;

class WumpusWorld {
public:
    WumpusWorld() {
        grid = vector<vector<char>>(WORLD_SIZE, vector<char>(WORLD_SIZE, EMPTY));
        pits.clear();
        wumpus = {-1, -1};
        agentPosition = {0, 0};
        agentAlive = true;
        agentHasArrow = true;

        placePits();
        placeWumpus();
        setWarnings();
    }

    void play() {
        while (agentAlive) {
            printWorld();
            char move;
            cout << "Enter move (w=up, a=left, s=down, d=right): ";
            cin >> move;

            if (move == 'w') moveAgent(UP, 0);
            else if (move == 'a') moveAgent(0, LEFT);
            else if (move == 's') moveAgent(DOWN, 0);
            else if (move == 'd') moveAgent(0, RIGHT);
            
            checkAgentStatus();
        }

        cout << "Game Over!" << endl;
    }

private:
    vector<vector<char>> grid;
    vector<pair<int, int>> pits;
    pair<int, int> wumpus;
    pair<int, int> agentPosition;
    bool agentAlive;
    bool agentHasArrow;

    void printWorld() {
        cout << "World (A = Agent, P = Pit, W = Wumpus, B = Breeze, S = Stench):\n";
        for (int i = 0; i < WORLD_SIZE; i++) {
            for (int j = 0; j < WORLD_SIZE; j++) {
                if (make_pair(i, j) == agentPosition) cout << AGENT << " ";
                else cout << grid[i][j] << " ";
            }
            cout << endl;
        }
    }

    void placePits() {
        srand(time(0));
        for (int i = 0; i < NUM_PITS; i++) {
            int x, y;
            do {
                x = rand() % WORLD_SIZE;
                y = rand() % WORLD_SIZE;
            } while ((x == agentPosition.first && y == agentPosition.second) || isPit(x, y));
            pits.push_back({x, y});
            grid[x][y] = PIT;
        }
    }

    void placeWumpus() {
        int x, y;
        do {
            x = rand() % WORLD_SIZE;
            y = rand() % WORLD_SIZE;
        } while ((x == agentPosition.first && y == agentPosition.second) || isPit(x, y));
        wumpus = {x, y};
        grid[x][y] = WUMPUS;
    }

    void setWarnings() {
        for (int i = 0; i < WORLD_SIZE; i++) {
            for (int j = 0; j < WORLD_SIZE; j++) {
                if (grid[i][j] == PIT) {
                    setBreeze(i, j);
                }
                if (make_pair(i, j) == wumpus) {
                    setStench(i, j);
                }
            }
        }
    }

    void setBreeze(int x, int y) {
        if (x - 1 >= 0) grid[x - 1][y] = BREEZE;
        if (x + 1 < WORLD_SIZE) grid[x + 1][y] = BREEZE;
        if (y - 1 >= 0) grid[x][y - 1] = BREEZE;
        if (y + 1 < WORLD_SIZE) grid[x][y + 1] = BREEZE;
    }

    void setStench(int x, int y) {
        if (x - 1 >= 0) grid[x - 1][y] = STENCH;
        if (x + 1 < WORLD_SIZE) grid[x + 1][y] = STENCH;
        if (y - 1 >= 0) grid[x][y - 1] = STENCH;
        if (y + 1 < WORLD_SIZE) grid[x][y + 1] = STENCH;
    }

    bool isPit(int x, int y) {
        for (auto pit : pits) {
            if (pit.first == x && pit.second == y) {
                return true;
            }
        }
        return false;
    }

    void moveAgent(int dx, int dy) {
        int newX = agentPosition.first + dx;
        int newY = agentPosition.second + dy;

        if (newX < 0 || newX >= WORLD_SIZE || newY < 0 || newY >= WORLD_SIZE) {
            cout << "Move out of bounds!" << endl;
            return;
        }

        agentPosition = {newX, newY};

        if (isPit(newX, newY)) {
            agentAlive = false;
            cout << "You fell into a pit!" << endl;
        } else if (make_pair(newX, newY) == wumpus) {
            if (agentHasArrow) {
                agentHasArrow = false;
                cout << "You shot the Wumpus!" << endl;
            } else {
                agentAlive = false;
                cout << "The Wumpus ate you!" << endl;
            }
        }
    }

    void checkAgentStatus() {
        if (agentPosition == wumpus) {
            cout << "You have found the Wumpus!" << endl;
            agentAlive = false;
        }
    }
};

int main() {
    WumpusWorld game;
    game.play();
    return 0;
}
