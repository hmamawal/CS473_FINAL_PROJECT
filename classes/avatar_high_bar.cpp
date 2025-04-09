#include "avatar_high_bar.hpp"
#include <glm/gtc/matrix_transform.hpp>

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                           glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize the toggle state variables
    this->is_raised = false;
    this->original_position = initial_position;
    this->key_pressed = false;
    
    // Initialize X-axis rotation animation variables
    this->is_rotating_around_bar = false;
    this->x_rotation_angle = 0.0f;
    this->time_since_move = 0.0f;
    this->m_key_pressed = false;
    this->default_position = initial_position;
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // Check for rotation toggle with M key
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!m_key_pressed) {
            m_key_pressed = true;
            
            // If we're stopping rotation, reset position to default
            if (is_rotating_around_bar) {
                position = default_position;
                x_rotation_angle = 0.0f;
            } else {
                // Store initial position when starting rotation
                original_position = position;
                x_rotation_angle = 0.0f;
                time_since_move = 0.0f;
            }
            
            // Toggle rotation state
            is_rotating_around_bar = !is_rotating_around_bar;
        }
    } else {
        m_key_pressed = false;
    }
    
    // Check for J key to stop rotation and perform J key functionality
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!key_pressed) {
            key_pressed = true;
            
            // Only allow J key functionality if not rotating
            if (!is_rotating_around_bar) {
                // Original J key toggle functionality
                if (!is_raised) {
                    if (position.y == original_position.y) {
                        // Raise the avatar
                        this->position.y += 1.0f;
                        is_raised = true;
                    } else {
                        // Lower the avatar
                        this->position.y = original_position.y;
                        is_raised = false;
                    }
                    
                } else {
                    this->position.y = original_position.y;
                    is_raised = false;
                }
            }
        }
    } else {
        key_pressed = false;
    }
    
    // Handle X-axis rotation animation if active
    if (is_rotating_around_bar) {
        // Accumulate time since last movement
        time_since_move += time_passed;
        
        // Move every 0.01 seconds
        if (time_since_move >= 0.01f) {
            // Reset timer
            time_since_move = 0.0f;
            
            // Perform movement based on current rotation quadrant
            if (x_rotation_angle < 90.0f) {
                // First quadrant: move z by -0.2, move y by 0.15, rotate by 5.0 around X
                position.z -= 0.2f;
                position.y += 0.15f;
                x_rotation_angle += 5.0f;
            } 
            else if (x_rotation_angle < 180.0f) {
                // Second quadrant: move z by 0.2, move y by 0.15, rotate by 5.0 around X
                position.z += 0.2f;
                position.y += 0.15f;
                x_rotation_angle += 5.0f;
            } 
            else if (x_rotation_angle < 270.0f) {
                // Third quadrant: move z by 0.2, move y by -0.15, rotate by 5.0 around X
                position.z += 0.2f;
                position.y -= 0.15f;
                x_rotation_angle += 5.0f;
            } 
            else if (x_rotation_angle < 360.0f) {
                // Fourth quadrant: move z by -0.2, move y by -0.15, rotate by 5.0 around X
                position.z -= 0.2f;
                position.y -= 0.15f;
                x_rotation_angle += 5.0f;
            }
            
            // Reset angle if we complete a full circle
            if (x_rotation_angle >= 360.0f) {
                x_rotation_angle = 0.0f;
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
    
    if (is_rotating_around_bar) {
        // Apply X-axis rotation based on the x_rotation_angle
        local = glm::rotate(local, glm::radians(x_rotation_angle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    // Print the current X-axis rotation angle
    //std::cout << "Current X-axis rotation angle: " << x_rotation_angle << " degrees" << std::endl;
    // print the this-initial rotation
    std::cout << "Initial rotation: " << this->initial_rotation << " degrees" << std::endl;

    // Always apply the initial rotation around Y (from parent class)
    local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::scale(local, this->scale);
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->body.Draw();
}

void AvatarHighBar::GetXRotationAngle(float &angle) {
    angle = this->x_rotation_angle;
}

void AvatarHighBar::GetPosition(glm::vec3 &pos) {
    pos = this->position;
}