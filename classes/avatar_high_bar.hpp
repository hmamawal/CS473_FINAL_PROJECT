#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP
#include "avatar.hpp"
#include "highbar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        bool is_on_bar;
        bool is_rotating;
        float rotation_angle;
        float rotation_speed;
        glm::vec3 original_position;
        glm::vec3 bar_position;
        HighBar* high_bar;
        float rotation_radius;
        float rotation_progress;
        float rotation_duration;
    
    public:
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state, HighBar* high_bar);
        void ProcessInput(GLFWwindow *window, float time_passed) override;
        void Draw(Shader *shader, bool use_shader=false) override;
        void JumpToHighBar();
        void DropFromHighBar();
        void ToggleHighBar();
        void StartRotation();
        void StopRotation();
        void UpdateRotation(float time_passed);
        ~AvatarHighBar();
};

#endif //AVATAR_HIGH_BAR_HPP