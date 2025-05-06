#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../classes/Shader.hpp"
#include "../classes/Font.hpp"
#include "../classes/avatar.hpp"
#include "../classes/avatar_high_bar.hpp"
#include "../classes/object_types.hpp"
#include "rendering.hpp"

// Function declarations for initialization tasks
void InitializeMouseSettings(GLFWwindow* window);
void CreateShaders(Shader*& shader_program_ptr, Shader*& font_program_ptr);
void CreateDepthShader(Shader*& depth_shader);
void CreatePostProcessingShader(Shader*& post_processing_shader);
void SetupAvatars(Avatar& baseAvatar, AvatarHighBar*& high_bar_avatar, GameModels& models);
void SetupCameraAndProjection(Shader* shader_program_ptr, unsigned int scr_width, unsigned int scr_height);
void SetupFontShader(Shader* font_program_ptr);
void SetupRendering();

#endif // INITIALIZATION_HPP