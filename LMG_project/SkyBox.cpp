#include "SkyBox.h"

// Constructor


SkyBox::SkyBox(std::vector<std::__cxx11::string> faces)
{

    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    for(unsigned int i=0; i<(sizeof(skyboxVertices) / sizeof(float)); i++)
    {
        this->vertices.push_back(skyboxVertices[i]);
    }


    this->loadCubeMap(faces);

    this->setupSkyBox();
}


// Auxiliary methods

void SkyBox::loadCubeMap(std::vector<std::string> faces)
{
    int width = 0;
    int height = 0;
    unsigned char* data = NULL;

    // Initialize the texture of the skybox
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

    // Load and bind textures
    for(unsigned int i = 0; i < faces.size(); i++)
    {
        // Load texture from file
        data = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        //std::cerr << "SOIL_LOADING error : " << SOIL_last_result() << std::endl;

        // Bind texture to cube map
        if(data != NULL)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cerr << "[WARNING] in SkyBox, could not load texture at path : " << faces[i].c_str() << std::endl;
        }

        SOIL_free_image_data(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}


void SkyBox::setupSkyBox()
{

    // Declare VAO, VBO
    glGenVertexArrays(1, &this->VAO);
    glCheckError();
    glGenBuffers(1, &this->VBO);
    glCheckError();

    // Memory allocation for VBO and EBO linked with the VAO
    glBindVertexArray(this->VAO);
    glCheckError();

    // Link the VBO with the vertices vector
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glCheckError();

    // Tell how to read position of each vertex in the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();

    glBindVertexArray(0);
    glCheckError();
}



// Methods

void SkyBox::draw(Shader &shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 cubeTransformationMatrix)
{
    // Change depth test function
    glDepthFunc(GL_LEQUAL);

    // Use shader and set uniform
    shader.use();
    shader.setMat4("viewMatrix", viewMatrix);
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setMat4("cubeModelMatrix", cubeTransformationMatrix);

    // bind texture for shader usage
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

    // draw skybox
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    // Set depth test function back to default
    glDepthFunc(GL_LESS);
}
















