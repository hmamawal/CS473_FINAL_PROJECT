#ifndef HIGHBAR_HPP
#define HIGHBAR_HPP
#include "basic_shape.hpp"

class HighBar {
    protected:
        BasicShape bar;
        glm::vec3 position;
        float height;
        float length;
        float radius;
        int shader_state;
    
    public:
        HighBar(BasicShape shape, glm::vec3 position, int shader_state);
        void Draw(Shader *shader, bool use_shader=false);
        glm::vec3 GetPosition() const;
        float GetHeight() const;
        ~HighBar();
};

#endif //HIGHBAR_HPP