Readme:
# CS473 Project 3: Gymnastics Simulator

## Overview
This project is a graphics application developed primarily in C++ using OpenGL. It simulates a gymnastics environment with various apparatus and avatars, allowing for interactive control of the camera, HUD, screen view, light direction, and gymnastics movements.

## Features
- **Gymnastics Environment**: Includes tumbling floor, vault table, high bar, and pommel horses.
- **Interactive Avatars**: Control avatars to perform gymnastics moves like backflips and giants.
- **First-Person View**: Toggle between standard and first-person camera perspectives.
- **Dynamic Lighting**: Adjustable directional light (color, on/off, and directional) and spotlight (on/off; serves as flashlight that moves with the player).
- **Environment Model**: Includes a Lou Gross building model with custom texturing.
- **Environment Initialization**: Sets up a GLFW window for rendering.
- **Input Handling**: Processes user inputs from the keyboard and mouse to navigate and control objects in the 3D space.
- **Shader Programs**: Uses GLSL shaders for rendering shapes, text, lighting, and shadows.
- **Vertex Array Objects (VAOs)**: Manages different types of objects, including basic shapes, textured shapes, and imported shapes.
- **Camera**: Provides camera controls for moving in the 3D environment.
- **HUD**: Displays camera position, light direction, and directions for using 'H' to toggle HUD.
- **Post Processing**: Applies effects to the rendered scene.
- **Cube Map**: Implements a cube map for environmental reflections (surroundings).

## Controls

### Camera & Navigation Controls
- `W` - Move camera forward
- `A` - Move camera left
- `S` - Move camera backward
- `D` - Move camera right
- `Mouse` - Look around (rotate camera)
- `C` - Toggle first-person view (puts camera at avatar's eye level on high bar)

### Avatar Controls
**Main Avatar (on floor):**
- `B` - Make the avatar do a backflip

**High Bar Avatar:**
- `J` - Make the avatar "jump" up to and off of the high bar
- `M` - Make the avatar do swings / "Giants" around the bar or stop doing them

### Lighting Controls
- `P` - Toggle directional light on/off
- `R` - Toggle directional light color (white/blue)
- `Arrow Keys` - Move directional light (up, down, left, right)
- `L` - Toggle spotlight on/off 

### HUD Controls
- `H` - Toggle HUD on/off 

### Post-Processing Effects
- `0` - No effect (default)
- `1` - Grayscale effect
- `2` - Blur effect
- `3` - Lighten effect (increases brightness by 30%)

### System Controls
- `ESC` - Exit application

## Tips & Information
- In first-person view (C key), you'll see from the high bar avatar's perspective
  - Before you toggle C, ensure you're already looking the direction of the Avatar. There is currently a camera issue.
- Camera position coordinates are displayed at the top right of the screen
- The apparatus includes: tumbling floor, vault table, high bar, and pommel horses
- The environment includes a Lou Gross building model with some cool texturing

## Getting Started
### Prerequisites
- **C++ Compiler**: Ensure you have a C++ compiler installed.
- **GLFW**: Install the GLFW library.
- **GLM**: Install the GLM library.
- **OpenGL**: Ensure you have OpenGL installed on your system.

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/hmamawal/CS473_FINAL_PROJECT.git
   cd CS473_FINAL_PROJECT
   ```

2. Build the main file with g++.

3. Run the executable:
   ```bash
   main.exe
   ```

## Project Structure
- `main.cpp`: Main entry point of the application.
- `utilities/`: Contains utility functions and classes.
- `classes/`: Contains various classes for handling shapes, fonts, and imported objects.
- `shaders/`: Contains GLSL shader files.
- `fonts/`: Contains font files for rendering text.
- `models/`: Contains 3D model files.
- `textures/`: Contains texture files.
- `fonts/`: Contains font files.

## Author
- **[Hunter Mamawal]** [hmamawal](https://github.com/hmamawal)

