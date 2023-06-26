//////////////
// #INCLUDE //
//////////////

// Library main include:

// C/C++:
#include <iostream>
#include <vector>
#include <chrono>
// Glew (include it before GL.h):
#include <GL/glew.h>

// FreeGLUT:   
#include <GL/freeglut.h>

// Engine
#include "cg_engine.h"
#include "spotLight.h"
#include "renderList.h"
#include "shaderGlobals.h"



//////////////
// DLL MAIN //
//////////////

int windowId;

// Elements
Node* currentScene;
std::vector<Material*> materials;
std::vector<Camera*> cameras;
std::vector<char*> guiText;
int activeCam = 1;
RenderList* renderlist;

// Appearance
bool wireframe = true;

// FPS:
int fps = 0;
int frames = 0;




/////////////
// SHADERS //
/////////////
ShaderGlobals shaders;
////////////////////////////
const char* vertShader = R"(
    #version 450 core

    // Uniforms:
    uniform mat4 invCamera;
    uniform mat4 projection;
    uniform float deltaFrameTime;
    mat3 normalMatrix;

    // Attributes:
    layout(location = 0) in vec3 in_Position;
    layout(location = 1) in vec3 in_Normal;
    layout(location = 2) in vec3 in_Color;
    layout(location = 3) in vec4 in_Transform;


    layout (std430, binding = 4) buffer ssboTransform
    {
        vec4 matrices[];
    };

    layout (std430, binding = 5) buffer ssboVelocity
    {
        vec4 velocity[];
    };

    

    // Varying:
    out vec4 fragPosition;
    out vec3 normal;
    out vec3 c;

    vec3 gravity = vec3(0.0, -9.81, 0.0);
    

    void main(void)
    {
        float mass = velocity[gl_InstanceID].w;

        if(deltaFrameTime > 0.0){
            // after need to sum forces
            vec3 f = mass * gravity;
            vec3 a = f / mass;

            if(matrices[gl_InstanceID].y <= matrices[gl_InstanceID].w){
                //velocity[gl_InstanceID].y *= -1.0;

                vec3 v0 = vec3(velocity[gl_InstanceID].x, velocity[gl_InstanceID].y, velocity[gl_InstanceID].z);
                float k = 0.5 * mass * length(v0) * length(v0);
                k *= 0.8;
                v0.y = sqrt(k / mass / 0.5);
                velocity[gl_InstanceID].x = v0.x;
                velocity[gl_InstanceID].y = v0.y;
                velocity[gl_InstanceID].z = v0.z;
            }

            vec3 v = vec3(0.0);

            v += a * deltaFrameTime;
            velocity[gl_InstanceID].x += v.x;
            velocity[gl_InstanceID].y += v.y;
            velocity[gl_InstanceID].z += v.z;

            matrices[gl_InstanceID].x += (velocity[gl_InstanceID].x * deltaFrameTime);
            matrices[gl_InstanceID].y += (velocity[gl_InstanceID].y * deltaFrameTime);
            matrices[gl_InstanceID].z += (velocity[gl_InstanceID].z * deltaFrameTime);
        }

        fragPosition = invCamera * mat4(matrices[gl_InstanceID].w, 0.0, 0.0, 0.0,
                                        0.0, matrices[gl_InstanceID].w, 0.0, 0.0,
                                        0.0, 0.0, matrices[gl_InstanceID].w, 0.0,
                                        matrices[gl_InstanceID].x, matrices[gl_InstanceID].y, matrices[gl_InstanceID].z, 1.0) * vec4(in_Position, 1.0);
        normalMatrix = inverse(transpose(mat3(mat4(matrices[gl_InstanceID].w, 0.0, 0.0, 0.0,
                                        0.0, matrices[gl_InstanceID].w, 0.0, 0.0,
                                        0.0, 0.0, matrices[gl_InstanceID].w, 0.0,
                                        matrices[gl_InstanceID].x, matrices[gl_InstanceID].y, matrices[gl_InstanceID].z, 1.0))));

        /*fragPosition = invCamera * mat4(in_Transform.w, 0.0, 0.0, 0.0,
                                        0.0, in_Transform.w, 0.0, 0.0,
                                        0.0, 0.0, in_Transform.w, 0.0,
                                        in_Transform.x, in_Transform.y, in_Transform.z, 1.0) * vec4(in_Position, 1.0);
        normalMatrix = inverse(transpose(mat3(mat4(in_Transform.w, 0.0, 0.0, 0.0,
                                        0.0,in_Transform.w, 0.0, 0.0,
                                        0.0, 0.0, in_Transform.w, 0.0,
                                        in_Transform.x, in_Transform.y, in_Transform.z, 1.0))));*/
        
        gl_Position = projection * fragPosition;
        normal = normalMatrix * in_Normal;
        c = normalize(in_Color);
    }
)";

