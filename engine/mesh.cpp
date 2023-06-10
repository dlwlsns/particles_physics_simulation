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

        glGenBuffers(1, &vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        glBufferData(GL_ARRAY_BUFFER, N * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &vboFace);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboFace);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        /*
        //glVertexPointer(3, GL_FLOAT, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        //glNormalPointer(GL_FLOAT, 0, nullptr);
        */

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboFace);

        glBindVertexArray(0);

        isVaoInit = true;
    }
}

void Mesh::render(glm::mat4 inverseCamera) {
    if (this->getMaterial() != nullptr)
        this->getMaterial()->render(inverseCamera);

    // Pass a triangle (object coordinates: the triangle is centered around the origin):
    glDepthFunc(GL_LESS);
   
    glBindVertexArray(vaoGlobal);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    
    //glDisable(GL_TEXTURE_2D);
}