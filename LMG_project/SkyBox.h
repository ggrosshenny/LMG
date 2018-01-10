#ifndef __SKYBOX_H
#define __SKYBOX_H

// Includes

#include "ErrorHandling.h"

// STL
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// System
#include <cstdio>

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

// Shader
#include "Shader.h"

class SkyBox
{
// Attributes
private:
    GLuint VAO;
    GLuint VBO;
    std::vector<float> vertices;

public:
    GLuint textureID;

// Constructors
public:

    SkyBox(){}

    SkyBox(std::vector<std::string> faces);


// Auxiliary methods
private:


    /**
     * @brief loadCubeMap load all textures needed to draw the skybox
     * @param faces list of textures composing the skybox
     * @return
     */
    void loadCubeMap(std::vector<std::string> faces);


    /**
     * @brief setupSkyBox setup all buffers needed to draw skybox
     */
    void setupSkyBox();


// methods
public:


    /**
     * @brief draw draw the skybox
     * @param shader used shader
     */
    void draw(Shader& shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 cubeTransformationMatrix);

};


#endif
