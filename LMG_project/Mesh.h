#ifndef __MESH_H
#define __MESH_H

// Includes

#include "ErroHandling.h"

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


// structs used to store mesh data


/**
 * @brief The Vertex struct is used to modelise a vertex with all its data
 */
struct Vertex
{
    /// Position of the vertex
    glm::vec3 position;
    /// Normal of the vertex
    glm::vec3 normal;
    /// testure coordinates of the vertex
    glm::vec2 textCoords;
    // Add more stuff ?
};



/**
 * @brief The Texture struct
 */
struct Texture
{
    /// Id in OpenGL of the texture
    GLuint id;
    /// Type of the texture (diffuse or specular)
    std::string type;
};


class Mesh
{
// Attributes
private:
    GLuint VBO;
    GLuint EBO;

public:
    GLuint VAO;

public:
    /// Vector of vertices
    std::vector<Vertex> vertices;
    /// Vector of indice of the points composing the mesh
    std::vector<GLuint> indices;
    /// Vector of textures used by the mesh
    std::vector<Texture> textures;


// Construtors
public:

    /**
     * @brief Mesh Constructor of the mesh object with minimal informations needed for mesh description
     * @param vertices vertices informations
     * @param indices indice of each vertex
     * @param textures textures used by the mesh
     */
    Mesh(std::vector<Vertex> vertices,std::vector<GLuint> indices,  std::vector<Texture> textures);


// Methods
public:


    /**
     * @brief draw draw the current mesh with the given shader by passing informations in the uniform of the shader
     * @param shader shader object containing vertex and fragment shaders
     */
    void draw(Shader& shader);


    /**
     * @brief drawdraw the current mesh with the given shader by passing informations in the uniform of the shader
     * @param shader shader object containing vertex and fragment shaders
     * @param messageAlreadySpread bool to know if we have to spread the error message
     */
    bool draw(Shader& shader, bool messageAlreadySpread);


    /**
     * @brief drawdraw the current mesh with the given shader by passing informations in the uniform of the shader
     * @param shader shader object containing vertex and fragment shaders
     * @param messageAlreadySpread bool to know if we have to spread the error message
     */
    bool draw(Shader& shader, GLuint skyboxTextureID, bool messageAlreadySpread);


// Auxiliary methods
private:

    /**
     * @brief setupMesh initialize the VAO, VBO and EBO buffers of the mesh and fill it with the mesh informations
     */
    void setupMesh();

};



#endif

























