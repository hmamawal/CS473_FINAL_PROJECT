#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

#include "../classes/Shader.hpp"
#include "../classes/basic_shape.hpp"
#include "../classes/avatar.hpp"
#include "../classes/avatar_high_bar.hpp"
#include "../classes/camera.hpp"
#include "../classes/Font.hpp"
#include "../classes/import_object.hpp"

// Structure to hold all VAO objects
struct RenderingVAOs {
    VAOStruct basic_vao;
    VAOStruct texture_vao;
    VAOStruct import_vao;
    VAOStruct skybox_vao;
};

// Structure to hold all model objects
struct GameModels {
    BasicShape baseModel;
    BasicShape tumbling_floor;
    BasicShape vault_table;
    BasicShape high_bar;
    BasicShape pommel_horse;
    BasicShape pommel_horse2;
    BasicShape LouGrossBuilding;
    BasicShape floor;
    BasicShape skybox;
    
    // Textures
    unsigned int floor_texture;
    unsigned int tumbling_floor_texture;
    unsigned int skybox_texture;
    std::vector<unsigned int> vault_table_textures;
    std::vector<unsigned int> building_textures;
};

// Setup and initialization functions
RenderingVAOs setupVAOs();
GameModels loadModels(RenderingVAOs& vaos, ImportOBJ& importer);
void setupShaders(Shader* shader_program, Font& arial_font, const RenderingVAOs& vaos);
void setupLighting(Shader* shader_program, const glm::vec3& light_color, const glm::vec4& light_direction, const Camera& camera);

// Rendering functions
void renderScene(Shader* shader_program, 
                GameModels& models, 
                Avatar& baseAvatar,
                AvatarHighBar* high_bar_avatar,
                const Camera& camera,
                const glm::vec3& light_color,
                const glm::vec4& light_direction,
                bool is_depth_pass);

// Render the skybox using a specific shader
void renderSkybox(Shader* skybox_shader, GameModels& models, const Camera& camera, unsigned int scr_width, unsigned int scr_height);

void renderHUDBackground(Shader* shader_program, float x1, float y1, float x2, float y2, glm::vec4 color);

// Updated to use Shader pointer instead of reference
void renderText(Shader* font_program, Font& arial_font, const Camera& camera);

// Cleanup functions
void cleanupResources(RenderingVAOs& vaos, GameModels& models);

#endif // RENDERING_HPP