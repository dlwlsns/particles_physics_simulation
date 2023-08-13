#version 450 core

// Uniforms:
uniform mat4 invCamera;
uniform mat4 projection;
mat3 normalMatrix;

// Attributes:
layout(location = 15) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

layout (std430, binding = 3) buffer vboTransform
{
    vec4 matrices[];
};

layout(std430, binding = 2) buffer vboColor
{
    vec4 colors[];
};


// Varying:
out vec4 fragPosition;
out vec3 normal;
out vec4 color;

void main(void)
{
    fragPosition = invCamera * mat4(matrices[gl_InstanceID].w, 0.0, 0.0, 0.0,
                                    0.0, matrices[gl_InstanceID].w, 0.0, 0.0,
                                    0.0, 0.0, matrices[gl_InstanceID].w, 0.0,
                                    matrices[gl_InstanceID].x, matrices[gl_InstanceID].y, matrices[gl_InstanceID].z, 1.0) * vec4(in_Position, 1.0);
    normalMatrix = inverse(transpose(mat3(mat4(matrices[gl_InstanceID].w, 0.0, 0.0, 0.0,
                                    0.0, matrices[gl_InstanceID].w, 0.0, 0.0,
                                    0.0, 0.0, matrices[gl_InstanceID].w, 0.0,
                                    matrices[gl_InstanceID].x, matrices[gl_InstanceID].y, matrices[gl_InstanceID].z, 1.0))));
        
    gl_Position = projection * fragPosition;
    normal = normalMatrix * in_Normal;


    color = vec4(normalize(colors[gl_InstanceID].xyz), colors[gl_InstanceID].w);
}