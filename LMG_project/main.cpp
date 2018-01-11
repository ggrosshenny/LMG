
/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/


#include "Shader.h"
#include "Model3D.h"
#include "Camera.h"
#include "SkyBox.h"
#include "HeightMap.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

// Screen properties
int SCR_WIDTH = 1300;
int SCR_HEIGHT = 1024;


// Mouse position catching
bool rightMouseButtonDown = false;
bool leftMouseButtonDown = false;
bool middleMouseButtonDown = false;
int lastMousePositionX = 0.0f;
int lastMousePositionY = 0.0f;


// Path to sources
std::string pathToSrc = "../LMG_project/";
std::string pathToShader = pathToSrc + "Shaders/";
std::string pathToMaps = pathToSrc + "HeightMap/";
std::string pathToTextures = pathToSrc + "Textures/";


// Shader programs
Shader shaderProgram;
Shader glassShader;
Shader skyboxShader;
Shader mapShader;


// Camera object
Camera camera(glm::vec3( 0.f, 2.f, 4.f ), glm::vec3( 0.f, 1.f, 0.f ));

// Map object
HeightMap map;

// SkyBox
    // - faces
std::vector<std::string>faces = {
    pathToSrc + "SkyBoxes/SkyBox1/right.jpg",
    pathToSrc + "SkyBoxes/SkyBox1/left.jpg",
    pathToSrc + "SkyBoxes/SkyBox1/top.jpg",
    pathToSrc + "SkyBoxes/SkyBox1/bottom.jpg",
    pathToSrc + "SkyBoxes/SkyBox1/back.jpg",
    pathToSrc + "SkyBoxes/SkyBox1/front.jpg"
};
SkyBox skybox;
bool isSkyboxActive = false;

// Models
std::vector<Model3D> modelsWithProgrammShader;
std::vector<Model3D> modelsWithGlassShader;
Model3D* currentModel;
int modelIdx = -1;
bool isModelWithProgramShader = true;


// Mesh parameters
glm::vec3 _meshColor;

// Lighting parameters
glm::mat3 normalMatrix;
glm::vec3 lightPosition;
glm::vec3 lightColor;
glm::vec3 kd;

// Animation parameters
float deltaTime = 0.0f;
float currentFrameTime = 0.0f;
float lastFrameTime = 0.0f;

// Camera dependant matrices & vectors
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::vec3 ray_origin;
glm::vec3 ray_direction;

// World 3D parameters matrix
glm::mat4 SceneTransformationMatrix;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
***************************** METHOD DEFINITION ******************************
******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

void display();
void idle( void );
void mousePassiveEventWithRedisplay(int x, int y);
bool initialize();
bool checkExtensions();
bool finalize();
void mousePressedEvent(int button, int state, int x, int y);
void mousePassiveEvent(int mousePositionX, int mousePositionY);
void keyPressedEvent(unsigned char key, int x, int y);


/******************************************************************************
 * Initialize all
 ******************************************************************************/
bool initialize()
{
    std::cout << "Initialize all..." << std::endl;

    bool statusOK = true;

    if ( statusOK )
    {
        statusOK = checkExtensions();
    }

    return statusOK;
}


/******************************************************************************
 * Finalize all
 ******************************************************************************/
bool finalize()
{
    bool statusOK = true;

    std::cout << "Finalize all..." << std::endl;

    return statusOK;
}

/******************************************************************************
 * Check GL extensions
 ******************************************************************************/
bool checkExtensions()
{
    bool statusOK = true;

    std::cout << "Check GL extensions..." << std::endl;

    return statusOK;
}


/******************************************************************************
 * Retrieve mouse position in world on click
 ******************************************************************************/


void currentModelUpdate(){

    if(modelIdx == -1){
        currentModel = &modelsWithProgrammShader[0];
        modelIdx = 0;
        isModelWithProgramShader = true;
    }
    else{
        if(isModelWithProgramShader){
            if(modelIdx == modelsWithProgrammShader.size()-1){
                currentModel = &modelsWithGlassShader[0];
                modelIdx = 0;
                isModelWithProgramShader = false;
            }
            else{
                modelIdx++;
                currentModel = &modelsWithProgrammShader[modelIdx];
            }
        }
        else{
            if(modelIdx == modelsWithGlassShader.size()-1){
                currentModel = &modelsWithProgrammShader[0];
                modelIdx = 0;
                isModelWithProgramShader = true;
            }
            else{
                modelIdx++;
                currentModel = &modelsWithGlassShader[modelIdx];
            }
        }
    }
    std::cout << " PROGRAMM : " << isModelWithProgramShader << " ID " << modelIdx << "@DD " << currentModel << std::endl;
}


/******************************************************************************
 * Callback to retrieve user's inputs
 ******************************************************************************/

