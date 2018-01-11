#include "HeightMap.h"


// Constructor


HeightMap::HeightMap(std::string heightMapPath, int precision)
{
    this->defaultColor = glm::vec3(defRed, defGreen, defBlue);

    // Init vertices normal
    for(unsigned int i=0; i<this->vertices.size(); i++)
    {
        this->vertices[i].normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    this->loadHeightMap(heightMapPath);
    this->setupMap(precision);
    this->colorTextureHasBeenSet = false;
}

HeightMap::HeightMap(std::string heightMapPath, std::string texturePath, int precision)
{
    this->defaultColor = glm::vec3(defRed, defGreen, defBlue);

    // Init vertices normal
    for(unsigned int i=0; i<this->vertices.size(); i++)
    {
        this->vertices[i].normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    this->colorTextureHasBeenSet = false;
    this->loadHeightMap(heightMapPath);
    this->colorTextureSetUp(texturePath);
    this->setupMap(precision);
}




// Auxiliary methods


float HeightMap::getHeightValue(int x, int y)
{
    return static_cast<float>(this->heightMapTexture[y*this->hMapHeight + x]);
}


void HeightMap::loadHeightMap(std::string texturePath)
{
    this->heightMapTexture = SOIL_load_image(texturePath.c_str(), &this->hMapWidth, &this->hMapHeight, 0, SOIL_LOAD_L);
}


void HeightMap::verticesNormalGeneration()
{
    int p1;
    int p2;
    int p3;
    glm::vec3 vectorP1P2;
    glm::vec3 vectorP1P3;
    glm::vec3 faceNormal;

    // For each vertex of the map
    for(int y=0; y<this->hMapHeight-1; y++)
    {
        for(int x=0; x<this->hMapWidth-1; x++)
        {
            // First triangle of the square //

            // Compute points index in std::vector
            p1 = y*this->hMapWidth + x;
            p2 = y*this->hMapWidth + x+1;
            p3 = (y+1)*this->hMapWidth + x+1;

            // Compute faceNormal of the first triangle
            vectorP1P2 = this->vertices[p2].position - this->vertices[p1].position;
            vectorP1P3 = this->vertices[p3].position - this->vertices[p1].position;
            faceNormal = glm::cross(vectorP1P2, vectorP1P3);

            // Compute vertex normal for each triangle vertex
            /*P1*/ this->vertices[p1].normal = glm::normalize(faceNormal + this->vertices[p1].normal);
            /*P2*/ this->vertices[p2].normal = glm::normalize(faceNormal + this->vertices[p2].normal);
            /*P3*/ this->vertices[p3].normal = glm::normalize(faceNormal + this->vertices[p3].normal);

            // Second triangle of the square //

            // Compute points index in std::vector
            p1 = y*this->hMapWidth + x;
            p2 = (y+1)*this->hMapWidth + x+1;
            p3 = (y+1)*this->hMapWidth + x;

            // Compute faceNormal of the first triangle
            vectorP1P2 = this->vertices[p2].position - this->vertices[p1].position;
            vectorP1P3 = this->vertices[p3].position - this->vertices[p1].position;
            faceNormal = glm::cross(vectorP1P2, vectorP1P3);

            // Compute vertex normal for each triangle vertex
            /*P1*/ this->vertices[p1].normal = glm::normalize(faceNormal + this->vertices[p1].normal);
            /*P2*/ this->vertices[p2].normal = glm::normalize(faceNormal + this->vertices[p2].normal);
            /*P3*/ this->vertices[p3].normal = glm::normalize(faceNormal + this->vertices[p3].normal);
        }
    }
}


void HeightMap::setupMap(int precision)
{
    hmapVertex vertex;
    GLuint vertexTopLeftPosition = 0;
    GLuint vertexTopRightPosition = 0;
    GLuint vertexBotRightPosition = 0;
    GLuint vertexBotLeftPosition = 0;

    // Create vertices data
    for(int x=0; x<(this->hMapHeight - precision); x+=precision)
    {
        for(int z=0; z<(this->hMapWidth - precision); z+=precision)
        {
            // Create each vertex of the map
            vertex.position = glm::vec3(x, this->getHeightValue(x, z), z);
            vertex.textCoords = glm::vec2(x%this->colorTextWidth, z%this->colorTextHeight );
            this->vertices.push_back(vertex);
        }
    }

    this->hMapHeight = this->hMapHeight/precision;
    this->hMapWidth = this->hMapWidth/precision;

    // Set up the indices of each triangles of the map
    for(int y=0; y<this->hMapHeight-1; y++)
    {
        for(int x=0; x<this->hMapWidth-1; x++)
        {
            vertexTopLeftPosition = y*this->hMapWidth + x;
            vertexTopRightPosition = y*this->hMapWidth + (x + 1);
            vertexBotRightPosition = (y+1)*this->hMapWidth + (x + 1);
            vertexBotLeftPosition = (y+1)*this->hMapWidth + x;

            // First triangle of the quad
            this->indices.push_back(vertexBotRightPosition);
            this->indices.push_back(vertexTopRightPosition);
            this->indices.push_back(vertexTopLeftPosition);

            // Second triangle of the quad
            this->indices.push_back(vertexTopLeftPosition);
            this->indices.push_back(vertexBotLeftPosition);
            this->indices.push_back(vertexBotRightPosition);

        }
    }

    this->verticesNormalGeneration();

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
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(hmapVertex), this->vertices.data(), GL_STATIC_DRAW);
    glCheckError();

    // Link the EBO with the indices vector
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);
    glCheckError();

    // Tell how to read position of each vertex in the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(hmapVertex), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    // Tell how to read position of each vertex in the VBO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(hmapVertex), (void*)offsetof(hmapVertex, normal));
    glCheckError();
    glEnableVertexAttribArray(1);
    glCheckError();
    // Tell how to read texture coordinates of each vertex in the VBO
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(hmapVertex), (void*)offsetof(hmapVertex, textCoords));
    glCheckError();
    glEnableVertexAttribArray(2);
    glCheckError();


    glBindVertexArray(0);
    glCheckError();
}


