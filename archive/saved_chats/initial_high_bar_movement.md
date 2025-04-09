hmamawal: @workspace how can i create a new avatar class in ./classes called avatar_high_bar that is the same as the avatar class, but it inherits it's traits from the avatar class (see test.hpp and .cpp for example of inheritance)

Workspace: Collecting workspace informationI'll show you how to create the `AvatarHighBar` class that inherits from `Avatar`. Based on the examples in [test.hpp](c:\Users\hunter.mamawal\Classes\CS473_Computer_Graphics\CS473_PROJECT2\classes\test.hpp) and [test.cpp](c:\Users\hunter.mamawal\Classes\CS473_Computer_Graphics\CS473_PROJECT2\classes\test.cpp), here's how you should implement it:

First, create the header file:

```cpp
#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Add any additional properties specific to AvatarHighBar
        
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        // Override any methods as needed or add new ones
        // Example: void ProcessInput(GLFWwindow *window, float time_passed) override;
};

#endif //AVATAR_HIGH_BAR_HPP
```

Then, create the implementation file:

```cpp
#include "avatar_high_bar.hpp"

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                            glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize any additional properties specific to AvatarHighBar
}

// Implement any additional methods or overridden methods
// Example:
// void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
//     // Custom implementation or call parent method
//     Avatar::ProcessInput(window, time_passed);
//     // Additional functionality
// }
```

This gives you a basic skeleton that inherits from the `Avatar` class, similar to how `TestChild` does it. You can add additional functionality as needed.

hmamawal: can i add new functions to the inherited class? how would i do so? can i add public properties/variables as well?

GitHub Copilot: ## Adding New Functions and Properties to the Inherited Class

Yes, you can absolutely add new functions and properties to your `AvatarHighBar` class. That's one of the main benefits of inheritance - extending the base class with new functionality.

Here's how to do it:

### 1. Adding New Properties/Variables

Add new member variables in the protected or public section of your class definition:

```cpp
#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // New protected member variables
        bool is_on_bar;
        float bar_height;
        glm::vec3 bar_position;
        
    public:
        // New public member variables
        float swing_angle;
        bool can_dismount;
        
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        // New methods
        void Mount(glm::vec3 bar_position);
        void Dismount();
        void Swing(float time_passed);
        bool IsOnBar() const;
        
        // Override methods from parent class
        void ProcessInput(GLFWwindow *window, float time_passed) override;
        void Draw(Shader *shader, bool use_shader=false) override;
};

#endif //AVATAR_HIGH_BAR_HPP
```

### 2. Implementing New Functions

Then implement these new functions in your `.cpp` file:

```cpp
#include "avatar_high_bar.hpp"
#include <glm/gtc/matrix_transform.hpp>

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                            glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize the new properties
    this->is_on_bar = false;
    this->bar_height = 7.0f;
    this->bar_position = glm::vec3(0.0f);
    this->swing_angle = 0.0f;
    this->can_dismount = false;
}

void AvatarHighBar::Mount(glm::vec3 bar_position) {
    if (!is_on_bar) {
        this->is_on_bar = true;
        this->bar_position = bar_position;
        // Move the avatar to the bar
        this->position = bar_position;
        // Reset any ongoing animations
        this->is_flipping = false;
        this->flip_height = 0.0f;
        this->flip_progress = 0.0f;
        // Initialize swing angle
        this->swing_angle = 0.0f;
    }
}

void AvatarHighBar::Dismount() {
    if (is_on_bar && can_dismount) {
        this->is_on_bar = false;
        // Perhaps trigger a dismount animation or jump
        this->is_flipping = true;
        this->flip_progress = 0.0f;
    }
}

void AvatarHighBar::Swing(float time_passed) {
    if (is_on_bar) {
        // Simple pendulum motion
        this->swing_angle += time_passed * 45.0f; // 45 degrees per second
        // Allow dismount when at appropriate swing angle
        this->can_dismount = (abs(sin(glm::radians(this->swing_angle))) > 0.8f);
    }
}

bool AvatarHighBar::IsOnBar() const {
    return this->is_on_bar;
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    if (is_on_bar) {
        // High bar specific controls
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            Swing(time_passed);
        }
        
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Dismount();
        }
    } 
    else {
        // Use regular Avatar controls when not on bar
        Avatar::ProcessInput(window, time_passed);
        
        // Additional key to mount the bar when close
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            // Check if close to bar (pseudo-code)
            float distance = glm::length(this->position - this->bar_position);
            if (distance < 2.0f) {
                Mount(this->bar_position);
            }
        }
    }
}

void AvatarHighBar::Draw(Shader *shader, bool use_shader) {
    if (is_on_bar) {
        if (use_shader) {
            shader->use();
        }
        
        glm::mat4 local(1.0);
        local = glm::translate(local, this->bar_position);
        // Apply swing rotation
        local = glm::rotate(local, glm::radians(sin(glm::radians(this->swing_angle)) * 90.0f), 
                            glm::vec3(0.0f, 0.0f, 1.0f));
        // Offset position to hang from bar
        local = glm::translate(local, glm::vec3(0.0f, -2.0f, 0.0f));
        local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
        local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
        local = glm::scale(local, this->scale);
        
        shader->setMat4("local", local);
        shader->setInt("shader_state", this->shader_state);
        this->body.Draw();
    } 
    else {
        // Use regular Avatar drawing when not on bar
        Avatar::Draw(shader, use_shader);
    }
}
```