////////////////////////////
// Directional Light
const char* directLightfragShader = R"(
   #version 450 core

   in vec4 fragPosition;
   in vec3 normal;  
   in vec3 c;
   
   out vec4 fragOutput;

   // Material properties:
   uniform vec3 matEmission;
   uniform vec3 matAmbient;
   uniform vec3 matDiffuse;
   uniform vec3 matSpecular;
   uniform float matShininess;

   // Light properties:
   uniform vec3 lightPosition; 
   uniform vec3 lightAmbient; 
   uniform vec3 lightDiffuse; 
   uniform vec3 lightSpecular;

   void main(void)
   {      
      // Final color:
      fragOutput = vec4(c, 1.0f);
      //fragOutput = vec4(nDotL, nDotL,nDotL, 1.0f);
   }
)";

/////////////////////////////
// BODY OF CLASS cg_engine //
/////////////////////////////

CgEngine* CgEngine::istance = 0;

CgEngine::CgEngine() {
    initFlag = false;
}

CgEngine* CgEngine::getIstance() {
    if (!CgEngine::istance)
        CgEngine::istance = new CgEngine();
    return CgEngine::istance;
}

/**
 * This method cycle through the list of all cameras (except for the gui camera).
 */
void CgEngine::cameraRotation() {
    activeCam++;

    if (activeCam >= cameras.size())
        activeCam = 1;

    Shader* current_shader = shaders.getShaderById(0);
    current_shader->setMatrix(current_shader->getParamLocation("projection"), cameras[activeCam]->getProjection());

    glUniformMatrix4fv(current_shader->getParamLocation("invCamera"), 1, GL_FALSE, glm::value_ptr(cameras[activeCam]->getInverse()));

    glutPostWindowRedisplay(windowId);
}

/**
 * This method toggle the wireframe.
 */
void CgEngine::toggleWireframe() {
    wireframe = !wireframe;

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glutPostWindowRedisplay(windowId);
}

/**
 * This function allows the client to set a keyboard callback function.
 */
void CgEngine::setKeyboardCallback(void (*func)(unsigned char, int, int)) {
    glutKeyboardFunc(func);
}

/**
 * This function allows the client to set a special callback function.
 */
void CgEngine::setSpecialCallback(void (*func)(int, int, int)) {
    glutSpecialFunc(func);
}

/**
 * This function allows the client to set a idle callback function.
 */
void CgEngine::setIdleCallback(void (*func)()) {
    glutIdleFunc(func);
}

unsigned int CgEngine::getElapsedTime() {
    return glutGet(GLUT_ELAPSED_TIME);
}

/**
 * This method add a line of text to the GUI.
 */
void CgEngine::addGuiText(char* text) {
    guiText.push_back(text);
}

/**
 * This function add the node and all his children to the RenderList.
 *
 * @param node node to add
 */
void CgEngine::parse(Node* scene) {
    if (scene->getParent() == nullptr) {
        while (cameras.size() > 1)
        {
            cameras.pop_back();
        }

        currentScene = scene;
    }

    if (dynamic_cast<const Camera*>(scene) != nullptr) {
        cameras.push_back(dynamic_cast<Camera*>(scene));

        //TODO: find a place to set first camera
        Shader* current_shader = shaders.getShaderById(0);
        glUniformMatrix4fv(current_shader->getParamLocation("invCamera"), 1, GL_FALSE, glm::value_ptr(cameras[activeCam]->getInverse()));
    }
    else if(dynamic_cast<const Sphere*>(scene) != nullptr) {
        //TODO: find a better way to add a new mesh type
        Sphere* s = dynamic_cast<Sphere*>(scene);
        
        renderlist->addItem(new RenderItem(s));
    }

    for (int i = 0; i < scene->getChildrenCount(); i++) {
        parse(scene->getChild(i));
    }

    
    //renderlist->sort();
}

