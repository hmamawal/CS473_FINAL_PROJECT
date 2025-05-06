// shaders/shadow_mapping.vert
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 local;

void main()
{
    gl_Position = lightSpaceMatrix * model * local * vec4(aPos, 1.0);
}