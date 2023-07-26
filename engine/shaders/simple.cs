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
    //https://en.wikipedia.org/wiki/Elastic_collision

    vec3 v1 = velocity[k].xyz * velocity[k].w;
    vec3 v2 = velocity[j].xyz * velocity[j].w;
    vec3 x1 = matrices_old[k].xyz;
    vec3 x2 = matrices_old[j].xyz;
    float m1 = force[k].w;
    float m2 = force[j].w;

    float m_sum = m1 + m2;
    vec3 x1x2 = x1 - x2;
    vec3 x2x1 = x2 - x1;
    float l_x1x2 = length(x1x2);
    float l_x2x1 = length(x2x1);

    vec3 v1f = v1 - (((2 * m2) / m_sum) * (dot((v1 - v2), x1x2) / (l_x1x2 * l_x1x2)) * x1x2);
    vec3 v2f = v2 - (((2 * m1) / m_sum) * (dot((v2 - v1), x2x1) / (l_x2x1 * l_x2x1)) * x2x1);

    velocity[k].xyz = normalize(v1f);
    velocity[k].w = length(v1f);

    velocity[j].xyz = normalize(v2f);
    velocity[j].w = length(v2f);

    //barrier();
}

void checkSphereCollision()
{
    for(int k = 0; k < 100; k++)
    {
        vec3 a = force[k].xyz / mass;
        vec3 v = (velocity[k].xyz * velocity[k].w) + (a * deltaFrameTime);
        vec3 pos = matrices_old[k].xyz + (v * deltaFrameTime);

        for (int b = 0; b < 100; b++)
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

        checkSphereCollision();

        checkPlaneCollision();

        // updating velocity and position
        vec3 a = force[i].xyz / mass;
        vec3 v = (velocity[i].xyz * velocity[i].w) + (a * deltaFrameTime);

        velocity[i].xyz = normalize(v);
        velocity[i].w = length(v);

        if(velocity[i].w > 0.0)
        {
            // something wierd if force is (0, 0, 0) this breaks
            matrices[i].xyz = matrices_old[i].xyz + (velocity[i].xyz * velocity[i].w * deltaFrameTime);
        }
        else
        {
            matrices[i].xyz = matrices_old[i].xyz;
        }

        force[i].xyz *= 0.0;
    }
}