void mousePressedEvent(int button, int state, int x, int y)
{
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
                // save the mouse button state and the mouse position in lastX, lastY
                if(state == GLUT_DOWN)
                {
                    if(!rightMouseButtonDown && !middleMouseButtonDown)
                    {
                        leftMouseButtonDown = true;
                        lastMousePositionX = x;
                        lastMousePositionY = y;
/*
                        currentModelUpdate();

                        if(currentModel != NULL){
                            std::cout << "model found " << std::endl;
                        }

                        currentModel = NULL;*/
                    }

                }
                else if(state == GLUT_UP)
                {
                    leftMouseButtonDown = false;
                }
                break;

            // Right mouse button
            case GLUT_RIGHT_BUTTON:
                // save the mouse button state and the mouse position in lastX, lastY
                if(state == GLUT_DOWN)
                {
                    if(!leftMouseButtonDown && !middleMouseButtonDown)
                    {
                        rightMouseButtonDown = true;
                        lastMousePositionX = x;
                        lastMousePositionY = y;
                    }

                }
                else if(state == GLUT_UP)
                {
                    rightMouseButtonDown = false;
                }
                break;

            // Scroll up
            case GLUT_MIDDLE_BUTTON:
                // middleMouseButtonDown
                if(state == GLUT_DOWN)
                {
                    if(!leftMouseButtonDown && !rightMouseButtonDown)
                    {
                        middleMouseButtonDown = true;
                        lastMousePositionX = x;
                        lastMousePositionY = y;
                    }

                }
                else if(state == GLUT_UP)
                {
                    middleMouseButtonDown = false;
                }
                break;


            default:
                break;
        }

}

void mousePassiveEvent(int mousePositionX, int mousePositionY)
{
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if(rightMouseButtonDown)
    {
        xOffset = lastMousePositionX - mousePositionX;
        yOffset = mousePositionY - lastMousePositionY;
        lastMousePositionX = mousePositionX;
        lastMousePositionY = mousePositionY;

        camera.processMouseMovement(xOffset, yOffset, true);
    }
    else if(leftMouseButtonDown)
    {
        // Move the current object (translation)
    }
    else if(middleMouseButtonDown)
    {
        xOffset = lastMousePositionX - mousePositionX;
        yOffset = mousePositionY - lastMousePositionY;
        lastMousePositionX = mousePositionX;
        lastMousePositionY = mousePositionY;

        // move to the right
        if(xOffset > 0)
        {
            camera.processMouseTranslation(RIGHT, deltaTime);
        }
        // Move to the left
        else if(xOffset < 0)
        {
            camera.processMouseTranslation(LEFT, deltaTime);
        }
        // Move down
        if(yOffset > 0)
        {
            camera.processMouseTranslation(DOWN, deltaTime);
        }
        // Move up
        else if(yOffset < 0)
        {
            camera.processMouseTranslation(UP, deltaTime);
        }
    }

}

void keyPressedEvent(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'z' :
            camera.processKeyboard(FORWARD, deltaTime);
            break;
        case 's' :
            camera.processKeyboard(BACKWARD, deltaTime);
            break;
        case 'q' :
            camera.processKeyboard(LEFT, deltaTime);
            break;
        case 'd' :
            camera.processKeyboard(RIGHT, deltaTime);
            break;
    }

    glutPostRedisplay();
}

void specialKeyPressedEvent(int key, int x, int y){

    switch(key)
    {
        case GLUT_KEY_PAGE_UP :
            currentModelUpdate();
        break;
        default : break;
    }
}


/******************************************************************************
 * Callback to display the scene
 ******************************************************************************/
void display( void )
{
    glEnable(GL_DEPTH_TEST);
    // Timer info
    currentFrameTime = static_cast<float>(glutGet( GLUT_ELAPSED_TIME ));
    // Get delta time
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    //--------------------
    // START frame
    //--------------------
    // Clear the color buffer (of the main framebuffer)
    // - color used to clear
    glClearColor( 0.f, 0.f, 0.f, 0.f );
    glClearDepth( 1.f );
    // - clear the "color" framebuffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    //--------------------
    // Send uniforms to GPU
    //--------------------



    // Retrieve camera parameters
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = glm::perspective( glm::radians(45.0f), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f );

//    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
//    modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));

    // Create the local transformation matrix for first model
    glm::mat4 modelMatrix;
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, -1.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-8.0f, 1.0f, 0.0f));
    modelsWithProgrammShader[0].setLocalTransformationMatrix(modelMatrix);

    // Create the local transformation matrix for second model
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, 1.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(8.0f, 1.0f, 0.0f));
    modelsWithGlassShader[0].setLocalTransformationMatrix(modelMatrix);

    // Init global variable used to pick models on click
    currentModel = NULL;