### Key Points

1. **Public vs Protected Variables:**
   - `public`: Accessible from anywhere
   - `protected`: Accessible within this class and any derived classes

2. **Override Keyword:**
   - Using `override` when overriding parent methods is good practice as it helps catch errors (if the parent method signature changes)

3. **Calling Parent Methods:**
   - You can call the parent implementation with `Avatar::MethodName()` when you want to build upon the existing behavior

4. **Constructor Inheritance:**
   - Always call the parent constructor as the first thing in your constructor with the initialization list

This implementation demonstrates how to create a specialized avatar that can mount, swing on, and dismount from a high bar while maintaining all the base functionality from the original `Avatar` class.

hmamawal: in main, import a new avatar_high_bar object at the same location as the high bar object

GitHub Copilot: I'll add code to import a new `AvatarHighBar` object at the same location as the high bar object in the main.cpp file. First, let's add the necessary include statement for the `avatar_high_bar.hpp` file and then create an instance of the class at the appropriate location.
````
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utilities/environment.hpp"
#include "utilities/build_shapes.hpp"
#include "classes/camera.hpp"
#include "classes/Font.hpp"
#include "classes/import_object.hpp"
#include "classes/avatar.hpp"
#include "classes/avatar_high_bar.hpp"
````


Made changes.

Now, let's add the `AvatarHighBar` object at the same location as the high bar object. I'll add this code right after where the high bar is created in the main function:
````
        // Draw the high bar
        shader_program.setInt("shader_state", IMPORTED_BASIC);
        glm::mat4 high_bar_local(1.0);
        high_bar_local = glm::translate(high_bar_local, glm::vec3(-10.0, 0.0, 0.0));
        shader_program.setMat4("model", identity);
        shader_program.setMat4("local", high_bar_local);
        high_bar.Draw();

        // Create AvatarHighBar at the same position as the high bar
        static AvatarHighBar high_bar_avatar(baseModel, 180.0f, glm::vec3(-10.0, 2.4, 0.0), IMPORTED_BASIC);
        high_bar_avatar.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
        high_bar_avatar.ProcessInput(window, delta_time);
        high_bar_avatar.Draw(&shader_program, false);
````


Made changes.

Now that I've added the `AvatarHighBar` object to your main.cpp file, let's update your build task to include the new `avatar_high_bar.cpp` file in the compilation process. This is necessary for the compiler to recognize and build your new class.
````
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
		"-IC:\\msys64\\ucrt64\\include",
		"-lglfw3",
		"-lgdi32",
		"-lglu32",
		"-lopengl32",
		"-o",
		"${fileDirname}\\${fileBasenameNoExtension}.exe"
	],
	// ...existing code...
}
````


Made changes.

Let's compile and run the program to see our `AvatarHighBar` in action:

hmamawal: how would i overide the processinput function to simply move the avatarhighbar up by 1.0 when the 'J' then move the player back down to it's original position when 'J' is pressed again?

GitHub Copilot: # Overriding ProcessInput for AvatarHighBar

To make your `AvatarHighBar` move up by 1.0 when 'J' is pressed and down again when 'J' is pressed again, you need to:

