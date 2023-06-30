#version 450 core

// Uniforms:
uniform mat4 invCamera;
uniform mat4 projection;
mat3 normalMatrix;

// Attributes:
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Color;
layout(location = 3) in vec4 in_Transform;


layout (std430, binding = 4) buffer ssboTransform
{
    vec4 matrices[];
};

layout (std430, binding = 5) buffer ssboVelocity
{
    vec4 velocity[];
};

// Varying:
out vec4 fragPosition;
out vec3 normal;
out vec3 color;

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
    color = normalize(in_Color);
}