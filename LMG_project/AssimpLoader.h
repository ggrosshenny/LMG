#ifndef __ASSIMPLOADER_H_
#define __ASSIMPLOADER_H_

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include<assimp/vector3.h>
#include<assimp/postprocess.h>
#include<assimp/DefaultLogger.hpp>
#include<assimp/LogStream.hpp>


/* TODO :
 *  - import an object file and store it into a scene* object
 *  - load_data : parse the given scene* object and store eache arrays in the given verteces
 *      -> class Model3D : store the object scene* and store datas in VAO and VBO to send it to the GPU
 */


#endif
