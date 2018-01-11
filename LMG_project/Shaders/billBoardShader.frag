#version 130

// INPUT
in vec2 textureCoordinates;
in vec3 FragPos;

// Uniform
//uniform vec3 mapColor;
uniform sampler2D texture_diffuse;

// Output
out vec4 fragmentColor;

void main( void )
{
  // Get texture color
  vec4 tempColor = texture(texture_diffuse, textureCoordinates);
  
  fragmentColor = tempColor.rgba;
}