//    const bool useMeshAnimation = true; // TODO: use keyboard to activate/deactivate
//    if ( useMeshAnimation )
//    {
//        modelMatrix = glm::rotate( modelMatrix, static_cast< float >( currentFrameTime ) * 0.001f, glm::vec3( 0.0f, 1.f, 0.f ) );
//    }


    // Lighting
    // - normalMatrix
    normalMatrix = glm::transpose(glm::inverse(viewMatrix*modelMatrix));
    // - Material
    kd = glm::vec3(1.0, 0.0, 0.0);
    // - lightColor
    lightColor = glm::vec3(1.0, 1.0, 1.0);
    // - lightPosition
    lightPosition = glm::vec3(1.0, 350.0, 0.0);
    // Mesh color
    _meshColor = glm::vec3( 0.f, 1.f, 0.f );


    //--------------------
    // Activate map shader program
    //--------------------
    mapShader.use();
    // Camera
    // - view matrix
    mapShader.setMat4("viewMatrix", viewMatrix);
    // - projection matrix
    mapShader.setMat4("projectionMatrix", projectionMatrix);
    // - scene transformation matrix
    mapShader.setMat4("sceneMatrix", SceneTransformationMatrix);
    // - cameraPosition
    mapShader.setVec3("viewPos", camera.cameraPosition);

    // Lighting
    // - normalMatrix
    mapShader.setMat3("normalMatrix", normalMatrix);
    // - lightPosition
    mapShader.setVec3("lightPosition", lightPosition);
    // - lightColor
    mapShader.setVec3("lightColor", lightColor);

    // Scale map
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 0.1f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-100.0f, -180.0f, -100.0f));

    mapShader.setMat4("mapModelMatrix", modelMatrix);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    map.draw(mapShader, "texture_diffuse", texture);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );



    //--------------------
    // Activate shader program
    //--------------------
    shaderProgram.use();
    // Camera
    // - view matrix
    shaderProgram.setMat4("viewMatrix", viewMatrix);
    // - projection matrix
    shaderProgram.setMat4("projectionMatrix", projectionMatrix);
    // - scene transformation matrix
    shaderProgram.setMat4("sceneMatrix", SceneTransformationMatrix);
    // - cameraPosition
    shaderProgram.setVec3("viewPos", camera.cameraPosition);

    // Mesh
    // Animation
    shaderProgram.setFloat("time", static_cast< float >( currentFrameTime ) );

    // Lighting
    // - normalMatrix
    shaderProgram.setMat3("normalMatrix", normalMatrix);
    // - Material
    shaderProgram.setVec3("kd", kd);
    // - lightPosition
    shaderProgram.setVec3("lightPosition", lightPosition);
    // - lightColor
    shaderProgram.setVec3("lightColor", lightColor);

    //--------------------
    // Render scene
    //--------------------
    // Set GL state(s) (fixed pipeline)
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Call draw method on each model (with programmShader as shader)
    if(!isSkyboxActive)
    {
        for(unsigned int i=0; i<modelsWithProgrammShader.size(); i++)
        {
            // - model matrix
            shaderProgram.setMat4("modelMatrix",  modelsWithProgrammShader[i].getLocalTransformationMatrix());
            modelsWithProgrammShader[i].draw(shaderProgram);
        }
    }
    else
    {
        for(unsigned int i=0; i<modelsWithProgrammShader.size(); i++)
        {
            // - model matrix
            shaderProgram.setMat4("modelMatrix",  modelsWithProgrammShader[i].getLocalTransformationMatrix());
            modelsWithProgrammShader[i].draw(shaderProgram, skybox.textureID);
        }
    }


    //--------------------
    // Activate glass shader program
    //--------------------
    glassShader.use();
    // Camera
    // - view matrix
    glassShader.setMat4("viewMatrix", viewMatrix);
    // - projection matrix
    glassShader.setMat4("projectionMatrix", projectionMatrix);
    // - scene transformation matrix
    glassShader.setMat4("sceneMatrix", SceneTransformationMatrix);
    // - cameraPosition
    glassShader.setVec3("viewPos", camera.cameraPosition);

    // Lighting
    // - normalMatrix
    glassShader.setMat3("normalMatrix", normalMatrix);
    // - refraction ratio
    glassShader.setFloat("refractionRatio", (1.0f/2.42f));

    //--------------------
    // Render scene
    //--------------------
    // Set GL state(s) (fixed pipeline)
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Call draw method on each model (with programmShader as shader)
    if(!isSkyboxActive)
    {
        for(unsigned int i=0; i<modelsWithGlassShader.size(); i++)
        {
            // - model matrix
            shaderProgram.setMat4("modelMatrix",  modelsWithGlassShader[i].getLocalTransformationMatrix());
            modelsWithGlassShader[i].draw(glassShader);
        }
    }
    else
    {
        for(unsigned int i=0; i<modelsWithGlassShader.size(); i++)
        {
            // - model matrix
            shaderProgram.setMat4("modelMatrix",  modelsWithGlassShader[i].getLocalTransformationMatrix());
            modelsWithGlassShader[i].draw(glassShader, skybox.textureID);
        }
    }



    if(isSkyboxActive)
    {
        // Scale skybox
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1000.0f, 1000.0f, 1000.0f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

        // draw skybox
        skybox.draw(skyboxShader, viewMatrix, projectionMatrix, modelMatrix);
    }

    // Reset GL state(s) (fixed pipeline)
    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Deactivate current shader program
    glUseProgram( 0 );


    //--------------------
    // END frame
    //--------------------
    // OpenGL commands are not synchrone, but asynchrone (stored in a "command buffer")
    glFlush();
    // Swap buffers for "double buffering" display mode (=> swap "back" and "front" framebuffers)
    glutSwapBuffers();
}

