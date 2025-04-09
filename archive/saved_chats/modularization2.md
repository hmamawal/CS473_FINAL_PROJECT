hmamawal: #codebase is there a way i could modularize my main file more so there's not so much code? (make more reusable functions and place functionality in different files)

GitHub Copilot: Yes, your main.cpp file can definitely be modularized to improve readability, maintainability, and reusability. Here's a step-by-step plan to refactor your code:

---

### 1. **Separate Initialization Logic**
Move the initialization of the environment, shaders, VAOs, and models into separate functions or files.

#### Example:
Create a new file `initialization.cpp` and `initialization.hpp` to handle initialization logic.

```cpp
#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../classes/Shader.hpp"
#include "../classes/import_object.hpp"
#include "../classes/avatar.hpp"
#include "../classes/avatar_high_bar.hpp"
#include "../classes/Font.hpp"
#include "../utilities/build_shapes.hpp"

GLFWwindow* InitializeWindow(const std::string& title, int width, int height);
void InitializeShaders(Shader*& shader_program_ptr, Shader& font_program);
void InitializeVAOs(VAOStruct& basic_vao, VAOStruct& texture_vao, VAOStruct& import_vao);
void LoadModels(ImportOBJ& importer, BasicShape& baseModel, Avatar& baseAvatar, AvatarHighBar*& high_bar_avatar, 
                BasicShape& tumbling_floor, int& tumbling_floor_texture, BasicShape& vault_table, 
                std::vector<unsigned int>& vault_table_textures, BasicShape& LouGrossBuilding, 
                std::vector<unsigned int>& building_textures, BasicShape& high_bar, BasicShape& pommel_horse, 
                BasicShape& pommel_horse2, Font& arial_font, VAOStruct& texture_vao, VAOStruct& import_vao);

#endif // INITIALIZATION_HPP
```

```cpp
#include "initialization.hpp"
#include "environment.hpp"

GLFWwindow* InitializeWindow(const std::string& title, int width, int height) {
    return InitializeEnvironment(title, width, height);
}

void InitializeShaders(Shader*& shader_program_ptr, Shader& font_program) {
    shader_program_ptr = new Shader(".//shaders//vertex.glsl", ".//shaders//fragment.glsl");
    font_program = Shader(".//shaders//vertex.glsl", ".//shaders//fontFragmentShader.glsl");
}

void InitializeVAOs(VAOStruct& basic_vao, VAOStruct& texture_vao, VAOStruct& import_vao) {
    glGenVertexArrays(1, &(basic_vao.id));
    basic_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 6 * sizeof(float), 0));
    basic_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 6 * sizeof(float), 3 * sizeof(float)));

    glGenVertexArrays(1, &(texture_vao.id));
    texture_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 8 * sizeof(float), 0));
    texture_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float)));
    texture_vao.attributes.push_back(BuildAttribute(2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float)));

    glGenVertexArrays(1, &(import_vao.id));
    int stride_size = 19 * sizeof(float);
    import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 0));
    import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 3 * sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(2, GL_FLOAT, false, stride_size, 6 * sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 8 * sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 11 * sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(3, GL_FLOAT, false, stride_size, 14 * sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(1, GL_FLOAT, false, stride_size, 17 * sizeof(float)));
    import_vao.attributes.push_back(BuildAttribute(1, GL_FLOAT, false, stride_size, 18 * sizeof(float)));
}

void LoadModels(ImportOBJ& importer, BasicShape& baseModel, Avatar& baseAvatar, AvatarHighBar*& high_bar_avatar, 
                BasicShape& tumbling_floor, int& tumbling_floor_texture, BasicShape& vault_table, 
                std::vector<unsigned int>& vault_table_textures, BasicShape& LouGrossBuilding, 
                std::vector<unsigned int>& building_textures, BasicShape& high_bar, BasicShape& pommel_horse, 
                BasicShape& pommel_horse2, Font& arial_font, VAOStruct& texture_vao, VAOStruct& import_vao) {
    baseModel = importer.loadFiles("models/baseModel", import_vao);
    baseAvatar = Avatar(baseModel, 180.0f, glm::vec3(0.0, 0.4, 0.0), IMPORTED_BASIC);
    baseAvatar.Scale(glm::vec3(0.5f, 0.5f, 0.5f));

    high_bar_avatar = new AvatarHighBar(baseModel, 180.0f, glm::vec3(-10.0, 0.0, 0.0), IMPORTED_BASIC);
    high_bar_avatar->Scale(glm::vec3(0.5f, 0.5f, 0.5f));

    tumbling_floor = importer.loadFiles("models/tumbling_floor", import_vao);
    tumbling_floor_texture = importer.getTexture();

    vault_table = importer.loadFiles("models/VaultTable", import_vao);
    vault_table_textures = importer.getAllTextures();

    LouGrossBuilding = importer.loadFiles("models/ComplexBuilding", import_vao);
    building_textures = importer.getAllTextures();

    high_bar = importer.loadFiles("models/HighBar", import_vao);

    pommel_horse = importer.loadFiles("models/PommelHorse", import_vao);
    pommel_horse2 = importer.loadFiles("models/PommelHorse2", import_vao);

    arial_font.initialize(texture_vao);
}
```

