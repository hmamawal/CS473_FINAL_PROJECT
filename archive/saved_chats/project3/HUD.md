hmamawal: #codebase for this project, i want to make a heads up display. what would be good things to display on this heads up display? the camera position is already shown. i was thinking that a prompt to the user describing the buttons and also a counter that says how many flips they've made the avatars do would be good as well. what do you think?

GitHub Copilot: # Creating an Enhanced Heads-Up Display for Your Project

Your idea for expanding the heads-up display is excellent! A good HUD should provide useful information while not overwhelming the user. Based on your codebase, here are some recommendations:

## Good HUD Elements to Add

### 1. Controls Guide
Showing available controls is very helpful for new users. You could display:
- Movement controls (WASD, arrow keys)
- Camera controls (mouse)
- Action keys for the gymnast (space for jump, etc.)
- Special commands (R for reset position, etc.)

### 2. Performance Metrics
The flip counter you suggested would be great! Other performance metrics could include:
- Total flips completed (front/back/side flips)
- Current rotation angle of the gymnast
- Height reached during jumps
- "Stuck landings" counter

### 3. Game State Information
- Current apparatus being used (high bar, pommel horse, etc.)
- Current exercise mode
- Session time/duration
- Score based on performance (if implementing a scoring system)

### 4. Technical Information
- Current frame rate (FPS)
- Current post-processing effect active
- Debug information (when in debug mode)

## Implementation Approach

You can extend the existing `renderText` function in your rendering system. Here's how you might approach this:

```cpp
void renderText(Shader* font_program, Font& arial_font, const Camera& camera, 
                const Avatar& baseAvatar, const AvatarHighBar* high_bar_avatar,
                int current_effect, int flip_count) {
    font_program->use();
    
    // Camera position (already implemented)
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);
    
    // Controls guide
    std::string controls = "Controls: WASD=Move, Space=Jump, R=Reset Camera";
    arial_font.DrawText(controls, glm::vec2(-0.9, 0.9), *font_program);
    
    // Performance metrics
    std::string flips = "Flips: " + std::to_string(flip_count);
    arial_font.DrawText(flips, glm::vec2(-0.9, 0.8), *font_program);
    
    // Current effect
    std::string effect = "Effect: ";
    switch(current_effect) {
        case 0: effect += "None"; break;
        case 1: effect += "Invert"; break;
        case 2: effect += "Grayscale"; break;
        // Add other effects
    }
    arial_font.DrawText(effect, glm::vec2(-0.9, 0.7), *font_program);
    
    // FPS counter (update every second)
    static int frame_count = 0;
    static float last_fps_time = 0.0f;
    static float fps = 0.0f;
    
    frame_count++;
    float current_time = glfwGetTime();
    if (current_time - last_fps_time >= 1.0f) {
        fps = frame_count / (current_time - last_fps_time);
        frame_count = 0;
        last_fps_time = current_time;
    }
    
    std::string fps_string = "FPS: " + std::to_string(static_cast<int>(fps));
    arial_font.DrawText(fps_string, glm::vec2(0.7, 0.9), *font_program);
}
```

## Tracking Flips

To track flips, you'd need to add logic to your avatar classes to detect complete rotations. A basic approach could be:

1. Add a `flip_counter` variable to your `Avatar` and `AvatarHighBar` classes
2. Monitor rotation angles in the movement logic
3. When a full 360° rotation occurs around an appropriate axis, increment the counter
4. Pass this counter to your HUD rendering function

## Implementation Notes

1. **Text Positioning**: Be careful with text positioning to avoid overlap and ensure readability
2. **Performance Impact**: Text rendering can be expensive, so monitor performance
3. **Toggleable HUD**: Consider adding a key to toggle the HUD on/off
4. **Scaling**: Ensure text scales properly with different screen resolutions
5. **Color Coding**: Use different colors for different types of information

Would you like me to provide more detailed code for any particular aspect of this HUD implementation?

hmamawal: #codebase how can i make a toggle to turn off the heads up display text? i want to press "H" to toggle this feature on and off

