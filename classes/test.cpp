#include "test.hpp"

TestChild::TestChild(std::string name, BasicShape shape, float orientation, 
                    glm::vec3 initial_position, int shader_state) : Avatar(shape,orientation,initial_position,shader_state) {
    this->name = name;
}