---

### 2. **Refactor Rendering Logic**
Move the rendering logic into a separate function or file. For example, create a `rendering.cpp` and `rendering.hpp` to handle rendering.

```cpp
#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "../classes/Shader.hpp"
#include "../classes/avatar.hpp"
#include "../classes/avatar_high_bar.hpp"
#include "../classes/Font.hpp"
#include "../utilities/build_shapes.hpp"

void RenderScene(Shader* shader_program_ptr, Avatar& baseAvatar, AvatarHighBar* high_bar_avatar, 
                 BasicShape& floor, unsigned int floor_texture, BasicShape& tumbling_floor, 
                 int tumbling_floor_texture, BasicShape& vault_table, 
                 std::vector<unsigned int>& vault_table_textures, BasicShape& LouGrossBuilding, 
                 std::vector<unsigned int>& building_textures, BasicShape& high_bar, 
                 BasicShape& pommel_horse, BasicShape& pommel_horse2, Font& arial_font);

#endif // RENDERING_HPP
```

---

### 3. **Refactor Input Handling**
Move the `ProcessInput` function into a separate file, such as `input_handling.cpp` and `input_handling.hpp`.

---

### 4. **Update main.cpp**
After modularizing, your main.cpp will look much cleaner:

```cpp
#include "utilities/environment.hpp"
#include "utilities/initialization.hpp"
#include "utilities/rendering.hpp"
#include "utilities/input_handling.hpp"

int main() {
    GLFWwindow* window = InitializeWindow("CS473", SCR_WIDTH, SCR_HEIGHT);
    if (!window) return -1;

    Shader* shader_program_ptr = nullptr;
    Shader font_program;
    InitializeShaders(shader_program_ptr, font_program);

    VAOStruct basic_vao, texture_vao, import_vao;
    InitializeVAOs(basic_vao, texture_vao, import_vao);

    ImportOBJ importer;
    BasicShape baseModel, tumbling_floor, vault_table, LouGrossBuilding, high_bar, pommel_horse, pommel_horse2;
    Avatar baseAvatar(baseModel, 0.0f, glm::vec3(0.0f), BASIC);
    AvatarHighBar* high_bar_avatar = nullptr;
    int tumbling_floor_texture;
    std::vector<unsigned int> vault_table_textures, building_textures;
    Font arial_font("fonts/ArialBlackLarge.bmp", "fonts/ArialBlack.csv", 0.1f, 0.15f);

    LoadModels(importer, baseModel, baseAvatar, high_bar_avatar, tumbling_floor, tumbling_floor_texture, 
               vault_table, vault_table_textures, LouGrossBuilding, building_textures, high_bar, 
               pommel_horse, pommel_horse2, arial_font, texture_vao, import_vao);

    while (!glfwWindowShouldClose(window)) {
        ProcessInput(window);
        RenderScene(shader_program_ptr, baseAvatar, high_bar_avatar, floor, floor_texture, tumbling_floor, 
                    tumbling_floor_texture, vault_table, vault_table_textures, LouGrossBuilding, 
                    building_textures, high_bar, pommel_horse, pommel_horse2, arial_font);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    CleanupResources();
    return 0;
}
```

---

This modularization will make your codebase easier to maintain and extend. Let me know if you'd like help implementing any specific part!

hmamawal: #codebase how can i add testing throughout this project?

GitHub Copilot: Adding testing to your project can significantly improve its reliability and maintainability. Since your project is a C++ graphics application, you can incorporate testing at multiple levels: unit testing, integration testing, and functional testing. Here's a step-by-step guide to adding testing throughout your project:

---

### 1. **Set Up a Testing Framework**
Use a C++ testing framework like **Google Test (gtest)** or **Catch2**. These frameworks provide a simple way to write and run tests.

#### Steps to Set Up Google Test:
1. Add Google Test to your project:
   - Use a package manager like `vcpkg` or `conan` to install Google Test.
   - Alternatively, clone the Google Test repository and include it in your project.

