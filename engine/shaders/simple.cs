#version 450 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// Uniforms:
uniform float deltaFrameTime;

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

vec3 normals[] = {vec3(0.0, 1.0, 0.0),
                    vec3(1.0, 0.0, 0.0),
                    vec3(-1.0, 0.0, 0.0),
                    vec3(0.0, 0.0, 1.0),
                    vec3(0.0, 0.0, -1.0)};

vec3 planeBounce(vec3 velocity, vec3 normal, float mass){
    vec3 v0 = vec3(velocity.x, velocity.y, velocity.z);
                
    // calculate direction of velocity vector
    vec3 v1 = v0 - 2 * (dot(v0, normal) * normal);
    v1 = normalize(v1);

    // calculate new velocity (lenght)
    float k = 0.5 * mass * length(v0) * length(v0);
    k *= 0.8;
    v1 = v1 * sqrt(k / mass / 0.5);

    // update velocity
    return v1;
}

void main(void)
{
    

    if(deltaFrameTime > 0.0){
        for(int i = 0; i < 1000; i++){
            float mass = velocity[i].w;

            float bounding[] = {matrices[i].w,
                    matrices[i].w - border,
                    matrices[i].w + border,
                    matrices[i].w - border,
                    matrices[i].w + border};

            // check if ball collide with plane
            if (matrices[i].y <= bounding[0]){
                velocity[i].xyz = planeBounce(vec3(velocity[i].xyz), normals[0], mass).xyz;
            }

            if(matrices[i].x <= bounding[1]){
                velocity[i].xyz = planeBounce(vec3(velocity[i].xyz), normals[1], mass).xyz;
            }

            if(matrices[i].x >= bounding[2]){
                velocity[i].xyz = planeBounce(vec3(velocity[i].xyz), normals[2], mass).xyz;
            }

            if(matrices[i].z <= bounding[3]){
                velocity[i].xyz = planeBounce(vec3(velocity[i].xyz), normals[3], mass).xyz;
            }

            if(matrices[i].z >= bounding[4]){
                velocity[i].xyz = planeBounce(vec3(velocity[i].xyz), normals[4], mass).xyz;
            }

            // after need to sum forces
            vec3 f = mass * gravity;
            vec3 a = f / mass;

            vec3 v = vec3(0.0);
            v += a * deltaFrameTime;

            velocity[i].x += v.x;
            velocity[i].y += v.y;
            velocity[i].z += v.z;

            // update position
            matrices[i].x += (velocity[i].x * deltaFrameTime);
            matrices[i].y += (velocity[i].y * deltaFrameTime);
            matrices[i].z += (velocity[i].z * deltaFrameTime);
        }
    }

}