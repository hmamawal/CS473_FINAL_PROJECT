#include "avatar_high_bar.hpp"

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                            glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize any additional properties specific to AvatarHighBar
}

// Implement any additional methods or overridden methods
// Example:
// void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
//     // Custom implementation or call parent method
//     Avatar::ProcessInput(window, time_passed);
//     // Additional functionality
// }