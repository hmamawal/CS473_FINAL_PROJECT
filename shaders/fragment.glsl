#version 330 core
//Need to send the fragment color to the next step in the pipeline
out vec4 FragColor;

//bring in the texture image coordinates (if you have them)
in vec2 texture_coordinates;

//bring in the normal and the fragment position (world)
in vec3 norm;
in vec3 fragment_position;

//bring in the imported information
in vec3 ambient_color;
in vec3 diffuse_color;
in vec3 specular_color;
flat in float opacity;

//bring in the index for the texture
flat in int index_for_texture;

//bring in the state of the fragment shader
flat in int fragment_shader_state;

uniform vec4 set_color;

//if it -is- textured, assume this 2D texture is mapped.
#define NR_TEXTURES 19
uniform sampler2D textures[NR_TEXTURES];

uniform vec4 view_position;

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    //add coefficients for attenuation here
    bool on;
};

uniform PointLight point_light;

vec4 CalcPointLight (PointLight light,vec3 norm,vec3 frag,vec3 eye);

//  0: BasicShape objects that just have a set color (basic)
//  1: BasicShape objects that have a texture
//  2: Imported BasicShape objects that use materials from Blender
//  3: Imported BasicShape objects that use materials and/or textures

void main()
{
    vec4 point_light_color = CalcPointLight(point_light,norm,
                                            fragment_position,view_position.xyz);
    //Material colors are integrated into the point light calculation
    if (fragment_shader_state == 2) {
        FragColor = point_light_color;
        return;
    }

    //Object is textured
    if ((fragment_shader_state == 1) || (fragment_shader_state == 3)) {
        //unfortunately you can't index an array with a variable in GLSL
        //  so you have to use a set of if statements.
        if (index_for_texture == 0) {
            FragColor = texture(textures[0],texture_coordinates);
        } else if (index_for_texture == 1) {
            FragColor = texture(textures[1],texture_coordinates);
        } else if (index_for_texture == 2) {
            FragColor = texture(textures[2],texture_coordinates);
        } else if (index_for_texture == 3) {
            FragColor = texture(textures[3],texture_coordinates);
        } else if (index_for_texture == 4) {
            FragColor = texture(textures[4],texture_coordinates);
        } else if (index_for_texture == 5) {
            FragColor = texture(textures[5],texture_coordinates);
        } else if (index_for_texture == 6) {
            FragColor = texture(textures[6],texture_coordinates);
        } else if (index_for_texture == 7) {
            FragColor = texture(textures[7],texture_coordinates);
        } else if (index_for_texture == 8) {
            FragColor = texture(textures[8],texture_coordinates);
        } else if (index_for_texture == 9) {
            FragColor = texture(textures[9],texture_coordinates);
        } else if (index_for_texture == 10) {
            FragColor = texture(textures[10],texture_coordinates);
        } else if (index_for_texture == 11) {
            FragColor = texture(textures[11],texture_coordinates);
        } else if (index_for_texture == 12) {
            FragColor = texture(textures[12],texture_coordinates);
        } else if (index_for_texture == 13) {
            FragColor = texture(textures[13],texture_coordinates);
        } else if (index_for_texture == 14) {
            FragColor = texture(textures[14],texture_coordinates);
        } else if (index_for_texture == 15) {
            FragColor = texture(textures[15],texture_coordinates);
        } else if (index_for_texture == 16) {
            FragColor = texture(textures[16],texture_coordinates);
        } else if (index_for_texture == 17) {
            FragColor = texture(textures[17],texture_coordinates);
        } else if (index_for_texture == 18) {
            FragColor = texture(textures[18],texture_coordinates);
        }
        if (index_for_texture == 99) {
            FragColor = point_light_color;
            return;
        }
        FragColor = point_light_color * FragColor;
        return;

    }

    //otherwise fragment state is 0 (use the set color)
    FragColor = point_light_color * set_color;
    
};

vec4 CalcPointLight (PointLight light,vec3 norm,vec3 frag,vec3 eye) {

    if (!light.on) {
        return vec4(0.0,0.0,0.0,1.0);
    }
    vec3 light_direction = light.position.xyz - frag.xyz;
    light_direction = normalize(light_direction);
    vec3 normal = normalize(norm);
    float diffuse_coeff = max(dot(normal,light_direction),0.0);

    vec3 view_direction = normalize(view_position.xyz-frag);
    vec3 reflect_direction = reflect(-light_direction,normal);
    float spec_coeff = pow(max(dot(view_direction,reflect_direction),0.0),256.0);

    //handle materials
    if ((fragment_shader_state == 2) || (fragment_shader_state == 3)) {
        //need to use the different material components in the calculation
        return (light.ambient * vec4(ambient_color,opacity) 
                + diffuse_coeff * light.diffuse * vec4(diffuse_color,opacity)
                + spec_coeff * light.specular * vec4(specular_color,opacity));
    } else {
        return (light.ambient 
            + diffuse_coeff * light.diffuse 
            + spec_coeff * light.specular);
    }




}