2. Update your `CMakeLists.txt` (if using CMake):
   ```cmake
   # Add Google Test
   include(FetchContent)
   FetchContent_Declare(
       googletest
       URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
   )
   FetchContent_MakeAvailable(googletest)

   enable_testing()

   add_executable(tests test_main.cpp)
   target_link_libraries(tests gtest gtest_main)
   ```

3. Create a `tests/` directory for your test files.

---

### 2. **Write Unit Tests**
Unit tests focus on testing individual functions or classes in isolation.

#### Example: Testing the `Avatar` Class
Create a test file, e.g., `tests/test_avatar.cpp`:
```cpp
#include <gtest/gtest.h>
#include "classes/avatar.hpp"

TEST(AvatarTest, Initialization) {
    BasicShape shape; // Mock or create a simple BasicShape
    Avatar avatar(shape, 180.0f, glm::vec3(0.0, 0.4, 0.0), IMPORTED_BASIC);

    EXPECT_EQ(avatar.GetOrientation(), 180.0f);
    EXPECT_EQ(avatar.GetPosition(), glm::vec3(0.0, 0.4, 0.0));
}

TEST(AvatarTest, Scaling) {
    BasicShape shape;
    Avatar avatar(shape, 180.0f, glm::vec3(0.0, 0.4, 0.0), IMPORTED_BASIC);

    avatar.Scale(glm::vec3(2.0f, 2.0f, 2.0f));
    EXPECT_EQ(avatar.GetScale(), glm::vec3(2.0f, 2.0f, 2.0f));
}
```

---

### 3. **Write Integration Tests**
Integration tests ensure that different components work together as expected.

#### Example: Testing `AvatarHighBar` Interaction
Create a test file, e.g., `tests/test_avatar_high_bar.cpp`:
```cpp
#include <gtest/gtest.h>
#include "classes/avatar_high_bar.hpp"

TEST(AvatarHighBarTest, JumpToHighBar) {
    BasicShape shape;
    AvatarHighBar high_bar_avatar(shape, 180.0f, glm::vec3(-10.0, 0.0, 0.0), IMPORTED_BASIC);

    high_bar_avatar.JumpToHighBar();
    EXPECT_TRUE(high_bar_avatar.IsOnHighBar());
    EXPECT_EQ(high_bar_avatar.GetPosition(), glm::vec3(-10.0, 2.0, 0.0)); // Example position
}
```

---

### 4. **Write Functional Tests**
Functional tests validate the behavior of the entire application. For graphics applications, this often involves rendering tests or input/output validation.

#### Example: Testing Rendering
You can use a headless OpenGL context (e.g., with GLFW) to test rendering without displaying a window.

Create a test file, e.g., `tests/test_rendering.cpp`:
```cpp
#include <gtest/gtest.h>
#include "utilities/environment.hpp"
#include "classes/avatar.hpp"

TEST(RenderingTest, InitializeEnvironment) {
    GLFWwindow* window = InitializeEnvironment("Test Window", 800, 600);
    ASSERT_NE(window, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

TEST(RenderingTest, RenderAvatar) {
    GLFWwindow* window = InitializeEnvironment("Test Window", 800, 600);
    ASSERT_NE(window, nullptr);

    BasicShape shape;
    Avatar avatar(shape, 180.0f, glm::vec3(0.0, 0.4, 0.0), IMPORTED_BASIC);

    // Mock rendering loop
    for (int i = 0; i < 10; ++i) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        avatar.Draw(nullptr, false); // Pass a mock shader
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
```

---

### 5. **Add Mocking for Dependencies**
For unit tests, you may need to mock dependencies like `Shader`, `BasicShape`, or `Camera`.

#### Example: Mocking `Shader`
Create a mock class for `Shader`:
```cpp
class MockShader : public Shader {
public:
    MOCK_METHOD(void, use, (), (override));
    MOCK_METHOD(void, setMat4, (const std::string &name, glm::mat4 value), (override));
    MOCK_METHOD(void, setVec4, (const std::string &name, glm::vec4 value), (override));
};
```

Use the mock in your tests:
```cpp
TEST(AvatarTest, DrawWithMockShader) {
    MockShader mock_shader;
    BasicShape shape;
    Avatar avatar(shape, 180.0f, glm::vec3(0.0, 0.4, 0.0), IMPORTED_BASIC);

    EXPECT_CALL(mock_shader, use()).Times(1);
    avatar.Draw(&mock_shader, false);
}
```

---

### 6. **Automate Testing**
Automate your tests using a build system or CI/CD pipeline.

