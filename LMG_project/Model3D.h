#ifndef __MODEL3D_H
#define __MODEL3D_H

#include "Mesh.h"
// Standard library
#include <map>
// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
// SOIL
#include <SOIL/SOIL.h>


struct classComp
{
    bool operator() (const std::string& path1, const std::string& path2) const
    { return path1<path2; }
};


class Model3D
{
// Attributes
private:
    /// List of meshes
    std::vector<Mesh> meshes;
    std::string directory;
    std::map<std::string, Texture, classComp> loadedTextures;


// Constructor
public:


    /**
     * @brief Model3D Constructor of the Model3D object
     * @param path path of the 3D model
     */
    Model3D(std::string path);


// Methods
public:


    /**
     * @brief draw draw all meshes of the 3D model with the given shader object
     * @param shader
     */
    void draw(Shader& shader);


    /**
     * @brief textureFromFile load and link to OpenGL a texture from a file
     * @param path file path
     * @param directory file directory
     * @return
     */
    static unsigned int textureFromFile(const std::string path, const std::string &directory);


// Auxiliary methods
private:


    /**
     * @brief loadModel call the assimp loader and create meshes from the loaded data
     * @param path path of the 3D model
     */
    void loadModel(std::string path);


    /**
     * @brief processNode extract each meshes of each nodes of the assimp scene from teh root node to the last one
     * @param node current node of the scene
     * @param scene assimp scene
     */
    void processNode(aiNode* node, const aiScene* scene);


    /**
     * @brief processMesh extract the mesh datas and create the mesh object
     * @param mesh assimp mesh
     * @param scene assimp scene
     * @return
     */
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);


    /**
     * @brief loadMaterialTextures load the textures of each mesh if the texture has not already been loaded
     * @param mat assimp material
     * @param type assimp texture type
     * @param typeName name of the type based on naming convention
     * @return
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};
























#endif