GitHub Copilot: # Adding a HUD Toggle Feature

You can add a toggle feature for your heads-up display using the 'H' key. Here's how to implement it:

## 1. Add a Global Variable

First, you'll need to add a boolean flag to track whether the HUD is visible. Add this to your input_handling.hpp file where you keep other toggle variables like `spotlight_on` and `point_light_on`:

```cpp
// In input_handling.hpp
extern bool hud_visible;
```

And initialize it in input_handling.cpp:

```cpp
// In input_handling.cpp
bool hud_visible = true; // HUD is visible by default
```

## 2. Add the Key Callback

Update your key callback function in input_handling.cpp to handle the 'H' key press:

```cpp
// In process_input or your key callback function in input_handling.cpp
if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !h_key_pressed) {
    hud_visible = !hud_visible;
    h_key_pressed = true;
}
else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) {
    h_key_pressed = false;
}
```

Also add the new state variable at the top of the file:

```cpp
// In input_handling.cpp, with your other key state variables
bool h_key_pressed = false;
```

## 3. Modify the renderText Function

Update your renderText function in rendering.cpp to check the hud_visible flag before drawing any text:

```cpp
void renderText(Shader* font_program, Font& arial_font, const Camera& camera) {
    // If HUD is toggled off, don't render any text
    if (!hud_visible) {
        return;
    }
    
    font_program->use();
    
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);
    
    // Add a hint about toggling the HUD
    arial_font.DrawText("Press 'H' to toggle HUD", glm::vec2(-0.95, -0.95), *font_program);
}
```

## 4. Update the Function Declaration in rendering.hpp

Make sure to update the function declaration in rendering.hpp to match:

```cpp
// In rendering.hpp
void renderText(Shader* font_program, Font& arial_font, const Camera& camera);
```

## 5. Add to Main.cpp

Don't forget to call the renderText function conditionally in your main render loop:

```cpp
// In your main render loop
if (hud_visible) {
    renderText(font_program_ptr, arial_font, camera);
}
```

However, since you already call renderText directly in your main loop, you don't need to modify this part - the check inside renderText will handle it.

This implementation lets you toggle the HUD on and off with the 'H' key. When the HUD is toggled off, no text will be rendered on the screen, giving you a cleaner view of your scene. When it's toggled on, your camera position and any other information you add will be displayed.

hmamawal: right now, i have to hold the H key to make the HUD appear. i want it to be visible always until i press the H key a single time. 

GitHub Copilot: # Fixing the HUD Toggle Behavior

I see the issue in your code. Currently, the HUD is visible only when you're holding the H key because of how your input handling is implemented. Let's fix this to make it a proper toggle:

```cpp
// Modify the H key handling section (around line 270-280):

    // HUD toggle with H key
    static bool h_key_pressed = false;  // Move this to be static like the others
    
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        if (!h_key_pressed) {
            h_key_pressed = true;
            hud_visible = !hud_visible;  // Toggle the state
            std::cout << "HUD toggled " << (hud_visible ? "ON" : "OFF") << std::endl;
        }
    } else {
        h_key_pressed = false;
    }
```

The problem was in these lines:

```cpp
if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
    if (!hud_visible) {
        hud_visible = true;
        std::cout << "HUD is now visible" << std::endl;
    }
} else {
    hud_visible = false;  // This immediately sets it to false when key is released
}
```

This was setting `hud_visible` to `false` whenever the H key wasn't being pressed, which is why you had to hold it down. The fix I provided:

1. Properly tracks when the H key is first pressed using a static variable
2. Only toggles the HUD state once per key press
3. Doesn't change the state when the key is released

With this change, pressing H once will toggle the HUD off, and pressing H again will toggle it back on. The HUD will remain in its current state until you press H again.

hmamawal: i want to add the l light direction to the heads up display. tell me how to add this

GitHub Copilot: # Adding Light Direction to the HUD Display

You can add the light direction to your heads-up display by modifying the `renderText` function in rendering.cpp. Here's how to implement it:

