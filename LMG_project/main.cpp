
/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/


#include "Shader.h"
#include "Model3D.h"
#include "Camera.h"
#include "SkyBox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

// Screen properties
int SCR_WIDTH = 800;
int SCR_HEIGHT = 648;

// Mouse position catching
bool firstTimeMousePositionCaught = true;
int lastMousePositionX = 0.0f;
int lastMousePositionY = 0.0f;



// Path to sources
std::string pathToSrc = "../LMG_project/";

// Shader programs
Shader shaderProgram;
Shader skyboxShader;

// Camera object
Camera camera(glm::vec3( 0.f, 2.f, 4.f ), glm::vec3( 0.f, 1.f, 0.f ));

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

// Models
std::vector<Model3D> models;

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

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
***************************** METHOD DEFINITION ******************************
******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

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
 * Callback to retrieve user's inputs
 ******************************************************************************/


void mousePressedEvent(int button, int state, int x, int y)
{

    switch(button)
    {
        // Scroll up
        case 3:
            camera.processMouseScroll(y);
            break;

        case 4:
            camera.processMouseScroll(y);
            break;

        default:
            break;
    }
}

void mousePassiveEvent(int mousePositionX, int mousePositionY)
{
    float xOffset = 0.0f;
    float yOffset = 0.0f;

    if(firstTimeMousePositionCaught)
    {
        lastMousePositionX = mousePositionX;
        lastMousePositionY = mousePositionY;
        firstTimeMousePositionCaught = false;
    }

    xOffset = lastMousePositionX - mousePositionX;
    yOffset = mousePositionY - lastMousePositionY;
    lastMousePositionX = mousePositionX;
    lastMousePositionY = mousePositionY;

    camera.processMouseMovement(xOffset, yOffset, true);

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
    const glm::mat4 viewMatrix = camera.getViewMatrix();
    const glm::mat4 projectionMatrix = glm::perspective( glm::radians(45.0f), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f );

    // Retrieve 3D model / scene parameters
    glm::mat4 SceneTransformationMatrix;
    SceneTransformationMatrix = glm::scale(SceneTransformationMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    SceneTransformationMatrix = glm::rotate(SceneTransformationMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    SceneTransformationMatrix = glm::translate(SceneTransformationMatrix, glm::vec3(0.0f, -12.5f, 10.0f));


//    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
//    modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));

    // Create the local transformation matrix for first model
    glm::mat4 modelMatrix;
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, -1.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-8.0f, 1.0f, 0.0f));
    models[0].setLocalTransformationMatrix(modelMatrix);

    // Create the local transformation matrix for second model
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, 1.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(8.0f, 1.0f, 0.0f));
    models[1].setLocalTransformationMatrix(modelMatrix);

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
    lightPosition = glm::vec3(1.0, 5.0, 0.0);
    // Mesh color
    _meshColor = glm::vec3( 0.f, 1.f, 0.f );


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

    // Call draw method on each model
    for(unsigned int i=0; i<models.size(); i++)
    {
        // - model matrix
        shaderProgram.setMat4("modelMatrix",  models[i].getLocalTransformationMatrix());
        models[i].draw(shaderProgram);
    }


    // Scale skybox
    modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

    // draw skybox
    skybox.draw(skyboxShader, viewMatrix, projectionMatrix, modelMatrix);

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
    // - get mouse inputs
    glutMouseFunc(mousePressedEvent);
    // - get mouse movements
    glutPassiveMotionFunc(mousePassiveEvent);
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
    shaderProgram = Shader(pathToSrc+"vertexShader.vert", pathToSrc+"fragmentShader.frag");
    skyboxShader = Shader(pathToSrc+"skyboxShader.vert", pathToSrc+"skyboxShader.frag");

    // Create skybox object
    skybox = SkyBox(faces);


    // Load objects
            // "Models/Crate/Crate1.obj"
            // "Models/Falcon/millenium-falcon.obj"
            // "Models/NanoSuit/nanosuit.obj" -> Works !
            // "Models/StarWars/test_obj/Arc170.obj"
    models.push_back(Model3D((pathToSrc + "Models/NanoSuit/nanosuit.obj")));
    models.push_back(Model3D((pathToSrc + "Models/NanoSuit/nanosuit.obj")));



    // Enter the GLUT main event loop (waiting for events: keyboard, mouse, refresh screen, etc...)
    glutMainLoop();

    // Clean all
    //finalize();
}
