#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

// Define the puzzle size
const int N = 3;

// Define the goal state
const vector<vector<int>> goalState = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

// Define a structure to represent a puzzle state
struct PuzzleState {
    vector<vector<int>> state;
    int gValue; // Cost to reach the current state
    int heuristic;

    // Constructor
    PuzzleState(vector<vector<int>> s, int g) : state(s), gValue(g), heuristic(calculateMisplacedTilesHeuristic(s)) {}

    // Function to calculate the Misplaced Tiles heuristic
    int calculateMisplacedTilesHeuristic(const vector<vector<int>>& s) const {
        int misplacedTiles = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (s[i][j] != 0 && s[i][j] != i * N + j + 1) {
                    // Check if the tile is misplaced
                    misplacedTiles++;
                }
            }
        }
        return misplacedTiles;
    }

    // Function to calculate the f-value
    int calculateFValue() const {
        return gValue + heuristic;
    }

    // Function to check if the state is the goal state
    bool isGoalState() const {
        return state == goalState;
    }

    // Function to print the puzzle state
    void printState() const {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << state[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

// Define a comparison function for the priority queue
struct CompareFValue {
    bool operator()(const PuzzleState& lhs, const PuzzleState& rhs) const {
        return lhs.calculateFValue() > rhs.calculateFValue();
    }
};

// Function to find the coordinates of the blank space
pair<int, int> findBlank(const vector<vector<int>>& state) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (state[i][j] == 0) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // Blank not found (error condition)
}

// Function to perform A* Search
void aStarSearch(const PuzzleState& initialState) {
    priority_queue<PuzzleState, vector<PuzzleState>, CompareFValue> pq;
    map<vector<vector<int>>, bool> visited;

    pq.push(initialState);

    while (!pq.empty()) {
        PuzzleState current = pq.top();
        pq.pop();

        if (visited[current.state]) {
            continue;
        }

        visited[current.state] = true;

        current.printState();

        if (current.isGoalState()) {
            cout << "Goal state reached!" << endl;
            return;
        }

        // Find the coordinates of the blank space
        pair<int, int> blankPos = findBlank(current.state);
        int blankRow = blankPos.first;
        int blankCol = blankPos.second;

        // Generate possible moves and add to the priority queue
        vector<pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& move : moves) {
            int newRow = blankRow + move.first;
            int newCol = blankCol + move.second;

            if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N) {
                vector<vector<int>> newState = current.state;
                swap(newState[blankRow][blankCol], newState[newRow][newCol]);

                PuzzleState nextState(newState, current.gValue + 1);
                if (!visited[nextState.state]) {
                    pq.push(nextState);
                }
            }
        }
    }

    cout << "Goal state not reached!" << endl;
}

int main() {
    // Define the initial state
    vector<vector<int>> initialState = {{1, 2, 3}, {4, 0, 5}, {7, 8, 6}};

    // Create the initial puzzle state
    PuzzleState initialPuzzleState(initialState, 0);

    // Perform A* Search with Misplaced Tiles heuristic
    aStarSearch(initialPuzzleState);

    return 0;
}
