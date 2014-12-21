#include "Renderer.h"
using namespace core;

// Testing
types::ShaderProgram *shProgram;
scene::Mesh *testMesh;
scene::Camera *cam;
glm::mat4 modelViewProjection, view, projection, model;
glm::mat4 modelView;
glm::mat4 normalMatrix;
types::ShaderProgram::UniformBlock *unfrBlockInfo;
// --

Renderer::Renderer(void)
{
}

Renderer *core::Renderer::Instance()
{
    if (!_rdInstance) { _rdInstance = new Renderer(); }

    return _rdInstance;
}

bool core::Renderer::load()
{
    return (bool)ogl_LoadFunctions();
}

void core::Renderer::setup()
{
    // Initialize Engine Data
    core::Data::Initialize();
    // Get Tools / Collections Instances
    this->_frameRate = utils::FrameRate::Instance();
    this->_time      = utils::Time::Instance();
    this->_textures  = collections::TexturesCollection::Instance();
    this->_lights    = collections::LightsCollection::Instance();
    // Setup OpenGL Flags
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Load member classes
    this->_elementalMatrices = new Matrices();
    // Testing
    testMesh = new scene::Mesh();
    testMesh->loadMesh("../TGC_SceneRenderer/resources/models/cube/cube.obj");
    // Shader
    shProgram = collections::stored::Shaders::getDefaultShader(core::AvailableShaders::Diffuse);
    // Camera
    cam = new scene::Camera();
    cam->fieldOfView  = 90.0f;
    cam->aspectRatio  = 4.0f / 3.0f;
    cam->nearClipping = 0.1f;
    cam->farClipping  = 100.0f;
    cam->projectionType = scene::Camera::Perspective;
    // Model View Projection Matrix
    view = cam->getViewMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    projection = cam->getProjectionMatrix();
    model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -5.0f));
    modelView = view * model;
    modelViewProjection = projection * modelView;
    // set elemental  matrices data info
    this->_elementalMatrices->setShaderProgram(shProgram);
    this->_elementalMatrices->setUniformBlockInfo();
    // Testing light UBO
    //unfrBlockInfo = shProgram->getUniformBlock("sharedLights");
    //unfrBlockInfo->indices = new GLuint[Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    //unfrBlockInfo->offset = new GLint[Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    //shProgram->setUniformBlockInfoIndexAndOffset(
    //    "sharedLights", unfrBlockInfo,
    //    (const char **)Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES,
    //    Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT
    //);
    this->_lights->createLight();
    this->_lights->createLight();
    this->_lights->setShaderProgram(shProgram);
    this->_lights->setUniformBlockInfo();
    this->_lights->getLight(0)->transform.setPosition(6.0, 0.0, -10.0);
    this->_lights->getLight(1)->transform.setPosition(-6.0, 0.0, -10.0);
    this->_lights->getLight(0)->setColor(0.0f, 1.0f, 0.0f);
    this->_lights->getLight(1)->setColor(0.0f, 0.0f, 1.0f);
    this->_lights->getLight(0)->intensity = 5.0f;
    this->_lights->getLight(1)->intensity = 5.0f;
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
    model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -15.0f)) *
            glm::rotate<float>(_time->totalTime() * 15.0f, glm::vec3(1, 0, 0)) *
            glm::rotate<float>(_time->totalTime() * 30.0f, glm::vec3(0, 1, 0)) *
            glm::rotate<float>(_time->totalTime() * 45.0f, glm::vec3(0, 0, 1));
    // set matrices
    _elementalMatrices->setModelMatrix(model);
    _elementalMatrices->setViewMatrix(view);
    _elementalMatrices->setProjectionMatrix(projection);
    _elementalMatrices->calculateMatrices();
    // Elemental Matrices UBO set
    _elementalMatrices->setUniformBlock();
    // Light UBO set
    this->_lights->setUniformBlock();
    testMesh->render();
}

Renderer *core::Renderer::_rdInstance = nullptr;
