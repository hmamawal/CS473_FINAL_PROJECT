#include "avatar_high_bar.hpp"
#include <glm/gtc/matrix_transform.hpp>

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                           glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize the toggle state variables
    this->is_raised = false;
    this->original_position = initial_position;
    this->key_pressed = false;
    
    // Initialize rotation animation variables
    this->is_rotating = false;
    this->rotation_angle = 0.0f;
    this->time_since_move = 0.0f;
    this->m_key_pressed = false;
    this->default_position = initial_position;
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // Check for rotation toggle with M key
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!m_key_pressed) {
            m_key_pressed = true;
            is_rotating = !is_rotating;
            
            if (is_rotating) {
                // Store initial position when starting rotation
                original_position = position;
                rotation_angle = 0.0f;
                time_since_move = 0.0f;
            }
        }
    } else {
        m_key_pressed = false;
    }
    
    // Check for J key to stop rotation and perform J key functionality
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!key_pressed) {
            key_pressed = true;
            
            // Stop rotation if it's active
            if (is_rotating) {
                is_rotating = false;
            }
            
            // Original J key toggle functionality
            if (!is_raised) {
                this->position.y += 1.0f;
                is_raised = true;
            } else {
                this->position.y = original_position.y;
                is_raised = false;
            }
        }
    } else {
        key_pressed = false;
    }
    
    // Handle rotation animation if active
    if (is_rotating) {
        // Accumulate time since last movement
        time_since_move += time_passed;
        
        // Move every 0.2 seconds
        if (time_since_move >= 0.1f) {
            // Reset timer
            time_since_move = 0.0f;
            
            // Perform movement based on current rotation quadrant
            if (rotation_angle < 90.0f) {
                // First quadrant: move z by -0.4, move y by 0.4, rotate by 10.0 around X
                position.z -= 0.4f;
                position.y += 0.3f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            } 
            else if (rotation_angle < 180.0f) {
                // Second quadrant: move z by 0.4, move y by 0.4, rotate by 10.0 around X
                position.z += 0.4f;
                position.y += 0.3f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            } 
            else if (rotation_angle < 270.0f) {
                // Third quadrant: move z by 0.4, move y by -0.4, rotate by 10.0 around X
                position.z += 0.4f;
                position.y -= 0.3f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            } 
            else if (rotation_angle < 360.0f) {
                // Fourth quadrant: move z by -0.4, move y by -0.4, rotate by 10.0 around X
                position.z -= 0.4f;
                position.y -= 0.3f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            }
            
            // Reset angle if we complete a full circle
            if (rotation_angle >= 360.0f) {
                rotation_angle = 0.0f;
            }
        }
    } else {
        // Only run parent ProcessInput when not rotating
        Avatar::ProcessInput(window, time_passed);
    }
}

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
        //local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
    }
    
    // Always apply the initial rotation
    local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::scale(local, this->scale);
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->body.Draw();
}