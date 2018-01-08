//#version 330 core
#version 130

// INPUT
in vec2 textureCoordinates;
in vec3 NormalInWorldSpace;
in vec3 FragPos;

// UNIFORM
  // Diffuse textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
  // Specular textures
uniform sampler2D texture_specular1;
  // skybox texture
uniform samplerCube skybox;
  // material coefficient
uniform vec3 kd;
  // light color
uniform vec3 lightColor;
  // light position
uniform vec3 lightPosition;
  // Position of the camera
uniform vec3 viewPos;

// OUTPUT
out vec4 fragmentColor;

// MAIN
void main( void )
{
    vec3 result;
    
    // Get envMap texture coordinate by reflexion
    vec3 viewDirectionVector = normalize(FragPos - viewPos);
    vec3 reflectionVector = reflect(viewDirectionVector, normalize(NormalInWorldSpace));
    
    // Get texture color
    float reflectionStrenght = 0.3;
    vec4 reflectedColor = mix(texture(texture_diffuse1, textureCoordinates), texture(skybox, reflectionVector), reflectionStrenght) ;
    vec4 tempColor = mix(texture(texture_diffuse1, textureCoordinates), reflectedColor, 0.5);
    // Ambient light
    float ambientStrenght = 0.45;
    vec3 ambient = ambientStrenght * lightColor;
    
    // Diffuse material
    vec3 normalizedNormal = normalize(NormalInWorldSpace);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diffuseCoeff = max(dot(normalizedNormal, lightDir), 0.0);
    vec3 diffuse = diffuseCoeff * lightColor;
    
    // Specular material
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalizedNormal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    result = (ambient + diffuse + specular) * vec3(tempColor);
    
    fragmentColor = vec4(result.r, result.g, result.b, 1.0);
    //fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}
            
