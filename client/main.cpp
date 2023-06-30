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

    Sphere* sphere = new Sphere("sphere", 2);
    
    float scale[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};

    Material* m0 = new Material("m0", glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 10.0);

    int maxVelocity = 5;
    srand((unsigned)time(NULL));
    for (int x = 0; x < 10; x++) {
        sphere->addTransform(glm::vec4(0.0f, 5.0f, 0.0f, scale[rand() % 5]));
        sphere->addVelocity(
            glm::vec4(
                rand() % (maxVelocity - (-maxVelocity) + 1) + (-maxVelocity),
                0.0f,
                rand() % (maxVelocity - (-maxVelocity) + 1) + (-maxVelocity), (rand() % 20) + 1
            )
        );
        sphere->colors.push_back(glm::vec3(rand() % 255, rand() % 255, rand() % 255));
    }
    sphere->setMaterial(m0);
    scene->addChild(sphere);

    // Add cameras to the scene
    Camera* staticCam = new PerspectiveCamera("static_cam", 1.0f, 2000.0f, 45.0f, 1.0f);
    glm::mat4 s_camera_M = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 15.0f, 10.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    staticCam->setObjectCoordinates(s_camera_M);

    scene->addChild(staticCam);

    Light* light = new DirectionalLight("light", 100);
    light->setObjectCoordinates(s_camera_M);

    scene->addChild(light);

    // Parse selected scene and run
    engine->parse(scene);
    engine->run();

    engine->free();

    return 0;
}
