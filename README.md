# CS473 Project 2

## Overview
This project is a graphics application developed primarily in C++ using OpenGL and GLFW. It sets up an environment for rendering 3D objects, handling user inputs, and displaying various shapes and models.

## Features
- **Environment Initialization**: Sets up a GLFW window for rendering.
- **Input Handling**: Processes user inputs from the keyboard and mouse to navigate the 3D space.
- **Shader Programs**: Uses GLSL shaders for rendering shapes and text.
- **Vertex Array Objects (VAOs)**: Manages different types of objects, including basic shapes, textured shapes, and imported shapes.
- **3D Models**: Imports and renders 3D models such as a "smiley", a "die", and a "ship".
- **Camera**: Provides camera controls for moving forward, backward, left, and right.
- **Lighting**: Sets up lighting parameters to illuminate the 3D objects.
- **Render Loop**: Continuously updates the scene based on user inputs and renders the objects.

## Getting Started
### Prerequisites
- **C++ Compiler**: Ensure you have a C++ compiler installed.
- **GLFW**: Install the GLFW library.
- **GLM**: Install the GLM library.
- **OpenGL**: Ensure you have OpenGL installed on your system.

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/hmamawal/CS473_PROJECT2.git
   cd CS473_PROJECT2
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Run the executable:
   ```bash
   ./CS473_PROJECT2
   ```

## Usage
- Use the `W`, `A`, `S`, `D` keys to move the camera forward, backward, left, and right, respectively.
- Use the mouse to look around.

## Project Structure
- `main.cpp`: Main entry point of the application.
- `utilities/`: Contains utility functions and classes.
- `classes/`: Contains various classes for handling shapes, fonts, and imported objects.
- `shaders/`: Contains GLSL shader files.
- `models/`: Contains 3D model files.
- `textures/`: Contains texture files.
- `fonts/`: Contains font files.

## License
This project is licensed under the MIT License.

## Acknowledgments
- **GLFW**: For providing the window and input handling library.
- **GLM**: For providing the mathematics library for graphics applications.
- **OpenGL**: For providing the graphics rendering API.

## Author
- **[Your Name]** - Initial work - [hmamawal](https://github.com/hmamawal)