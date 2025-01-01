# BattleShipAI
# Player Example AI

This repository contains a starter implementation of a C++ AI for a board game competition framework. The goal of the AI is to strategically place ships and decide optimal shooting locations based on a probability-driven approach.

---

## Features

1. **Ship Placement**  
   - Automatically determines safe and strategic positions for ship placement.  
   - Avoids collisions with previously placed ships.  
   - Optimized placement based on opponent's shooting patterns.

2. **Shooting Strategy**  
   - Calculates probabilities for the likelihood of a hit in each cell.  
   - Considers nearby hits and misses to improve targeting.  
   - Dynamically adapts the shot logic throughout the game.

3. **Opponent Tracking**  
   - Keeps track of opponent's shots.  
   - Updates internal boards based on hits, misses, and kills.  
   - Adjusts strategy based on opponent's behavior.

4. **Match Lifecycle Management**  
   - Handles setup, per-round initialization, and cleanup processes.  
   - Maintains game state across multiple rounds and matches.

---

## Getting Started

### Prerequisites
- C++17 or later.
- Standard build tools (e.g., `gcc`, `clang`, or Visual Studio).
- The game framework with required headers and libraries.

### Building the Project
1. Clone the repository.
2. Compile the `player_example.cpp` file along with the required game framework files:
   ```bash
   g++ -o player_example player_example.cpp -std=c++17
   ```
3. Run the compiled program with the appropriate socket path as an argument:
   ```bash
   ./player_example /path/to/socket
   ```

---

## File Overview

### `player_example.cpp`
The main implementation of the AI logic. This file includes:
- **Ship Placement**: `choose_ship_place()`, `safeShip()`, and `markShip()`.
- **Shooting Logic**: `choose_shot()`, `hprob()`, and `vprob()`.
- **Match Management**: Functions like `handle_setup_match()`, `handle_game_over()`, and `handle_match_over()`.

### `player_example.h`
Header file defining the `Nate_Dave` class that extends the base `Player` class from the game framework. It declares the methods and members used in `player_example.cpp`.

---

## AI Logic Highlights

1. **Ship Placement (`choose_ship_place`)**
   - Attempts to place ships in areas with low likelihood of opponent shots.
   - Uses probability maps to avoid high-risk areas.
   - Ensures ships are placed within bounds and do not overlap.

2. **Shooting Strategy (`choose_shot`)**
   - Combines horizontal and vertical probabilities to target optimal cells.
   - Avoids shooting in cells already marked as misses or kills.
   - Adjusts strategy dynamically based on the board state.

3. **Match Lifecycle**
   - Initializes game boards and clears them between rounds.
   - Efficiently manages memory to allocate and deallocate game state data.

---

## Usage Notes

- **AI Name**: The AI is currently named `Player Example C++`.
- **Authors**: Nate Paarlberg & Manzi Dave Rugari.
- The AI is designed to work seamlessly with the provided game framework. Modifications to the main match-handling logic are not recommended unless you fully understand the underlying architecture.




