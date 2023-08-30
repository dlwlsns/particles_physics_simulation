//////////////
// #INCLUDE //
//////////////

// Library header:
#include "cg_engine.h"

// C/C++:
#include <iostream>

CgEngine* engine;
Node* scene;

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
    engine = CgEngine::getIstance();
    engine->init(argc, argv);

    // Set callbacks
    engine->setKeyboardCallback(keyboardCallback);
    //engine->setSpecialCallback(specialCallback);

    // Load scene
    scene = new Node("Root");

    float border = 0.5f;

    Sphere* sphere = new Sphere("sphere", 2);

    // Generate sphere inside the scene
    srand((unsigned)time(NULL));
    for (int x = 0; x < 50000; x++) {
        sphere->addTransform(
            glm::vec4(
                (-border* 0.9) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (border * 0.9 - (-border * 0.9)))),
                (1.0) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.0f))),
                (-border * 0.9) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (border * 0.9 - (-border * 0.9)))),
                0.01f
            )
        );

        sphere->addVelocity(glm::vec4(0.0f));
        sphere->addForce(glm::vec4(glm::vec3(0.0f), 1.0f));
        sphere->colors.push_back(glm::vec4(rand() % 255, rand() % 255, rand() % 255, 1.0f));
    }

    Material* m0 = new Material("m0", glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 10.0);
    sphere->setMaterial(m0);
    scene->addChild(sphere);

    // Add camera to the scene
    Camera* staticCam = new PerspectiveCamera("static_cam", 1.0f, 5.0f, 45.0f, 1.0f);
    glm::mat4 s_camera_M = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 2.0f, 2.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    staticCam->setObjectCoordinates(s_camera_M);
    scene->addChild(staticCam);

    // Add light to the scene
    Light* light = new DirectionalLight("light", 100);
    light->setObjectCoordinates(s_camera_M);
    scene->addChild(light);

    // Set grid optimization parameters
    engine->setBorderDimension(0.5f);
    engine->setCellCount(20);
    engine->setGridCenter(glm::vec3(0.0f, 5.0f, 0.0f));

    // Parse selected scene and run
    engine->parse(scene);
    engine->run();

    engine->free();

    return 0;
}
