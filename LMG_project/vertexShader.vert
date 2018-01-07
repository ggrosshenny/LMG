//#version 330 core
#version 130

// INPUT
in vec3 position;
in vec3 normal;
in vec2 textCoords;

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
  // - animation
//uniform float time;


// OUTPUT
out vec2 textureCoordinates;
out vec3 FragPos;
out vec3 NormalInWorldSpace;

// MAIN
void main( void )
{
    // Send position to Clip-space
    gl_Position = projectionMatrix * viewMatrix * sceneMatrix * modelMatrix * vec4( position, 1.0 );
    
    // Compute normal position in world space
    NormalInWorldSpace = normalMatrix * normal;
    
    // Compute fragment position in world space
    FragPos = vec3(modelMatrix * vec4(position, 1.0));

    // Send texture coordinates to the fragment shader
    textureCoordinates = textCoords;
}
