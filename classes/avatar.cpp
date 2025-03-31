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
    this->jump_height = 3.0f;  // Maximum height of jump
    this->flip_duration = 1.0f; // Time to complete flip in seconds
}


void Avatar::ProcessInput(GLFWwindow *window, float time_passed) {
    if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS) {
        float change_x = this->speed * time_passed * cos(glm::radians(this->current_rotation));
        float change_z = this->speed * time_passed * (-sin(glm::radians(this->current_rotation)));
        this->position += glm::vec3(change_x,0.0, change_z);
        if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS) {
            this->current_rotation += this->rotate_speed * time_passed;
        }
        if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS) {
            this->current_rotation += -(this->rotate_speed) * time_passed;
        }
        
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !is_flipping) {
        is_flipping = true;
        flip_progress = 0.0f;
    }
    // Handle ongoing flip animation
    if (is_flipping) {
        flip_progress += time_passed / flip_duration;
        
        // Calculate height using a parabolic curve
        flip_height = jump_height * sin(flip_progress * 3.14159f);
        
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
        // Apply backflip rotation (360 degrees around X-axis)
        local = glm::rotate(local, glm::radians(flip_progress * 360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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