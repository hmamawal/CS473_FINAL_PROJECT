#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Add any additional properties specific to AvatarHighBar
        bool is_raised;
        glm::vec3 original_position;
        bool key_pressed; // To prevent multiple toggles on a single key press
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        void ProcessInput(GLFWwindow *window, float time_passed);
};

#endif //AVATAR_HIGH_BAR_HPP