void HeightMap::colorTextureSetUp(std::string texturePath)
{
    // Load texture
    this->colorTexture = SOIL_load_image(texturePath.c_str(), &this->colorTextWidth, &this->colorTextHeight, 0, SOIL_LOAD_RGB);
    //std::cerr << "SOIL_LOADING error : " << SOIL_last_result() << std::endl;

    // Generate and bind texture
    if(this->colorTexture != NULL)
    {
        // Generate the texture in OpenGL
        glGenTextures(1, &this->colorTextureID);

        // Bind the texture in OpenGL
        glBindTexture(GL_TEXTURE_2D, this->colorTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->colorTextWidth, this->colorTextHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, this->colorTexture);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        this->colorTextureHasBeenSet = true;
    }
    else
    {
        std::cerr << "[WARNING] in HeightMap, could not load texture at path : " << texturePath.c_str() << std::endl;

    }

    SOIL_free_image_data(this->colorTexture);
}


void HeightMap::draw(Shader& shader, std::string UniformaNameInShader, hmapDrawType drawType)
{
    GLint uniformColorTextureID;

    if(drawType == texture)
    {
        // Verify if a texture was loaded
        if(!this->colorTextureHasBeenSet)
        {
            std::cerr << "[WARNING] : color texture is not set in HeightMap. Cannot apply texture on the map !" << std::endl;
        }
        else
        {
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
                glBindTexture(GL_TEXTURE_2D, this->colorTextureID);
                glCheckError();
            }
            else if(!this->messageAlreadySpread) // If not
            {
                std::cerr << "[WARNING]: the uniform " << UniformaNameInShader << " was not declared in the shader. It will not be used." << std::endl;
                this->messageAlreadySpread = true;
            }
        }
    }
    else if(drawType == color)
    {
        // Retrieve the uniform
        uniformColorTextureID = glGetUniformLocation(shader._shaderId, UniformaNameInShader.c_str());
        glCheckError();
        // If the uniform eists in the shader
        if(uniformColorTextureID != -1)
        {
            shader.setVec3(UniformaNameInShader.c_str(), this->defaultColor);
        }
        else if(!this->messageAlreadySpread) // If not
        {
            std::cerr << "[WARNING]: the uniform " << UniformaNameInShader << " was not declared in the shader. It will not be used." << std::endl;
            this->messageAlreadySpread = true;
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

