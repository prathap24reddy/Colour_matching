# Block Shooter

Block Shooter is a C++ game that features a brick-shooting mechanic on a grid-based playing field. This project is designed for systems with memory-mapped I/O, such as FPGA-based development boards that support VGA graphics and hardware key inputs. The game initializes a grid of colored bricks and challenges the player to shoot matching bricks to clear the board.

---

## Features

- **Grid-Based Gameplay:**  
  A grid of bricks is generated with random colors. The player uses a slider to aim and shoot bricks upward.

- **Memory-Mapped Graphics:**  
  Direct manipulation of VGA and character buffers via fixed memory addresses provides low-level control of screen rendering.

- **Hardware Key Input:**  
  The game reads hardware key inputs (via memory-mapped I/O) to move the slider left or right and to fire bricks.

- **Recursive Brick Popping:**  
  Matching adjacent bricks are removed recursively when hit, allowing for chain reactions.

- **Main Menu & Game Modes:**  
  A main page provides options for a trial game (placeholder) or the main game mode.

---

## Requirements

- **Hardware:**  
  A development board or simulator that supports the following memory-mapped addresses:
  - VGA pixel buffer at `0x08000000`
  - Character buffer at `0x09000000`
  - Hardware keys at `0xFF200050`

- **Software:**  
  - A C++ compiler (e.g., `g++`, `clang++`)
  - Standard C++ libraries
  - Optionally, a Make utility if you wish to use the provided Makefile

> **Note:** This project is targeted at specific embedded hardware. If you're compiling and running on a desktop environment, you may need to modify the memory addresses and associated code.

---

## Getting Started

### Cloning the Repository

Clone the repository using Git:

```bash
git clone https://github.com/yourusername/block-shooter.git
cd block-shooter
```

### Building the Project

If you have a Makefile in the repository, simply run:

```bash
make
```

Alternatively, compile the project manually with a command such as:

```bash
g++ -std=c++11 -o blockshooter main.cpp
```

Replace `main.cpp` with the name of the source file if different.

### Running the Game

After building the project, run the executable:

```bash
./blockshooter
```

Follow the on-screen instructions:
- **Main Menu:**  
  - Press key **0** for the trial game (currently a placeholder).
  - Press key **1** for the main game.
- **In-Game Controls:**  
  - Use designated hardware keys to move the slider left or right.
  - Press the fire key to shoot bricks upward.

---

## Project Structure

- **main.cpp:**  
  The main source file containing game logic, drawing routines, and hardware interfacing.

- **README.md:**  
  This file, providing an overview of the project, features, and instructions.

- **Makefile (optional):**  
  A Makefile to simplify the build process.

---

## Customization & Further Development

- **Trial Game Mode:**  
  Currently a placeholder function (`trail_game()`). Extend this mode with additional gameplay features or tutorials.

- **Game Mechanics:**  
  Refactor or expand game logic, such as adding scoring details, levels, or additional brick types.

- **Hardware Adaptation:**  
  Modify memory-mapped addresses and key input handling to suit different hardware platforms.

---