```cpp
void renderText(Shader* font_program, Font& arial_font, const Camera& camera) {
    
    // If HUD is toggled off, don't render any text
    if (!hud_visible) {
        return;
    }

    font_program->use();
    
    // Camera position text
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);

    // Light direction text
    std::string light_string = "Light Dir (";
    std::string light_x = std::to_string(light_direction.x);
    std::string light_y = std::to_string(light_direction.y);
    std::string light_z = std::to_string(light_direction.z);

    light_string += light_x.substr(0, light_x.find(".") + 3) + ",";
    light_string += light_y.substr(0, light_y.find(".") + 3) + ",";
    light_string += light_z.substr(0, light_z.find(".") + 3) + ")";
    
    arial_font.DrawText(light_string, glm::vec2(-0.1, 0.65), *font_program);

    // Add a hint about toggling the HUD
    arial_font.DrawText("Press 'H' to toggle HUD", glm::vec2(-0.95, -0.95), *font_program);
}
```

This modification:

1. Adds a new string `light_string` to display the light direction vector
2. Creates formatted string values for each component of the light direction
3. Positions the light direction text slightly below the camera position text (at y=0.65 instead of 0.75)
4. Follows the same formatting style as your camera position display

You'll also need to make sure that `light_direction` is accessible in your rendering.cpp file. Since it's already included in input_handling.hpp and you're already including that file in rendering.cpp, it should be accessible.

If you want to include additional information about the light, such as whether it's on/off or its color, you could expand this even further:

```cpp
// Light state and color information
std::string light_state = "Light: ";
light_state += point_light_on ? "ON" : "OFF";
light_state += " | Color: (";
light_state += std::to_string(point_light_color.r).substr(0, 3) + ",";
light_state += std::to_string(point_light_color.g).substr(0, 3) + ",";
light_state += std::to_string(point_light_color.b).substr(0, 3) + ")";

arial_font.DrawText(light_state, glm::vec2(-0.1, 0.55), *font_program);
```

This would add a third line showing whether the light is on or off and what color it's currently set to.

hmamawal: how can i add a semi-transparent rectangle behidn the text?

GitHub Copilot: # Adding a Semi-Transparent Background Behind HUD Text

To add a semi-transparent rectangle behind your HUD text, you'll need to:

1. Create a simple quad/rectangle rendering function
2. Render this rectangle before drawing the text
3. Apply appropriate transparency settings

Here's how to implement it:

## 1. First, add a function to draw a 2D rectangle in rendering.cpp

```cpp
// Add this function before renderText
void renderHUDBackground(Shader* shader_program, float x1, float y1, float x2, float y2, glm::vec4 color) {
    shader_program->use();
    
    // Save current OpenGL state
    GLboolean depth_test_enabled;
    glGetBooleanv(GL_DEPTH_TEST, &depth_test_enabled);
    
    // Disable depth testing temporarily
    glDisable(GL_DEPTH_TEST);
    
    // Set the shader uniforms
    glm::mat4 identity(1.0f);
    shader_program->setMat4("model", identity);
    shader_program->setMat4("view", identity);
    shader_program->setMat4("projection", glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f));
    shader_program->setVec4("color", color);
    shader_program->setInt("shader_state", BASIC); // Use basic shader state
    
    // Create and setup a simple quad VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Define the rectangle vertices (position only)
    float vertices[] = {
        x1, y1, 0.0f,  // Bottom left
        x2, y1, 0.0f,  // Bottom right
        x2, y2, 0.0f,  // Top right
        x1, y2, 0.0f   // Top left
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Draw the rectangle
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Clean up
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // Restore depth testing if it was enabled
    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
}
```

## 2. Modify the renderText function to draw backgrounds first

