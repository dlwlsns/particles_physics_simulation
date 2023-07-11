#version 450 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// Uniforms:
uniform float deltaFrameTime;


layout(std430, binding = 3) buffer vboTransform
{
    vec4 matrices_old[];
};

layout (std430, binding = 4) buffer ssboTransform
{
    vec4 matrices[];
};

layout (std430, binding = 5) buffer ssboVelocity
{
    vec4 velocity[];
};

vec3 gravity = vec3(0.0, -9.81, 0.0);
    
float border = 5.0;

vec3 normals[] = {
    vec3(0.0, 1.0, 0.0),
    vec3(1.0, 0.0, 0.0),
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.0, -1.0)
};

mat4 calculateReflectionMatrix(vec3 normal)
{
    vec3 n = normalize(normal);

    return mat4(
        1.0 - 2.0 * n.x * n.x, -2.0 * n.x * n.y, -2.0 * n.x * n.z, 0.0,
        -2.0 * n.y * n.x, 1.0 - 2.0 * n.y * n.y, -2.0 * n.y * n.z, 0.0,
        -2.0 * n.z * n.x, -2.0 * n.z * n.y, 1.0 - 2.0 * n.z * n.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

vec3 planeBounce(int i, vec3 normal){
    vec3 v0 = vec3(velocity[i].x, velocity[i].y, velocity[i].z);
    float mass = velocity[i].w;

    // calculate direction of velocity vector
    vec3 v1 = v0 - (2 * (dot(v0, normal) * normal));

    // calculate energy loss
    float l = length(v1);
    float k = 0.5 * mass * l * l;
    k *= 0.8;

    // calculate new velocity (lenght)
    v1 = normalize(v1);
    v1 *= sqrt((k * 2) / mass);

    //truncate if speed is less than 1 mm/s
    if (length(v1) <= 0.001) v1 = vec3(0.0);

    return v1;
}



void main(void)
{
    int i = int(gl_WorkGroupID.x);

    if (deltaFrameTime > 0.0){
        float mass = velocity[i].w;

        float bounding[] = {
            matrices_old[i].w,
            matrices_old[i].w - border,
            matrices_old[i].w + border,
            matrices_old[i].w - border,
            matrices_old[i].w + border
        };

        // after need to sum forces
        vec3 f = mass * gravity;
        vec3 a = f / mass;

        vec3 v = vec3(0.0);
        v += a * deltaFrameTime;

        velocity[i].xyz += v.xyz;
        vec3 pos = matrices_old[i].xyz + (velocity[i].xyz * deltaFrameTime);

        bool check = false;
        // check if ball collide with plane
        //TODO: if bouncing move ball to position where hit wall
        if (pos.y <= bounding[0])
            {
                velocity[i].xyz = planeBounce(i, normals[0]).xyz;
                check = true;
            }

            if (pos.x <= bounding[1])
            {
                velocity[i].xyz = planeBounce(i, normals[1]).xyz;
                check = true;
            }

            if (pos.x >= bounding[2])
            {
                velocity[i].xyz = planeBounce(i, normals[2]).xyz;
                check = true;
            }

            if (pos.z <= bounding[3])
            {
                velocity[i].xyz = planeBounce(i, normals[3]).xyz;
                check = true;
            }

            if (pos.z >= bounding[4])
            {
                velocity[i].xyz = planeBounce(i, normals[4]).xyz;
                check = true;
            }

        if (check) 
            matrices[i].xyz = pos.xyz + (velocity[i].xyz * deltaFrameTime);
        else
            matrices[i].xyz = pos.xyz;
    }
}