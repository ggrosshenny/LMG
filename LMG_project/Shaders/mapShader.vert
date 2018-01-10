#version 130

// INPUT
in vec3 position;
in vec3 normal;
in vec2 textCoords;

// Uniform
  // - camera
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mapModelMatrix;
uniform mat3 normalMatrix;

// Output
out vec3 FragPos;
out vec2 textureCoordinates;
out vec3 NormalInWorldSpace;


void main( void )
{
  textureCoordinates = textCoords;
    // Compute normal position in world space
  NormalInWorldSpace = normalMatrix * normal;
    // Compute fragment position in world space
  FragPos = vec3(mapModelMatrix * vec4(position, 1.0));
  //textureCoordinates = position.xy;

  gl_Position = projectionMatrix * viewMatrix * mapModelMatrix * vec4( position, 1.0 );
}
