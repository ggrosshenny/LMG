#version 130

// INPUT
in vec3 NormalInWorldSpace;
in vec3 FragPos;

// UNIFORM
  // skybox texture
uniform samplerCube skybox;
  // Type of refraction
uniform float refractionRatio;
  // Position of the camera
uniform vec3 viewPos;

// OUTPUT
out vec4 fragmentColor;

void main(void)
{
  float alpha = 1.0f;
  // Get envMap texture coordinate by reflexion
    vec3 viewDirectionVector = normalize(FragPos - viewPos);
    vec3 refractionVector = refract(viewDirectionVector, normalize(NormalInWorldSpace), refractionRatio);
    
     fragmentColor = vec4(texture(skybox, refractionVector).rgb, alpha);
}
