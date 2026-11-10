# Physics Engine

A 2D physics sandbox built with C++17 and SFML 3. It currently supports circles, dynamic rectangles, static rectangles, gravity, collision response, keyboard-controlled movement, and saving/loading the simulation state.

## Features

- Fixed-timestep physics simulation at 60 updates per second
- Frame-time clamp to prevent a spiral of death after long pauses
- Circle physics with gravity and circle-to-circle collision response
- Dynamic rectangle physics with gravity, movement, angular velocity, and collision response
- Static rectangles that act as platforms or walls
- Circle-to-rectangle and rectangle-to-rectangle collisions
- Four static boundary walls around the 800 x 600 window
- Out-of-bounds cleanup for dynamic objects
- Save and load support through `save.json`
- Reset support for restoring the default boundary walls

## Controls

| Input | Action |
|---|---|
| `Z` | Select circle spawn mode |
| `X` | Select dynamic rectangle spawn mode |
| `C` | Select static rectangle spawn mode |
| Left mouse button | Spawn the selected object at the cursor |
| Arrow keys | Move the first rectangle |
| `Q` / `E` | Rotate the first rectangle |
| `R` | Reset the simulation and boundary walls |
| `S` | Save the current simulation to `save.json` |
| `L` | Load the simulation from `save.json` |

## Requirements

- CMake 3.16 or newer
- A C++17-compatible compiler
- Git
- Internet access on the first CMake configure, so CMake can download SFML and nlohmann/json

## Build

From the project directory:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

The executable is generated as:

```text
build/PhysicsEngine.exe
```

## Project Structure

- `src/main.cpp` - Window creation, game loop, collision resolution, rendering, and fixed-timestep simulation
- `src/PhysicsObject.h` / `src/PhysicsObject.cpp` - Circle and rectangle physics data and updates
- `src/InputHandler.h` / `src/InputHandler.cpp` - Keyboard, mouse, and object-spawn handling
- `src/savemanager.h` / `src/savemanager.cpp` - Save and load functionality
- `CMakeLists.txt` - Build configuration and dependencies

## Roadmap

Planned improvements include:

- Completing the remaining rectangle rotation theorem and using it consistently for accurate rotated-rectangle collision detection and response
- Adding support for different polygon shapes
- Improving polygon collision detection, rotation, inertia, and impulse response
- Expanding object limits and simulation controls

## License

No license has been specified for this project yet.
