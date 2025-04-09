#include "camera.hpp"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up,float yaw, float pitch):
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY), Zoom(ZOOM), firstPersonMode(false)
    {
        Position = position;
        defaultPosition = position;
        lastPosition = position;
        WorldUp = up;
        Yaw = yaw;
        defaultYaw = yaw;
        lastYaw = yaw;
        Pitch = pitch;
        defaultPitch = pitch;
        lastPitch = pitch;
        updateCameraVectors();
}
    // Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch) :
                   Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
                   MouseSensitivity(SENSITIVITY), Zoom(ZOOM), firstPersonMode(false)
    {
        Position = glm::vec3(posX, posY, posZ);
        defaultPosition = Position;
        lastPosition = Position;
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        defaultYaw = yaw;
        lastYaw = yaw;
        Pitch = pitch;
        defaultPitch = pitch;
        lastPitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        // Skip keyboard processing if in first-person mode
        if (firstPersonMode) return;
        
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
        // Skip mouse movement processing if in first-person mode
        if (firstPersonMode) return;
        
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

    // Toggle first-person mode
void Camera::ToggleFirstPersonMode() {
    firstPersonMode = !firstPersonMode;
    
    if (firstPersonMode) {
        // Store current position and orientation when entering first-person mode
        lastPosition = Position;
        lastYaw = Yaw;
        lastPitch = Pitch;
    } else {
        // Restore the last camera position when exiting first-person mode
        RestoreLastView();
    }
}

// Set the camera position and orientation to match avatar
void Camera::SetToAvatarView(const glm::vec3& avatarPosition, float avatarRotation) {
    // Set camera position at eye level
    Position = avatarPosition + glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Set camera orientation based on avatar rotation
    // Convert avatar's Y-axis rotation to camera's yaw (with 180 degree adjustment)
    Yaw = avatarRotation;//-avatarRotation + 180.0f;
    
    // Update camera vectors to reflect new orientation
    updateCameraVectors();
}

// Restore the last camera position and orientation from before entering first-person mode
void Camera::RestoreLastView() {
    Position = lastPosition;
    Yaw = lastYaw;
    Pitch = lastPitch;
    updateCameraVectors();
}

// Restore the default camera position and orientation
void Camera::RestoreDefaultView() {
    Position = defaultPosition;
    Yaw = defaultYaw;
    Pitch = defaultPitch;
    updateCameraVectors();
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors(){
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