/**
 * This method start the render of the scene.
 */
void CgEngine::run() {
    // Enter the main FreeGLUT processing loop:
    if (renderlist->size() == 0) {
        std::cout << "Scene not loaded." << std::endl;
        return;
    }

    if (cameras.size() == 1) {
        std::cout << "No camera loaded." << std::endl;
        return;
    }

    glutMainLoop();
}

/**
 * Deinitialization method.
 * @return true on success, false on error
 */
bool CgEngine::free()
{
    // Really necessary?
    if (!initFlag)
    {
        std::cout << "ERROR: class not initialized" << std::endl;
        return false;
    }

    // Done:
    initFlag = false;
    return true;
}


///////////////
// CALLBACKS //
///////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	  
/**
 * Debug message callback for OpenGL. See https://www.opengl.org/wiki/Debug_Output
 */
void __stdcall DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
    //std::cout << "OpenGL says: \"" << std::string(message) << "\"" << std::endl;
    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: " << message << std::endl;
    std::cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER";
        break;
    }
    std::cout << std::endl;

    std::cout << "id: " << id << std::endl;
    std::cout << "severity: ";
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    }
    std::cout << std::endl;
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

/**
 * This callback is invoked each second.
 * 
 * @param value passepartout value
 */
void timerCallback(int value)
{
    // Update values:
    fps = frames;
    frames = 0;
    glutSetWindowTitle(("Particles Physics Simulation - " + std::to_string(fps) + " FPS").c_str());

    // Register the next update:
    glutTimerFunc(1000, timerCallback, 0);
}

/**
 * This is the main rendering routine automatically invoked by FreeGLUT.
 */
void displayCallback()
{
    auto t_start = std::chrono::high_resolution_clock::now();

    // Clear the screen:         
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f); // RGBA components
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);

    // Render Nodes
    renderlist->render(cameras[activeCam]->getInverse());

    // Swap this context's buffer:  
    frames++;
    glutSwapBuffers();

    renderlist->deltaFrameTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() / 1000;

    // Force rendering refresh:
    glutPostWindowRedisplay(windowId);
}

/**
 * This callback is invoked each time the window gets resized (and once also when created).
 * 
 * @param width new window width
 * @param height new window height
 */
void reshapeCallback(int width, int height)
{
    glViewport(0, 0, width, height);

    for (int i = 0; i < cameras.size(); i++) {
        if (i == 0) {
            OrthographicCamera* gui = dynamic_cast<OrthographicCamera*>(cameras[i]);

            gui->setXMax(width);
            gui->setYMax(height);

            gui->setProjection(glm::ortho(gui->getXMin(), gui->getXMax(), gui->getYMin(), gui->getYMax(), gui->getNearPlane(), gui->getFarPlane()));
            continue;
        }

        if (dynamic_cast<const PerspectiveCamera*>(cameras[i]) != nullptr) {
            PerspectiveCamera* pCamera = dynamic_cast<PerspectiveCamera*>(cameras[i]);

            pCamera->setAspectRatio((float)width / (float)height);

            pCamera->setProjection(glm::perspective(glm::radians(pCamera->getFov()), pCamera->getAspectRatio(), pCamera->getNearPlane(), pCamera->getFarPlane()));
        }
        else {
            OrthographicCamera* oCamera = dynamic_cast<OrthographicCamera*>(cameras[i]);
            oCamera->setXMin(width/-200.0f);
            oCamera->setYMin(height/-200.0f);
            oCamera->setXMax(width/200.0f);
            oCamera->setYMax(height/200.0f);

            oCamera->setProjection(glm::ortho(oCamera->getXMin(), oCamera->getXMax(), oCamera->getYMin(), oCamera->getYMax(), oCamera->getNearPlane(), oCamera->getFarPlane()));
        }
    }

    Shader* current_shader = shaders.getShaderById(0);
    current_shader->setMatrix(current_shader->getParamLocation("projection"), cameras[activeCam]->getProjection());
}


///////////
// UTILS //
///////////

/**
 * This method print all the OpenGL details for the current machine.
 */
void getGlDetails() {
    // Check OpenGL version:
    std::cout << "OpenGL context" << std::endl;
    std::cout << "   version  . . : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "   vendor . . . : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "   renderer . . : " << glGetString(GL_RENDERER) << std::endl;

    // Log context properties:
    std::cout << "OpenGL properties:" << std::endl;
    std::cout << "   Driver . . . :  " << glGetString(GL_RENDERER) << std::endl;
}



