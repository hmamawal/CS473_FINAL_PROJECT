#include "avatar.hpp"
#include <glm/gtc/matrix_transform.hpp>


Avatar::Avatar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state) {
    this->body = shape;
    this->initial_rotation = orientation;
    this->current_rotation = 0.0;
    this->position = initial_position;
    this->shader_state = shader_state;
    //both speed and rotate_speed could be separate parameters
    this->speed = 5.0;
    this->rotate_speed = 90.0;
    this->scale = glm::vec3(1.0);
}


void Avatar::ProcessInput(GLFWwindow *window, float time_passed) {
    if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS) {
        float change_x = this->speed * time_passed * cos(glm::radians(this->current_rotation));
        float change_z = this->speed * time_passed * (-sin(glm::radians(this->current_rotation)));
        this->position += glm::vec3(change_x,0.0, change_z);
        if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS) {
            this->current_rotation += this->rotate_speed * time_passed;
        }
        if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS) {
            this->current_rotation += -(this->rotate_speed) * time_passed;
        }
    }

}

void Avatar::Scale(glm::vec3 new_scale) {
    this->scale = new_scale;
}

void Avatar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        //prevents us from unnecessarily 'switching'
        //  shader programs when we don't have to.
        shader->use();
    }
    glm::mat4 local(1.0);
    local = glm::translate(local,this->position);
    local = glm::rotate(local,glm::radians(this->current_rotation),glm::vec3(0.0,1.0,0.0));
    local = glm::rotate(local,glm::radians(this->initial_rotation),glm::vec3(0.0,1.0,0.0));
    local = glm::scale(local,this->scale);
    shader->setMat4("local",local);
    shader->setInt("shader_state",this->shader_state);
    this->body.Draw();
}


Avatar::~Avatar() {
   //Do not deallocate automatically when this object is destroyed as
   // it will destroy the data.
    // this->body.DeallocateShape();
}

void Avatar::DeallocateShape() {
    this->body.DeallocateShape();
}