#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>


#include "mesh.h"

Mesh::Mesh(char* name) : Node(name), material(nullptr), boundingSphere(false), isVaoInit(false) {}

Mesh::~Mesh() {
    std::cout << "Deleted mesh" << std::endl;
}

Material* Mesh::getMaterial() {
    return material;
}

void Mesh::setMaterial(Material* material) {
    this->material = material;
}

void Mesh::addVertex(glm::vec3 vertex) {
    this->verticies.push_back(vertex);
}

std::vector<glm::vec3> Mesh::getVertecies() {
    return verticies;
}

void Mesh::addNormal(glm::vec3 normal) {
    this->normals.push_back(normal);
}

void Mesh::addFace(unsigned int face[3]) {
    this->faces.push_back(face[0]);
    this->faces.push_back(face[1]);
    this->faces.push_back(face[2]);
}

std::vector<unsigned int> Mesh::getFaces() {
    return faces;
}

void Mesh::setBoundingSphereRadius(float radius) {
    this->boundingSphereRadius = radius;
}

float Mesh::getBoundingSphereRadius() {
    return this->boundingSphereRadius;
}

void Mesh::toggleBoundingSphere() {
    this->boundingSphere = !this->boundingSphere;
}

bool Mesh::isBoundingSphereEnabled() {
    return this->boundingSphere;
}

void Mesh::addTransform(glm::vec4 matrix) {
    this->matrices.push_back(matrix);
    this->pingPongMatrices.push_back(matrix);
}

std::vector<glm::vec4> Mesh::getMatrices() {
    return this->matrices;
}

void Mesh::addVelocity(glm::vec4 matrix) {
    this->velocities.push_back(matrix);
}

std::vector<glm::vec4> Mesh::getVelocities() {
    return this->velocities;
}

#include <glm/gtx/string_cast.hpp>
void Mesh::initVAO()
{
    if (!isVaoInit) {
        glGenVertexArrays(1, &vaoGlobal);
        glBindVertexArray(vaoGlobal);

        unsigned int N = verticies.size();

        //glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_NORMAL_ARRAY);
        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glGenBuffers(1, &vboVertex);
        glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
        glBufferData(GL_ARRAY_BUFFER, N * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        glBufferData(GL_ARRAY_BUFFER, N * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &vboFace);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboFace);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);

        glGenBuffers(1, &vboColor);
        glBindBuffer(GL_ARRAY_BUFFER, vboColor);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
        std::cout << colors.size() << std::endl;
        for (int i = 0; i < colors.size(); i++) {
            std::cout << glm::to_string(colors[i]) << std::endl;
        }

        glGenBuffers(1, &vboTransform);
        glBindBuffer(GL_ARRAY_BUFFER, vboTransform);
        glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::vec4), &matrices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);

        glGenBuffers(1, &ssboTransform);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboTransform);
        glBufferData(GL_SHADER_STORAGE_BUFFER, pingPongMatrices.size() * sizeof(glm::vec4), &pingPongMatrices[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssboTransform);
        std::cout << matrices.size() << std::endl;

        glGenBuffers(1, &ssboVelocity);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboVelocity);
        glBufferData(GL_SHADER_STORAGE_BUFFER, velocities.size() * sizeof(glm::vec4), &velocities[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssboVelocity);

        glBindVertexArray(0);

        std::cout << "Verticies: " << N << " - Faces: " << faces.size()/3 << " - Normals: " << normals.size() << std::endl;

        isVaoInit = true;
    }
}

unsigned int Mesh::getVAO() {
    return this->vaoGlobal;
}

void Mesh::render(glm::mat4 inverseCamera) {
    if (this->getMaterial() != nullptr)
        this->getMaterial()->render(inverseCamera);
}

#include "shaderGlobals.h"
bool ppBuffer = true;
void Mesh::pingPongBufferSwap() {
    glBindVertexArray(vaoGlobal);
    Shader* current_shader = shaders.getShaderById(0);
    GLuint t0 = glGetAttribLocation(current_shader->glId, "ssboTransform");
    GLuint t1 = glGetAttribLocation(current_shader->glId, "in_Transform");

    if (ppBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, vboTransform);
        glVertexAttribPointer(t1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(t1);
        glVertexAttribDivisor(t1, 1);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboTransform);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, t0, ssboTransform);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, vboTransform);
        glVertexAttribPointer(t0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(t0);
        glVertexAttribDivisor(t0, 1);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboTransform);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, t1, ssboTransform);
    }
    
    ppBuffer = !ppBuffer;
}