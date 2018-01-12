#include "BillBoardCloud.h"


// Constructor


BillBoardCloud::BillBoardCloud(std::vector<std::string> texturesPath)
{
    float x = 0;
    float z = 0;
    int textureHeight = 0;
    int textureWidth = 0;
    int textureID = 0;
    int billBoardsNumber = texturesPath.size();
    float radius = 0;

    glm::vec3 topLeftVertex;
    glm::vec3 topRightVertex;
    glm::vec3 botRightVertex;
    glm::vec3 botLeftVertex;

    BillBoard bBoard;

    for(unsigned int i=0; i<texturesPath.size(); i++)
    {
        textureID = this->loadTexture(texturesPath[i], &textureHeight, &textureWidth);

        radius = textureWidth/2;

        // Top x
        x = radius * std::cos( ( (2*i*M_PI) / billBoardsNumber) / (M_PI/2) );
        // Top z
        z = radius * std::sin( ( (2*i*M_PI) / billBoardsNumber) / (M_PI/2) );
        // topLeftVertex
        topLeftVertex = glm::vec3(x, textureHeight, z);
        // botLeftVertex
        botLeftVertex = glm::vec3(x, 0, z);

        // Bot x
        x = radius * std::cos(M_PI + ( ( (2*i*M_PI) / billBoardsNumber) / (M_PI/2) ) );
        // Top z
        z = radius * std::sin(M_PI + ( ( (2*i*M_PI) / billBoardsNumber) / (M_PI/2) ) );
        // topRightVertex
        topRightVertex = glm::vec3(x, textureHeight, z);
        // botRightVertex
        botRightVertex = glm::vec3(x, 0, z);

        bBoard = BillBoard(textureID, textureWidth, textureHeight, topLeftVertex, topRightVertex, botRightVertex, botLeftVertex);

        billBoards.push_back(bBoard);
    }
}


GLuint BillBoardCloud::loadTexture(std::string texturePath, int* height, int* width)
{
    // Texture's data
    unsigned char* textureData;
    GLuint textureID = 0;


    // Generate the texture in OpenGL
    glGenTextures(1, &textureID);

    // Retrieve texture data
    textureData = SOIL_load_image(texturePath.c_str(), width, height, 0, SOIL_LOAD_RGBA);
    //std::cerr << "SOIL_LOADING error : " << SOIL_last_result() << std::endl;

    if(textureData != NULL)
    {
        // Bind the texture in OpenGL
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

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

    return textureID;
}


void BillBoardCloud::draw(Shader shader, std::string uniformNameInShader)
{
    for(unsigned int i=0; i<this->billBoards.size(); i++)
    {
        billBoards[i].draw(shader, uniformNameInShader);
    }
}
