#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Existing properties
        bool is_raised;
        glm::vec3 original_position;
        bool key_pressed; // To prevent multiple toggles on a single key press
        
        // Rotation properties with clearer naming
        bool is_rotating_around_bar;  // Is the avatar currently rotating around the bar (X-axis)
        float x_rotation_angle;       // Current angle for X-axis rotation (0-360 degrees)
        float time_since_move;        // Time accumulator for movement timing
        bool m_key_pressed;           // Track M key state to prevent multiple toggles
        glm::vec3 default_position;   // Default position of the avatar
        
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        void ProcessInput(GLFWwindow *window, float time_passed);
        void Draw(Shader *shader, bool use_shader);
        void GetXRotationAngle(float &angle);
        void GetPosition(glm::vec3 &pos);
};

#endif //AVATAR_HIGH_BAR_HPP