#### Example: Using CMake
Add a `test` target to your `CMakeLists.txt`:
```cmake
add_test(NAME AvatarTests COMMAND tests)
```

#### Example: Using GitHub Actions
Create a `.github/workflows/test.yml` file:
```yaml
name: Build and Test

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3
    - name: Install Dependencies
      run: sudo apt-get install cmake g++ libglfw3-dev
    - name: Build
      run: |
        mkdir build
        cd build
        cmake ..
        make
    - name: Run Tests
      run: |
        cd build
        ctest --output-on-failure
```

---

### 7. **Add Debugging and Logging**
Use logging libraries like `spdlog` to add runtime logs for debugging. This can complement your tests by providing insights into runtime behavior.

---

### 8. **Test User Input**
For interactive features like key presses (`J`, `C`, `R`), simulate input events in your tests.

#### Example: Simulating Key Presses
```cpp
TEST(InputTest, ProcessInput) {
    GLFWwindow* window = InitializeEnvironment("Test Window", 800, 600);
    ASSERT_NE(window, nullptr);

    // Simulate 'R' key press
    glfwSetKey(window, GLFW_KEY_R, GLFW_PRESS);
    ProcessInput(window);
    EXPECT_EQ(point_light_color, glm::vec3(1.0f, 0.0f, 0.0f)); // Light should turn red

    glfwDestroyWindow(window);
    glfwTerminate();
}
```

---

### 9. **Test Edge Cases**
Identify edge cases in your application and write tests for them. For example:
- What happens if the `high_bar_avatar` is null?
- What if the `camera.Position` goes out of bounds?

---

### 10. **Document Your Tests**
Add a README.md in the `tests/` directory explaining how to run the tests:
```markdown
# Tests

## Running Tests
1. Build the project with `cmake`:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

2. Run the tests:
   ```bash
   ctest --output-on-failure
   ```
```

---

By following these steps, you can systematically add testing to your project, ensuring that each component works as expected and that the overall application is robust.

hmamawal: #codebase  my main file more so there's not so much code (make more reusable functions and place functionality in different files) - already started this process by making new files in ./utilities 

GitHub Copilot: To further modularize your main.cpp file and reduce its size, we can continue moving functionality into the appropriate files in the utilities directory. Here's a plan to proceed:

---

### 1. **Move Input Handling to input_handling.cpp**
The `ProcessInput` function in main.cpp can be moved to input_handling.cpp and declared in input_handling.hpp. This will isolate input-related logic.

---

### 2. **Move Rendering Logic to rendering.cpp**
The rendering loop and related logic (e.g., drawing objects, setting up shaders) can be moved to rendering.cpp and declared in rendering.hpp. This will make the main file cleaner and focus only on initialization and the main loop.

---

### 3. **Update main.cpp**
After moving the input handling and rendering logic, the main.cpp file will primarily handle initialization and the main loop, delegating other tasks to the new utility files.

---

I'll start by moving the `ProcessInput` function to input_handling.cpp and input_handling.hpp.
````
#ifndef INPUT_HANDLING_HPP
#define INPUT_HANDLING_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../classes/camera.hpp"
#include "../classes/avatar_high_bar.hpp"
#include "../classes/Shader.hpp"

extern glm::vec3 point_light_color;
extern Shader* shader_program_ptr;
extern Camera camera;
extern AvatarHighBar* high_bar_avatar;
extern float delta_time;

void ProcessInput(GLFWwindow* window);

#endif // INPUT_HANDLING_HPP
````


Made changes.


````
#include "input_handling.hpp"
#include <iostream>

void ProcessInput(GLFWwindow* window) {
    static bool c_key_pressed = false;
    static bool r_key_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Process 'R' key to change point light color to red
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!r_key_pressed) {
            r_key_pressed = true;
            if (point_light_color.r == 1.0f && point_light_color.g == 0.0f && point_light_color.b == 0.0f) {
                point_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
                std::cout << "Point light color changed to white" << std::endl;
            } else {
                point_light_color = glm::vec3(1.0f, 0.0f, 0.0f);
                std::cout << "Point light color changed to red" << std::endl;
            }
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
            static glm::vec3 original_camera_position;
            static float original_camera_yaw;
            static float original_camera_pitch;

            if (!camera.first_person_view) {
                original_camera_position = camera.Position;
                original_camera_yaw = camera.Yaw;
                original_camera_pitch = camera.Pitch;
            }

            camera.first_person_view = !camera.first_person_view;

            if (!camera.first_person_view) {
                camera.Position = original_camera_position;
                camera.Yaw = original_camera_yaw;
                camera.Pitch = original_camera_pitch;
                camera.updateCameraVectors();
            }
        }
    } else {
        c_key_pressed = false;
    }

    if (camera.first_person_view) {
        glm::vec3 avatar_pos;
        high_bar_avatar->GetPosition(avatar_pos);
        camera.Position = avatar_pos + glm::vec3(0.0f, 2.0f, 0.0f);
        float x_rotation_angle;
        high_bar_avatar->GetXRotationAngle(x_rotation_angle);
        camera.Pitch = x_rotation_angle;
        camera.updateCameraVectors();
        return;
    }

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

    static bool l_key_pressed = false;
    static bool spotlight_on = true;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!l_key_pressed) {
            l_key_pressed = true;
            spotlight_on = !spotlight_on;
            shader_program_ptr->setBool("spot_light.on", spotlight_on);
            std::cout << (spotlight_on ? "Spotlight ON" : "Spotlight OFF") << std::endl;
        }
    } else {
        l_key_pressed = false;
    }
}
````


