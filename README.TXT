# CS473 FINAL PROJECT: GYMNASTICS SIMULATOR

## INSTALLATION

### Prerequisites
- **C++ Compiler**: g++ with C++17 support
- **GLFW Library**: For window and OpenGL context management
- **GLM Library**: For mathematics operations
- **OpenGL**: 3.3 or higher

### Building the Project
1. Clone or download the repository
2. Build using g++:
   ```
   g++ main.cpp classes/*.cpp utilities/*.cpp utilities/glad.c -IC:/msys64/ucrt64/include -lglfw3 -lgdi32 -lglu32 -lopengl32 -o main.exe
   ```
   Or use the provided build task in VS Code.

3. Run the executable:
   ```
   ./main.exe
   ```

## USER CONTROLS

### Camera & Navigation Controls
- **W** - Move camera forward
- **A** - Move camera left
- **S** - Move camera backward
- **D** - Move camera right
- **Mouse** - Look around (rotate camera)
- **C** - Toggle first-person view (puts camera at avatar's eye level on high bar)
  - Note: Before toggling first-person view, ensure you're looking in the direction of the Avatar. There is currently an orientation issue when enabling first-person view.

### Avatar Controls
**Main Avatar (on floor):**
- **B** - Make the avatar do a backflip

**High Bar Avatar:**
- **J** - Make the avatar jump up to and off of the high bar
- **M** - Make the avatar do swings / "Giants" around the bar or stop doing them

### Lighting Controls
- **P** - Toggle directional light on/off
- **R** - Toggle directional light color (white/red)
- **Arrow Keys** - Move directional light (up, down, left, right)
- **L** - Toggle spotlight on/off (flashlight that moves with the camera)

### HUD Controls
- **H** - Toggle HUD on/off (shows camera position, light direction, and controls)

### Post-Processing Effects
- **0** - No effect (default)
- **1** - Grayscale effect
- **2** - Blur effect
- **3** - Lighten effect (increases brightness by 30%)

### System Controls
- **ESC** - Exit application

## FEATURES

- **Gymnastics Equipment**: Tumbling floor, vault table, high bar, and pommel horses
- **Interactive Avatars**: Control avatars to perform gymnastics moves
- **First-Person View**: See the world from the high bar avatar's perspective
- **Dynamic Lighting**: Adjustable directional light and spotlight
- **Shadow Mapping**: Objects cast realistic shadows
- **Post-Processing Effects**: Apply various visual effects to the scene
- **Cubemap Skybox**: Immersive environment surrounding the gymnasium
- **Heads-Up Display (HUD)**: Contextual information and control hints

## TROUBLESHOOTING

1. **Shadow Issues**: If shadows appear "acne-like" or flickering, this is a known limitation. The shadow bias has been adjusted to minimize the effect but may still be present in some angles.

2. **First-Person View**: When toggling to first-person view with the **C** key, ensure you're already looking in the direction of the High Bar Avatar. The camera transition may cause disorientation otherwise.

3. **Performance**: If the application runs slowly, try:
   - Closing other applications
   - Running without post-processing effects (press **0**)
   - Toggling off the spotlight (**L** key) as it's more performance-intensive

## CONTACT

For issues or questions, please contact hmamawal@github.com or open an issue on the GitHub repository.