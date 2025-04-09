hmamawal: change the draw function to apply the transformations associated with having pressed the 'M' key. keep in mind that the already present draw functionality is from another function and does not need to be strictly followed

GitHub Copilot: ## Solution for updating the AvatarHighBar Draw function

Let me revise the Draw function to correctly handle the transformations when the 'M' key has been pressed. The current implementation has variables like `flip_height` and `flip_progress` that are referenced but not defined anywhere in the provided code.

I'll modify the Draw function to directly use the rotation_angle variable that's already being updated in the ProcessInput method, making it more consistent with the existing rotation logic.

### [avatar_high_bar.cpp](file:///c%3A/Users/hunter.mamawal/Classes/CS473_Computer_Graphics/CS473_PROJECT2/classes/avatar_high_bar.cpp)

Updating the Draw function to handle the 'M' key rotation properly:

````cpp
// ...existing code...

void AvatarHighBar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        shader->use();
    }
    glm::mat4 local(1.0);
    local = glm::translate(local, this->position);
    
    if (is_rotating) {
        // Apply X-axis rotation based on the rotation_angle
        local = glm::rotate(local, glm::radians(rotation_angle), glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        // Apply standard Y-axis rotation when not in rotation mode
        local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
    }
    
    // Always apply the initial rotation
    local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::scale(local, this->scale);
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->body.Draw();
}
````

This change simplifies the Draw function by directly using the rotation_angle that's updated in the ProcessInput method. It applies an X-axis rotation when is_rotating is true, and the standard Y-axis rotation otherwise. The initial rotation is applied in both cases.

Made changes.

hmamawal: ensure the 'J' key functionality cannot be used if the character is in active roation mode

GitHub Copilot: ## Solution to disable J key functionality during rotation

I'll modify the `ProcessInput` method to prevent the J key functionality from working when the character is in active rotation mode.

### [avatar_high_bar.cpp](file:///c%3A/Users/hunter.mamawal/Classes/CS473_Computer_Graphics/CS473_PROJECT2/classes/avatar_high_bar.cpp)

I'll update the J key handling to check if rotation is active before processing the J key functionality:

````cpp
// ...existing code...

    // Check for J key to stop rotation and perform J key functionality
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!key_pressed) {
            key_pressed = true;
            
            // Only allow J key functionality if not rotating
            if (!is_rotating) {
                // Original J key toggle functionality
                if (!is_raised) {
                    this->position.y += 1.0f;
                    is_raised = true;
                } else {
                    this->position.y = original_position.y;
                    is_raised = false;
                }
            }
            // Note: Removed the code that stops rotation when J is pressed
        }
    } else {
        key_pressed = false;
    }
    
    // ...existing code...
````

This change ensures that the J key functionality (raising and lowering the avatar) cannot be used while the character is in rotation mode. I've also removed the code that was previously stopping rotation when J was pressed, since we want to keep rotation active until M is pressed again.

Made changes.

hmamawal: ensure if rotation is happening then the 'M' key is pressed, the character goes back to it's default position

GitHub Copilot: I'll modify the code to ensure that when the 'M' key is pressed to stop rotation, the character returns to its default position.

### [avatar_high_bar.cpp](file:///c%3A/Users/hunter.mamawal/Classes/CS473_Computer_Graphics/CS473_PROJECT2/classes/avatar_high_bar.cpp)

```cpp
// ...existing code...

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // Check for rotation toggle with M key
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!m_key_pressed) {
            m_key_pressed = true;
            
            // If we're stopping rotation, reset position to default
            if (is_rotating) {
                position = default_position;
                rotation_angle = 0.0f;
            } else {
                // Store initial position when starting rotation
                original_position = position;
                rotation_angle = 0.0f;
                time_since_move = 0.0f;
            }
            
            // Toggle rotation state
            is_rotating = !is_rotating;
        }
    } else {
        m_key_pressed = false;
    }
    
    // ...existing code...
```

This change ensures that when the M key is pressed while the character is rotating (toggling is_rotating from true to false), the character's position is reset to the default_position that was stored during initialization. This will make the character return to its original position when rotation is stopped.

Made changes.