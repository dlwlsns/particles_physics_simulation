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

layout(std430, binding = 7) buffer vboColor
{
    vec4 color[];
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

void planeBounce(int i, vec3 normal){
    float mass = force[i].w;

    // calculate direction of velocity vector
    velocity[i].xyz -= (2 * (dot(velocity[i].xyz, normal) * normal));

    // calculate energy loss and new velocity length
    float l = velocity[i].w;
    float k = 0.5 * mass * l * l;
    k *= 1.0;

    velocity[i].w = sqrt((k * 2) / mass);

    //truncate if speed is less than 1 mm/s
    if (velocity[i].w <= 0.001) velocity[i].w = 0.0;
}



void main(void)
{
    // ID of the ball is equal to the shader instance's ID
    int i = int(gl_WorkGroupID.x);

    if (deltaFrameTime > 0.0){
        if(velocity[i].w > 0.0)
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

            if (check)
                matrices[i].xyz = pos.xyz + (velocity[i].xyz * velocity[i].w * deltaFrameTime);
            else
                matrices[i].xyz = pos.xyz;
        }
    }

    int cell = -1;
    float dim = 10.0;
    int n_cells = 2;
    float cell_size = dim / n_cells;
    

    vec3 min = vec3(-5.0, 0.0, -5.0);
    vec3 max = vec3(5.0, 10.0, 5.0);

    int x_cell = int((matrices[i].x - min.x) / cell_size);
    int y_cell = int((matrices[i].y - min.y) / cell_size);
    int z_cell = int((matrices[i].z - min.z) / cell_size);

    cell = x_cell + n_cells * (y_cell + n_cells * z_cell);

    vec3 col = vec3(0, 0, 0);
    for(int c = 0; c < n_cells * n_cells * n_cells; c++)
    {
        if(cell == c)
        {
            color[i].xyz = vec3(255 / (c % 2), 255 / (c % 3), 255 / (c % 5));
            break;
        }
    }
}