#ifndef __BILLBOARD_H
#define __BILLBOARD_H


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


class BillBoard
{
// Attributes
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    // Texture
    GLuint textureID;
    int textureHeight;
    int textureWidth;

    // BillBoard data
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    // Debug
    bool messageAlreadySpread;

// Constructor
public:

    BillBoard() {}

    /**
     * @brief BillBoard default constructor. Load texture, create a plane and draw it with the given texture
     * @param texturePath
     */
    BillBoard(std::string texturePath);


    /**
     * @brief BillBoard constructor used for billboards cloud. Create the billboard with the given positions and the given texture id
     * @param textureID texture id in OpenGL
     * @param topLeftVertex
     * @param topRightVertex
     * @param botRightVertex
     * @param botLeftVertex
     */
    BillBoard(GLuint textureID, glm::vec3 topLeftVertex, glm::vec3 topRightVertex, glm::vec3 botRightVertex, glm::vec3 botLeftVertex);



// Auxiliary methods
private:

    /**
     * @brief loadTexture load texture and bind it to OpenGL programm
     * @param texturePath
     */
    void loadTexture(std::string texturePath);


    /**
     * @brief generateVertexCoordinates generate the 4 vertex positions of the billboard
     */
    void generateVerticesPosition();


    /**
     * @brief setUpBillBoard generate vertex array and buffers for the billboard
     */
    void setUpBillBoard();


// Methods
public:


    /**
     * @brief draw draw the billboard
     */
    void draw(Shader shader, std::string UniformaNameInShader);
};


#endif
