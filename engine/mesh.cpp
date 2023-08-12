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

void Mesh::addForce(glm::vec4 matrix) {
    this->forces.push_back(matrix);
}

std::vector<glm::vec4> Mesh::getForces() {
    return this->forces;
}

void Mesh::addColor(glm::vec4 color) {
    this->colors.push_back(color);
}

std::vector<glm::vec4> Mesh::getColors() {
    return this->colors;
}

void Mesh::initVAO()
{
    if (!isVaoInit) {
        glGenVertexArrays(1, &vaoGlobal);
        glBindVertexArray(vaoGlobal);

        unsigned int N = verticies.size();

        // Static mesh data

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

        // Dynamic mesh data

        /*glGenBuffers(1, &vboColor);
        glBindBuffer(GL_ARRAY_BUFFER, vboColor);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);*/
        glGenBuffers(1, &vboColor);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, vboColor);
        glBufferData(GL_SHADER_STORAGE_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vboColor);

        


        glGenBuffers(1, &vboTransform);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, vboTransform);
        glBufferData(GL_SHADER_STORAGE_BUFFER, matrices.size() * sizeof(glm::vec4), &matrices[0], GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vboTransform);

        glGenBuffers(1, &ssboTransform);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboTransform);
        glBufferData(GL_SHADER_STORAGE_BUFFER, matrices.size() * sizeof(glm::vec4), &matrices[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssboTransform);

        glGenBuffers(1, &vboVelocity);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, vboVelocity);
        glBufferData(GL_SHADER_STORAGE_BUFFER, velocities.size() * sizeof(glm::vec4), &velocities[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vboVelocity);

        glGenBuffers(1, &ssboVelocity);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboVelocity);
        glBufferData(GL_SHADER_STORAGE_BUFFER, velocities.size() * sizeof(glm::vec4), &velocities[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssboVelocity);

        glGenBuffers(1, &ssboForce);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboForce);
        glBufferData(GL_SHADER_STORAGE_BUFFER, forces.size() * sizeof(glm::vec4), &forces[0], GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, ssboForce);

        glBindVertexArray(0);

        std::cout << "Verticies: " << N << " - Faces: " << faces.size()/3 << " - Normals: " << normals.size() << " Matrices: " << matrices.size() << " Colors: " << colors.size() << std::endl;

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

bool check = false;
void Mesh::pingPongBufferSwap() {

    if (check) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vboTransform);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssboTransform);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, vboVelocity);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssboVelocity);
    }
    else {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboTransform);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, vboTransform);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, vboVelocity);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssboVelocity);
    }

    check = !check;
}