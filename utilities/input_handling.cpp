#include "input_handling.hpp"
#include <iostream>

// Make spotlight_on and point_light_on global variables accessible from other files
bool spotlight_on = true;  // Initialize to true
bool point_light_on = true;  // Initialize to true
bool hud_visible = true; // HUD is visible by default

// Variables to track arrow key states
static bool up_key_pressed = false;
static bool down_key_pressed = false;
static bool left_key_pressed = false;
static bool right_key_pressed = false;

void ProcessInput(GLFWwindow *window) {
    static bool c_key_pressed = false;
    static bool r_key_pressed = false;
    static bool l_key_pressed = false;
    static bool p_key_pressed = false;
    static bool effect_key_pressed = false;
    static bool h_key_pressed = false;

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
            shader_program_ptr->setVec4("directional_light.ambient", glm::vec4(0.5f * point_light_color, 1.0f));
            shader_program_ptr->setVec4("directional_light.diffuse", glm::vec4(point_light_color, 1.0f));
            shader_program_ptr->setVec4("directional_light.specular", glm::vec4(0.5f * point_light_color, 1.0f));
        }
    } else {
        r_key_pressed = false;
    }

    // Process arrow keys to control directional light direction
    float light_change_speed = 0.1f;
    bool light_direction_changed = false;
    
    // Up arrow key - change Y component
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!up_key_pressed) {
            up_key_pressed = true;
            light_direction.y -= light_change_speed;
            light_direction_changed = true;
            std::cout << "Light direction Y: " << light_direction.y << std::endl;
        }
    } else {
        up_key_pressed = false;
    }
    
    // Down arrow key - change Y component
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!down_key_pressed) {
            down_key_pressed = true;
            light_direction.y += light_change_speed;
            light_direction_changed = true;
            std::cout << "Light direction Y: " << light_direction.y << std::endl;
        }
    } else {
        down_key_pressed = false;
    }
    
    // Left arrow key - change X component
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!left_key_pressed) {
            left_key_pressed = true;
            light_direction.x -= light_change_speed;
            light_direction_changed = true;
            std::cout << "Light direction X: " << light_direction.x << std::endl;
        }
    } else {
        left_key_pressed = false;
    }
    
    // Right arrow key - change X component
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!right_key_pressed) {
            right_key_pressed = true;
            light_direction.x += light_change_speed;
            light_direction_changed = true;
            std::cout << "Light direction X: " << light_direction.x << std::endl;
        }
    } else {
        right_key_pressed = false;
    }
    
    // Update the light direction in the shader if it changed
    if (light_direction_changed && shader_program_ptr) {
        shader_program_ptr->use();
        shader_program_ptr->setVec4("directional_light.direction", light_direction);
        std::cout << "Light direction updated to: (" << 
            light_direction.x << ", " << 
            light_direction.y << ", " << 
            light_direction.z << ")" << std::endl;
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

    // Process 'P' key to toggle point light
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!p_key_pressed) {
            p_key_pressed = true;
            point_light_on = !point_light_on;
            
            // Make sure we update the shader with the current state
            if (shader_program_ptr) {
                shader_program_ptr->use();
                shader_program_ptr->setBool("point_light.on", point_light_on);
            }

            // Print feedback
            if (point_light_on) {
                std::cout << "Point Light ON" << std::endl;
            } else {
                std::cout << "Point Light OFF" << std::endl;
            }
        }
    } else {
        p_key_pressed = false;
    }

    // Check for post-processing effects toggle
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        if (!effect_key_pressed) {
            effect_key_pressed = true;
            current_effect = 0; // No effect
            std::cout << "Post-processing effect: None" << std::endl;
        }
    } 
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!effect_key_pressed) {
            effect_key_pressed = true;
            current_effect = 1; // Invert colors
            std::cout << "Post-processing effect: Invert colors" << std::endl;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!effect_key_pressed) {
            effect_key_pressed = true;
            current_effect = 2; // Grayscale
            std::cout << "Post-processing effect: Grayscale" << std::endl;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (!effect_key_pressed) {
            effect_key_pressed = true;
            current_effect = 3; // Edge detection
            std::cout << "Post-processing effect: Edge detection" << std::endl;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        if (!effect_key_pressed) {
            effect_key_pressed = true;
            current_effect = 4; // Box blur
            std::cout << "Post-processing effect: Blur" << std::endl;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (!effect_key_pressed) {
            effect_key_pressed = true;
            current_effect = 5; // Sharpen
            std::cout << "Post-processing effect: Sharpen" << std::endl;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        if (!effect_key_pressed) {
            effect_key_pressed = true;
            current_effect = 6; // Sepia tone
            std::cout << "Post-processing effect: Sepia" << std::endl;
        }
    }
    else {
        effect_key_pressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        if (!h_key_pressed) {
            h_key_pressed = true;
            hud_visible = !hud_visible;  // Toggle the state
            std::cout << "HUD toggled " << (hud_visible ? "ON" : "OFF") << std::endl;
        }
    } else {
        h_key_pressed = false;
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