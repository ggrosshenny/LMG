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
  // Lightning
uniform mat3 normalMatrix;

uniform int imgWidth;
uniform int imgHeight;

// OUTPUT
out vec2 textureCoordinates;

// MAIN
void main( void )
{
    // Compute fragment position in world space
    vec4 tempPos = projectionMatrix * viewMatrix * sceneMatrix * modelMatrix * vec4( position, 1.0 );
    textureCoordinates = vec2(((position.x)/imgWidth) + (imgWidth/2), ((imgHeight - position.y)/imgHeight)) + imgHeight;


    // Send position to Clip-space
    gl_Position = tempPos;
}
