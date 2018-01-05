#include "Mesh.h"


// ========================================
// Construction and initialization methdods

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->setupMesh();
}


void Mesh::setupMesh()
{
    // Declare VAO, VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Memory allocation for VBO and EBO linked with the VAO
    glBindVertexArray(VAO);

    // Link the VBO with the vertices vector
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Link the EBO with the indices vector
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Tell how to read position of each vertex in the VBO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Tell how to read normal of each vertex in the VBO
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // Tell how to read texture coordinates of each vertex in the VBO
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));

    glBindVertexArray(0);
}


// ===========
// Draw method


void Mesh::draw(Shader &shader)
{
    // Each texture in the shader has to be of the form :
    //      - texture_diffuseN
    //      - texture_specularN
    // Where N is the texture number
    // This format let us specify as many textures as we want without any problem
    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    std::string number;
    std::string name;
    GLint textureUniformId;

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        // active the good texture
        glActiveTexture(GL_TEXTURE0 + i);

        name = textures[i].type;
        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }

        // Retrive the uniform
        textureUniformId = glGetUniformLocation(shader._shaderId, (name + number).c_str());
        // If the uniform eists in the shader
        if(textureUniformId != -1)
        {
            // Bind the texture
            glUniform1i(textureUniformId, i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        else // If not
        {
            std::cerr << "[WARNING]: the uniform " << name << i << " was not declared in the shader. It will not be used." << std::endl;
        }
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
