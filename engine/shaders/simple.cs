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

layout(std430, binding = 6) buffer ssboForce
{
    vec4 force[];
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

void planeBounce(int i, vec3 normal){
    float mass = force[i].w;

    // calculate direction of velocity vector
    velocity[i].xyz -= (2 * (dot(velocity[i].xyz, normal) * normal));

    // calculate energy loss and new velocity length
    float l = velocity[i].w;
    float k = 0.5 * mass * l * l;
    k *= 0.9;

    velocity[i].w = sqrt((k * 2) / mass);
}



void main(void)
{
    // ID of the ball is equal to the shader instance's ID
    int i = int(gl_WorkGroupID.x);

    if (deltaFrameTime > 0.0){
        if (i == 0)
        {
            for (int j = 0; j < 500; j++)
            {
                for (int b = 0; b < 500; b++)
                {
                    if (b != j)
                    {
                        if (length(matrices_old[b].xyz - matrices_old[j].xyz) <= (matrices_old[j].w + matrices_old[b].w))
                        {
                            //https://physics.stackexchange.com/questions/681396/elastic-collision-3d-eqaution

                            vec3 v1 = velocity[j].xyz * velocity[j].w;
                            vec3 v2 = velocity[b].xyz * velocity[b].w;
                            vec3 x1 = matrices_old[j].xyz;
                            vec3 x2 = matrices_old[b].xyz;
                            float m1 = force[j].w;
                            float m2 = force[b].w;

                            v1 -= ((2 * m2) / (m1 + m2)) * dot((v1 - v2), normalize(x1 - x2)) * (x1 - x2);
                            v2 -= ((2 * m1) / (m1 + m2)) * dot((v2 - v1), normalize(x2 - x1)) * (x2 - x1);

                            velocity[j].xyz = normalize(v1);
                            velocity[j].w = length(v1);
                            velocity[b].xyz = normalize(v2);
                            velocity[b].w = length(v2);
                        }
                    }
                }
            }
        }

        if (velocity[i].w > 0.0)
        {
            float mass = force[i].w;

            float bounding[] = {
                matrices_old[i].w,
                matrices_old[i].w - border,
                matrices_old[i].w + border,
                matrices_old[i].w - border,
                matrices_old[i].w + border
            };

            

            // after need to sum forces
            force[i].xyz = mass * gravity;
            vec3 a = force[i].xyz / mass;

            // updating velocity
            vec3 v = velocity[i].xyz * velocity[i].w;
            v += a * deltaFrameTime;

            velocity[i].xyz = normalize(v);
            velocity[i].w = length(v);

            // updating temp position
            vec3 pos = matrices_old[i].xyz + (velocity[i].xyz * velocity[i].w * deltaFrameTime);

            bool check = false;
            // check if ball collide with plane
            //TODO: if bouncing move ball to position where hit wall
            if (pos.y <= bounding[0])
            {
                planeBounce(i, normals[0]);
                check = true;
            }

            if (pos.x <= bounding[1])
            {
                planeBounce(i, normals[1]);
                check = true;
            }

            if (pos.x >= bounding[2])
            {
                planeBounce(i, normals[2]);
                check = true;
            }

            if (pos.z <= bounding[3])
            {
                planeBounce(i, normals[3]);
                check = true;
            }

            if (pos.z >= bounding[4])
            {
                planeBounce(i, normals[4]);
                check = true;
            }

            //truncate if speed is less than 1 mm/s
            if (velocity[i].w <= 0.0001) velocity[i].w = 0.0;

            if (check)
                matrices[i].xyz = pos.xyz + (velocity[i].xyz * velocity[i].w * deltaFrameTime);
            else
                matrices[i].xyz = pos.xyz;
        }
    }
}