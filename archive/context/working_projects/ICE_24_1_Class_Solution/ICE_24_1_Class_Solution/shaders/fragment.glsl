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
#define NR_TEXTURES 6
uniform sampler2D textures[NR_TEXTURES];
uniform sampler2D shadow_map;

uniform vec4 view_position;

struct DirectionalLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    //add coefficients for attenuation here
    bool on;
};

in vec4 fragment_position_light_space;

uniform DirectionalLight directional_light;

vec4 CalcDirectionalLight (DirectionalLight light,vec3 norm,vec3 frag,vec3 eye);

float ShadowCalculation();

//  0: BasicShape objects that just have a set color (basic)
//  1: BasicShape objects that have a texture
//  2: Imported BasicShape objects that use materials from Blender
//  3: Imported BasicShape objects that use materials and/or textures
//  4: Depth Rendering

void main()
{
    vec4 directional_light_color = CalcDirectionalLight(directional_light,norm,
                                            fragment_position,view_position.xyz);
    
    if (fragment_shader_state == 4) {
        return;
    }
    
    //Material colors are integrated into the point light calculation
    if (fragment_shader_state == 2) {
        FragColor = directional_light_color;
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
        }
        if (index_for_texture == 99) {
            FragColor = directional_light_color;
            return;
        }
        FragColor = directional_light_color * FragColor;
        return;

    }

    //otherwise fragment state is 0 (use the set color)
    FragColor = directional_light_color * set_color;
    
};

vec4 CalcDirectionalLight (DirectionalLight light,vec3 norm,vec3 frag,vec3 eye) {

    if (!light.on) {
        return vec4(0.0,0.0,0.0,1.0);
    }
    //vec3 light_direction = light.position.xyz - frag.xyz;
    vec3 light_direction = -light.direction.xyz;
    light_direction = normalize(light_direction);
    vec3 normal = normalize(norm);
    float diffuse_coeff = max(dot(normal,light_direction),0.0);

    vec3 view_direction = normalize(view_position.xyz-frag);
    vec3 reflect_direction = reflect(-light_direction,normal);
    float spec_coeff = pow(max(dot(view_direction,reflect_direction),0.0),256.0);

    float shadow = ShadowCalculation();
    //shadow = 0.0;

    //handle materials
    if ((fragment_shader_state == 2) || (fragment_shader_state == 3)) {
        //need to use the different material components in the calculation
        vec3 final_color =  ((1-shadow)*light.ambient.rgb * ambient_color);
        final_color += (1-shadow)*(diffuse_coeff * light.diffuse.rgb * diffuse_color);
        final_color += (1-shadow)*(spec_coeff * light.specular.rgb * specular_color);
        return vec4(final_color,opacity);
    } else {
        vec3 final_color =  ((1-shadow)*light.ambient.rgb 
            + (1-shadow)*diffuse_coeff * light.diffuse.rgb
            + (1-shadow)*spec_coeff * light.specular.rgb);
        return vec4(final_color,1.0);
    }




}

float ShadowCalculation() {
    // perform perspective divide
    vec3 projCoords = fragment_position_light_space.xyz / fragment_position_light_space.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow_map, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}