```cpp
void renderText(Shader* font_program, Font& arial_font, const Camera& camera) {
    
    // If HUD is toggled off, don't render any text
    if (!hud_visible) {
        return;
    }

    // Draw semi-transparent backgrounds for HUD elements
    // Parameters: shader, x1, y1, x2, y2, color (RGBA)
    
    // Background for camera position info
    renderHUDBackground(font_program, -0.15f, 0.73f, 0.4f, 0.8f, glm::vec4(0.0f, 0.0f, 0.2f, 0.5f));
    
    // Background for light direction info
    renderHUDBackground(font_program, -0.15f, 0.63f, 0.4f, 0.7f, glm::vec4(0.0f, 0.0f, 0.2f, 0.5f));
    
    // Background for HUD toggle hint
    renderHUDBackground(font_program, -0.99f, -0.97f, -0.65f, -0.90f, glm::vec4(0.0f, 0.0f, 0.2f, 0.5f));
    
    // Now draw the text as before
    font_program->use();
    
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);

    // Light direction text
    std::string light_string = "Light Dir (";
    std::string light_x = std::to_string(light_direction.x);
    std::string light_y = std::to_string(light_direction.y);
    std::string light_z = std::to_string(light_direction.z);

    light_string += light_x.substr(0, light_x.find(".") + 3) + ",";
    light_string += light_y.substr(0, light_y.find(".") + 3) + ",";
    light_string += light_z.substr(0, light_z.find(".") + 3) + ")";
    
    arial_font.DrawText(light_string, glm::vec2(-0.1, 0.65), *font_program);

    // Add a hint about toggling the HUD
    arial_font.DrawText("Press 'H' to toggle HUD", glm::vec2(-0.95, -0.95), *font_program);
}
```

## 3. Add the function declaration to rendering.hpp

Make sure to add the function signature to your rendering.hpp file:

```cpp
// Add to rendering.hpp
void renderHUDBackground(Shader* shader_program, float x1, float y1, float x2, float y2, glm::vec4 color);
```

## 4. Update the shader to handle color

If your shader doesn't already handle a color uniform, you'll need to add this to your basic shader. In your fragment shader (fragment.glsl), add:

```glsl
// Add to fragment shader
uniform vec4 color;

void main() {
    // ... existing code
    
    // In the BASIC case:
    if (shader_state == 0) { // BASIC
        FragColor = color;
    }
    
    // ... rest of the code
}
```

## Design Tips

1. **Adjust the coordinates** carefully to fit exactly behind your text. You may need to tweak the x1, y1, x2, y2 values.

2. **Color and opacity** - The glm::vec4 color parameter lets you control both color and transparency. The fourth value is alpha (0.0 = fully transparent, 1.0 = opaque). A value of 0.5 gives 50% transparency.

3. **Create a consistent style** - Use a similar background color for all HUD elements for visual consistency.

4. **Consider performance** - This implementation creates and destroys VAOs for each background. If you have many HUD elements, consider creating a single VAO in your initialization and reusing it.

5. **Grouping elements** - You might want to group related HUD elements under a single background rectangle rather than having separate ones.

6. **Position adaptation** - For more flexibility, you could add a margin parameter to automatically make the background slightly larger than the text area.

This implementation gives you semi-transparent backgrounds for your HUD text that makes it more readable against changing backgrounds while maintaining a clean, modern look.

hmamawal: the boxes for the semi-transparent boxes are not showing up

GitHub Copilot: # Fixing the Semi-Transparent Background Boxes

The issue with your semi-transparent background boxes not appearing is likely related to either the shader or the placement of the text and boxes. Let's address both issues:

## 1. Fix Your Main Fragment Shader

The fragment shader is missing the ability to handle the color parameter for basic rendering. Update your fragment shader to handle the uniform color:

```glsl
// Add near the top of fragment.glsl, with other uniforms
uniform vec4 color;

// Then in the main function - make sure the BASIC shader state handles the color:

//otherwise fragment state is 0 (use the set color)
if (fragment_shader_state == 0) { // BASIC
    FragColor = color;  // Use the color directly for basic rendering
} else {
    FragColor = combined_light * set_color;
}
FragColor = applyPostProcessing(FragColor);
```

## 2. Fix the Position in renderText Function

The positions in your renderText function don't match the text positions. Let's align them:

