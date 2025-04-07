#ifndef GYM_AVATAR_HPP
#define GYM_AVATAR_HPP
#include "avatar.hpp"

class GymAvatar : public Avatar {
    private:
        // High bar properties
        glm::vec3 high_bar_position;
        float high_bar_radius;
        
        // State properties
        bool is_on_high_bar;
        bool is_jumping_to_bar;
        
        // Animation properties
        float rotation_angle;
        float rotation_speed;
        float jump_progress;
        float jump_duration;
        glm::vec3 start_position;
        
    public:
        GymAvatar(BasicShape shape, float orientation, glm::vec3 initial_position, 
                 int shader_state, glm::vec3 high_bar_pos, float bar_radius = 0.1f);
        
        void ProcessInput(GLFWwindow *window, float time_passed) override;
        void Draw(Shader *shader, bool use_shader = false) override;
        
        // High bar interaction methods
        void JumpToHighBar();
        void RotateOnHighBar(float time_passed);
        void DismountFromHighBar();
        bool IsNearHighBar() const;
};

#endif //GYM_AVATAR_HPP