1. Add a toggle state variable to track whether the avatar is up or down
2. Override the `ProcessInput` method to check for the 'J' key
3. Implement the up/down functionality based on the toggle state

Here's how to modify your avatar_high_bar.hpp and avatar_high_bar.cpp files:

First, update your header file to add the toggle state:

```cpp
#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Add state to track if avatar is raised
        bool is_raised;
        glm::vec3 original_position;
        bool key_pressed; // To prevent multiple toggles on a single key press
        
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        // Override ProcessInput to implement the J key toggling
        void ProcessInput(GLFWwindow *window, float time_passed) override;
};

#endif //AVATAR_HIGH_BAR_HPP
```

Then, implement the method in your cpp file:

```cpp
#include "avatar_high_bar.hpp"

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                           glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize the toggle state variables
    this->is_raised = false;
    this->original_position = initial_position;
    this->key_pressed = false;
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // Call the parent class ProcessInput to maintain original functionality
    Avatar::ProcessInput(window, time_passed);
    
    // Check for J key press
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        // Only toggle if the key wasn't already pressed
        if (!key_pressed) {
            key_pressed = true;
            
            if (!is_raised) {
                // Move up by 1.0
                this->position.y += 1.0f;
                is_raised = true;
            } else {
                // Move back to original height
                this->position.y = original_position.y;
                is_raised = false;
            }
        }
    } else {
        // Reset key state when key is released
        key_pressed = false;
    }
}
```

This implementation:

1. Adds three new member variables:
   - `is_raised` - tracks whether the avatar is currently raised
   - `original_position` - stores the initial position to return to
   - `key_pressed` - prevents multiple toggles from a single key press

2. Calls the parent class's `ProcessInput` method first to keep the existing functionality

3. Implements the toggle logic:
   - When 'J' is pressed, check if it's a new press
   - Toggle between raised and lowered positions
   - When 'J' is released, reset the key state to allow for the next toggle

This approach ensures the avatar only toggles once per key press, not continuously while the key is held down.

hmamawal: @workspace /fix #problems:avatar_high_bar.hpp 

Workspace: The error is occurring because there's a syntax issue in your avatar.hpp file. Looking at the declaration of `ProcessInput` in the `Avatar` class, there's a space between `ProcessInput` and the opening parenthesis, which causes the method signature to not match exactly.

Let's fix both files:

```cpp
#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Add any additional properties specific to AvatarHighBar
        bool is_raised;
        glm::vec3 original_position;
        bool key_pressed; // To prevent multiple toggles on a single key press
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        // Match the exact signature in the parent class (including spaces)
        void ProcessInput(GLFWwindow *window, float time_passed);
};

#endif //AVATAR_HIGH_BAR_HPP
```

The issue is that in the avatar.hpp file, the method is declared as:
```cpp
void ProcessInput (GLFWwindow *window, float time_passed);
```

But in your avatar_high_bar.hpp file, you've declared it as:
```cpp
void ProcessInput(GLFWwindow *window, float time_passed) override;
```

The space between the method name and parentheses and the `override` keyword are causing the mismatch. Remove the `override` keyword and match the spacing exactly with the parent class's declaration.

