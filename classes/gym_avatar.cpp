#include "gym_avatar.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GymAvatar::GymAvatar(BasicShape shape, float orientation, glm::vec3 initial_position, 
                   int shader_state, glm::vec3 high_bar_pos, float bar_radius)
    : Avatar(shape, orientation, initial_position, shader_state) {
    
    // Initialize high bar properties
    this->high_bar_position = high_bar_pos;
    this->high_bar_radius = bar_radius;
    
    // Initialize state
    this->is_on_high_bar = false;
    this->is_jumping_to_bar = false;
    
    // Initialize animation properties
    this->rotation_angle = 0.0f;
    this->rotation_speed = 120.0f; // Degrees per second
    this->jump_progress = 0.0f;
    this->jump_duration = 1.0f; // 1 second to jump to bar
    this->start_position = initial_position;
    
    // Position gymnast directly under the high bar at ground level
    this->position = glm::vec3(high_bar_position.x, initial_position.y, high_bar_position.z);
}

void GymAvatar::ProcessInput(GLFWwindow *window, float time_passed) {
    // If we're not on the high bar or jumping to it, use normal avatar controls
    if (!is_on_high_bar && !is_jumping_to_bar) {
        // Handle standard avatar movement (from parent class)
        Avatar::ProcessInput(window, time_passed);
        
        // Check for "J" key press to begin high bar interaction
        static bool j_key_pressed = false;
        bool j_key_current = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
        
        if (j_key_current && !j_key_pressed && IsNearHighBar()) {
            std::cout << "J key pressed while near high bar." << std::endl;
            JumpToHighBar();
        }
        j_key_pressed = j_key_current;
    } 
    // Handle high bar interaction
    else if (is_on_high_bar) {
        // Rotate on the high bar
        RotateOnHighBar(time_passed);
        
        // Check for "J" key press again to dismount
        static bool j_key_pressed_dismount = false;
        bool j_key_current = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
        
        if (j_key_current && !j_key_pressed_dismount) {
            std::cout << "J key pressed while on high bar - dismounting." << std::endl;
            DismountFromHighBar();
        }
        j_key_pressed_dismount = j_key_current;
    }
    // Handle jumping to the high bar animation
    else if (is_jumping_to_bar) {
        jump_progress += time_passed / jump_duration;
        
        if (jump_progress >= 1.0f) {
            // Reached the high bar
            jump_progress = 1.0f;
            is_jumping_to_bar = false;
            is_on_high_bar = true;
            
            // Position the avatar at the high bar
            position = high_bar_position;
            // Adjust to grip the bar from above
            position.y = high_bar_position.y + high_bar_radius; // Position hands at the bar
            std::cout << "Reached the high bar!" << std::endl;
        } else {
            // Interpolate position using smooth curve for natural jumping motion
            float t = jump_progress;
            float height_factor = 4.0f * t * (1.0f - t); // Parabolic curve for height
            
            // Keep X and Z fixed since we should be directly under the bar
            position.x = high_bar_position.x;
            position.z = high_bar_position.z;
            
            // Add vertical component with parabolic curve for natural arc
            float target_y = high_bar_position.y + high_bar_radius; // Position at the bar
            float base_y = start_position.y + (target_y - start_position.y) * t;
            position.y = base_y + height_factor * 2.0f; // Add jump height
        }
    }
}

void GymAvatar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        shader->use();
    }
    
    glm::mat4 local(1.0);
    local = glm::translate(local, this->position);
    
    // If on high bar, handle special rotation around the bar
    if (is_on_high_bar) {
        // Create a transformation that properly rotates the gymnast around the high bar
        
        // Reset position to origin first
        local = glm::mat4(1.0f);
        
        // First translate to the high bar position (this is our rotation center)
        local = glm::translate(local, high_bar_position);
        
        // Rotate around the high bar (around X-axis for proper gymnast rotation)
        local = glm::rotate(local, glm::radians(rotation_angle), glm::vec3(1.0f, 0.0f, 0.0f));
        
        // Translate down from bar to position the gymnast's hands at the bar
        // This offset determines where the gymnast's center will be relative to the bar
        local = glm::translate(local, glm::vec3(0.0f, -0.8f, 0.0f));
        
        // Rotate the gymnast model to face correctly while rotating
        local = glm::rotate(local, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    // For regular avatar states, use standard transformations
    else {
        // Apply jump animation if in progress
        if (is_jumping_to_bar) {
            // Add additional rotation to prepare for bar grip - rotate forward around X-axis
            float prep_rotation = jump_progress * 90.0f;
            local = glm::rotate(local, glm::radians(prep_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        
        // Apply standard avatar rotations
        local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
        local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    }
    
    // Apply scaling
    local = glm::scale(local, this->scale);
    
    // Set matrix and shader state
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    
    // Draw the avatar
    this->body.Draw();
}

void GymAvatar::JumpToHighBar() {
    if (!is_on_high_bar && !is_jumping_to_bar) {
        is_jumping_to_bar = true;
        jump_progress = 0.0f;
        start_position = position;
        std::cout << "Jumping to high bar!" << std::endl;
    }
}

void GymAvatar::RotateOnHighBar(float time_passed) {
    // Update rotation around the high bar
    rotation_angle += rotation_speed * time_passed;
    
    // Keep rotation angle within 0-360 for simplicity
    if (rotation_angle >= 360.0f) {
        rotation_angle -= 360.0f;
    }
}

void GymAvatar::DismountFromHighBar() {
    if (is_on_high_bar) {
        is_on_high_bar = false;
        
        // Calculate dismount position based on rotation angle (angle around X-axis)
        float dismount_distance = 3.0f;
        
        // Calculate direction vector based on rotation angle
        // For X-axis rotation, we need to adjust the Y and Z components
        float y_component = -sin(glm::radians(rotation_angle));
        float z_component = -cos(glm::radians(rotation_angle));
        
        // Add distance in the calculated direction
        glm::vec3 dismount_direction(0.0f, y_component, z_component);
        position = high_bar_position + dismount_direction * dismount_distance;
        
        // Ensure landing at ground level
        position.y = std::max(0.4f, position.y); // Don't go below ground level
        
        std::cout << "Dismounting from high bar!" << std::endl;
        
        // Reset rotation
        rotation_angle = 0.0f;
    }
}

bool GymAvatar::IsNearHighBar() const {
    // Calculate distance to the high bar (horizontal distance only)
    float distance = glm::length(
        glm::vec2(position.x - high_bar_position.x, position.z - high_bar_position.z)
    );
    
    // Consider the avatar near the bar if within a certain threshold
    // Increase detection radius to make interaction easier
    return distance < 5.0f; // Within 5 units of the bar (increased from 2)
}