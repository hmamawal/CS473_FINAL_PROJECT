#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utilities/environment.hpp"
#include "utilities/build_shapes.hpp"
#include "classes/camera.hpp"
#include "classes/Font.hpp"
#include "classes/import_object.hpp"
#include "classes/avatar.hpp"

//Enumeration Type: ObjectType
//Determines how shaders will draw different objects:
//  0: BasicShape objects that just have a set color (basic)
//  1: BasicShape objects that have a texture
//  2: Imported BasicShape objects that use materials from Blender
//  3: Imported BasicShape objects that use materials and/or textures
enum ObjectType {
    BASIC, 
    TEXTURED, 
    IMPORTED_BASIC, 
    IMPORTED_TEXTURED 
};

/*ProcessInput
 * Accepts a GLFWwindow pointer as input and processes 
 * user key and mouse button inpbut.  
 * Returns nothing.
 */
void ProcessInput(GLFWwindow *window);

/*mouse_callback
 *Accepts a GLFWwindow pointer, and a double for the xpos and the ypos.  
 *Invoked when the mouse is moved once it is set using the glfwSetCursorPosCallback
 * function.  
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


//Global Variables
//----------------
//Screen width and height
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//clear color
glm::vec4 clear_color (0.0,0.0,0.0,1.0);

//mouse variables
bool first_mouse{true};
float last_x{0.0};
float last_y{0.0};

//Camera Object
//set up the camera
Camera camera(glm::vec3(0.0f,1.0f,25.0f),glm::vec3(0.0f,1.0f,0.0f),-90.0f,0.0f);
float delta_time{0.001};
float last_frame{0.0};

//location of the point light
glm::vec4 light_position (0.0,20.0,20.0,1.0);

//Font for text display
Font arial_font ("fonts/ArialBlackLarge.bmp","fonts/ArialBlack.csv",0.1,0.15);

//shader state (determines how to draw items)
ObjectType shader_state = BASIC;

int main()
{
    //Initialize the environment, if it fails, return -1 (exit)
    GLFWwindow *window = InitializeEnvironment("CS473",SCR_WIDTH,SCR_HEIGHT);
    if (window == NULL) {
        std::cout<<"Failed to initialize GLFWwindow"<<std::endl;
        return -1;
    }

    //initialize last_x and last_y to the middle of the screen
    last_x = (1.0*SCR_WIDTH)/2.0f;
    last_y = (1.0*SCR_HEIGHT)/2.0f;

    //set up the mouse call back and then 'lock' the mouse to our
    // program to enable mouse camera control
    glfwSetCursorPosCallback(window, mouse_callback); 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    //Create the shader programs for the shapes and the font
    // (you *could* combine these into one program)
    //-------------------------
    Shader shader_program(".//shaders//vertex.glsl",".//shaders//fragment.glsl");
    Shader font_program(".//shaders//vertex.glsl",".//shaders//fontFragmentShader.glsl");

    //Create the VAOs
    //-------------------------
    VAOStruct basic_vao; 
    //Handles location (x,y,z) and surface normal (snx,sny,snz)
    glGenVertexArrays(1,&(basic_vao.id));
    basic_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,6*sizeof(float),0));
    basic_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,6*sizeof(float),3*sizeof(float)));
    

    VAOStruct texture_vao;
    //Handles basic shapes that have a single texture:
    //  location (x,y,z)
    //  surface normal (snx,sny,snz)
    //  texture coordinates (s,t)
    glGenVertexArrays(1,&(texture_vao.id));
    texture_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,8*sizeof(float),0));
    texture_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,8*sizeof(float),3*sizeof(float)));
    texture_vao.attributes.push_back(BuildAttribute(2,GL_FLOAT,false,8*sizeof(float),6*sizeof(float)));

    VAOStruct import_vao;
    //Handles imported data from blender:
    //  location (x,y,z)
    //  surface normal (snx,sny,snz)
    //  texture coordinates (s,t)
    //  ambient color (r,g,b)
    //  diffuse color (r,g,b)
    //  specular color (r,g,b)
    //  opacity 0-1.0
    //  index for the texture associated with the vertex
    glGenVertexArrays(1,&(import_vao.id));
    int stride_size = 19*sizeof(float);
    import_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,stride_size,0));
    import_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,stride_size,3*sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(2,GL_FLOAT,false,stride_size,6*sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,stride_size,8*sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,stride_size,11*sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(3,GL_FLOAT,false,stride_size,14*sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(1,GL_FLOAT,false,stride_size,17*sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(1,GL_FLOAT,false,stride_size,18*sizeof(float)));
    
    
    ImportOBJ importer;

    //import the models
    BasicShape smiley = importer.loadFiles("models/smiley",import_vao);
    //BasicShape die = importer.loadFiles("models/die",import_vao);
    //BasicShape tumbling_floor = importer.loadFiles("models/tumbling_floor", import_vao);

    // import the models using separate importer instances
    ImportOBJ importer_die;
    BasicShape die = importer_die.loadFiles("models/die", import_vao);
    int die_texture = importer_die.getTexture();

    ImportOBJ importer_floor;
    BasicShape tumbling_floor = importer_floor.loadFiles("models/tumbling_floor", import_vao);
    int tumbling_floor_texture = importer_floor.getTexture();


    //import the die -- note that getAllTextures supports multiple textures
    //int die_texture = importer.getTexture();
    //int tumbling_floor_texture = importer.getTexture();

    // std::vector<unsigned int> die_textures = importer.getAllTextures();
    // for (int i = 0; i < die_textures.size(); i++) {
    //     std::cout<<"Texture:"<<i<<" "<<die_textures[i]<<std::endl;
    // }
    Avatar ship((importer.loadFiles("models/ship",import_vao)),90.0f,glm::vec3(2.0,1.0,0.0),IMPORTED_BASIC);;
    ship.Scale(glm::vec3(0.4,0.4,0.4));

    arial_font.initialize(texture_vao);

    //Create the shapes
    //-------------------------
    unsigned int floor_texture = GetTexture("./textures/hull_texture.png");
    
    BasicShape floor = GetTexturedRectangle(texture_vao,glm::vec3(-50.0,-50.0,0.0),100.0,100.0,20.0,false);

    //set up the shader program    
    shader_program.use();
    glm::mat4 identity (1.0);
    
    glm::mat4 model = glm::rotate(identity,glm::radians(-90.0f),glm::vec3(1.0,0.0,0.0));
    shader_program.setMat4("model",model);
    glm::mat4 view = glm::translate(identity,glm::vec3(0.0,10.0,0.0));
    view = glm::rotate(view,glm::radians(-90.0f),glm::vec3(1.0,0.0,0.0));
    shader_program.setMat4("view",view);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(1.0f*SCR_WIDTH)/(1.0f*SCR_HEIGHT),0.1f,100.0f);
    shader_program.setMat4("projection",projection);

    //lighting 
    glm::vec3 light_color (1.0);
    glm::vec3 ambient_color = 0.1f*light_color;
    
    shader_program.setVec4("point_light.ambient",glm::vec4(0.1f*light_color,1.0));
    shader_program.setVec4("point_light.diffuse",glm::vec4(light_color,1.0f));
    shader_program.setVec4("point_light.specular",glm::vec4(0.5f*light_color,1.0f));
    shader_program.setVec4("point_light.position",light_position);
    shader_program.setBool("point_light.on",true);

    shader_program.setVec4("view_position",glm::vec4(camera.Position,1.0));

    //font shader settings
    font_program.use();
    font_program.setMat4("local",identity);
    font_program.setMat4("model",identity);
    font_program.setMat4("view",identity);
    font_program.setMat4("projection",glm::ortho(-1.0,1.0,-1.0,1.0,-1.0,1.0));
    font_program.setVec4("transparentColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    font_program.setFloat("alpha", 0.3);
    font_program.setInt("texture1", 0);

    //Set up the depth test and blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // adjust for real time
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame; 
        // input
        // -----
        ProcessInput(window);
        ship.ProcessInput(window,delta_time);

        // render
        // ------
        glClearColor(clear_color.r,clear_color.g,clear_color.b,clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //use the shader program (this is necessary because we 
        // use the font shader program at the end of the render loop)
        shader_program.use();

        //set up the view matrix (camera)
        view = camera.GetViewMatrix();
        shader_program.setMat4("view",view);
        shader_program.setVec4("view_position",glm::vec4(camera.Position,1.0));
        

        //Draw the smiley
        shader_program.setInt("shader_state",IMPORTED_BASIC);
        glm::mat4 smiley_model(1.0);
        glm::mat4 smiley_local = glm::translate(identity,glm::vec3(0.0,1.0,0.0));
        //smiley_local = glm::rotate(smiley_local,glm::radians(180.0f),glm::vec3(0.0,1.0,0.0));
        shader_program.setMat4("model",smiley_model);
        shader_program.setMat4("local",smiley_local);
        smiley.Draw();

        //draw the ship, don't need to use the shader program here
        //   since we already did at the top of the render loop.
        ship.Draw(&shader_program,false);

        //Draw the floor
        shader_program.setInt("shader_state",TEXTURED);
        shader_program.setMat4("model",model);
        shader_program.setMat4("local",identity);
        
        glm::mat4 floor_local(1.0);
        floor_local = glm::translate(floor_local,glm::vec3(0.0,0.0,-0.01));
        shader_program.setMat4("local",floor_local);
        //note that if you change the active texture to account for multiple
        
        glBindTexture(GL_TEXTURE_2D,floor_texture);
        floor.Draw();

        //draw the die
        shader_program.setInt("shader_state",IMPORTED_TEXTURED);
        glm::mat4 die_local(1.0);
        die_local = glm::translate(die_local,glm::vec3(-2.0,1.0,0.0));
        die_local = glm::scale(die_local,glm::vec3(0.5,0.5,0.5));
        //die_local = glm::rotate(die_local,glm::radians(180.0f),glm::vec3(0.0,0.0,1.0));
        shader_program.setMat4("model",identity);
        shader_program.setMat4("local",die_local);
        glBindTexture(GL_TEXTURE_2D,die_texture);
        //Below is an example of how to handle multiple textures on a single imported
        //  object (optional)
        // for (int i = 0; i < die_textures.size(); i++) {
        //     glActiveTexture(GL_TEXTURE0+i);
        //     std::string texture_string = "textures["+std::to_string(i)+"]";
        //     shader_program.setInt(texture_string,i);
        //     glBindTexture(GL_TEXTURE_2D,die_textures[i]);
        // }
        die.Draw();
        glActiveTexture(GL_TEXTURE0); 

        // Draw the gymnastics tumbling floor
        shader_program.setInt("shader_state", IMPORTED_TEXTURED);
        glm::mat4 tumbling_floor_local(1.0);
        tumbling_floor_local = glm::translate(tumbling_floor_local, glm::vec3(0.0, 1.0, 0.0));
        shader_program.setMat4("model", identity);
        shader_program.setMat4("local", tumbling_floor_local);
        glBindTexture(GL_TEXTURE_2D, tumbling_floor_texture);
        tumbling_floor.Draw();

        if (camera.Position.y < 0.5) {
            camera.Position.y = 0.5;
        }
        //Draw the text so that it stays with the camera
        font_program.use();
        std::string display_string = "Camera (";
        std::string cam_x = std::to_string(camera.Position.x);
        std::string cam_y = std::to_string(camera.Position.y);
        std::string cam_z = std::to_string(camera.Position.z);

        display_string += cam_x.substr(0,cam_x.find(".")+3) +",";
        display_string += cam_y.substr(0,cam_y.find(".")+3) +",";
        display_string += cam_z.substr(0,cam_z.find(".")+3) +")";
        
        arial_font.DrawText(display_string,glm::vec2(-0.1,0.75),font_program);

    
       
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &(basic_vao.id));
    glDeleteVertexArrays(1,&(import_vao.id));
    glDeleteVertexArrays(1,&(texture_vao.id));

    
    smiley.DeallocateShape();
    die.DeallocateShape();


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD,delta_time);
    }

    if (glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD,delta_time);
    }

    if (glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT,delta_time);
    }

    if (glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT,delta_time);
    }


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (first_mouse)
    {
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

    camera.ProcessMouseMovement(xoffset,yoffset);

    
}  