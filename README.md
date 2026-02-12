# 3D Mathematical Graphing Engine

GPU-powered mathematical graphing engine built with **C++** and **OpenGL 4.6**. The engine enables users to input and plot a variety of mathematical functions with interactive capabilities. Built as senior capstone project for CS-4600.

## Features

- **Real-Time Curve Rendering**
- **Interactive Viewport**
- **Dynamic Function Input and Recompilation**
- **Mathematical Function Evaluation**

## Building

### Prerequisites
- C++17 capable compiler
- CMake 3.10+
- OpenGL 4.6 compatible GPU and drivers

### Steps

```bash
# Clone repository
git clone https://github.com/Duccs/CS-4600.git
cd CS-4600

# Configure and build
mkdir build && cd build
cmake ..
cmake --build .
```

GLFW and ImGui are fetched automatically when building with CMake. GLAD is included in the repository under `lib/glad/`.

Compiled executables are found in build directory.
