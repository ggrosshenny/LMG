//#version 330 core
#version 130

// INPUT

// UNIFORM
uniform vec3 meshColor;
  // Diffuse textures
uniform vec3 texture_diffuse1;
uniform vec3 texture_diffuse2;
  // Specular textures
uniform vec3 texture_specular1;

// OUTPUT
out vec4 fragmentColor;

// MAIN
void main( void )
{
    vec3 tempColor = texture_diffuse1;
    fragmentColor = vec4( meshColor, 1.0 );
}
            
