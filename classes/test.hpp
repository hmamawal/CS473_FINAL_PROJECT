#ifndef TEST_HPP
#define TEST_HPP

#include "avatar.hpp"

class TestChild : public Avatar {
    protected:
        std::string name;
    public:
        TestChild(std::string name, BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        std::string GetName() {
            return this->name;
        }

};

#endif //TEST_HPP