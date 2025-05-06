#version 330 core

//Accounts for position data VBOs and those with position and texture 
//  coordinates.
//x, y, z coordinates for the vertex
layout (location = 0) in vec3 aPos;
//normal data for the vertex
layout (location = 1) in vec3 aNorm;
//texture coordinates for the vertex
layout (location = 2) in vec2 aCoord;
//ambient color data for the vertex
layout (location = 3) in vec3 aColor;
//diffuse color data for the vertex
layout (location = 4) in vec3 dColor;
//specular color data for the vertex
layout (location = 5) in vec3 sColor;
//opacitdy value for the vertex
layout (location = 6) in float opacity_value;
//index for the texture (for multiple textures on an imported object)
layout (location = 7) in float texture_index;


//note: "flat" is a key word that indicates this value stays the same 
//  and is not altered between vertices (like color and texture coordinates are)

//outputs to the fragment shader
//texture coordinates
out vec2 texture_coordinates;
//texture index (default is 0)
flat out int index_for_texture;
//opacity value
flat out float opacity;
//normal vector
out vec3 norm;
//color data
out vec3 ambient_color;
out vec3 diffuse_color;
out vec3 specular_color;
//fragment position
out vec3 fragment_position;

// //pass state
// flat out int textured;
// flat out int imported;

// //A flag used to indicate that the drawn shape is texture-mapped.
// uniform bool is_textured;
// uniform bool is_imported;

//state for the shader
uniform int shader_state;
//  0: BasicShape objects that just have a set color (basic)
//  1: BasicShape objects that have a texture
//  2: Imported BasicShape objects that use materials from Blender
//  3: Imported BasicShape objects that use materials and/or textures
flat out int fragment_shader_state;

//transformation matrices for the projection, view (camera), model (world), and 
//  transform (local).  
uniform mat4 projection, view, model, local;

uniform mat4 lightSpaceMatrix; // Add this

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace; // Add this
} vs_out;

void main()
{
  // Calculate fragment position first
  fragment_position = (model * local * vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;
  
  // Initialize the vs_out structure properly
  vs_out.FragPos = fragment_position;
  vs_out.Normal = mat3(transpose(inverse(model*local))) * aNorm;
  vs_out.TexCoords = aCoord;
  
  // Now use the initialized vs_out.FragPos
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
  
  // Rest of your code
  norm = mat3(transpose(inverse(model*local))) * aNorm;
  fragment_shader_state = shader_state;
  
  if (shader_state == 1) {
    // Basic shape with texture (pass s, t and index = 0)
    texture_coordinates = aCoord;
    index_for_texture = 0;
  } else if (shader_state == 2) {
    // Imported object using materials (no textures)
    ambient_color = aColor;
    diffuse_color = dColor;
    specular_color = sColor;
    opacity = opacity_value;
  } else if (shader_state == 3) {
    // Imported object using materials and textures
    ambient_color = aColor;
    diffuse_color = dColor;
    specular_color = sColor;
    opacity = opacity_value;
    index_for_texture = int(texture_index);
  } else {
    // Shader state is 0 so set_color is used in the fragment shader.
  }

  // Figure out this vertex's location after applying the necessary matrices.
  gl_Position = projection * view * vec4(fragment_position, 1.0);
};