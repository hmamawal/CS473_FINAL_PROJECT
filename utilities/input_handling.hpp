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
extern glm::vec4 light_direction; // Added reference to the directional light's direction
extern Shader* shader_program_ptr;
extern AvatarHighBar* high_bar_avatar;
extern bool first_mouse;
extern float last_x;
extern float last_y;
extern glm::vec3 original_camera_position;
extern float original_camera_yaw;
extern float original_camera_pitch;
extern bool spotlight_on; // Declaration for spotlight toggle state
extern bool point_light_on; // Declaration for point light toggle state
extern bool hud_visible;

// External variables for post-processing
extern int current_effect;
extern bool effect_key_pressed;

#endif // INPUT_HANDLING_HPP