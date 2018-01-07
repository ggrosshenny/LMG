#version 130

// INPUT
in vec3 textureCoordinates;

// Uniform
uniform samplerCube skybox;

// Output
out vec4 fragmentColor;

void main( void )
{
  vec4 color = texture(skybox, textureCoordinates);
  fragmentColor = vec4(color.r, color.g, color.b, 1.0);
  //fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}

