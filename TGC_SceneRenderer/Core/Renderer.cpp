#include "Renderer.h"
using namespace core;

// Testing
types::ShaderProgram *shProgram;
scene::Mesh *testMesh;
scene::Camera *cam;
glm::mat4 MVP, view, projection, model;
glm::mat4 MV;
// --

Renderer::Renderer(void)
{
}

bool core::Renderer::load()
{
    return (bool)ogl_LoadFunctions();
}

void core::Renderer::setup()
{
    // Get Tools / Collections Instances
    this->frameRate = utils::FrameRate::Instance();
    this->time = utils::Time::Instance();
    this->_texCollection = collections::Textures::Instance();
    // Load Misc Resources
    ResourcesLoader::loadDefaultTexture(_texCollection);
    ResourcesLoader::loadStoredShaders();
    // Setup OpenGL Flags
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Testing
    testMesh = new scene::Mesh();
    testMesh->loadMesh("../TGC_SceneRenderer/resources/models/cube/cube.obj");
    // Shader
    shProgram = collections::stored::Shaders::getDefaultShader(collections::stored::Shaders::Diffuse);
    // Camera
    cam = new scene::Camera();
    cam->fieldOfView = 90.0f;
    cam->aspectRatio = 4.0f / 3.0f;
    cam->nearClipping = 0.1f;
    cam->farClipping = 100.0f;
    cam->projectionType = scene::Camera::Perspective;
    // Model View Projection Matrix
    view = cam->getViewMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    projection = cam->getProjectionMatrix();
    model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -10.0f));
    MV = view * model;
    MVP = projection * MV;
}

void core::Renderer::loop()
{
    // Clear the color and depth buffers.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Testing
    shProgram->use();
    view = cam->getViewMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    projection = cam->getProjectionMatrix();
    model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -15.0f)) * glm::rotate<float>(time->totalTime() * 75.0f, glm::vec3(0.0, 1.0, 0.0));
    MV = view * model;
    MVP = projection * MV;
    shProgram->setUniform("inputMatrices.modelViewProjection", MVP);
    // shProgram->setUniform("diffuseMap", 0);
    testMesh->render();
}
