# Sand Falling Simulator (C++ & SDL2)

Interactive falling-sand sandbox built in C++ using SDL2.  
Click to drop sand and watch it fall, stack, and slide with simple physics.  
Includes a Restart button, pixel-art grid and smooth simulation loop.

---

## Features
- Falling sand physics using a cellular-automata model
- Pixel grid visual style
- Click to spawn sand
- Restart button to clear the screen
- SDL2 rendering with VSync
- Easy to extend (new materials like water, stone, etc.)

---

## Requirements (macOS)
You need these installed:
- Homebrew
- Xcode Command Line Tools
- SDL2
- SDL2_ttf (for rendering text )

Install if needed:

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
xcode-select --install
brew install sdl2 sdl2_ttf

---

## Add Font File
Place a `.ttf` font in the same folder as `sand.cpp`, for example:

OpenSans-Regular.ttf

---

## Build & Run (macOS)
Compile using:

clang++ sand.cpp -o sand \
  -I/opt/homebrew/include \
  -I/opt/homebrew/opt/sdl2/include \
  -I/opt/homebrew/opt/sdl2_ttf/include \
  -L/opt/homebrew/lib \
  -L/opt/homebrew/opt/sdl2/lib \
  -L/opt/homebrew/opt/sdl2_ttf/lib \
  -lSDL2 -lSDL2_ttf

Then run:

./sand

---

## Controls
Hold Left Mouse Button → Spawn falling sand  
Click Restart Button → Clear the world  
Close Window → Exit

---

## How It Works
Each sand grain acts as a cell in a fixed grid.

Every frame, each grain tries in order:
1. Move down
2. Move down-left or down-right if blocked
3. Otherwise stay put

This produces realistic piling and sliding behavior.

---

## Planned Improvements
- Water, stone, smoke materials
- Right-click → eraser
- Brush size options
- UI toolbar to select materials
- Randomized sand color variation
- Save/load sandbox state

---

## License
MIT License

---

## Author
Pranav Kumar
