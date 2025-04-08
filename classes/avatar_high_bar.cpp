#include "avatar_high_bar.hpp"
#include <glm/gtc/matrix_transform.hpp>

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state, HighBar* high_bar)
    : Avatar(shape, orientation, initial_position, shader_state) {
    this->high_bar = high_bar;
    this->is_on_bar = false;
    this->is_rotating = false;
    this->rotation_angle = 0.0f;
    this->rotation_speed = 180.0f; // Degrees per second
    this->original_position = initial_position;
    
    // Calculate the bar position (under the bar)
    glm::vec3 bar_pos = high_bar->GetPosition();
    this->bar_position = glm::vec3(bar_pos.x, bar_pos.y, bar_pos.z - 0.5f); // Position below the bar
    
    this->rotation_radius = 0.8f; // Distance from bar to avatar during rotation
    this->rotation_progress = 0.0f;
    this->rotation_duration = 3.0f; // Full rotation duration in seconds
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // If on the bar, handle high bar specific controls
    if (this->is_on_bar) {
        // Toggle on/off the high bar with the J key
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            this->DropFromHighBar();
        }
        
        // Start rotation with the M key
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !this->is_rotating) {
            this->StartRotation();
        }
        
        // Stop rotation with the N key
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && this->is_rotating) {
            this->StopRotation();
        }
        
        // Update rotation if currently rotating
        if (this->is_rotating) {
            this->UpdateRotation(time_passed);
        }
    } else {
        // Normal avatar movement when not on the bar
        Avatar::ProcessInput(window, time_passed);
        
        // Jump to the high bar with the J key if close enough
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            // Check if we're close enough to the high bar
            glm::vec3 bar_pos = this->high_bar->GetPosition();
            float distance = glm::distance(this->position, bar_pos);
            
            if (distance < 5.0f) { // If within 5 units of the high bar
                this->JumpToHighBar();
            }
        }
    }
}

void AvatarHighBar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        shader->use();
    }
    
    glm::mat4 local(1.0);
    local = glm::translate(local, this->position);
    
    // If on the bar and rotating, apply special transformations
    if (this->is_on_bar && this->is_rotating) {
        // No need to reapply position here since it's already set in UpdateRotation
    } else if (this->is_on_bar) {
        // When on the bar but not rotating, adjust the orientation to look like hanging
        local = glm::rotate(local, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    
    // Apply standard rotations
    local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::scale(local, this->scale);
    
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->body.Draw();
}

void AvatarHighBar::JumpToHighBar() {
    if (!this->is_on_bar) {
        // Store the current position as the original position
        this->original_position = this->position;
        
        // Move to the bar position
        this->position = this->bar_position;
        
        // Update state
        this->is_on_bar = true;
        this->is_rotating = false;
        this->rotation_angle = 0.0f;
    }
}

void AvatarHighBar::DropFromHighBar() {
    if (this->is_on_bar) {
        // Return to the original position
        this->position = this->original_position;
        
        // Update state
        this->is_on_bar = false;
        this->is_rotating = false;
    }
}

void AvatarHighBar::ToggleHighBar() {
    if (this->is_on_bar) {
        this->DropFromHighBar();
    } else {
        // Check if we're close enough to the high bar
        glm::vec3 bar_pos = this->high_bar->GetPosition();
        float distance = glm::distance(this->position, bar_pos);
        
        if (distance < 5.0f) { // If within 5 units of the high bar
            this->JumpToHighBar();
        }
    }
}

void AvatarHighBar::StartRotation() {
    if (this->is_on_bar && !this->is_rotating) {
        this->is_rotating = true;
        this->rotation_angle = 0.0f;
        this->rotation_progress = 0.0f;
    }
}

void AvatarHighBar::StopRotation() {
    if (this->is_rotating) {
        this->is_rotating = false;
        
        // Return to hanging position
        this->position = this->bar_position;
    }
}

void AvatarHighBar::UpdateRotation(float time_passed) {
    if (!this->is_rotating) {
        return;
    }
    
    // Update the rotation angle
    this->rotation_angle += this->rotation_speed * time_passed;
    
    // Normalize rotation angle to 0-360 degrees
    while (this->rotation_angle >= 360.0f) {
        this->rotation_angle -= 360.0f;
    }
    
    // Update rotation progress
    this->rotation_progress += time_passed / this->rotation_duration;
    
    // Calculate new position based on rotation around the bar
    float angle_rad = glm::radians(this->rotation_angle);
    
    // Get the high bar position and dimensions
    glm::vec3 bar_pos = this->high_bar->GetPosition();
    float bar_radius = this->high_bar->GetRadius();
    
    // Calculate position relative to the bar using the actual bar dimensions
    // Get the model dimensions if available, otherwise use the predefined rotation radius
    glm::vec3 model_dims = this->high_bar->GetModelDimensions();
    float actual_bar_radius = (model_dims.y > 0) ? model_dims.y / 2.0f : bar_radius;
    
    // Calculate new position with the center of rotation at the high bar
    // Use actual dimensions for more accurate positioning
    float rotation_distance = this->rotation_radius + actual_bar_radius;
    float x = bar_pos.x; // x-coordinate of the high bar
    float y = bar_pos.y + sin(angle_rad) * 0.5f; // y-coordinate with slight swing
    float z = bar_pos.z - rotation_distance * cos(angle_rad); // z-coordinate with rotation
    
    // Update the avatar position
    this->position = glm::vec3(x, y, z);
}

AvatarHighBar::~AvatarHighBar() {
    // The BasicShape will be deallocated in the main function
    // so we don't need to deallocate it here
    // And we don't own the high_bar pointer, so don't delete it
}