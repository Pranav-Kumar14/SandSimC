# Sand Falling Simulator (C++ & SDL2)

A simple falling-sand sandbox built in C++ using SDL2. Click to place sand and watch it fall and slide like granular physics. Built as a beginner-friendly cellular automata simulation with pixel-art style rendering.

## Features
- Falling sand physics
- Pixel-style grid rendering
- Click to spawn sand
- Smooth animation using SDL2
- Lightweight and easy to extend (water, stone, etc.)

## Requirements (macOS)
You need:
- Homebrew
- Xcode Command Line Tools
- SDL2

Install them:

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
xcode-select --install
brew install sdl2

## Build & Run (macOS)
Compile using:

clang++ sand.cpp -o sand -I/opt/homebrew/opt/sdl2/include -L/opt/homebrew/opt/sdl2/lib -lSDL2

Then run:

./sand

If SDL2 include errors occur, try:

clang++ sand.cpp -o sand -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2

## Controls
- Hold Left Mouse Button → Spawn falling sand
- Close Window → Exit

## How It Works
Each sand grain is represented as a cell in a grid.

Simulation rule per frame:
1. Move down if empty
2. Else move down-left or down-right if empty
3. Else stay

This creates natural falling/stacking behavior.

## Planned Improvements
- More materials: water, stone, smoke
- Brush size option
- UI to select materials
- Sand color variations
- Save/Load worlds

## License
MIT License

## Author
Pranav Kumar