```cpp
void renderText(Shader* font_program, Font& arial_font, const Camera& camera) {
    
    // If HUD is toggled off, don't render any text
    if (!hud_visible) {
        return;
    }

    // Background for camera position info (positioned at the top of the screen)
    renderHUDBackground(font_program, -0.15f, 0.70f, 0.4f, 0.85f, glm::vec4(0.0f, 0.0f, 0.2f, 0.5f));
    
    // Background for light direction info (positioned below camera info)
    renderHUDBackground(font_program, -0.15f, 0.60f, 0.4f, 0.70f, glm::vec4(0.0f, 0.0f, 0.2f, 0.5f));
    
    // Background for HUD toggle hint (positioned at bottom left)
    renderHUDBackground(font_program, -0.99f, -0.99f, -0.65f, -0.90f, glm::vec4(0.0f, 0.0f, 0.2f, 0.5f));

    font_program->use();
    
    // Draw text on top of backgrounds
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);

    // Light direction text
    std::string light_string = "Light Dir (";
    std::string light_x = std::to_string(light_direction.x);
    std::string light_y = std::to_string(light_direction.y);
    std::string light_z = std::to_string(light_direction.z);

    light_string += light_x.substr(0, light_x.find(".") + 3) + ",";
    light_string += light_y.substr(0, light_y.find(".") + 3) + ",";
    light_string += light_z.substr(0, light_z.find(".") + 3) + ")";
    
    arial_font.DrawText(light_string, glm::vec2(-0.1, 0.65), *font_program);

    // Add a hint about toggling the HUD
    arial_font.DrawText("Press 'H' to toggle HUD", glm::vec2(-0.95, -0.95), *font_program);
}
```

## 3. Check the Shader State

Make sure your `renderHUDBackground` function is correctly setting the shader state to BASIC (0) and passing the proper color. Let's verify the uniform is being set correctly:

```cpp
// In renderHUDBackground function, ensure this line is present:
shader_program->setVec4("set_color", color);  // Try this if "color" doesn't work
```

## 4. Enable Alpha Blending

Make sure alpha blending is enabled before drawing the background rectangles:

```cpp
// Add at the beginning of renderHUDBackground
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// And restore at the end if needed
```

Try these changes and your semi-transparent backgrounds should appear correctly positioned behind your text elements.

hmamawal: even after those changes, the boxes are still not appearing how i would hope. i do see a small black box on the screen, but it is barely noticable

GitHub Copilot: # Fixing the Semi-Transparent HUD Background Boxes

The black box you're seeing suggests that the background boxes are rendering, but either their transparency isn't working properly or their color is not as expected. Let's fix this by:

1. Making the backgrounds more visible
2. Ensuring proper blending
3. Adjusting the shader to handle transparency correctly

Here are the changes to make:

## 1. Fix the renderHUDBackground Function

```cpp
// Modify the renderHUDBackground function to ensure proper blending and color setting

void renderHUDBackground(Shader* shader_program, float x1, float y1, float x2, float y2, glm::vec4 color) {
    // Save current OpenGL state
    GLboolean depth_test_enabled;
    glGetBooleanv(GL_DEPTH_TEST, &depth_test_enabled);
    
    // Set up blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Disable depth testing temporarily
    glDisable(GL_DEPTH_TEST);
    
    // Use the shader
    shader_program->use();
    
    // Set up simple orthographic view
    glm::mat4 identity(1.0f);
    shader_program->setMat4("model", identity);
    shader_program->setMat4("view", identity);
    shader_program->setMat4("projection", glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
    
    // Set color - make sure to set both uniforms
    shader_program->setVec4("color", color);
    shader_program->setVec4("set_color", color);
    shader_program->setInt("shader_state", 0); // BASIC = 0, ensure this matches your enum
    
    // Create quad
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Define the rectangle vertices
    float vertices[] = {
        x1, y1, 0.0f,  // Bottom left
        x2, y1, 0.0f,  // Bottom right
        x2, y2, 0.0f,  // Top right
        x1, y2, 0.0f   // Top left
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Draw
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Clean up
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // Restore previous state
    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
}
```

