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
int activeCam = 0;
RenderList* renderlist;

// Appearance
bool wireframe = true;

// FPS:
int fps = 0;
int frames = 0;
float deltaFrameTime = 0;

/////////////
// SHADERS //
/////////////
ShaderGlobals shaders;
Shader* cs;
Shader* light;

GLuint cs_program, cs_shader;

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
        activeCam = 0;

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

void CgEngine::setBorderDimension(float dimension) {
    if (dimension < 0) {
        dimension *= -1;
    }

    this->border = dimension;
}

void CgEngine::setCellCount(unsigned int cells) {
    if (cells == 0) {
        this->cellCount = 1;
    }else
        this->cellCount = cells;
}

void CgEngine::setGridCenter(glm::vec3 center) {
    this->gridCenter = center;
}

/**
 * This function add the node and all his children to the RenderList.
 *
 * @param node node to add
 */
void CgEngine::parse(Node* scene) {
    if (scene->getParent() == nullptr) {
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

    if (cameras.size() < 1) {
        std::cout << "No camera loaded." << std::endl;
        return;
    }

    
    int n_items = renderlist->get(0)->matrices.size();

    for (int i = 0; i < this->cellCount * this->cellCount * this->cellCount; i++) {
        counters.push_back(0);
    }

    for (int i = 0; i < this->cellCount * this->cellCount * this->cellCount * n_items; i++) {
        cells.push_back(0);
    }

    glGenBuffers(1, &ssboGrid);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboGrid);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->cellCount * this->cellCount * this->cellCount * n_items * sizeof(int), &cells[0], GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, ssboGrid);

    
    glGenBuffers(1, &ssboGridCounter);
    // bind the buffer and define its initial storage capacity
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, ssboGridCounter);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * this->cellCount * this->cellCount * this->cellCount, &counters[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, ssboGridCounter);

    

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
    if (type == GL_DEBUG_TYPE_OTHER) {
        return;
    }

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

void CgEngine::updateGrid() {
    // 8000 is a limitation forced by arrays in compute shaders
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, ssboGridCounter);
    glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * 8000, static_cast<void*>(counters.data()));
}

void CgEngine::updateUniforms() {
    // update delta time
    GLuint location = glGetUniformLocation(shaders.getActiveShader()->glId, "deltaFrameTime");
    glUniform1f(location, deltaFrameTime);

    if (deltaFrameTime < 0.1f) {
        // set sphere count
        location = glGetUniformLocation(shaders.getActiveShader()->glId, "sphereCount");
        glUniform1i(location, renderlist->get(0)->matrices.size());

        // set borders
        location = glGetUniformLocation(shaders.getActiveShader()->glId, "border");
        glUniform1f(location, this->border);

        // set cell count
        location = glGetUniformLocation(shaders.getActiveShader()->glId, "n_cells");
        glUniform1i(location, this->cellCount);

        // set grid center
        location = glGetUniformLocation(shaders.getActiveShader()->glId, "grid_center");
        glUniform3f(location, this->gridCenter.x, this->gridCenter.y, this->gridCenter.z);
    }
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
    shaders.activateShader(0);
    renderlist->render(cameras[activeCam]->getInverse());

    // Swap this context's buffer:  
    frames++;
    glutSwapBuffers();

    // run compute shader
    shaders.activateShader(1);

    // reset atomic counters and uniforms
    CgEngine::getIstance()->updateGrid();
    CgEngine::getIstance()->updateUniforms();

    // dispatch compute shader
    glDispatchCompute((GLuint)(renderlist->get(0)->matrices.size()), 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    deltaFrameTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count() / 1000;

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
    glutInitWindowSize(1920, 1080);
    glutTimerFunc(1000, timerCallback, 0);

    //Set context to opengl4
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG); // <-- Debug flag required by the OpenGL debug callback    

    // Set some optional flags:
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // Create the window with a specific title:   
    windowId = glutCreateWindow("Particles Physics Simulation");

    // Init Glew (*after* the context creation):
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();

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
            std::cout << "[ERROR] OpenGL 4.5 not supported\n" << std::endl;
            return -1;
        }

    // Register OpenGL debug callback:
    glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    // The OpenGL context is now initialized...
    glEnable(GL_DEPTH_TEST);

    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Set lighting options
    glEnable(GL_CULL_FACE);

    // Set callback functions:
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);

    getGlDetails();

    ////////////////////////////////
    // SHADERS STUFF
    ////////////////////////////////

    // Compile vertex shader:
    Shader* vs = new Shader("Vertex");
    vs->loadFromFile(Shader::TYPE_VERTEX, "../engine/shaders/simple.vert");

    // Compile fragment shader:
    Shader* fs = new Shader("Fragment");
    fs->loadFromFile(Shader::TYPE_FRAGMENT, "../engine/shaders/directLight.frag");

    // Setup shader program:
    light = new Shader("directLight");
    light->build(vs, fs);
    light->use();
    light->bind(15, "in_Position");
    light->bind(1, "in_Normal");
    light->bind(3, "in_Transform");

    cs = new Shader("ComputeShader");

    Shader* cs1 = new Shader("Compute");
    cs1->loadFromFile(Shader::TYPE_COMPUTE, "../engine/shaders/sphere.comp");
    
    cs->build(cs1);

    shaders.addShader(light);
    shaders.addShader(cs);

    shaders.activateShader(0);

    renderlist = new RenderList("renderlist");

    // Done:
    initFlag = true;

    return true;
}
