#include "highbar.hpp"
#include <glm/gtc/matrix_transform.hpp>

HighBar::HighBar(BasicShape shape, glm::vec3 position, int shader_state) {
    this->bar = shape;
    this->position = position;
    this->shader_state = shader_state;
    this->height = 4.0f;  // Default height for the high bar
    this->length = 6.0f;  // Assumed length of the high bar
    this->radius = 0.1f;  // Assumed radius of the high bar
}

void HighBar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        shader->use();
    }
    
    glm::mat4 local(1.0);
    local = glm::translate(local, this->position);
    
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->bar.Draw();
}

glm::vec3 HighBar::GetPosition() const {
    return this->position;
}

float HighBar::GetHeight() const {
    return this->height;
}

HighBar::~HighBar() {
    // The BasicShape will be deallocated in the main function
    // so we don't need to deallocate it here
}