#version 450 core

in vec4 fragPosition;
in vec3 normal;  
in vec4 color;
   
out vec4 fragOutput;

// Material properties:
uniform vec3 matEmission;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

// Light properties:
uniform vec3 lightPosition; 
uniform vec3 lightAmbient; 
uniform vec3 lightDiffuse; 
uniform vec3 lightSpecular;

void main(void)
{      
    // Final color:
    fragOutput = color;
    //fragOutput = vec4(nDotL, nDotL,nDotL, 1.0f);
}