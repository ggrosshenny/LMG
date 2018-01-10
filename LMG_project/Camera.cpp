#include "Camera.h"


// Constructors


Camera::Camera(glm::vec3 position, glm::vec3 up)
{
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraSpeed = SPEED;
    this->mouseSensitiviy = SENSITIVITY;
    this->zoom = ZOOM;
    this->cameraPosition = position;
    this->worldUp = up;
    this->yaw = YAW;
    this->pitch = PITCH;

    this->updateCameraVector();
}


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraSpeed = SPEED;
    this->mouseSensitiviy = SENSITIVITY;
    this->zoom = ZOOM;
    this->cameraPosition = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;

    this->updateCameraVector();
}


// Auxiliary methods


void Camera::updateCameraVector()
{
    /// Compute the new front vector by calculating eular angles
    glm::vec3 front;
    front.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
    front.y = std::sin(glm::radians(this->pitch));
    front.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
    this->cameraFront = glm::normalize(front);

    /// Update right and up vectors of the camera
    this->cameraRight = glm::normalize(glm::cross(this->cameraFront, worldUp));
    this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));
}


// Methods


glm::mat4 Camera::getViewMatrix()
{
    //                 point fixed by camera,     center of the camera,                camera up vector
    return glm::lookAt(this->cameraPosition, this->cameraPosition + this->cameraFront, this->cameraUp);
}


void Camera::processKeyboard(movementType direction, float deltaTime)
{
    float velocity = this->cameraSpeed * deltaTime;

    switch(direction)
    {
        case FORWARD :
            this->cameraPosition += this->cameraFront * velocity;
            break;
        case BACKWARD :
            this->cameraPosition -= this->cameraFront * velocity;
            break;
        case LEFT :
            this->cameraPosition -= this->cameraRight * velocity;
            break;
        case RIGHT :
            this->cameraPosition += this->cameraRight * velocity;
            break;
        default:
            break;
    }
}


void Camera::processMouseMovement(float xOffset, float yOffset, bool constraintPitch)
{
    xOffset *= this->mouseSensitiviy;
    yOffset *= this->mouseSensitiviy;

    this->yaw -= xOffset;
    this->pitch -= yOffset;

    // Prevent from screen flipping
    if(constraintPitch)
    {
        if(this->pitch > 89.0f)
        {
            this->pitch = 89.0f;
        }
        if(this->pitch < -89.0f)
        {
            this->pitch = -89.0f;
        }
    }

    this->updateCameraVector();
}


void Camera::processMouseTranslation(movementType direction, float deltaTime)
{
    float velocity = (this->cameraSpeed * deltaTime) / 60;

    switch(direction)
    {
        case DOWN :
            this->cameraPosition += this->cameraUp * velocity;
            break;
        case UP :
            this->cameraPosition -= this->cameraUp * velocity;
            break;
        case LEFT :
            this->cameraPosition -= this->cameraRight * velocity;
            break;
        case RIGHT :
            this->cameraPosition += this->cameraRight * velocity;
            break;
        default:
            break;
    }
}


void Camera::processMouseScroll(float yOffset)
{
    if((this->zoom >= 1.0f) && (this->zoom <= 45.0f) )
    {
        this->zoom -= yOffset;
    }
    if(this->zoom <= 1.0f)
    {
        this->zoom = 1.0f;
    }
    if(this->zoom >= 45.0f)
    {
        this->zoom = 45.0f;
    }
}


