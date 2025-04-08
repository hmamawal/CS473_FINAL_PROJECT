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
    // get high bar height
    float bar_height = high_bar->GetHeight();
    this->bar_position = glm::vec3(bar_pos.x, bar_pos.y + 0.2, bar_pos.z); // Position below the bar
    
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
        // Apply rotation to match gymnast's orientation to their position on the circle
        float orientation_angle = this->rotation_angle;
        
        // Rotate gymnast to match their position in the circular path
        local = glm::rotate(local, glm::radians(orientation_angle), glm::vec3(1.0f, 0.0f, 0.0f));
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
        
        // Get gymnast and bar dimensions
        float gymnast_height = this->GetModelHeight();
        // print gymnast height for debugging
        std::cout << "Gymnast height: " << gymnast_height << std::endl;
        glm::vec3 bar_pos = this->high_bar->GetPosition();
        glm::vec3 bar_dims = this->high_bar->GetModelDimensions();
        float bar_thickness = (bar_dims.y > 0) ? bar_dims.y : (this->high_bar->GetRadius() * 2);
        
        // Calculate the position to place the gymnast's hands at the bar
        // Estimate that arms reach is approximately 40% of height from the top of the head
        float hanging_y_offset = -gymnast_height; //* 0.4f;
        
        // Calculate position (centered on x, hands at bar height, adjusted z)
        this->bar_position = glm::vec3(
            bar_pos.x,                        // Center on bar x-coordinate
            bar_pos.y + hanging_y_offset,     // Position hands at bar height
            bar_pos.z - bar_thickness * 0.6f  // Slightly in front of bar
        );
        
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
    
    // If reached full rotation, optionally stop or continue
    if (this->rotation_progress >= 1.0f) {
        // Reset progress but continue rotating
        this->rotation_progress = 0.0f;
    }
    
    // Calculate new position based on rotation around the bar
    float angle_rad = glm::radians(this->rotation_angle);
    
    // Get the high bar position and dimensions
    glm::vec3 bar_pos = this->high_bar->GetPosition();
    float bar_radius = this->high_bar->GetRadius();
    
    // Get model dimensions if available
    glm::vec3 model_dims = this->high_bar->GetModelDimensions();
    float actual_bar_radius = (model_dims.y > 0) ? model_dims.y / 2.0f : bar_radius;
    
    // Calculate gymnast's arm length based on actual gymnast height
    float gymnast_height = this->GetModelHeight();
    float arm_length = gymnast_height * 0.4f; // Arms are approximately 40% of total height
    
    // Calculate new position with the center of rotation at the high bar
    // Use actual dimensions for more accurate positioning
    float rotation_distance = arm_length + actual_bar_radius;
    float x = bar_pos.x; // x-coordinate of the high bar
    
    // Make the y-coordinate slightly higher at the top of the circle for realism
    float height_offset = (sin(angle_rad) > 0) ? 0.1f * sin(angle_rad) : 0.0f;
    float y = bar_pos.y + sin(angle_rad) * rotation_distance + height_offset;
    
    // Adjust z position based on cosine of angle
    float z = bar_pos.z - rotation_distance * cos(angle_rad);
    
    // Update the avatar position
    this->position = glm::vec3(x, y, z);
}

float AvatarHighBar::GetModelHeight() const {
    // Get dimensions from the BasicShape
    glm::vec3 dimensions = this->body.GetDimensions();
    
    // Apply the scale factor to get actual rendered height
    float scaled_height = dimensions.y * this->scale.y;
    
    return scaled_height;
}

float AvatarHighBar::GetApproximateHeight() const {
    // Base height is roughly 2.0 units in model space
    // Multiply by the y-scale to get approximate rendered height
    return 2.0f * this->scale.y;
}

AvatarHighBar::~AvatarHighBar() {
    // The BasicShape will be deallocated in the main function
    // so we don't need to deallocate it here
    // And we don't own the high_bar pointer, so don't delete it
}