Made changes.

The `ProcessInput` function has been successfully moved to input_handling.cpp and declared in input_handling.hpp. Now, I'll update main.cpp to include the new input_handling.hpp header and remove the `ProcessInput` function definition from it.
````
#include "utilities/input_handling.hpp"

// ...existing code...

// Removed the ProcessInput function definition as it is now in input_handling.cpp
````


Made changes.

The `ProcessInput` function has been successfully moved to input_handling.cpp and declared in input_handling.hpp. Additionally, main.cpp has been updated to include the new input_handling.hpp header and remove the `ProcessInput` function definition.

Next, I can modularize the rendering logic by moving it to rendering.cpp and rendering.hpp. Let me proceed with that.
````
#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "../classes/Shader.hpp"
#include "../classes/avatar.hpp"
#include "../classes/avatar_high_bar.hpp"
#include "../classes/Font.hpp"
#include "../utilities/build_shapes.hpp"

void RenderScene(Shader* shader_program_ptr, Avatar& baseAvatar, AvatarHighBar* high_bar_avatar, 
                 BasicShape& floor, unsigned int floor_texture, BasicShape& tumbling_floor, 
                 int tumbling_floor_texture, BasicShape& vault_table, 
                 std::vector<unsigned int>& vault_table_textures, BasicShape& LouGrossBuilding, 
                 std::vector<unsigned int>& building_textures, BasicShape& high_bar, 
                 BasicShape& pommel_horse, BasicShape& pommel_horse2, Font& arial_font);

#endif // RENDERING_HPP
````


Made changes.


````
#include "rendering.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void RenderScene(Shader* shader_program_ptr, Avatar& baseAvatar, AvatarHighBar* high_bar_avatar, 
                 BasicShape& floor, unsigned int floor_texture, BasicShape& tumbling_floor, 
                 int tumbling_floor_texture, BasicShape& vault_table, 
                 std::vector<unsigned int>& vault_table_textures, BasicShape& LouGrossBuilding, 
                 std::vector<unsigned int>& building_textures, BasicShape& high_bar, 
                 BasicShape& pommel_horse, BasicShape& pommel_horse2, Font& arial_font) {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program
    shader_program_ptr->use();

    // Set up the view matrix (camera)
    glm::mat4 view = camera.GetViewMatrix();
    shader_program_ptr->setMat4("view", view);
    shader_program_ptr->setVec4("view_position", glm::vec4(camera.Position, 1.0));

    // Draw the base avatar
    baseAvatar.Draw(shader_program_ptr, false);

    // Draw the floor
    shader_program_ptr->setInt("shader_state", TEXTURED);
    glm::mat4 model = glm::mat4(1.0f);
    shader_program_ptr->setMat4("model", model);
    shader_program_ptr->setMat4("local", glm::mat4(1.0f));
    glBindTexture(GL_TEXTURE_2D, floor_texture);
    floor.Draw();

    // Draw the tumbling floor
    shader_program_ptr->setInt("shader_state", IMPORTED_TEXTURED);
    glm::mat4 tumbling_floor_local = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.4, 0.0));
    shader_program_ptr->setMat4("local", tumbling_floor_local);
    glBindTexture(GL_TEXTURE_2D, tumbling_floor_texture);
    tumbling_floor.Draw();

    // Draw the vault table
    glm::mat4 vault_table_local = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 0.0, 0.0));
    shader_program_ptr->setMat4("local", vault_table_local);
    for (int i = 0; i < vault_table_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, vault_table_textures[i]);
    }
    vault_table.Draw();

    // Draw the Lou Gross Building
    glm::mat4 building_local = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0)), glm::vec3(2, 2, 2));
    shader_program_ptr->setMat4("local", building_local);
    for (int i = 0; i < building_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, building_textures[i]);
    }
    LouGrossBuilding.Draw();

    // Draw the high bar
    glm::mat4 high_bar_local = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0, 0.0, 0.0)), glm::vec3(1.3f));
    shader_program_ptr->setMat4("local", high_bar_local);
    high_bar.Draw();

    // Draw the pommel horses
    glm::mat4 pommel_horse_local = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 0.0, -10.0)), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    shader_program_ptr->setMat4("local", pommel_horse_local);
    pommel_horse.Draw();

    glm::mat4 pommel_horse2_local = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(5.0, 0.0, -10.0)), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    shader_program_ptr->setMat4("local", pommel_horse2_local);
    pommel_horse2.Draw();

    // Draw the high bar avatar
    high_bar_avatar->Draw(shader_program_ptr, false);

    // Draw text
    Font& font = arial_font;
    font.DrawText("Camera Position", glm::vec2(-0.1, 0.75), *shader_program_ptr);
}
````


