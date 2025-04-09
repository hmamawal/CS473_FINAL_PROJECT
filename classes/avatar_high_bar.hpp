#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Existing properties
        bool is_raised;
        glm::vec3 original_position;
        bool key_pressed; // To prevent multiple toggles on a single key press
        
        // New properties for the rotation animation
        bool is_rotating;         // Is the avatar currently rotating around the bar
        float rotation_angle;     // Current angle in the rotation (0-360 degrees)
        float time_since_move;    // Time accumulator for movement timing
        bool m_key_pressed;       // Track M key state to prevent multiple toggles
        glm::vec3 default_position; // Default position of the avatar
        float orientation;        // Orientation of the avatar (rotation angle)
        
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        void ProcessInput(GLFWwindow *window, float time_passed);
        void Draw(Shader *shader, bool use_shader);
        
        // Getter methods for camera positioning
        glm::vec3 GetPosition() const;
        float GetRotationAngle() const; 
};

#endif //AVATAR_HIGH_BAR_HPP