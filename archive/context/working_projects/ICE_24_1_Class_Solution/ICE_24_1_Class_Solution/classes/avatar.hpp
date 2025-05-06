#ifndef AVATAR_HPP
#define AVATAR_HPP
#include "basic_shape.hpp"

class Avatar {
    protected:
        BasicShape body;
        float initial_rotation;
        float current_rotation;
        float speed;
        glm::vec3 position;
        glm::vec3 scale;
        int shader_state;
    
    public:
        Avatar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        void ProcessInput (GLFWwindow *window, float time_passed);
        void Scale (glm::vec3 new_scale);
        void Draw (Shader *shader = NULL, int shader_state = -1, bool use_shader = false);
        ~Avatar();
        void DeallocateAvatar();
};


#endif //AVATAR_HPP