## 2. Update the renderText Function with More Visible Backgrounds

```cpp
void renderText(Shader* font_program, Font& arial_font, const Camera& camera) {
    // If HUD is toggled off, don't render any text
    if (!hud_visible) {
        return;
    }

    // Use more visible colors for the backgrounds
    glm::vec4 bgColor = glm::vec4(0.1f, 0.1f, 0.3f, 0.7f); // More blue, more visible
    glm::vec4 accentColor = glm::vec4(0.2f, 0.2f, 0.4f, 0.7f); // Slightly different for contrast
    
    // Background for camera position info (positioned precisely)
    renderHUDBackground(font_program, -0.15f, 0.70f, 0.4f, 0.85f, bgColor);
    
    // Background for light direction info
    renderHUDBackground(font_program, -0.15f, 0.60f, 0.4f, 0.70f, accentColor);
    
    // Background for HUD toggle hint
    renderHUDBackground(font_program, -0.99f, -0.99f, -0.65f, -0.90f, bgColor);
    
    // Now draw the text
    font_program->use();
    
    // Camera position
    std::string display_string = "Camera (";
    std::string cam_x = std::to_string(camera.Position.x);
    std::string cam_y = std::to_string(camera.Position.y);
    std::string cam_z = std::to_string(camera.Position.z);

    display_string += cam_x.substr(0, cam_x.find(".") + 3) + ",";
    display_string += cam_y.substr(0, cam_y.find(".") + 3) + ",";
    display_string += cam_z.substr(0, cam_z.find(".") + 3) + ")";
    
    arial_font.DrawText(display_string, glm::vec2(-0.1, 0.75), *font_program);

    // Light direction text
    std::string light_string = "Light Dir (";
    std::string light_x = std::to_string(light_direction.x);
    std::string light_y = std::to_string(light_direction.y);
    std::string light_z = std::to_string(light_direction.z);

    light_string += light_x.substr(0, light_x.find(".") + 3) + ",";
    light_string += light_y.substr(0, light_y.find(".") + 3) + ",";
    light_string += light_z.substr(0, light_z.find(".") + 3) + ")";
    
    arial_font.DrawText(light_string, glm::vec2(-0.1, 0.65), *font_program);

    // Add a hint about toggling the HUD
    arial_font.DrawText("Press 'H' to toggle HUD", glm::vec2(-0.95, -0.95), *font_program);
}
```

## 3. Ensure the Font Shader Handles Color and Transparency Correctly

The problem might also be in the font fragment shader. Make sure it's set up to handle transparency properly. While you already have the fontFragmentShader.glsl shader handling transparency, let's double check its uniform setting in `SetupFontShader`:

```cpp
void SetupFontShader(Shader* font_program_ptr) {
    // Font shader settings
    font_program_ptr->use();
    glm::mat4 identity(1.0);
    font_program_ptr->setMat4("local", identity);
    font_program_ptr->setMat4("model", identity);
    font_program_ptr->setMat4("view", identity);
    font_program_ptr->setMat4("projection", glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0));
    font_program_ptr->setVec4("transparentColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    font_program_ptr->setFloat("alpha", 1.0); // Make sure alpha is set to 1.0 for full opacity
    font_program_ptr->setInt("texture1", 0);
    
    std::cout << "Font shader setup complete" << std::endl;
}
```

## 4. Fix the Drawing Order in main.cpp

Make sure you're drawing the HUD after all other rendering but before the frame buffer swap. This appears to be correct in your code, but let's double check that the HUD is rendered in the correct pass:

```cpp
// In main.cpp, make sure renderText is called at the right point in the render loop
// Display text (HUD) - should be done after the scene is rendered but before swapping buffers
renderText(font_program_ptr, arial_font, camera);
```

These changes should give you more visible, properly transparent background boxes for your HUD. If you're still having issues after implementing these changes, try using very bright colors (like bright red `glm::vec4(1.0f, 0.0f, 0.0f, 0.8f)`) temporarily to confirm the boxes are rendering in the correct positions.