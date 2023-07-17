#include "sphere.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// https://www.youtube.com/watch?v=sLqXFF8mlEU&t=36s
void Sphere::createFace(glm::vec3 normal, int resolution) {
    glm::vec3 axisA = glm::vec3(normal.y, normal.z, normal.x);
    glm::vec3 axisB = glm::cross(normal, axisA);
    std::vector<glm::vec3> verticies(resolution * resolution);
    std::vector<int> triangles((resolution - 1) * (resolution - 1) * 6);
    std::vector<glm::vec3> normals;
    int triIndex = 0;

    for (int y = 0; y < resolution; y++) {
        for (int x = 0; x < resolution; x++) {
            int vertexIndex = x + y * resolution;

            glm::vec2 t = glm::vec2(x, y) / (resolution - 1.0f);
            glm::vec3 point = normal + axisA * (2 * t.x - 1) + axisB * (2 * t.y - 1);
            verticies[vertexIndex] = point;

            if (x != (resolution - 1) && y != (resolution - 1)) {
                triangles.push_back(vertexIndex);
                triangles.push_back(vertexIndex + resolution + 1);
                triangles.push_back(vertexIndex + resolution);

                triangles.push_back(vertexIndex);
                triangles.push_back(vertexIndex + 1);
                triangles.push_back(vertexIndex + resolution + 1);
            }
        }
    }

    int facesCount = this->getVertecies().size();

    for (int i = 0; i < triangles.size(); i += 3) {
        unsigned int faces[3] = { triangles[i] + facesCount, triangles[i + 1] + facesCount, triangles[i + 2] + facesCount };
        this->addFace(faces);
    }

    for (int i = 0; i < verticies.size(); i++) {
        float x2 = verticies[i].x * verticies[i].x;
        float y2 = verticies[i].y * verticies[i].y;
        float z2 = verticies[i].z * verticies[i].z;
        float x = verticies[i].x * sqrt(1 - (y2 + z2) / 2 + (y2 * z2) / 3);
        float y = verticies[i].y * sqrt(1 - (z2 + x2) / 2 + (z2 * x2) / 3);
        float z = verticies[i].z * sqrt(1 - (x2 + y2) / 2 + (x2 * y2) / 3);

        this->addVertex(glm::vec3(x, y, z));
        this->addNormal(glm::normalize(glm::vec3(x, y, z) - glm::vec3(0.0)));
    }
}

Sphere::Sphere(char* name, int resolution) : Mesh(name) {
    glm::vec3 faceNormals[6] = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)

    };

    for (int i = 0; i < 6; i++) {
        createFace(faceNormals[i], resolution + 1);
    }
}

Sphere::~Sphere() {

}

void Sphere::render(glm::mat4 inverseCamera) {
    Mesh::render(inverseCamera);
}