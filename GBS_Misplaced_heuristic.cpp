import heapq
import time
from typing import List, Tuple

N = 3

goal_state = [[0, 1, 2], [3, 4, 5], [6, 7, 8]]

class PuzzleState:
    def __init__(self, state: List[List[int]]):
        self.state = state
        self.heuristic = self.calculate_misplaced_tiles_heuristic()
    
    def calculate_misplaced_tiles_heuristic(self) -> int:
        misplaced_tiles = sum(
            1 for i in range(N) for j in range(N) 
            if self.state[i][j] != 0 and self.state[i][j] != goal_state[i][j]
        )
        return misplaced_tiles
    
    def is_goal_state(self) -> bool:
        return self.state == goal_state
    
    def print_state(self):
        for row in self.state:
            print(" ".join(map(str, row)))
        print()
    
    def __lt__(self, other):
        return self.heuristic < other.heuristic

def find_blank(state: List[List[int]]) -> Tuple[int, int]:
    for i, row in enumerate(state):
        for j, val in enumerate(row):
            if val == 0:
                return i, j
    raise ValueError("Blank tile not found in the state")

def greedy_best_first_search(initial_state: PuzzleState):
    start_time = time.time()
    pq = []
    heapq.heappush(pq, initial_state)
    visited = set()
    iterations = 0

    while pq:
        iterations += 1
        current = heapq.heappop(pq)
        state_tuple = tuple(map(tuple, current.state))
        
        if state_tuple in visited:
            continue
        
        visited.add(state_tuple)
        current.print_state()
        
        if current.is_goal_state():
            end_time = time.time()
            print(f"Goal state reached in {iterations} iterations.")
            print(f"Time taken: {end_time - start_time:.6f} seconds.")
            return
        
        blank_row, blank_col = find_blank(current.state)
        moves = [(-1, 0), (1, 0), (0, -1), (0, 1)]
        
        for dr, dc in moves:
            new_row, new_col = blank_row + dr, blank_col + dc
            if 0 <= new_row < N and 0 <= new_col < N:
                new_state = [row[:] for row in current.state]
                new_state[blank_row][blank_col], new_state[new_row][new_col] = new_state[new_row][new_col], new_state[blank_row][blank_col]
                
                next_state = PuzzleState(new_state)
                if tuple(map(tuple, next_state.state)) not in visited:
                    heapq.heappush(pq, next_state)
    
    print("Goal state not reached!")
    end_time = time.time()
    print(f"Total iterations: {iterations}")
    print(f"Time taken: {end_time - start_time:.6f} seconds.")

if __name__ == "__main__":
    test_cases = [
        [[8, 0, 6], [5, 4, 7], [2, 3, 1]],
        [[1, 2, 3], [4, 5, 6], [7, 8, 0]],  # Already solved case
        [[0, 1, 3], [4, 2, 5], [7, 8, 6]]   # More complex case
    ]
    
    for i, test_case in enumerate(test_cases):
        print(f"Test Case {i+1}:")
        initial_puzzle_state = PuzzleState(test_case)
        greedy_best_first_search(initial_puzzle_state)
        print("-" * 30)
