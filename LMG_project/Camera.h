#ifndef __CAMERA_H
#define __CAMERA_H

// STL
#include <cstdio>
#include <iostream>
#include <cmath>

// Graphics
// - GLEW (always before "gl.h")
#include <GL/glew.h>
// - GL
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
// - GLUT
#include <GL/glut.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines

/// YAW default
#define YAW -90.0f
/// PITCH default
#define PITCH 0.0f
/// SPEED default
#define SPEED 0.15f
/// SENSITIVITY default
#define SENSITIVITY 0.2f
/// ZOOM default
#define ZOOM 45.0f


enum movementType{ FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN};


class Camera
{
// Attributes
public:
    // Camera attributes

    /// Position of the camera (i.e center)
    glm::vec3 cameraPosition;
    /// Target of the camera (i.e cameraEYE)
    glm::vec3 cameraFront;
    /// up vector of the camera
    glm::vec3 cameraUp;
    /// right vector of the camera
    glm::vec3 cameraRight;
    /// up vector of the world
    glm::vec3 worldUp;

    // Eular angles

    /// yaw - y rotation
    float yaw;
    /// pitch - x rotation
    float pitch;

    // used for camera movement

    /// Speed of the camera movement
    float cameraSpeed;
    /// Mouse movement sensitivity
    float mouseSensitiviy;
    /// Zoom value of the camera
    float zoom;


// Constructors
public:

    /**
     * @brief Camera with glm::vec3 data
     * @param position vector of the camera position
     * @param up vector of the world up direction
     */
    Camera(glm::vec3 position, glm::vec3 up);

    /**
     * @brief Camera Constructor of the camera class with glm::vec3 data and given yaw and pitch
     * @param position vector of the camera position
     * @param up vector of the world up direction
     * @param yaw default yaw value
     * @param pitch defautl pitch value
     */
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);


// Auxiliary methods
private:


    /**
     * @brief updateCameraVector Compute the cameraFront vector with eular angles
     */
    void updateCameraVector();


// methods
public:


    /**
     * @brief getViewMatrix return the glm::lookAt matrix of the camera
     * @return
     */
    glm::mat4 getViewMatrix();


    /**
     * @brief processKeyboard change camera position when receiving keyboard inputs
     * @param direction direction in which the camera has to move
     * @param deltaTime used to make the movement camera independent from the calculation capacity of the computer
     */
    void processKeyboard(movementType direction, float deltaTime);


    /**
     * @brief processMouseMovement change camera eular angle when receiving a passive mouse input (mouse movements)
     * @param xOffset
     * @param yOffset
     * @param constraintPitch boolean to know if we have to apply a constraint on pitch or not
     */
    void processMouseMovement(float xOffset, float yOffset, bool constraintPitch);


    /**
     * @brief processMouseTranslation change camera position with mouse movement
     * @param xOffset
     * @param yOffset
     */
    void processMouseTranslation(movementType direction, float deltaTime);


    /**
     * @brief processMouseScroll change the zoom of the camera
     * @param yOffset zoom value
     */
    void processMouseScroll(float yOffset);

};


#endif
