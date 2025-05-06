#include "avatar.hpp"
#include <glm/gtc/matrix_transform.hpp>


Avatar::Avatar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state) {
    this->body = shape;
    this->initial_rotation = orientation;
    this->current_rotation = 0.0;
    this->position = initial_position;
    this->shader_state = shader_state;
    //both speed and rotate_speed could be separate parameters
    this->speed = 5.0;
    this->rotate_speed = 90.0;
    this->scale = glm::vec3(1.0);
    this->is_flipping = false;
    this->flip_progress = 0.0f;
    this->flip_height = 0.0f;
    this->jump_height = 5.0f;  // Increased from 3.0f for more dramatic effect
    this->flip_duration = 1.2f; // Slightly longer to give time for the full sequence
}


void Avatar::ProcessInput(GLFWwindow *window, float time_passed) {
    
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !is_flipping) {
        is_flipping = true;
        flip_progress = 0.0f;
    }
    // Handle ongoing flip animation
    if (is_flipping) {
        flip_progress += time_passed / flip_duration;
        
        // Modified height curve - rises quickly then falls
        // Use a modified sine curve that front-loads the jump
        if (flip_progress < 0.3f) {
            // Rise quickly to ~90% of height in the first 30% of animation
            flip_height = jump_height * (flip_progress / 0.3f) * 0.9f;
        } else if (flip_progress < 0.8f) {
            // Hold near max height during main rotation
            flip_height = jump_height * 0.9f + (jump_height * 0.1f * sin((flip_progress - 0.3f) * 3.14159f / 0.5f));
        } else {
            // Fall for the last 20%
            flip_height = jump_height * (1.0f - ((flip_progress - 0.8f) / 0.2f));
        }
        
        // Reset when animation completes
        if (flip_progress >= 1.0f) {
            is_flipping = false;
            flip_height = 0.0f;
            flip_progress = 0.0f;
        }
    }

}

void Avatar::Scale(glm::vec3 new_scale) {
    this->scale = new_scale;
}

void Avatar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        shader->use();
    }
    glm::mat4 local(1.0);
    local = glm::translate(local, this->position);
    
    // Add vertical offset for jump
    if (is_flipping) {
        local = glm::translate(local, glm::vec3(0.0f, flip_height, 0.0f));
        
        // Start rotation only after initial rise (20% into animation)
        // Complete rotation before landing (90% of animation)
        if (flip_progress > 0.2f && flip_progress < 0.9f) {
            // Map 0.2-0.9 range to 0-1 for rotation progress
            float rotation_progress = (flip_progress - 0.2f) / 0.7f;
            // Apply faster rotation in middle of jump
            local = glm::rotate(local, glm::radians(rotation_progress * 360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        } else if (flip_progress >= 0.9f) {
            // Keep final rotation
            local = glm::rotate(local, glm::radians(360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
    
    local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::scale(local, this->scale);
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->body.Draw();
}


Avatar::~Avatar() {
    this->body.DeallocateShape();
}