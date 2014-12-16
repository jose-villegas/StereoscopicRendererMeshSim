#include "Renderer.h"
using namespace core;

#define DRAWTEST
#ifdef DRAWTEST
types::ShaderProgram *shProgram;
scene::Mesh *testMesh;
scene::Camera *cam;
glm::mat4 MVP;
#endif // DRAW_TEST_TRIANGLE

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
    // Setup OpenGL Flags
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Triangle Setup
#ifdef DRAWTEST
    testMesh = new scene::Mesh();
    testMesh->loadMesh("../TGC_SceneRenderer/resources/models/cube/cube.obj");
    // Shader
    shProgram = new types::ShaderProgram();
    types::Shader *frag = new types::Shader(types::Shader::Fragment);
    types::Shader *vert = new types::Shader(types::Shader::Vertex);
    frag->loadFromFile("../TGC_SceneRenderer/resources/shaders/default_frag.glsl");
    vert->loadFromFile("../TGC_SceneRenderer/resources/shaders/default_vert.glsl");
    frag->compile();
    vert->compile();
    shProgram->attachShader(*frag);
    shProgram->attachShader(*vert);
    shProgram->link();
    shProgram->addUniform("MVP");
    shProgram->addUniform("diffuseMap");
    shProgram->addUniform("normalMap");
    // Camera
    cam = new scene::Camera();
    cam->fieldOfView = 90.0f;
    cam->aspectRatio = 4.0f / 3.0f;
    cam->nearClipping = 0.1f;
    cam->farClipping = 1000.0f;
    cam->projectionType = scene::Camera::Perspective;
    // Model View Projection Matrix
    glm::mat4 view = cam->getViewMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    glm::mat4 projection = cam->getProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0);
    MVP = projection * view * model;
#endif
}

void core::Renderer::loop()
{
    // Clear the color and depth buffers.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DRAWTEST
    shProgram->use();
    shProgram->setUniform("MVP", MVP
                          * glm::translate(glm::vec3(0, 0, -15))
                          * glm::rotate<float>(glm::mat4(1), time->totalTime() * 50.0f, glm::vec3(1.0, 0.0, 0.0)));
    shProgram->setUniform("diffuseMap", 0);
    shProgram->setUniform("normalMap", 0);
    testMesh->render();
#endif
}
