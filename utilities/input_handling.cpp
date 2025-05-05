#include "input_handling.hpp"
#include <iostream>

// Make spotlight_on a global variable accessible from other files
bool spotlight_on = true;  // Initialize to true

void ProcessInput(GLFWwindow *window) {
    static bool c_key_pressed = false;
    static bool r_key_pressed = false;
    static bool l_key_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Process 'R' key to change point light color
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!r_key_pressed) {
            r_key_pressed = true;
            
            // Toggle between red and white
            if (point_light_color.r == 1.0f && point_light_color.g == 0.0f && point_light_color.b == 0.0f) {
                // Change back to white
                point_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
                std::cout << "Point light color changed to white" << std::endl;
            } else {
                // Change to red
                point_light_color = glm::vec3(1.0f, 0.0f, 0.0f);
                std::cout << "Point light color changed to red" << std::endl;
            }
            
            // Update the light color in the shader
            shader_program_ptr->setVec4("point_light.ambient", glm::vec4(0.5f * point_light_color, 1.0f));
            shader_program_ptr->setVec4("point_light.diffuse", glm::vec4(point_light_color, 1.0f));
            shader_program_ptr->setVec4("point_light.specular", glm::vec4(0.5f * point_light_color, 1.0f));
        }
    } else {
        r_key_pressed = false;
    }

    // Process 'C' key to toggle first-person view
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (!c_key_pressed) {
            c_key_pressed = true;

            if (!camera.first_person_view) {
                // Save the original camera state before entering first-person view
                original_camera_position = camera.Position;
                original_camera_yaw = camera.Yaw;
                original_camera_pitch = camera.Pitch;
            }

            // Toggle first-person view
            camera.first_person_view = !camera.first_person_view;

            if (!camera.first_person_view) {
                // Restore the original camera state when exiting first-person view
                camera.Position = original_camera_position;
                camera.Yaw = original_camera_yaw;
                camera.Pitch = original_camera_pitch;
                camera.updateCameraVectors();
            }
        }
    } else {
        c_key_pressed = false;
    }

    // Update camera position and orientation if in first-person view
    if (camera.first_person_view) {
        // Get the position and rotation angle from high_bar_avatar
        glm::vec3 avatar_pos;
        high_bar_avatar->GetPosition(avatar_pos);

        // Position the camera at the avatar's position but slightly higher (eye level)
        camera.Position = avatar_pos + glm::vec3(0.0f, 2.0f, 0.0f);

        // Get the X rotation angle (pitch) from avatar
        float x_rotation_angle;
        high_bar_avatar->GetXRotationAngle(x_rotation_angle);

        // Update camera pitch based on avatar's X rotation
        camera.Pitch = x_rotation_angle;

        // Update camera vectors with the new pitch
        camera.updateCameraVectors();

        return; // Skip regular camera controls when in first-person
    }

    // Regular camera controls (only when not in first-person view)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, delta_time);
    }

    // Process 'L' key to toggle spotlight
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!l_key_pressed) {
            l_key_pressed = true;
            spotlight_on = !spotlight_on;
            
            // Make sure we update the shader with the current state
            if (shader_program_ptr) {
                shader_program_ptr->use();
                shader_program_ptr->setBool("spot_light.on", spotlight_on);
            }

            // Print feedback
            if (spotlight_on) {
                std::cout << "Spotlight ON" << std::endl;
            } else {
                std::cout << "Spotlight OFF" << std::endl;
            }
        }
    } else {
        l_key_pressed = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }
  
    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; 
    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.7f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.ProcessMouseMovement(xoffset, yoffset);
}