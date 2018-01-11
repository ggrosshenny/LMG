#ifndef __BILLBOARDCLOUD_H
#define __BILLBOARDCLOUD_H

// Includes

#include "ErrorHandling.h"

// STL
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// System
#include <cstdio>
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

// Shader
#include "Shader.h"

// Billboard
#include "BillBoard.h"

class BillBoardCloud
{
// Attributes
private:
    std::vector<BillBoard> billBoards;


// Constructor
public:


    BillBoardCloud() {}


    /**
     * @brief BillBoardCloud Default constructor
     * @param texturesPath
     */
    BillBoardCloud(std::vector<std::string> texturesPath);


// Auxiliary methods
private:


    /**
     * @brief loadTexture Load texture and bind it to OpenGL programm
     * @param texturePath
     * @param height
     * @param width
     * @return
     */
    GLuint loadTexture(std::string texturePath, int* height, int* width);


// Methods
public:


    /**
     * @brief draw Draw the billboard cloud
     * @param shader
     * @param uniformNameInShader
     */
    void draw(Shader shader, std::string uniformNameInShader);
};


#endif
