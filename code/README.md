# 🦖 Chrome T-Rex AI: C++ Perceptron

A implementation of the classic Google Chrome T-Rex game, built in **C++** using **Raylib**. The centerpiece of this project is its autonomous player: a custom-built, single-layer Neural Network (**Perceptron**) designed, implemented, and trained completely from scratch, only using Rosenblat's classical Perceptron theory.

## ✨ Features

- **Pure C++ Neural Network**: A from-scratch Perceptron implementation featuring mathematical activation boundaries, dynamic weight adjustments (Delta Rule), and online supervised learning.
- **Decoupled Architecture (Model-View)**.
- **Fast-Forward Training**: By uncoupling physics from the graphical frame rate, the engine can simulate the world 2x faster than the human eye can see, drastically accelerating the AI's "trial-and-error" learning phase.
- **Decoupled Hitboxes & Sprites**: Collision detection uses strict and forgiving logical dimensions (e.g., `48x48`), while the visual renderer dynamically offsets and scales original graphical assets without distortion.
- **Stateless "Time Machine" Memory**: The Perceptron lacks recurrent memory. To teach it how to anticipate jumps, we engineered a retroactive 25-frame memory buffer that correctly labels past mistakes leading up to a fatal crash.

## 🛠️ Tech Stack

- **Language**: C++17
- **Graphics Framework**: [Raylib](https://www.raylib.com/)
- **Build System**: CMake

## 🚀 Getting Started

### Prerequisites

Make sure you have `CMake` and a modern C++ compiler (like `g++` or `clang`) installed on your Linux environment. The Raylib dependency is automatically fetched and managed by CMake.

### Build and Run

1. Clone the repository and navigate to the project root:

   ```bash
   cd T-Rex
   ```

2. Create a build directory and configure CMake:

   ```bash
   mkdir build && cd build
   cmake ..
   ```

3. Compile the project using multiple cores:

   ```bash
   make -j4
   ```

4. Run the simulation:

   ```bash
   ./dino_run
   ```

## 🧠 How the AI Learns (Phase 1)

The AI relies on a purely geometric awareness of its surroundings. Instead of passing nominal categories (e.g., Cactus vs. Pterodactyl), the Perceptron is fed normalized structural data:

1. Current Game Speed
2. Distance (X) to the next obstacle
3. Absolute height (Y) of the obstacle
4. Obstacle width (to gauge required air-time)

When the T-Rex inevitably crashes into a cactus, the simulation pauses. The game engine reaches back into its 25-frame chronological buffer, overwriting the "mistake" with the absolute target truth: *"You should have jumped earlier."* The Perceptron is then retrained via the Delta Rule, recalculating its weights dynamically to avoid repeating the exact same failure in the next generation.

## 🚧 Planned Roadmap

- [x] **Phase 1**: Core physics engine, dynamic sprite rendering, and successful Perceptron training in raw C++.
- [ ] **Phase 2**: Expanding the network architecture to a Multi-Layer Perceptron (MLP) for more complex maneuvers (e.g., ducking under Pterodactyls).
- [ ] **Phase 3**: Porting the entire ecosystem to the web via **WebAssembly (Emscripten)** to allow the AI to train natively inside the browser.
