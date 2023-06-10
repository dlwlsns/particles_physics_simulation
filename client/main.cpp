//////////////
// #INCLUDE //
//////////////

// Library header:
#include "cg_engine.h"

// C/C++:
#include <iostream>
#include <vector>

CgEngine* engine;
Node* scene;

Node* search(Node* node, char* name) {
    if (strcmp(node->getName(), name) == 0) {
        return node;
    }

    for (int i = 0; i < node->getChildrenCount(); i++) {
        Node* res = search(node->getChild(i), name);

        if (res != nullptr) {
            return res;
        }
    }

    return nullptr;
}

// https://www.youtube.com/watch?v=sLqXFF8mlEU&t=36s
void createFace(glm::vec3 normal, int resolution, Mesh* mesh) {
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

    int facesCount = mesh->getVertecies().size();
    
    for (int i = 0; i < triangles.size(); i+=3) {
        unsigned int faces[3] = {triangles[i] + facesCount, triangles[i+1] + facesCount, triangles[i+2] + facesCount};
        mesh->addFace(faces);
    }

    for (int i = 0; i < triangles.size()/3; i++) {
        mesh->addNormal(normal);
    }

    for (int i = 0; i < verticies.size(); i++) {
        float x2 = verticies[i].x * verticies[i].x;
        float y2 = verticies[i].y * verticies[i].y;
        float z2 = verticies[i].z * verticies[i].z;
        float x = verticies[i].x * sqrt(1 - (y2 + z2) / 2 + (y2 * z2) / 3);
        float y = verticies[i].y * sqrt(1 - (z2 + x2) / 2 + (z2 * x2) / 3);
        float z = verticies[i].z * sqrt(1 - (x2 + y2) / 2 + (x2 * y2) / 3);

        mesh->addVertex(glm::vec3(x, y, z));
    }
}

Mesh* generateSphere(int resolution) {
    Mesh* sphere = new Mesh("sphere");

    glm::vec3 faceNormals[6] = {
        glm::vec3(0.0f, 1.0f, 0.0f), 
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
        
    };

    for (int i = 0; i < 6; i++) {
        createFace(faceNormals[i], resolution+1, sphere);
    }

    return sphere;
}

/**
 * This callback is invoked each time a keyboard key is pressed.
 * @param key key pressed id
 * @param mouseX mouse X coordinate
 * @param mouseY mouse Y coordinate
 */
void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
    switch (key)
    {
        case 'w': // Wireframe toggle
            engine->toggleWireframe();
            break;
        case 'c': // Camera cycle
            engine->cameraRotation();
            break;
        default:
            break;
    }
}

/**
 * This callback is invoked each time a special keyboard key is pressed.
 * @param key key pressed id
 * @param mouseX mouse X coordinate
 * @param mouseY mouse Y coordinate
 */
void specialCallback(int key, int mouseX, int mouseY){}

//////////
// MAIN //
//////////

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char *argv[])
{
    // Init and use the lib:
    CgEngine* engine = CgEngine::getIstance();
    engine->init(argc, argv);

    // Set callbacks
    engine->setKeyboardCallback(keyboardCallback);
    //engine->setSpecialCallback(specialCallback);

    // Load scene
    scene = new Node("Root");

    Mesh* sphere = generateSphere(10);
    glm::mat4 sphere_M = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
    sphere->setObjectCoordinates(sphere_M);

    scene->addChild(sphere);

    // Add cameras to the scene
    Camera* staticCam = new PerspectiveCamera("static_cam", 1.0f, 2000.0f, 45.0f, 1.0f);
    glm::mat4 s_camera_M = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, 20.0f, 20.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    staticCam->setObjectCoordinates(s_camera_M);

    scene->addChild(staticCam);

    // Parse selected scene and run
    engine->parse(scene);
    engine->run();

    engine->free();

    return 0;
}
