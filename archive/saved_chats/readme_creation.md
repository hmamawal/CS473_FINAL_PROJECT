hmamawal: combine this readme: <<<=====================================
CS473 PROJECT 2: GYMNASTICS SIMULATOR
=====================================

This application simulates a gymnastics environment with various apparatus and avatars.

--------------------------
CAMERA & NAVIGATION CONTROLS
--------------------------
W       - Move camera forward
A       - Move camera left
S       - Move camera backward
D       - Move camera right
Mouse   - Look around (rotate camera)
C       - Toggle first-person view (puts camera at avatar's eye level on high bar)

--------------------------
AVATAR CONTROLS
--------------------------
Main Avatar (on floor):
B Key       - Make the avatar do a backflip

High Bar Avatar:
J Key       - Make the avatar "jump" up to and off of the high bar
M Key       - Make the avatar do swings / "Giants" around the bar or stop doing them

--------------------------
LIGHTING CONTROLS
--------------------------
R       - Toggle point light color (white/red)

--------------------------
SYSTEM CONTROLS
--------------------------
ESC     - Exit application

--------------------------
TIPS & INFORMATION
--------------------------
- In first-person view (C key), you'll see from the high bar avatar's perspective
   - Before you toggle C, ensure you're already looking the direction of the Avatar. There is currently a camera issue.
- Camera position coordinates are displayed at the top right of the screen
- The apparatus includes: tumbling floor, vault table, high bar, and pommel horses
- The environment includes a Lou Gross building model with some cool texturing

--------------------------
TECHNICAL NOTES
--------------------------
- Point light illuminates the entire scene
- Camera-attached spotlight provides focused lighting
- First-person view locks camera to the avatar's movement
- Minimum camera height is enforced to prevent clipping through the floor>>> with the current one

GitHub Copilot: I'll combine the information from the new gymnastics simulator content with the existing README, ensuring the structure remains cohesive and all the new features and controls are properly documented.

## Step-by-step solution

1. Update the main title and overview to reflect the gymnastics simulator theme
2. Add the new features section with gymnastics apparatus and avatars
3. Add the detailed controls sections (Camera & Navigation, Avatar Controls, Lighting, System)
4. Include the technical notes and tips sections
5. Keep the existing structure for prerequisites, installation, project structure, etc.

### [README.md](file:///c%3A/Users/hunter.mamawal/Classes/CS473_Computer_Graphics/CS473_PROJECT2/README.md)

Update the README to incorporate the gymnastics simulator content while maintaining the structural elements from the original.

````markdown
# CS473 Project 2: Gymnastics Simulator

## Overview
This project is a graphics application developed primarily in C++ using OpenGL and GLFW. It simulates a gymnastics environment with various apparatus and avatars, allowing for interactive control of the camera and gymnastics movements.

## Features
- **Gymnastics Environment**: Includes tumbling floor, vault table, high bar, and pommel horses.
- **Interactive Avatars**: Control avatars to perform gymnastics moves like backflips and giants.
- **First-Person View**: Toggle between standard and first-person camera perspectives.
- **Dynamic Lighting**: Adjustable point light and camera-attached spotlight.
- **Environment Model**: Includes a Lou Gross building model with custom texturing.
- **Environment Initialization**: Sets up a GLFW window for rendering.
- **Input Handling**: Processes user inputs from the keyboard and mouse to navigate the 3D space.
- **Shader Programs**: Uses GLSL shaders for rendering shapes and text.
- **Vertex Array Objects (VAOs)**: Manages different types of objects, including basic shapes, textured shapes, and imported shapes.
- **Camera**: Provides camera controls for moving in the 3D environment.

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
- `R` - Toggle point light color (white/red)

### System Controls
- `ESC` - Exit application

## Tips & Information
- In first-person view (C key), you'll see from the high bar avatar's perspective
  - Before you toggle C, ensure you're already looking the direction of the Avatar. There is currently a camera issue.
- Camera position coordinates are displayed at the top right of the screen
- The apparatus includes: tumbling floor, vault table, high bar, and pommel horses
- The environment includes a Lou Gross building model with some cool texturing

## Technical Notes
- Point light illuminates the entire scene
- Camera-attached spotlight provides focused lighting
- First-person view locks camera to the avatar's movement
- Minimum camera height is enforced to prevent clipping through the floor

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
````

Made changes.