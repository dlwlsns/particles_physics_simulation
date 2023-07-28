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

// ID of the ball is equal to the shader instance's ID
int i = int(gl_WorkGroupID.x);

float border = 0.5;

float mass = force[i].w;

void applyForce(vec3 newForce)
{
    force[i].xyz += newForce;
}

void applyGravity()
{
    applyForce(vec3(0.0, -9.81, 0.0) * force[i].w);
}

vec3 normals[] = {
    vec3(0.0, 1.0, 0.0),
    vec3(1.0, 0.0, 0.0),
    vec3(-1.0, 0.0, 0.0),
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.0, -1.0)
};

float bounding[] = {
    matrices_old[i].w,
    matrices_old[i].w - border,
    matrices_old[i].w + border,
    matrices_old[i].w - border,
    matrices_old[i].w + border
};

void planeBounce(vec3 normal)
{
    // calculate vector reflection
    velocity[i].xyz -= (2 * (dot(velocity[i].xyz, normal) * normal));

    // calculate energy loss and new velocity length
    float l = velocity[i].w;
    float k = 0.5 * mass * l * l;
    k *= 0.9;

    velocity[i].w = sqrt((k * 2) / mass);

    applyForce(vec3(0.0, 9.81, 0.0) * force[i].w);
}

void checkPlaneCollision()
{
    // calculate temp position
    vec3 a = force[i].xyz / mass;
    vec3 v = (velocity[i].xyz * velocity[i].w) + (a * deltaFrameTime);
    vec3 pos = matrices_old[i].xyz + (v * deltaFrameTime);

    // check sphere-plane collisions
    if (pos.y <= bounding[0])
    {
        planeBounce(normals[0]);
    }

    if (pos.x <= bounding[1])
    {
        planeBounce(normals[1]);
    }

    if (pos.x >= bounding[2])
    {
        planeBounce(normals[2]);
    }

    if (pos.z <= bounding[3])
    {
        planeBounce(normals[3]);
    }

    if (pos.z >= bounding[4])
    {
        planeBounce(normals[4]);
    }
}

void sphereBounce(int k, int j){
    // get various values
    vec3 v1 = velocity[k].xyz * velocity[k].w;
    vec3 v2 = velocity[j].xyz * velocity[j].w;
    vec3 x1 = matrices_old[k].xyz;
    vec3 x2 = matrices_old[j].xyz;
    float m1 = force[k].w;
    float m2 = force[j].w;

    // calculate energy
    vec3 n = normalize(x2 - x1);
    float m_eff = 1 / ((1 / m1) + (1 / m2));
    float v_imp = dot(n, (v1 - v2));
    float energy = (1.0 + 0.9) * m_eff * v_imp;

    // calculate new velocities
    vec3 v1f = v1 - ((energy / m1) * n);
    vec3 v2f = v2 + ((energy / m2) * n);

    // update velocities
    velocity[k].xyz = normalize(v1f);
    velocity[k].w = length(v1f);

    velocity[j].xyz = normalize(v2f);
    velocity[j].w = length(v2f);
}

void checkSphereCollision()
{
    for(int k = 0; k < 200; k++)
    {
        vec3 a = force[k].xyz / mass;
        vec3 v = (velocity[k].xyz * velocity[k].w) + (a * deltaFrameTime);
        vec3 pos = matrices_old[k].xyz + (v * deltaFrameTime);

        for (int b = 0; b < 200; b++)
        {
            if (b != k)
            {
                float m1 = force[b].w;
                vec3 a1 = force[b].xyz / m1;
                vec3 v1 = (velocity[b].xyz * velocity[b].w) + (a1 * deltaFrameTime);
                vec3 pos1 = matrices_old[b].xyz + (v1 * deltaFrameTime);

                if (length(pos1 - pos) <= (matrices_old[k].w + matrices_old[b].w))
                {
                    sphereBounce(k, b);
                }
            }
        }
    }
    
}

void main(void)
{
    if (deltaFrameTime > 0.0){
        applyGravity();

        if (i == 0)
        {
            checkSphereCollision();
        }

        checkPlaneCollision();

        // updating velocity and position
        vec3 a = force[i].xyz / mass;
        vec3 v = (velocity[i].xyz * velocity[i].w) + (a * deltaFrameTime);

        velocity[i].xyz = normalize(v);
        velocity[i].w = length(v);

        matrices[i].xyz = matrices_old[i].xyz + (velocity[i].xyz * velocity[i].w * deltaFrameTime);

        force[i].xyz *= 0.0;
    }
}