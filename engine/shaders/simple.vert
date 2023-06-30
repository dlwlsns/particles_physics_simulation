#version 450 core

// Uniforms:
uniform mat4 invCamera;
uniform mat4 projection;
uniform float deltaFrameTime;
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

vec3 gravity = vec3(0.0, -9.81, 0.0);
    
float border = 5.0;
float bounding[] = {matrices[gl_InstanceID].w,
                    matrices[gl_InstanceID].w - border,
                    matrices[gl_InstanceID].w + border,
                    matrices[gl_InstanceID].w - border,
                    matrices[gl_InstanceID].w + border};

vec3 normals[] = {vec3(0.0, 1.0, 0.0),
                    vec3(1.0, 0.0, 0.0),
                    vec3(-1.0, 0.0, 0.0),
                    vec3(0.0, 0.0, 1.0),
                    vec3(0.0, 0.0, -1.0)};

void main(void)
{
    float mass = velocity[gl_InstanceID].w;

    if(deltaFrameTime > 0.0){
        // check if ball collide with plane
        if(matrices[gl_InstanceID].y <= bounding[0]){
            vec3 v0 = vec3(velocity[gl_InstanceID].x, velocity[gl_InstanceID].y, velocity[gl_InstanceID].z);
                
            // calculate direction of velocity vector
            vec3 n = vec3(0.0, 1.0, 0.0);
            vec3 v1 = v0 - 2 * (dot(v0, n) * n);
            v1 = normalize(v1);

            // calculate new velocity (lenght)
            float k = 0.5 * mass * length(v0) * length(v0);
            k *= 0.8;
            v1 = v1 * sqrt(k / mass / 0.5);

            // update velocity
            velocity[gl_InstanceID].x = v1.x;
            velocity[gl_InstanceID].y = v1.y;
            velocity[gl_InstanceID].z = v1.z;
        }

        if(matrices[gl_InstanceID].x <= bounding[1]){
            vec3 v0 = vec3(velocity[gl_InstanceID].x, velocity[gl_InstanceID].y, velocity[gl_InstanceID].z);
                
            // calculate direction of velocity vector
            vec3 n = vec3(1.0, 0.0, 0.0);
            vec3 v1 = v0 - 2 * (dot(v0, n) * n);
            v1 = normalize(v1);

            // calculate new velocity (lenght)
            float k = 0.5 * mass * length(v0) * length(v0);
            k *= 0.8;
            v1 = v1 * sqrt(k / mass / 0.5);

            // update velocity
            velocity[gl_InstanceID].x = v1.x;
            velocity[gl_InstanceID].y = v1.y;
            velocity[gl_InstanceID].z = v1.z;
        }

        if(matrices[gl_InstanceID].x >= bounding[2]){
            vec3 v0 = vec3(velocity[gl_InstanceID].x, velocity[gl_InstanceID].y, velocity[gl_InstanceID].z);
                
            // calculate direction of velocity vector
            vec3 n = vec3(-1.0, 0.0, 0.0);
            vec3 v1 = v0 - 2 * (dot(v0, n) * n);
            v1 = normalize(v1);

            // calculate new velocity (lenght)
            float k = 0.5 * mass * length(v0) * length(v0);
            k *= 0.8;
            v1 = v1 * sqrt(k / mass / 0.5);

            // update velocity
            velocity[gl_InstanceID].x = v1.x;
            velocity[gl_InstanceID].y = v1.y;
            velocity[gl_InstanceID].z = v1.z;
        }

        if(matrices[gl_InstanceID].z <= bounding[3]){
            vec3 v0 = vec3(velocity[gl_InstanceID].x, velocity[gl_InstanceID].y, velocity[gl_InstanceID].z);
                
            // calculate direction of velocity vector
            vec3 n = vec3(0.0, 0.0, 1.0);
            vec3 v1 = v0 - 2 * (dot(v0, n) * n);
            v1 = normalize(v1);

            // calculate new velocity (lenght)
            float k = 0.5 * mass * length(v0) * length(v0);
            k *= 0.8;
            v1 = v1 * sqrt(k / mass / 0.5);

            // update velocity
            velocity[gl_InstanceID].x = v1.x;
            velocity[gl_InstanceID].y = v1.y;
            velocity[gl_InstanceID].z = v1.z;
        }

        if(matrices[gl_InstanceID].z >= bounding[4]){
            vec3 v0 = vec3(velocity[gl_InstanceID].x, velocity[gl_InstanceID].y, velocity[gl_InstanceID].z);
                
            // calculate direction of velocity vector
            vec3 n = vec3(0.0, 0.0, -1.0);
            vec3 v1 = v0 - 2 * (dot(v0, n) * n);
            v1 = normalize(v1);

            // calculate new velocity (lenght)
            float k = 0.5 * mass * length(v0) * length(v0);
            k *= 0.8;
            v1 = v1 * sqrt(k / mass / 0.5);

            // update velocity
            velocity[gl_InstanceID].x = v1.x;
            velocity[gl_InstanceID].y = v1.y;
            velocity[gl_InstanceID].z = v1.z;
        }

        // after need to sum forces
        vec3 f = mass * gravity;
        vec3 a = f / mass;

        vec3 v = vec3(0.0);
        v += a * deltaFrameTime;

        velocity[gl_InstanceID].x += v.x;
        velocity[gl_InstanceID].y += v.y;
        velocity[gl_InstanceID].z += v.z;

        // update position
        matrices[gl_InstanceID].x += (velocity[gl_InstanceID].x * deltaFrameTime);
        matrices[gl_InstanceID].y += (velocity[gl_InstanceID].y * deltaFrameTime);
        matrices[gl_InstanceID].z += (velocity[gl_InstanceID].z * deltaFrameTime);
    }

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