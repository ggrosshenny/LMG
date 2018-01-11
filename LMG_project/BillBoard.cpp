#include "BillBoard.h"


// Constructors


BillBoard::BillBoard(std::string texturePath)
{
    // Generate texture in OpenGL
    this->loadTexture(texturePath);

    // Generate billboard's vertices positon
    this->generateVerticesPosition();

    // Set up billboard
    this->setUpBillBoard();

    this->messageAlreadySpread = false;
}


BillBoard::BillBoard(GLuint textureID, glm::vec3 topLeftVertex, glm::vec3 topRightVertex, glm::vec3 botRightVertex, glm::vec3 botLeftVertex)
{
    // Store texture id in OpenGL
    this->textureID = textureID;

    // Store vertices position
    this->vertices.push_back(topLeftVertex);
    this->vertices.push_back(topRightVertex);
    this->vertices.push_back(botRightVertex);
    this->vertices.push_back(botLeftVertex);

    // Set up billboard
    this->setUpBillBoard();

    this->messageAlreadySpread = false;
}

void BillBoard::loadTexture(std::string texturePath)
{
    // Texture's data
    unsigned char* textureData;


    // Generate the texture in OpenGL
    glGenTextures(1, &this->textureID);

    // Retrieve texture data
    textureData = SOIL_load_image(texturePath.c_str(), &this->textureWidth, &this->textureHeight, 0, SOIL_LOAD_RGBA);
    //std::cerr << "SOIL_LOADING error : " << SOIL_last_result() << std::endl;

    if(textureData != NULL)
    {
        // Bind the texture in OpenGL
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textureWidth, this->textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glCheckError();
        glGenerateMipmap(GL_TEXTURE_2D);
        glCheckError();

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cerr << "[WARNING] in BillBoard, could not load texture at path : " << texturePath.c_str() << std::endl;

    }

    SOIL_free_image_data(textureData);
}


void BillBoard::generateVerticesPosition()
{
    glm::vec3 vertexPosition;

    // Top left vertex
    vertexPosition = glm::vec3(0, this->textureHeight, 0);
    this->vertices.push_back(vertexPosition);

    // Top right vertex
    vertexPosition = glm::vec3(this->textureWidth, this->textureHeight, 0);
    this->vertices.push_back(vertexPosition);

    // Bot right vertex
    vertexPosition = glm::vec3(this->textureWidth, 0, 0);
    this->vertices.push_back(vertexPosition);

    // Bot left vertex
    vertexPosition = glm::vec3(0, 0, 0);
    this->vertices.push_back(vertexPosition);

    for(unsigned int i=0; i<this->vertices.size(); i++)
    {
        std::cout << "(" << this->vertices[i].x << ", " << this->vertices[i].y << ", " << this->vertices[i].z << ")" << std::endl;
    }
}



 void BillBoard::setUpBillBoard()
 {

     // First triangle of the quad
     this->indices.push_back(2); // Top right
     this->indices.push_back(0); // Bot right
     this->indices.push_back(1); // Top Left

     // Second triangle of the quad
     this->indices.push_back(3); // Bot right
     this->indices.push_back(0); // Bot left
     this->indices.push_back(2); // Top left

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
     glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->vertices.data(), GL_STATIC_DRAW);
     glCheckError();

     // Link the EBO with the indices vector
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
     glCheckError();
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);
     glCheckError();

     // Tell how to read position of each vertex in the VBO
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
     glCheckError();
     glEnableVertexAttribArray(0);
     glCheckError();


     glBindVertexArray(0);
     glCheckError();
 }


void BillBoard::draw(Shader shader, std::string UniformaNameInShader)
{
    GLint uniformColorTextureID;

     // Add the map texture
    glActiveTexture(GL_TEXTURE0);

    // Retrieve the uniform
    uniformColorTextureID = glGetUniformLocation(shader._shaderId, UniformaNameInShader.c_str());
    glCheckError();
    // If the uniform eists in the shader
    if(uniformColorTextureID != -1)
    {
        // Bind the texture
        glUniform1i(uniformColorTextureID, 0);
        glCheckError();
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glCheckError();
    }
    else if(!this->messageAlreadySpread) // If not
    {
        std::cerr << "[WARNING]: the uniform " << UniformaNameInShader << " was not declared in the shader. It will not be used." << std::endl;
        this->messageAlreadySpread = true;
    }

    shader.setInt("imgWidth", this->textureWidth);
    glCheckError();
    shader.setInt("imgHeight", this->textureHeight);
    glCheckError();

    // draw mesh
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(this->VAO);
    glCheckError();
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
    glCheckError();
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}









