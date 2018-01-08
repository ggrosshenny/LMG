#version 130

// INPUT
in vec3 position;

// Uniform
  // - camera
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cubeModelMatrix;

// Output
out vec3 textureCoordinates;


void main( void )
{
  textureCoordinates = position;
  vec4 pos = projectionMatrix * viewMatrix * cubeModelMatrix * vec4( position, 1.0 );
  gl_Position = pos.xyww;
}



