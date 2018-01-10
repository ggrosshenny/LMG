#version 130

// INPUT
in vec2 textureCoordinates;
in vec3 NormalInWorldSpace;
in vec3 FragPos;

// Uniform
//uniform vec3 mapColor;
uniform sampler2D texture_diffuse;
  // light color
uniform vec3 lightColor;
  // light position
uniform vec3 lightPosition;
  // Position of the camera
uniform vec3 viewPos;

// Output
out vec4 fragmentColor;

void main( void )
{
  vec3 result;
  vec3 lightDir;
  
  // Get texture color
  vec4 tempColor = texture(texture_diffuse, textureCoordinates);
  
  // Ambient light
  float ambientStrenght = 0.45;
  vec3 ambient = ambientStrenght * lightColor;
  
  // Diffuse material
  vec3 normalizedNormal = normalize(NormalInWorldSpace);
  lightDir = normalize(lightPosition - FragPos);
  float diffuseCoeff = max(dot(normalizedNormal, lightDir), 0.0);
  vec3 diffuse = diffuseCoeff * lightColor;

  result = (ambient + diffuse) * vec3(tempColor);

  //fragmentColor = vec4(tempColor.r, tempColor.g, tempColor.b, 1.0);
  fragmentColor = vec4(result.r, result.g, result.b, 1.0);
  
  //fragmentColor = vec4(mapColor.r, mapColor.g, mapColor.b, 1.0);
}
