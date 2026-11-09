*This project has been created as part of the 42 curriculum by yotsurud, and tobaba.*

# ft_vox

## Overview
- [Description](#description)
- [Dependencies](#dependencies)
- [Instructions](#instructions)
- [Project Structure](#project-structure)
- [Main features](#main-features)
- [Controls](#controls)
- [Resources](#resources)
- [AI Usage](#ai-usage)

## Description

This project is an exercise designed to learn the basics of 3D rendering using OpenGL.
It loads an `.obj` file and displays the 3D object in a window.
The object can be rotated, moved, and scaled using keyboard inputs.
Additionally, a texture can be loaded and applied to the 3D object.

## Dependencies

### Ubuntu

```bash
sudo apt update && sudo apt install -y build-essential cmake xorg-dev libgl1-mesa-dev libglfw3-dev
```

### MacOS
```bash
brew install glfw
```

GLFW is already installed in the 42 school environment, so no additional installation is required.
## Instructions

### Requirements
- **OS:** Linux / macOS
- **Compiler:** C++17 compatible compiler
- **Graphics API:** OpenGL 3.3 Core Profile
- **Window / Input:** GLFW
- **OpenGL Loader:** GLAD 0.1.36 (automatically generated during the build)
- **Build Tool:** Make
- **Additional Requirements:** Python 3, pip, and an internet connection for the initial build

GLAD is not included in this repository. During the initial build, `install_glad.sh` installs the GLAD generator in a temporary directory and generates the OpenGL 3.3 Core Profile loader under `includes/glad/`.
```bash
includes/
└── glad/
    ├── include/
    │   ├── glad/
    │   │   └── glad.h
    │   └── KHR/
    │       └── khrplatform.h
    └── src/
        └── glad.c
```

### Installation
```bash
git clone <repository of this project>
cd project-name
```

### Compilation
Compile the project with:
```bash
make
```
On the first build, the Makefile automatically runs `install_glad.sh` to generate the required GLAD files. Subsequent builds reuse the generated files.
This creates the `SCOP` executable.

Other available commands:
- `make clean` - removes object and dependency files
- `make fclean` - removes object files, dependency files, and the executable
- `make re` - rebuilds the project

#### Regenerating GLAD
If any of the generated GLAD files are missing or corrupted, remove the includes/glad/ directory and rebuild the project:
```bash
rm -rf includes/glad
make re
```
The build process will automatically regenerate the required GLAD files.
    
### Usage
```bash
./SCOP <obj_file> <bmp_file>
```

## Project Structure

The project is organized into modules based on their responsibilities.

- `gl/`: OpenGL rendering and window management
- `math/`: Vector and matrix calculations
- `parser/`: OBJ and BMP file parsing
- `validate/`: Mesh geometry diagnostics
- `utils/`: Utility functions
```bash
.
├── Makefile
├── README.md
├── includes
│   ├── Common.hpp
│   ├── gl
│   │   ├── Camera.hpp
│   │   ├── Crosshair.hpp
│   │   ├── Manipulator.hpp
│   │   ├── Shader.hpp
│   │   ├── TextureArray.hpp
│   │   └── Window.hpp
│   ├── glad
│   ├── math
│   │   ├── Mat4.hpp
│   │   ├── Vec2.hpp
│   │   ├── Vec3.hpp
│   │   └── Vec4.hpp
│   ├── stb
│   ├── voxel
│   │   ├── ChunkData.hpp
│   │   ├── ChunkMesh.hpp
│   │   └── VoxCommon.hpp
│   └── world
│       ├── DrawWorld.hpp
│       └── TerrainGenerator.hpp
├── scripts
│   ├── install_glad.sh
│   └── install_stb.sh
├── shaders
│   ├── crosshair.frag
│   ├── crosshair.vert
│   ├── fragment.glsl
│   └── vertex.glsl
├── srcs
└── textures
```

## Main features

### Mandatory
- OpenGL 3.3 Core Profile rendering
- Model, view, and projection transformations
- Perspective projection with aspect-ratio correction
- Depth testing
- Texture mapping
- Load and apply image textures using OpenGL
- Object rotation, translation, and scaling using keyboard controls
- GLSL vertex and fragment shaders

### Bonus: 


### Bonus: 


### Bonus: 


### Additional Feature: Mesh Diagnostics


### Demonstration

- Planar star: Demonstrates the difference between Triangle Fan and Ear Clipping triangulation.
- Non-coplanar pentagon: Demonstrates triangulation and rendering of a non-coplanar polygon.
- Non-coplanar star: Demonstrates the combined handling of concave and non-coplanar geometry.

Triangle Fan triangulation is retained for comparison during evaluation.
To compare Triangle Fan and Ear Clipping triangulation, switch the function call in Object::setVertexData() and rebuild the project.

By default, Ear Clipping triangulation is enabled.

Limitation: The triangulation assumes that the polygon has a valid, non-self-intersecting 2D projection. Arbitrary non-coplanar polygons may require additional handling.

## Controls

The object can be manipulated using the following keyboard controls:

| Key | Action |
|---|---|
| `←` / `→` | Turn left and right |
| `↑` / `↓` | Turn up and down |
| `A` / `D` | Move along the X-axis |
| `W` / `S` | Move along the Z-axis |
| `SPACE` / `RIGHT SHIFT` | Move along the Y-axis |
| `C` / `V` | Increase / decrease texture repetition |
| `ESC` | Close the window |

## Resources

- [LearnOpenGL](https://learnopengl.com/)
- [GLFW Documentation](https://www.glfw.org/docs/latest/quick.html)
- [OpenGL Reference](https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf)
- [GLAD](https://glad.dav1d.de)
- [std::isfinite](https://cpprefjp.github.io/reference/cmath/isfinite.html)
- [Area of Polygon](https://gihyo.jp/dev/serial/01/geometry/0008)
- [Normal vector of a polygon](https://web.ist.utl.pt/~antonio.menezes.leitao/Docs/Books/BookJulia/Parametric_Representation/Surface_Normals/index.html)
- [mktemp, trap usage](https://www.linuxmaster.jp/linux_skill/2026/05/mktemp.html)
- [std::array<>](https://cpprefjp.github.io/reference/array/array.html)
- [Texture sample](https://minecraftjapan.miraheze.org/wiki/)
- [Frustum Culling](https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling)
- [Making map](https://www.redblobgames.com/maps/terrain-from-noise/?utm_source=chatgpt.com)
- [Making mountain](https://devforum.roblox.com/t/creating-procedural-mountains-a-fractal-noise-tutorial/1494362?utm_source=chatgpt.com)

## AI Usage

AI was used as a learning and development support tool for the following tasks:

- Understanding OpenGL concepts and the rendering pipeline
- Understanding model, view, and projection transformations
- Reviewing class responsibilities and code structure
- Understanding the OBJ and BMP file formats
- Understanding UV mapping and texture stretching
- Debugging and reviewing code
- Improving documentation and README structure