/**
 * Initialization method.
 * @return true on success, false on error
 */
bool CgEngine::init(int argc, char* argv[])
{
    // Prevent double init:
    if (initFlag)
    {
        std::cout << "ERROR: class already initialized" << std::endl;
        return false;
    }

    // FreeGLUT can parse command-line params, in case:
    glutInit(&argc, argv);

    // Init and use the lib:
    // Init context:
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1280, 720);
    glutTimerFunc(1000, timerCallback, 0);

    //Set context to opengl4
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    //glutInitContextFlags(GLUT_DEBUG); // <-- Debug flag required by the OpenGL debug callback    

    // Set some optional flags:
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // Create the window with a specific title:   
    windowId = glutCreateWindow("Particles Physics Simulation");

    // Init Glew (*after* the context creation):
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();

    // OpenGL 2.1 is required:
    /*if (!glewIsSupported("GL_VERSION_2_1"))
    {
        std::cout << "OpenGL 2.1 not supported" << std::endl;
        return 0;
    }*/

    if (error != GLEW_OK)
    {
        std::cout << "[ERROR] " << glewGetErrorString(error) << std::endl;
        return -1;
    }
    else
        if (GLEW_VERSION_4_5)
            std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
        else
        {
            std::cout << "[ERROR] OpenGL 4.4 not supported\n" << std::endl;
            return -1;
        }

    // Register OpenGL debug callback:
    glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    // The OpenGL context is now initialized...
    glEnable(GL_DEPTH_TEST);

    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Set lighting options
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_NORMALIZE);

    //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

    // Set callback functions:
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);

    getGlDetails();

    ////////////////////////////////
    // SHADERS STUFF
    ////////////////////////////////

    // Compile vertex shader:
    Shader* vs = new Shader("Vertex");
    vs->loadFromMemory(Shader::TYPE_VERTEX, vertShader);

    // Compile fragment shader:
    Shader* fs = new Shader("Fragment");
    fs->loadFromMemory(Shader::TYPE_FRAGMENT, directLightfragShader);

    // Setup shader program:
    Shader* shader = new Shader("directLight");
    shader->build(vs, fs);
    shader->use();
    shader->bind(0, "in_Position");
    shader->bind(1, "in_Normal");
    shader->bind(2, "in_Transform");
    shader->bind(10, "in_Color");


    /*
    int matEmissionLoc = shader->getParamLocation("matEmission");
    int matAmbientLoc = shader->getParamLocation("matAmbient");
    int matDiffuseLoc = shader->getParamLocation("matDiffuse");
    int matSpecularLoc = shader->getParamLocation("matSpecular");
    int matShininessLoc = shader->getParamLocation("matShininess");

    int lightPositionLoc = shader->getParamLocation("lightPosition");
    int lightAmbientLoc = shader->getParamLocation("lightAmbient");
    int lightDiffuseLoc = shader->getParamLocation("lightDiffuse");
    int lightSpecularLoc = shader->getParamLocation("lightSpecular");

    // Set initial material and light params:
    shader->setVec3(matEmissionLoc, glm::vec3(0.0f, 0.0f, 0.0f));
    shader->setVec3(matAmbientLoc, glm::vec3(0.1f, 0.1f, 0.1f));
    shader->setVec3(matDiffuseLoc, glm::vec3(0.7f, 0.7f, 0.7f));
    shader->setVec3(matSpecularLoc, glm::vec3(0.6f, 0.6f, 0.6f));
    shader->setFloat(matShininessLoc, 128.0f);

    shader->setVec3(lightAmbientLoc, glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setVec3(lightDiffuseLoc, glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setVec3(lightSpecularLoc, glm::vec3(1.0f, 1.0f, 1.0f));

    shader->setVec3(lightPositionLoc, glm::vec3(10, 10, 0));*/

    shaders.addShader(shader);

    shaders.activateShader(0);

    renderlist = new RenderList("renderlist");

    // Add an Orthographic Camera for the GUI
    Camera* gui = new OrthographicCamera("GUI", -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    cameras.push_back(gui);

    // Done:
    initFlag = true;

    return true;
}