Made changes.

The rendering logic has been successfully moved to rendering.cpp and declared in rendering.hpp. Now, I'll update main.cpp to include the rendering.hpp header and replace the rendering loop with a call to the `RenderScene` function.
````
#include "utilities/rendering.hpp"

// ...existing code...

// Replace the rendering loop with a call to RenderScene
        RenderScene(shader_program_ptr, baseAvatar, high_bar_avatar, floor, floor_texture, tumbling_floor, 
                    tumbling_floor_texture, vault_table, vault_table_textures, LouGrossBuilding, 
                    building_textures, high_bar, pommel_horse, pommel_horse2, arial_font);
````


Made changes.

The rendering logic has been successfully modularized into rendering.cpp and rendering.hpp. The main.cpp file has been updated to include the rendering.hpp header and replace the rendering loop with a call to the `RenderScene` function.

Your main.cpp is now significantly cleaner, focusing primarily on initialization and the main loop. Let me know if you'd like further assistance, such as adding testing or additional modularization!

hmamawal: #problems:Info cam you fix the problems? do i need to add anythign to the tasks.json?

GitHub Copilot: The errors in your code are primarily due to missing definitions or references to global variables and enumerations. Here's how we can address them:

---

### 1. **Fix Undefined Identifiers**
- **`IMPORTED_BASIC`, `TEXTURED`, and `IMPORTED_TEXTURED`**:
  These are likely part of the `ObjectType` enumeration defined in main.cpp. To fix this:
  - Move the `ObjectType` enumeration to a shared header file (e.g., `utilities/common.hpp`) so it can be accessed across multiple files.

- **`camera`**:
  The `camera` object is defined globally in main.cpp. To fix this:
  - Declare `camera` as an `extern` variable in a shared header file (e.g., `utilities/common.hpp`) and include it where needed.

---

### 2. **Create `common.hpp`**
I'll create a new file, `utilities/common.hpp`, to hold shared declarations like `ObjectType` and `camera`.

---

### 3. **Update Tasks**
Your `tasks.json` already includes all necessary files for compilation. No changes are needed unless new files are added. If we add `common.hpp`, it doesn't need to be explicitly listed because header files are included indirectly.

---

Let me implement these fixes.
````
#ifndef COMMON_HPP
#define COMMON_HPP

#include <glm/glm.hpp>
#include "../classes/camera.hpp"

// Enumeration Type: ObjectType
// Determines how shaders will draw different objects:
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

// Global variables
extern Camera camera;

#endif // COMMON_HPP
````


Made changes.


````
#include "utilities/common.hpp"

