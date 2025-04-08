#include "avatar_high_bar.hpp"

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                           glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize the toggle state variables
    this->is_raised = false;
    this->original_position = initial_position;
    this->key_pressed = false;
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // Call the parent class ProcessInput to maintain original functionality
    Avatar::ProcessInput(window, time_passed);
    
    // Check for J key press
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        // Only toggle if the key wasn't already pressed
        if (!key_pressed) {
            key_pressed = true;
            
            if (!is_raised) {
                // Move up by 1.0
                this->position.y += 1.0f;
                is_raised = true;
            } else {
                // Move back to original height
                this->position.y = original_position.y;
                is_raised = false;
            }
        }
    } else {
        // Reset key state when key is released
        key_pressed = false;
    }
}