*This project has been created as part of the 42 curriculum by yotsurud.*

# SCOP

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
    ├── img
    ├── includes
    │   ├── gl
    │   ├── glad        # Automatically generated during build
    │   │   ├── include
    │   │   └── src
    │   ├── math
    │   ├── parser
    │   ├── utils
    │   └── validate
    ├── resources
    ├── shaders
    ├── srcs
    │   ├── gl
    │   ├── math
    │   ├── parser
    │   ├── utils
    │   └── validate
    ├── install_glad.sh
    └── Makefile 
```

## Main features

### Mandatory
- Custom OBJ file parser
- Custom 24-bit BMP image loader
- OpenGL 3.3 Core Profile rendering
- Model, view, and projection transformations
- Perspective projection with aspect-ratio correction
- Depth testing
- Grayscale face coloring
- Texture mapping
- Smooth transition between grayscale and textured rendering using GLSL `mix()`
- Load and apply image textures using OpenGL
- Object rotation, translation, and scaling using keyboard controls
- GLSL vertex and fragment shaders

### Bonus: Complex Polygon Handling
This project supports triangulation of concave polygons using the Ear Clipping algorithm.
For non-coplanar polygons, Newell's Method is used to estimate a representative normal vector and select an appropriate 2D projection plane. Ear Clipping is then performed on the projected vertices, while the original 3D coordinates are preserved for rendering.
- Ear Clipping triangulation for concave polygons
- Non-coplanar polygon handling using Newell's Method for projection-plane selection
- Triangle Fan triangulation for comparison and demonstration

### Bonus: Texture Mapping
This project supports automatic UV coordinate generation using two mapping methods:
- **Axis-based mapping**: Projects texture coordinates onto the XY, XZ, or YZ plane according to the face orientation.
- **Normal-based mapping**: Uses each face's normal vector to construct a local projection basis for UV coordinates.

- Switch between mapping methods and adjust texture repetition using keyboard controls.

Controls:
- T: Toggle texture mapping on / off
- C: Increase texture repetition
- V: Decrease texture repetition
- M: Switch between axis-based and normal-based UV mapping

### Bonus: Magic Pocket
Press P to add a new instance of the loaded object with a randomized position, scale, and rotation axis.

- P: Add a new object instance
- K: Remove all added instances, keeping the first instance
- SPACE: Rotate each instance around its own randomly assigned axis

All instances share the same mesh data (VAO/VBO), avoiding duplicate GPU mesh uploads.

### Additional Feature: Mesh Diagnostics
- Degenerate Triangle Detection
- Face Winding Validation
- Geometric Edge Analysis
- Zero-Length Edge Detection
- Triangle Shape Analysis
- Non-Coplanar Face Detection

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
| `←` / `→` | Rotate around the Y-axis |
| `↑` / `↓` | Rotate around the X-axis |
| `Q` / `E` | Rotate around the Z-axis |
| `A` / `D` | Move along the X-axis |
| `R` / `F` | Move along the Y-axis |
| `W` / `S` | Move along the Z-axis |
| `Z` / `X` | Scale the object up / down |
| `C` / `V` | Increase / decrease texture repetition |
| `SPACE` | Rotate each instance around its own randomly assigned axis |
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

## AI Usage

AI was used as a learning and development support tool for the following tasks:

- Understanding OpenGL concepts and the rendering pipeline
- Understanding model, view, and projection transformations
- Reviewing class responsibilities and code structure
- Understanding the OBJ and BMP file formats
- Understanding UV mapping and texture stretching
- Debugging and reviewing code
- Improving documentation and README structure
