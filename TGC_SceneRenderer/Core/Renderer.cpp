#include "Renderer.h"
using namespace Core;

#define DRAWTEST
#ifdef DRAWTEST
Types::ShaderProgram *shProgram;
Scene::Mesh *testMesh;
Scene::Camera *cam;
glm::mat4 MVP;
#endif // DRAW_TEST_TRIANGLE

Renderer::Renderer(void)
{
}

bool Core::Renderer::load()
{
    return (bool)ogl_LoadFunctions();
}

void Core::Renderer::setup()
{
    // Get Tools / Collections Instances
    this->frameRate = Utils::FrameRate::Instance();
    this->time = Utils::Time::Instance();
    this->_texCollection = ECollections::Textures::Instance();
    // Setup OpenGL Flags
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Triangle Setup
#ifdef DRAWTEST
    testMesh = new Scene::Mesh();
    testMesh->loadMesh("../TGC_SceneRenderer/Resources/DefaultModels/Cube/cube.obj");
    // Shader
    shProgram = new Types::ShaderProgram();
    Types::Shader *frag = new Types::Shader(Types::Shader::Fragment);
    Types::Shader *vert = new Types::Shader(Types::Shader::Vertex);
    frag->loadFromFile("../TGC_SceneRenderer/Resources/Shaders/default_frag.glsl");
    vert->loadFromFile("../TGC_SceneRenderer/Resources/Shaders/default_vert.glsl");
    frag->compile();
    vert->compile();
    shProgram->attachShader(*frag);
    shProgram->attachShader(*vert);
    shProgram->link();
    shProgram->addUniform("MVP");
    shProgram->addUniform("diffuseMap");
    shProgram->addUniform("normalMap");
    // Camera
    cam = new Scene::Camera();
    cam->fieldOfView = 90.0f;
    cam->aspectRatio = 4.0f / 3.0f;
    cam->nearClipping = 0.1f;
    cam->farClipping = 1000.0f;
    cam->projectionType = Scene::Camera::Perspective;
    // Model View Projection Matrix
    glm::mat4 view = cam->getViewMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    glm::mat4 projection = cam->getProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0);
    MVP = projection * view * model;
#endif
}

void Core::Renderer::loop()
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
