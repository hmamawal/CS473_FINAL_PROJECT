#ifndef INPUT_HANDLING_HPP
#define INPUT_HANDLING_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../classes/camera.hpp"
#include "../classes/avatar_high_bar.hpp"
#include "../classes/Shader.hpp"

// Function declarations
void ProcessInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// External variables used by input handling
extern Camera camera;
extern float delta_time;
extern glm::vec3 point_light_color;
extern Shader* shader_program_ptr;
extern AvatarHighBar* high_bar_avatar;
extern bool first_mouse;
extern float last_x;
extern float last_y;
extern glm::vec3 original_camera_position;
extern float original_camera_yaw;
extern float original_camera_pitch;
extern bool spotlight_on; // Add declaration for spotlight toggle state

#endif // INPUT_HANDLING_HPP