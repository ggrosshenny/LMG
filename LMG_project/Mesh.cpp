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
    glGenVertexArrays(1, &this->VAO);
    glCheckError();
    glGenBuffers(1, &this->VBO);
    glCheckError();
    glGenBuffers(1, &this->EBO);
    glCheckError();

    // Memory allocation for VBO and EBO linked with the VAO
    glBindVertexArray(this->VAO);
    glCheckError();

    // Link the VBO with the vertices vector
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);
    glCheckError();

    // Link the EBO with the indices vector
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);
    glCheckError();

    // Tell how to read position of each vertex in the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    // Tell how to read normal of each vertex in the VBO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glCheckError();
    glEnableVertexAttribArray(1);
    glCheckError();
    // Tell how to read texture coordinates of each vertex in the VBO
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));
    glCheckError();
    glEnableVertexAttribArray(2);
    glCheckError();


    glBindVertexArray(0);
    glCheckError();

//    std::cout << "VAO : " << this->VAO << ", VBO : " << this->VBO << ", EBO : " << this->EBO << std::endl;

//    std::cout << "vertex size : " << this->vertices.size() << std::endl;
//    for(unsigned int i=0; i<this->indices.size(); i++)
//    {
//        std::cout << "position : (" << this->vertices[i].position.x << ", " << this->vertices[i].position.y << ", " <<  this->vertices[i].position.z << ")" << std::endl;
//        std::cout << "\tnormal : (" << this->vertices[i].normal.x << ", " << this->vertices[i].normal.y << ", " << this->vertices[i].normal.z << ")" << std::endl;
//        std::cout << "\ttextCoordinates : (" << this->vertices[i].textCoords.x << ", " << this->vertices[i].textCoords.y << ")" << std::endl;

//    }

//    std::cout << "indices size : " << this->indices.size() << std::endl;

//    for(unsigned int i=0; i<this->indices.size(); i++)
//    {
//        std::cout << "indice[" << i << "] = " << this->indices[i] << std::endl;
//    }

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
    std::string currentTextureName;
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
        currentTextureName = name + number;

        // Retrieve the uniform
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
            std::cerr << "[WARNING]: the uniform " << currentTextureName << " was not declared in the shader. It will not be used." << std::endl;
        }
    }

    // draw mesh
    glBindVertexArray(this->VAO);
    glCheckError();
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
    glCheckError();
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}


bool Mesh::draw(Shader& shader, bool messageAlreadySpread)
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
    std::string currentTextureName;
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
        currentTextureName = name + number;

        // Retrieve the uniform
        textureUniformId = glGetUniformLocation(shader._shaderId, (name + number).c_str());
        // If the uniform eists in the shader
        if(textureUniformId != -1)
        {
            // Bind the texture
            glUniform1i(textureUniformId, i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        else if(!messageAlreadySpread) // If not
        {
            std::cerr << "[WARNING]: the uniform " << currentTextureName << " was not declared in the shader. It will not be used." << std::endl;
        }
    }

    // draw mesh
    glBindVertexArray(this->VAO);
    glCheckError();
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
    glCheckError();
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    return true;
}