hmamawal: how would i add functionality to the processinput function to where the object will continuously perform the following transformaitons: move z by -0.1, move y by 0.1, rotate by 10.0. i want it to do this until it has rotated by 90 degrees, then it will do the following for anothe r90 degrees: move z by 0.1, move y by 0.1, rotate by 10.0. for the next 90.0 degrees: move z by 0.1, move y by -0.1, rotate by 10.0. then for the last 90 degrees: move z by 0.1, move y by -0.1, rotate by 10.0. i want this sequence to start to continuously happen once the key 'M' is pressed and only stop when the key 'M' is pressed again or 'J' is pressed. each movement will be made every 0.2 seconds. This is to simulate the character rotating about a certain point on the highbar (like they're swinging around the high bar)

GitHub Copilot: # Adding High Bar Rotation Animation to AvatarHighBar

To implement the high bar rotation animation as you described, we need to:

1. Add state variables to track rotation progress and timing
2. Add a toggle for the animation with 'M' key
3. Implement the different movement phases based on rotation angle
4. Make it reset when 'J' or 'M' is pressed again

Here's how you can modify your avatar_high_bar.hpp and avatar_high_bar.cpp files:

First, update your header file to add the needed variables:

```cpp
#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Existing properties
        bool is_raised;
        glm::vec3 original_position;
        bool key_pressed; // To prevent multiple toggles on a single key press
        
        // New properties for the rotation animation
        bool is_rotating;         // Is the avatar currently rotating around the bar
        float rotation_angle;     // Current angle in the rotation (0-360 degrees)
        float time_since_move;    // Time accumulator for movement timing
        bool m_key_pressed;       // Track M key state to prevent multiple toggles
        
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        void ProcessInput(GLFWwindow *window, float time_passed);
};

#endif //AVATAR_HIGH_BAR_HPP
```

Now implement the updated `ProcessInput` method in your cpp file:

```cpp
#include "avatar_high_bar.hpp"
#include <glm/gtc/matrix_transform.hpp>

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                           glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize the toggle state variables
    this->is_raised = false;
    this->original_position = initial_position;
    this->key_pressed = false;
    
    // Initialize rotation animation variables
    this->is_rotating = false;
    this->rotation_angle = 0.0f;
    this->time_since_move = 0.0f;
    this->m_key_pressed = false;
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // Check for rotation toggle with M key
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!m_key_pressed) {
            m_key_pressed = true;
            is_rotating = !is_rotating;
            
            if (is_rotating) {
                // Store initial position when starting rotation
                original_position = position;
                rotation_angle = 0.0f;
                time_since_move = 0.0f;
            }
        }
    } else {
        m_key_pressed = false;
    }
    
    // Check for J key to stop rotation and perform J key functionality
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!key_pressed) {
            key_pressed = true;
            
            // Stop rotation if it's active
            if (is_rotating) {
                is_rotating = false;
            }
            
            // Original J key toggle functionality
            if (!is_raised) {
                this->position.y += 1.0f;
                is_raised = true;
            } else {
                this->position.y = original_position.y;
                is_raised = false;
            }
        }
    } else {
        key_pressed = false;
    }
    
    // Handle rotation animation if active
    if (is_rotating) {
        // Accumulate time since last movement
        time_since_move += time_passed;
        
        // Move every 0.2 seconds
        if (time_since_move >= 0.2f) {
            // Reset timer
            time_since_move = 0.0f;
            
            // Perform movement based on current rotation quadrant
            if (rotation_angle < 90.0f) {
                // First quadrant: move z by -0.1, move y by 0.1, rotate by 10.0
                position.z -= 0.1f;
                position.y += 0.1f;
                rotation_angle += 10.0f;
            } 
            else if (rotation_angle < 180.0f) {
                // Second quadrant: move z by 0.1, move y by 0.1, rotate by 10.0
                position.z += 0.1f;
                position.y += 0.1f;
                rotation_angle += 10.0f;
            } 
            else if (rotation_angle < 270.0f) {
                // Third quadrant: move z by 0.1, move y by -0.1, rotate by 10.0
                position.z += 0.1f;
                position.y -= 0.1f;
                rotation_angle += 10.0f;
            } 
            else if (rotation_angle < 360.0f) {
                // Fourth quadrant: move z by -0.1, move y by -0.1, rotate by 10.0
                position.z -= 0.1f;
                position.y -= 0.1f;
                rotation_angle += 10.0f;
            }
            
            // Reset angle if we complete a full circle
            if (rotation_angle >= 360.0f) {
                rotation_angle = 0.0f;
            }
            
            // Update avatar rotation to match movement
            current_rotation = rotation_angle;
        }
    } else {
        // Only run parent ProcessInput when not rotating
        Avatar::ProcessInput(window, time_passed);
    }
}
```

This implementation:

1. Adds new variables to track:
   - `is_rotating` - whether the rotation animation is active
   - `rotation_angle` - current angle in the 360-degree rotation
   - `time_since_move` - time accumulator for the 0.2-second movement timer
   - `m_key_pressed` - tracks M key state for toggle

2. Implements the M key toggle to start/stop rotation

3. Makes J key stop the rotation and perform its original function

4. When rotating:
   - Moves the avatar every 0.2 seconds based on time accumulation
   - Applies different movement patterns for each 90-degree quadrant:
     - 0-90°: z-0.1, y+0.1, rotate+10.0
     - 90-180°: z+0.1, y+0.1, rotate+10.0
     - 180-270°: z+0.1, y-0.1, rotate+10.0
     - 270-360°: z-0.1, y-0.1, rotate+10.0
   - Resets to the beginning when a full 360° is complete

5. Disables the normal movement controls when rotating

These movements will create a circular path around the high bar, simulating a gymnast swinging around it.

hmamawal: i want it to rotate about the x axis, not the y axis

GitHub Copilot: I'll update the code to make the avatar rotate around the x-axis instead of the y-axis during the high bar rotation animation. This will create a more realistic gymnast-like rotation around the high bar.

```cpp
#include "avatar_high_bar.hpp"
#include <glm/gtc/matrix_transform.hpp>

AvatarHighBar::AvatarHighBar(BasicShape shape, float orientation, 
                           glm::vec3 initial_position, int shader_state) 
    : Avatar(shape, orientation, initial_position, shader_state) {
    // Initialize the toggle state variables
    this->is_raised = false;
    this->original_position = initial_position;
    this->key_pressed = false;
    
    // Initialize rotation animation variables
    this->is_rotating = false;
    this->rotation_angle = 0.0f;
    this->time_since_move = 0.0f;
    this->m_key_pressed = false;
}

void AvatarHighBar::ProcessInput(GLFWwindow *window, float time_passed) {
    // Check for rotation toggle with M key
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!m_key_pressed) {
            m_key_pressed = true;
            is_rotating = !is_rotating;
            
            if (is_rotating) {
                // Store initial position when starting rotation
                original_position = position;
                rotation_angle = 0.0f;
                time_since_move = 0.0f;
            }
        }
    } else {
        m_key_pressed = false;
    }
    
    // Check for J key to stop rotation and perform J key functionality
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!key_pressed) {
            key_pressed = true;
            
            // Stop rotation if it's active
            if (is_rotating) {
                is_rotating = false;
            }
            
            // Original J key toggle functionality
            if (!is_raised) {
                this->position.y += 1.0f;
                is_raised = true;
            } else {
                this->position.y = original_position.y;
                is_raised = false;
            }
        }
    } else {
        key_pressed = false;
    }
    
    // Handle rotation animation if active
    if (is_rotating) {
        // Accumulate time since last movement
        time_since_move += time_passed;
        
        // Move every 0.2 seconds
        if (time_since_move >= 0.2f) {
            // Reset timer
            time_since_move = 0.0f;
            
            // Perform movement based on current rotation quadrant
            if (rotation_angle < 90.0f) {
                // First quadrant: move z by -0.1, move y by 0.1, rotate by 10.0 around X
                position.z -= 0.1f;
                position.y += 0.1f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            } 
            else if (rotation_angle < 180.0f) {
                // Second quadrant: move z by 0.1, move y by 0.1, rotate by 10.0 around X
                position.z += 0.1f;
                position.y += 0.1f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            } 
            else if (rotation_angle < 270.0f) {
                // Third quadrant: move z by 0.1, move y by -0.1, rotate by 10.0 around X
                position.z += 0.1f;
                position.y -= 0.1f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            } 
            else if (rotation_angle < 360.0f) {
                // Fourth quadrant: move z by -0.1, move y by -0.1, rotate by 10.0 around X
                position.z -= 0.1f;
                position.y -= 0.1f;
                rotation_angle += 10.0f;
                // Set rotation around X-axis instead of Y
                current_rotation = rotation_angle;
            }
            
            // Reset angle if we complete a full circle
            if (rotation_angle >= 360.0f) {
                rotation_angle = 0.0f;
            }
        }
    } else {
        // Only run parent ProcessInput when not rotating
        Avatar::ProcessInput(window, time_passed);
    }
}
```

The issue is that in the base `Avatar` class, the `current_rotation` is applied around the Y-axis. We need to override the `Draw` method to apply rotation around the X-axis when rotating. Let's add this to the avatar_high_bar.hpp file and implement it:

```cpp
#ifndef AVATAR_HIGH_BAR_HPP
#define AVATAR_HIGH_BAR_HPP

#include "avatar.hpp"

class AvatarHighBar : public Avatar {
    protected:
        // Existing properties
        bool is_raised;
        glm::vec3 original_position;
        bool key_pressed; // To prevent multiple toggles on a single key press
        
        // New properties for the rotation animation
        bool is_rotating;         // Is the avatar currently rotating around the bar
        float rotation_angle;     // Current angle in the rotation (0-360 degrees)
        float time_since_move;    // Time accumulator for movement timing
        bool m_key_pressed;       // Track M key state to prevent multiple toggles
        
    public:
        // Constructor that passes parameters to the parent Avatar class
        AvatarHighBar(BasicShape shape, float orientation, glm::vec3 initial_position, int shader_state);
        
        void ProcessInput(GLFWwindow *window, float time_passed);
        
        // Override Draw method to handle X-axis rotation
        void Draw(Shader *shader, bool use_shader=false);
};

#endif //AVATAR_HIGH_BAR_HPP
```

Then implement the Draw method:

```cpp
// Add this method after ProcessInput

void AvatarHighBar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        shader->use();
    }
    
    glm::mat4 local(1.0);
    local = glm::translate(local, this->position);
    
    // Add vertical offset for jump
    if (is_flipping) {
        local = glm::translate(local, glm::vec3(0.0f, flip_height, 0.0f));
        
        // Start rotation only after initial rise (20% into animation)
        // Complete rotation before landing (90% of animation)
        if (flip_progress > 0.2f && flip_progress < 0.9f) {
            // Map 0.2-0.9 range to 0-1 for rotation progress
            float rotation_progress = (flip_progress - 0.2f) / 0.7f;
            // Apply faster rotation in middle of jump
            local = glm::rotate(local, glm::radians(rotation_progress * 360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        } else if (flip_progress >= 0.9f) {
            // Keep final rotation
            local = glm::rotate(local, glm::radians(360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
    
    if (is_rotating) {
        // Apply rotation around X-axis when rotating around high bar
        local = glm::rotate(local, glm::radians(rotation_angle), glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        // Apply normal Y-axis rotation when not rotating
        local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
    }
    
    // Apply initial rotation (always around Y-axis)
    local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::scale(local, this->scale);
    
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->body.Draw();
}
```

These changes will make the avatar rotate around the X-axis during the high bar rotation animation, while maintaining normal Y-axis rotation for regular movement. The movement pattern still traces a circular path, but now the avatar will rotate appropriately like a gymnast on a high bar.

hmamawal: @workspace /fix #terminalSelection 

Workspace: I see the error from the terminal: `undefined reference to 'AvatarHighBar::Draw(Shader*, bool)'`. This indicates that you've declared the `Draw` method in your avatar_high_bar.hpp file, but haven't provided an implementation in avatar_high_bar.cpp.

Here's how to implement the `Draw` method for `AvatarHighBar`:

```cpp
// Add this implementation at the end of the file

void AvatarHighBar::Draw(Shader *shader, bool use_shader) {
    if (use_shader) {
        shader->use();
    }
    glm::mat4 local(1.0);
    local = glm::translate(local, this->position);
    
    // Add rotation for the highbar avatar
    if (is_rotating) {
        // Modify the rotation to happen around the X-axis for the highbar animation
        local = glm::rotate(local, glm::radians(rotation_angle), glm::vec3(1.0, 0.0, 0.0));
    } else {
        // Handle regular flipping animation from parent class
        if (is_flipping) {
            local = glm::translate(local, glm::vec3(0.0f, flip_height, 0.0f));
            
            if (flip_progress > 0.2f && flip_progress < 0.9f) {
                float rotation_progress = (flip_progress - 0.2f) / 0.7f;
                local = glm::rotate(local, glm::radians(rotation_progress * 360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            } else if (flip_progress >= 0.9f) {
                local = glm::rotate(local, glm::radians(360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            }
        }
        
        // Apply regular Y-axis rotation for navigation
        local = glm::rotate(local, glm::radians(this->current_rotation), glm::vec3(0.0, 1.0, 0.0));
    }
    
    // Always apply initial rotation
    local = glm::rotate(local, glm::radians(this->initial_rotation), glm::vec3(0.0, 1.0, 0.0));
    local = glm::scale(local, this->scale);
    
    shader->setMat4("local", local);
    shader->setInt("shader_state", this->shader_state);
    this->body.Draw();
}
```

This implementation handles both the special rotation animation for the high bar avatar and the regular flipping animation inherited from the base Avatar class. The key difference is that when `is_rotating` is true, it applies rotation around the X-axis based on the stored `rotation_angle` value, which is updated in your `ProcessInput` method.