/******************************************************************************
 * Callback continuously called when events are not being received
 ******************************************************************************/
void idle( void )
{
    // Mark current window as needing to be redisplayed
    glutPostRedisplay();
}

/******************************************************************************
 * Main function
 ******************************************************************************/
int main( int argc, char** argv )
{
    std::cout << "LMG Project" << std::endl;

    // Initialize the GLUT library
    glutInit( &argc, argv );

    //glutInitContextVersion( 3, 3 );
    //glutInitContextProfile( GLUT_COMPATIBILITY_PROFILE );

    // Grahics window
    // - configure the main framebuffer to store rgba colors,
    //   and activate double buffering (for fluid/smooth visualization)
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE |GLUT_DEPTH);
    // - window size and position
    glutInitWindowSize( SCR_WIDTH, SCR_HEIGHT );
    glutInitWindowPosition( 50, 50 );
    // - create the window
    glutCreateWindow( "Projet LMG" );

    // Callbacks
    // - callback called when displaying window (user custom fonction pointer: "void f( void )")
    glutDisplayFunc( display );
    // - callback continuously called when events are not being received
    glutIdleFunc( idle );
    // - get mouse movements
    glutMotionFunc(mousePassiveEvent);
    // - get mouse inputs
    glutMouseFunc(mousePressedEvent);
    // - get special keyboard input
    glutSpecialFunc(specialKeyPressedEvent);
    // - get keyboard inputs
    glutKeyboardFunc(keyPressedEvent);



    // Initialize the GLEW library
    // - mandatory to be able to use OpenGL extensions,
    //   because OpenGL core API is made of OpenGL 1 and other functions are null pointers (=> segmentation fault !)
    //   Currently, official OpenGL version is 4.5 (or 4.6)
    GLenum error = glewInit();
    if ( error != GLEW_OK )
    {
        fprintf( stderr, "Error: %s\n", glewGetErrorString( error ) );
        exit( -1 );
    }

    // Initialize all your resources (graphics, data, etc...)
    checkExtensions();

    // Build shader
    shaderProgram = Shader(pathToShader+"vertexShader.vert", pathToShader+"fragmentShader.frag");
    glassShader = Shader(pathToShader+"glassShader.vert", pathToShader+"glassShader.frag");
    skyboxShader = Shader(pathToShader+"skyboxShader.vert", pathToShader+"skyboxShader.frag");
    mapShader = Shader(pathToShader+"mapShader.vert", pathToShader+"mapShader.frag");

    // Create skybox object
    skybox = SkyBox(faces);
    isSkyboxActive = true;

    // Create map object
    map = HeightMap(pathToMaps + "Heightmap2.png", pathToTextures + "terrain_01.jpg", 5);

    // Load objects
            // "Models/Crate/Crate1.obj"
            // "Models/Falcon/millenium-falcon.obj"
            // "Models/NanoSuit/nanosuit.obj" -> Works !
            // "Models/StarWars/test_obj/Arc170.obj"
    modelsWithProgrammShader.push_back(Model3D((pathToSrc + "Models/NanoSuit/nanosuit.obj")));
    modelsWithGlassShader.push_back(Model3D((pathToSrc + "Models/NanoSuit/nanosuit.obj")));
    currentModel = NULL;

    // Init view & projection matrices
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = glm::perspective( glm::radians(45.0f), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f );
    // Retrieve 3D model / scene parameters
    SceneTransformationMatrix = glm::scale(SceneTransformationMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    SceneTransformationMatrix = glm::rotate(SceneTransformationMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    SceneTransformationMatrix = glm::translate(SceneTransformationMatrix, glm::vec3(0.0f, -12.5f, 10.0f));

    // Enter the GLUT main event loop (waiting for events: keyboard, mouse, refresh screen, etc...)
    glutMainLoop();

    // Clean all
    //finalize();
}