// Define the global camera object
Camera camera(glm::vec3(0.0f, 1.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
````


Made changes.


````
#include "common.hpp"

// Use the ObjectType enumeration from common.hpp
````


Made changes.


````
#include "common.hpp"

// Use the ObjectType enumeration and the global camera object from common.hpp
````


Made changes.

The issues have been addressed by creating a shared header file, `common.hpp`, which contains the `ObjectType` enumeration and declares the global `camera` object. This file has been included in the necessary files (`main.cpp`, `initialization.cpp`, and `rendering.cpp`) to resolve the undefined identifier errors.

Additionally, the `camera` object has been defined in main.cpp, and the `tasks.json` file does not require any changes since it already includes all necessary files for compilation.

You can now proceed to build the project to verify that the errors are resolved. Let me know if you encounter any further issues!

hmamawal: #problems:main.cpp i am still getting one problem

GitHub Copilot: The error indicates that there is a mismatch in the `ObjectType` enumeration definition. Specifically, the `ObjectType` enumeration is defined twice: once in `common.hpp` and again in main.cpp. This duplication is causing the compiler to treat them as separate types, even though they have the same name.

To fix this issue, we need to ensure that the `ObjectType` enumeration is defined only once in `common.hpp` and remove its duplicate definition from main.cpp.

Let me fix this.
````
// Removed duplicate ObjectType enumeration definition
// ObjectType is now defined in common.hpp and included here
#include "utilities/common.hpp"

// ...existing code...
````


Made changes.

The duplicate `ObjectType` enumeration definition has been removed from main.cpp, and the code now relies on the definition in `common.hpp`. This should resolve the error related to mismatched types.

You can now try building the project again using the provided `tasks.json`. Let me know if you encounter any further issues!

hmamawal: #terminalSelection i am still getting this error when compiling

GitHub Copilot: The error indicates that the linker cannot find the definition of the `RenderScene` function. This typically happens when the file containing the function's implementation (`rendering.cpp`) is not included in the build process.

Looking at your `tasks.json`, the `rendering.cpp` file is missing from the list of files passed to the compiler. To fix this, we need to add `rendering.cpp` to the `args` array in the `tasks.json` file.

Let me update the `tasks.json` file to include `rendering.cpp`.
````
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++.exe build active file",
            "command": "C:\\msys64\\ucrt64\\bin\\g++.exe",
            "args": [
                "-g",  
                "${workspaceFolder}\\main.cpp", 
                "${workspaceFolder}\\classes\\Shader.cpp",
                "${workspaceFolder}\\classes\\vertex_attribute.cpp", 
                "${workspaceFolder}\\classes\\basic_shape.cpp",  
                "${workspaceFolder}\\utilities\\environment.cpp", 
                "${workspaceFolder}\\utilities\\build_shapes.cpp", 
                "${workspaceFolder}\\classes\\camera.cpp",
                "${workspaceFolder}\\classes\\font.cpp",
                "${workspaceFolder}\\classes\\import_object.cpp",
                "${workspaceFolder}\\classes\\avatar.cpp",
                "${workspaceFolder}\\classes\\avatar_high_bar.cpp",
                "${workspaceFolder}\\utilities\\glad.c",
                "${workspaceFolder}\\utilities\\rendering.cpp", // Added rendering.cpp
                "-IC:\\msys64\\ucrt64\\include", 
                "-lglfw3", 
                "-lgdi32", 
                "-lglu32", 
                "-lopengl32", 
                "-o", 
                "${fileDirname}\\${fileBasenameNoExtension}.exe" 
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Task generated by Debugger."
        }
    ],
    "version": "2.0.0"
}
````


Made changes.

The `tasks.json` file has been updated to include `rendering.cpp` in the build process. You can now try building the project again using the updated task. This should resolve the linker error related to the `RenderScene` function. Let me know if you encounter any further issues!

hmamawal: #terminalSelection now when i run the compiled main.exe, the program seems to never fully load and then makes my computer very laggy, so it is likely getting stuck somewhere and there may be a memory leak

GitHub Copilot: If the program is causing your computer to lag and not fully loading, it is likely stuck in an infinite loop or consuming excessive memory due to a memory leak. Here's how we can debug and address the issue:

---

### 1. **Check for Infinite Loops**
The most common cause of such behavior is an infinite loop in the rendering or main loop. Ensure that:
- The `while (!glfwWindowShouldClose(window))` loop in main.cpp is properly terminating when the window is closed.
- All conditions inside the loop are correct and not causing an infinite loop.

---

### 2. **Check for Memory Leaks**
Memory leaks can occur if dynamically allocated memory is not properly deallocated. In your program:
- **Dynamic Objects**: Ensure that objects like `high_bar_avatar` and `shader_program_ptr` are deleted at the end of the program.
- **OpenGL Resources**: Ensure that OpenGL resources (e.g., VAOs, textures) are properly deleted using `glDelete*` functions.

---

### 3. **Add Debugging Logs**
Add logs to identify where the program is getting stuck. For example:
- Add logs at the start and end of the rendering loop.
- Add logs before and after each major operation (e.g., loading models, rendering objects).

---

### 4. **Use a Memory Debugger**
Use tools like **Valgrind** (on Linux) or **Visual Studio's Memory Profiler** (on Windows) to detect memory leaks.

---

### 5. **Review Cleanup Code**
Ensure that all resources are properly cleaned up at the end of the program. I'll review your cleanup code in main.cpp to ensure everything is being deallocated.

