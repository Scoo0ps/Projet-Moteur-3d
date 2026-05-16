# 🎮 Moteur 3D - PRAP (Custom C++ 3D Rasterization Engine)

[![C++](https://img.shields.io/badge/C++-11%2F14%2F17-blue.svg?style=flat&logo=c%2B%2B)](https://isocpp.org/)
[![SDL2](https://img.shields.io/badge/Library-SDL2-violet.svg?style=flat&logo=sdl)](https://www.libsdl.org/)
[![Doxygen](https://img.shields.io/badge/Docs-Doxygen-darkgreen.svg?style=flat)](https://www.doxygen.nl/)

A from-scratch **3D Software Rasterization Engine** written entirely in pure, modern C++ using **SDL2** for low-level pixel buffering. This project bypasses hardware APIs like OpenGL or DirectX, handling the complete mathematics behind matrix transformations, perspective projection, triangle clipping, visibility sorting, and scanline rendering natively on the CPU.

---

## 📌 Features
* **Custom Mathematics Pipeline:** Fully native implementation of vector algebra including 3D/2D points, scalar/cross products, addition, subtraction, and normalization.
* **CPU Perspective Projection:** Simulates accurate focal scaling based on a customizable field of view (FoV), near plane configurations, and camera coordinate conversions.
* **Advanced Rasterization & Scanline Filling:** Implements a robust top/bottom split triangle fill algorithm using a dynamic edge-slope scanline approach.
* **Double Depth-Buffering / Visibility Management:** Evaluates and implements both the **Painter's Algorithm** (average depth grouping) and an accurate pixel-level **Z-Buffer** to successfully eliminate rendering overlap artifacts.
* **Procedural Primitive Generation:** Native algorithms to generate meshes for geometric shapes like adjustable cubes (`Pave3d`) and parametric spheres (`Sphere3d` split via configurable meridians and parallels).
* **Asynchronous Camera Control:** Smooth, multi-axis spatial exploration using un-buffered asynchronous keyboard states.

---

## 📐 The Transformation & Graphics Pipeline

Every 3D vertex mapped onto the scene progresses through an operational processing flow manually controlled within the source code:

1. **Camera-Space Conversion:** Centers global coordinates relative to the moving eye camera matrix ($P' = P - C$).
2. **Perspective Projection Mapping:** Coordinates shrink based on distance scale and camera viewing configurations:
$$x_{screen} = \frac{x \cdot fov}{z} + \frac{width}{2}$$
$$y_{screen} = \frac{-y \cdot fov}{z} + \frac{height}{2}$$
3. **Triangle Splitting:** Triangles are sorted vertically by their $y$-coordinates and split into flat-top and flat-bottom pairs for stable horizontal scanline loop rendering.
4. **Z-Buffer Filtering & Presenting:** Pushes validated pixels to a streaming `SDL_Texture` context matching the target 60 FPS refresh configurations.

---

## 🏗️ Architecture & Class Structure

The framework is strictly categorized under the unified `Engine3D` namespace to support modular design constraints. 

* **`Sdl`**: Encapsulates window contexts, direct low-level pixel buffer access (`uint32_t*`), and depth array buffer clearing mechanisms.
* **`Camera`**: Tracks position vectors, orientation pointers, and spatial transformation offsets.
* **`Scene`**: Central node controlling the graphics pipeline, back-face culling, depth sorting, and rendering calls.
* **`Geometry`**: The mathematical core modeling objects (`Point3d`, `Point2d`, `Triangle3d`, `Quad3d`, `Pave3d`, `Sphere3d`).

---

## ⌨️ Controls
* **Arrow Keys ($\uparrow, \downarrow, \leftarrow, \rightarrow$):** Move the camera forwards, backwards, or strafe side-to-side.
* **Spacebar:** Fly upwards.
* **Left Control:** Descend downwards.
* **Escape:** Terminate engine environment instantly.

---

## 🛠️ Build and Setup

### Prerequisites
Make sure you have **CMake**, a **C++ standard compiler** (GCC/Clang/MSVC), and the **SDL2 development library** installed locally on your system.

### Build Instructions
```bash
# Clone the repository
git clone [https://github.com/YourUsername/Moteur3D-PRAP.git](https://github.com/YourUsername/Moteur3D-PRAP.git)
cd Moteur3D-PRAP

# Configure and compile using CMake
mkdir build && cd build
cmake ..
cmake --build .

# Launch the software engine
./Moteur3D_PRAP
