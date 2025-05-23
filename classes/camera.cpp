
#include "camera.hpp"
#include <iostream>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up,float yaw, float pitch):
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY), Zoom(ZOOM), first_person_view(false)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
}
    // Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch) :
                   Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
                   MouseSensitivity(SENSITIVITY), Zoom(ZOOM), first_person_view(false)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
    {
        if (first_person_view) {
            // In first person view, the camera is looking at the front vector
            // print the camera position and front vector
            //std::cout << "Camera Position: " << Position.x << ", " << Position.y << ", " << Position.z << std::endl;
            return glm::lookAt(Position, Position + Front, Up);
        }
        else {
            // Normal third-person camera mode
            return glm::lookAt(Position, Position + Front, Up);
        }
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        // if the camera is in first person view, then none of the following movement will be allowed
        if (first_person_view) return;
        float velocity = MovementSpeed * deltaTime;
        glm::vec3 forward = Front;
        forward.y = 0.0f; // prevent upward movement
        if (direction == FORWARD)
            Position += glm::normalize(forward) * velocity;
        if (direction == BACKWARD)
            Position -= glm::normalize(forward) * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        if (first_person_view) return; // prevent movement in first person view
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void Camera::ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void Camera::updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }

