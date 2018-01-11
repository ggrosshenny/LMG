//#version 330 core
#version 130

// INPUT
in vec3 position;

// UNIFORM
  // - camera
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
  // Scene
uniform mat4 sceneMatrix;
  // - 3D model
uniform mat4 modelMatrix;

uniform int imgWidth;
uniform int imgHeight;

// OUTPUT
out vec3 FragPos;
out vec2 textureCoordinates;

// MAIN
void main( void )
{
    // Compute fragment position in world space

    mat4 modelView = viewMatrix;
    // First row
    modelView[0][0] = 1.0;
    modelView[0][1] = 0.0;
    modelView[0][2] = 0.0;
    
    // Third row
    modelView[2][0] = 0.0;
    modelView[2][1] = 0.0;
    modelView[2][2] = 1.0;

    // Send position to Clip-space
    gl_Position = projectionMatrix * modelView * sceneMatrix * modelMatrix * vec4( position, 1.0 );
    
    FragPos = position.xyz;
    textureCoordinates = vec2(((imgWidth - position.x)/imgWidth), ((imgHeight - position.y)/imgHeight));
}
