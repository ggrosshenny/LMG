#include "Model3D.h"


// ===========
// Constructor

Model3D::Model3D(std::string path)
{
    this->WarningMessageForShaderAlreadyShown = false;
    this->localTransformationMatrix = glm::mat4(1.0f);
    this->loadModel(path);
}


// ===================
// Getters and setters

glm::mat4 Model3D::getLocalTransformationMatrix()
{
    return this->localTransformationMatrix;
}

void Model3D::setLocalTransformationMatrix(glm::mat4 matrix)
{
    this->localTransformationMatrix = matrix;
}

void Model3D::setLocalTransformationMatrix(glm::vec3 scaleVector, float rotationAngle, glm::vec3 rotateVector, glm::vec3 translateVector)
{
    this->localTransformationMatrix = glm::mat4(1.0f);
    this->localTransformationMatrix = glm::scale(this->localTransformationMatrix, scaleVector);
    this->localTransformationMatrix = glm::rotate(this->localTransformationMatrix, rotationAngle, rotateVector);
    this->localTransformationMatrix = glm::translate(this->localTransformationMatrix, translateVector);
}


// =======
// Methods


void Model3D::draw(Shader &shader)
{
    for(unsigned int i = 0; i<this->meshes.size(); i++)
    {
        this->WarningMessageForShaderAlreadyShown = this->meshes[i].draw(shader, this->WarningMessageForShaderAlreadyShown);
    }
}

void Model3D::draw(Shader& shader, GLuint skyboxTextureID)
{
    for(unsigned int i = 0; i<this->meshes.size(); i++)
    {
        this->WarningMessageForShaderAlreadyShown = this->meshes[i].draw(shader, skyboxTextureID, this->WarningMessageForShaderAlreadyShown);
    }
}


unsigned int Model3D::textureFromFile(const std::string path, const std::string &directory)
{
    // Get full path of the texture
    std::string fileName = std::string(path);
    fileName = directory + '/' + fileName;
    //std::cout << "textureFromFile : " << fileName << std::endl;
    // Texture ID in OpenGL environment
    GLuint textureID;
    // Texture's data
    int width = 0;
    int height = 0;
    unsigned char* textureData;


    // Generate the texture in OpenGL
    glGenTextures(1, &textureID);

    // Retrieve texture data
    textureData = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    //std::cerr << "SOIL_LOADING error : " << SOIL_last_result() << std::endl;

    if(textureData != NULL)
    {
        // Bind the texture in OpenGL
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cerr << "[WARNING] in Model3D, could not load texture at path : " << fileName.c_str() << std::endl;

    }

    SOIL_free_image_data(textureData);

    return textureID;
}


// =================
// Auxiliary methods

void Model3D::loadModel(std::string path)
{
    Assimp::Importer import;

    // Load the model
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);


    // Check loaded model validity
    if((scene == NULL) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
    {
        std::cerr << "[ERROR] in Model3D : " << import.GetErrorString() << std::endl;
        return;
    }
    this->directory = path.substr(0, path.find_last_of('/'));

    // travel through nodes to create each meshes of the model
    processNode(scene->mRootNode, scene);
}


void Model3D::processNode(aiNode* node, const aiScene* scene)
{
    aiMesh* mesh;

    // Get mesh's data for all meshes of the node
    for(unsigned int i = 0; i<node->mNumMeshes; i++)
    {
        mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }

    // Do the same for all children of the node
    for(unsigned int i = 0; i<node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
}


Mesh Model3D::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    // For data retrieving
        // Vertices
    Vertex currentVertex;
    glm::vec3 currentVertexPosition;
    glm::vec3 currentVertexNormal;
    glm::vec2 currentVertexTextureCoordinate;
        // Indices
    aiFace face;
        // Material
    aiMaterial *material;
    std::vector<Texture> diffuseMaps;
    std::vector<Texture> specularMaps;

    // Retrieve vertices data
    for(unsigned int i=0; i<mesh->mNumVertices; i++)
    {
        // Retrieve vertex position
        currentVertexPosition.x = mesh->mVertices[i].x;
        currentVertexPosition.y = mesh->mVertices[i].y;
        currentVertexPosition.z = mesh->mVertices[i].z;
        currentVertex.position = currentVertexPosition;

        // Retrive vertex normal
        currentVertexNormal.x = mesh->mNormals[i].x;
        currentVertexNormal.y = mesh->mNormals[i].y;
        currentVertexNormal.z = mesh->mNormals[i].z;
        currentVertex.normal = currentVertexNormal;

        // Verify if the vertex has texture coordinates
        if(mesh->mTextureCoords[0] != NULL)
        {
            // Retrive texture coordinate
            currentVertexTextureCoordinate.x = mesh->mTextureCoords[0][i].x;
            currentVertexTextureCoordinate.y = mesh->mTextureCoords[0][i].y;
            currentVertex.textCoords = currentVertexTextureCoordinate;
        }
        else
        {
            currentVertex.textCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(currentVertex);
    }

    // Retrive vertices indice
    for(unsigned int i=0; i<mesh->mNumFaces; i++)
    {
        face = mesh->mFaces[i];
        for(unsigned int j=0; j<face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Retrieve material
    if(mesh->mMaterialIndex >= 0)
    {
        // Retrieve diffuse material
        material = scene->mMaterials[mesh->mMaterialIndex];
        diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Retrieve specular material
        specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model3D::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    // Store textures
    std::vector<Texture> textures;
    // Retrive texture's data
    aiString textureString;
    Texture currentTexture;
    // Used to know if the texture was already loaded
    bool isAlreadyLoaded = false;
    std::map<std::string, Texture, classComp>::const_iterator it;

    for(unsigned int i=0; i<mat->GetTextureCount(type); i++)
    {
        mat->GetTexture(type, i, &textureString);
        isAlreadyLoaded = false;

        // If the texture was alredy loaded
        if((it = this->loadedTextures.find(textureString.C_Str())) != this->loadedTextures.end())
        {

            textures.push_back(it->second);
            isAlreadyLoaded = true;
        }

        // If the texture was not loaded
        if(!isAlreadyLoaded)
        {
            // Load the texture in OpenGL and retrieve its id
            currentTexture.id = this->textureFromFile(textureString.C_Str(), this->directory);
            currentTexture.type = typeName;
            // Add the texture to the mesh's textures
            textures.push_back(currentTexture);
            // Add the texture to the already loaded textures
            this->loadedTextures[textureString.C_Str()] = currentTexture;
        }
    }

    return textures;
}
