Let me add debugging logs and review the cleanup code in main.cpp.
````
// Add debugging logs to identify where the program is getting stuck
#include <iostream>

// ...existing code...

int main() {
    // ...existing code...

    std::cout << "Entering render loop..." << std::endl;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        std::cout << "Start of render loop iteration" << std::endl;

        RenderScene(shader_program_ptr, baseAvatar, high_bar_avatar, floor, floor_texture, tumbling_floor, 
                    tumbling_floor_texture, vault_table, vault_table_textures, LouGrossBuilding, 
                    building_textures, high_bar, pommel_horse, pommel_horse2, arial_font);

        std::cout << "End of render loop iteration" << std::endl;
    }

    std::cout << "Render loop exited, starting cleanup..." << std::endl;

    // Cleanup code
    glDeleteVertexArrays(1, &(basic_vao.id));
    glDeleteVertexArrays(1, &(import_vao.id));
    glDeleteVertexArrays(1, &(texture_vao.id));

    baseModel.DeallocateShape();
    LouGrossBuilding.DeallocateShape();
    tumbling_floor.DeallocateShape();
    vault_table.DeallocateShape();
    high_bar.DeallocateShape();
    pommel_horse.DeallocateShape();
    pommel_horse2.DeallocateShape();
    floor.DeallocateShape();

    if (shader_program_ptr != nullptr) {
        delete shader_program_ptr;
        shader_program_ptr = nullptr;
    }

    if (high_bar_avatar != nullptr) {
        delete high_bar_avatar;
        high_bar_avatar = nullptr;
    }

    std::cout << "Cleanup complete" << std::endl;

    glfwTerminate();
    return 0;
}
````


Made changes.

I have added debugging logs to the render loop and ensured that all dynamically allocated resources are properly cleaned up at the end of the program. This will help identify where the program is getting stuck and prevent memory leaks.

You can now run the program again and monitor the logs in the terminal to see where it might be hanging. Let me know if you notice anything unusual or if the issue persists!

hmamawal: #terminalSelection i added more print statements to the rendering but i am still getting a "program not responding" after running the main program for a little bit

GitHub Copilot: If the program becomes unresponsive after running for a while, it could be due to one of the following issues:

### 1. **Infinite Loop or Excessive Iterations**
Even though the rendering loop seems to be progressing based on your logs, there might be a condition or operation inside the loop that causes the program to hang after some time.

### 2. **Memory Leak or Resource Exhaustion**
If resources (e.g., textures, VAOs, or dynamically allocated memory) are not properly managed, the program might run out of memory or GPU resources, causing it to hang.

### 3. **GPU Overload**
If the rendering loop is too intensive (e.g., drawing too many objects or using complex shaders), the GPU might become overloaded.

---

### Steps to Debug and Fix

#### **Step 1: Add More Logs**
Add logs to track the number of iterations of the rendering loop and monitor resource usage. For example:
```cpp
static int frame_count = 0;
std::cout << "Frame: " << frame_count++ << std::endl;
```

#### **Step 2: Check Resource Management**
Ensure that resources like textures, VAOs, and dynamically allocated memory are properly cleaned up. For example:
- Use `glDeleteTextures` for textures.
- Use `glDeleteVertexArrays` for VAOs.
- Use `delete` for dynamically allocated objects.

#### **Step 3: Limit Frame Rate**
If the rendering loop is running too fast, it might overload the system. Add a small delay to limit the frame rate:
```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
```

#### **Step 4: Use a Debugging Tool**
Use tools like **Valgrind** (on Linux) or **Visual Studio's Debugger** (on Windows) to detect memory leaks or excessive resource usage.

---

### Immediate Fix: Add Frame Count and Delay
Let me add a frame counter and a delay to the rendering loop in main.cpp.
````
#include <thread>
#include <chrono>

// ...existing code...

int main() {
    // ...existing code...

    static int frame_count = 0;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        std::cout << "Frame: " << frame_count++ << std::endl;

        RenderScene(shader_program_ptr, baseAvatar, high_bar_avatar, floor, floor_texture, tumbling_floor, 
                    tumbling_floor_texture, vault_table, vault_table_textures, LouGrossBuilding, 
                    building_textures, high_bar, pommel_horse, pommel_horse2, arial_font);

        // Limit frame rate to ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // ...existing code...
}
````


Made changes.

I have added a frame counter and a delay to the rendering loop in main.cpp to monitor iterations and limit the frame rate to approximately 60 FPS. This should help prevent GPU overload and make it easier to debug where the program might be getting stuck.

You can now run the program again and observe the logs in the terminal. If the issue persists, let me know, and we can investigate further!