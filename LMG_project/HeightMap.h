#ifndef __HEIGHTMAP_H
#define __HEIGHTMAP_H


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

// SOIL
#include <SOIL/SOIL.h>

// Shader
#include "Shader.h"


struct hmapVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textCoords;
};

enum hmapDrawType{texture, color};

#define defRed 0.2f
#define defGreen 0.5f
#define defBlue 0.2f


class HeightMap
{
// Attributes
private:
    // Data
    std::vector<hmapVertex>vertices;
    std::vector<GLuint> indices;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    // Texture and height map
    unsigned char* heightMapTexture;
    unsigned char* colorTexture;
    GLuint colorTextureID;
    bool colorTextureHasBeenSet;

    // Height map data
    int hMapWidth = 0;
    int hMapHeight = 0;
    int colorTextWidth = 0;
    int colorTextHeight = 0;


    // For draw usage
    glm::vec3 defaultColor;
    bool messageAlreadySpread = false;

public:
    // Transformation matrix
    glm::mat4 transformationMatrix;


// Constructor
public:


    /**
     * @brief HeightMap default constructor
     */
    HeightMap(){}


    /**
     * @brief HeightMap Constructor with the given texture path
     * @param texturePath file path
     */
    HeightMap(std::string heightMapPath, int precision);


    /**
     * @brief HeightMap Constructor with the given height map and the color texture
     * @param heightMapPath
     * @param texturePath
     * @param precision
     */
    HeightMap(std::string heightMapPath, std::string texturePath, int precision);


// Auxiliary methods
private:


    /**
     * @brief getHeightValue return the height value at the given indices
     * @param x
     * @param y
     * @return
     */
    float getHeightValue(int x, int y);


    /**
     * @brief loadTexture load texture from file and generate an height map texture and a color texture to generate the scene map
     * @param texturePath path to the texture file
     */
    void loadHeightMap(std::string texturePath);


    /**
     * @brief colorTextureSetUp
     * @param texturePath
     */
    void colorTextureSetUp(std::string texturePath);


    /**
     * @brief verticesNormalGeneration Compute the normal of each vertex and store it in vertices vector
     */
    void verticesNormalGeneration();


    /**
     * @brief setupMap set up the vertices data with the height map
     */
    void setupMap(int precision);


// Methods
public:


    /**
     * @brief draw draw the map with the current shader
     * @param shader
     */
    void draw(Shader& shader, std::string UniformaNameInShader, enum hmapDrawType drawType);


};


#endif
































