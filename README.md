# SWGCG301 Graphics Project

A real-time graphics project written in **C++20** with **OpenGL 3.3 (core profile)**, **GLFW** and **GLAD**,
built for *SWGCG 301: Graphics and Multimedia* at Zewail City of Science and Technology.

The course project is split into three phases. This README tracks what is done and what is in progress.

## Status

| Phase | Goal | Status |
|---|---|---|
| 1 | Window, OpenGL context, shaders, 2D shapes, keyboard and mouse input | Done |
| 2 | ECS-based engine: scene loaded from a data file, assets, materials, forward renderer, camera | In progress |
| 3 | Lighting (multiple light types), post-processing, sky, and a small 3D game on the engine | Planned |

## Phase 1: what it does

- Opens a 1920x1080 window with an OpenGL 3.3 core, forward-compatible context.
- Loads, compiles and links GLSL shaders from files (`shaders/main.vert`, `shaders/main.frag`) with compile and link error logging.
- Passes per-vertex color from the vertex shader to the fragment shader through an interface block, so colors are interpolated across each triangle.
- Draws five different shapes, each in its own class derived from a shared `Shape` base class that owns the VAO, VBO and EBO:
  triangle, hexagon, star, heart and smiley face.
- Each shape is created once before the main loop and follows the mouse cursor (the cursor position is converted to normalized device coordinates using the real window size).
- Shapes keep their proportions when the window is resized: the vertex shader divides x by the aspect ratio.
- Vsync is enabled.

### Controls

| Input | Action |
|---|---|
| `1` | Triangle |
| `2` | Hexagon |
| `3` | Star |
| `4` | Heart |
| `5` | Smiley face |
| Mouse | The shape follows the cursor |
| `Esc` | Quit |

The selected shape stays on screen until another number key is pressed.

## Project structure

```
main.cpp          window, main loop, input handling
ShaderLoader.*    loads, compiles and links a shader program from files
Shapes/           Shape base class and one class per shape
shaders/          main.vert / main.frag (GLSL 330 core)
vendor/           GLFW and GLAD
CMakeLists.txt    build configuration
```

## Build and run

Requirements: CMake 3.10 or newer, a C++20 compiler, and a GPU/driver with OpenGL 3.3 support.

```bash
git clone https://github.com/mido151/SWGCG301-Graphics-Project.git
cd SWGCG301-Graphics-Project
cmake -B build
cmake --build build --config Release
```

The build copies the `shaders/` folder next to the executable. Run the executable from its output folder
(on Windows with Visual Studio this is `build/Release`).

## What I learned

- How the GPU render pipeline is driven by shader programs and how vertex data reaches it through VBOs, EBOs and VAOs.
- Passing data between pipeline stages with varyings and interface blocks.
- Mapping mouse coordinates to normalized device coordinates.
- Structuring rendering code around a small class hierarchy instead of one large `main()`.

## Roadmap

- [ ] Phase 2: entity-component-system architecture and scene deserialization
- [ ] Phase 2: asset loaders, materials with pipeline state, forward renderer with transparency sorting
- [ ] Phase 3: light component and lit materials, multiple lights, sky, post-processing
- [ ] Phase 3: a small 3D game using the engine

## Credits

Team project for SWGCG 301. Shapes were split between the team members.
