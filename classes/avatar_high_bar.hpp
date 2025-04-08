#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Add any additional properties specific to AvatarHighBar
        
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        // Override any methods as needed or add new ones
        // Example: void ProcessInput(GLFWwindow *window, float time_passed) override;
};

#endif //AVATAR_HIGH_BAR_HPP