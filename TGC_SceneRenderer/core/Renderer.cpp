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
    // Get Tools / Collections Instances
    this->frameRate = utils::FrameRate::Instance();
    this->time = utils::Time::Instance();
    this->_texCollection = collections::TexturesCollection::Instance();
    ShadersData::Initialize();
    // Load Misc Resources
    ResourcesLoader::loadDefaultTexture(_texCollection);
    ResourcesLoader::loadStoredShaders();
    // Setup OpenGL Flags
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Load member classes
    this->_elementalMatrices = new Matrices();
    this->_elementalMatrices->setUniformBlockInfo();
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
    // Testing light UBO
    unfrBlockInfo = shProgram->getUniformBlock("sharedLights");
    unfrBlockInfo->indices = new GLuint[ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    unfrBlockInfo->offset = new GLint[ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    shProgram->setUniformBlockInfoIndexAndOffset(
        "sharedLights", unfrBlockInfo,
        (const char **)ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES,
        ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT
    );
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
            glm::rotate<float>(time->totalTime() * 15.0f, glm::vec3(1, 0, 0)) *
            glm::rotate<float>(time->totalTime() * 30.0f, glm::vec3(0, 1, 0)) *
            glm::rotate<float>(time->totalTime() * 45.0f, glm::vec3(0, 0, 1));
    // set matrices
    _elementalMatrices->setModelMatrix(model);
    _elementalMatrices->setViewMatrix(view);
    _elementalMatrices->setProjectionMatrix(projection);
    _elementalMatrices->calculateMatrices();
    _elementalMatrices->setUniforms();
    // Light UBO
    shProgram->bindUniformBlock("sharedLights");
    // Copy values to buffer object memory addresses
    float intensity = 2.5, attenuation = 3.0, innerConeAngle = 0.0, outerConeAngle = 1.0;
    int lightType = 1; unsigned int count = 2;
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[0], glm::value_ptr(glm::vec3(6.0, 0.0, -10.0)), sizeof(glm::vec3)); // position
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[1], glm::value_ptr(glm::vec3(0.0, 0.0, -1.0)), sizeof(glm::vec3)); // direction
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[2], glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)), sizeof(glm::vec3)); // color
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[3], &intensity, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[4], &attenuation, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[5], &innerConeAngle, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[6], &outerConeAngle, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[7], &lightType, sizeof(int));
    // Light 2
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[8], glm::value_ptr(glm::vec3(-6.0, 0.0, -10.0)), sizeof(glm::vec3)); // position
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[9], glm::value_ptr(glm::vec3(0.0, 0.0, -1.0)), sizeof(glm::vec3)); // direction
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[10], glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)), sizeof(glm::vec3)); // color
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[11], &intensity, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[12], &attenuation, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[13], &innerConeAngle, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[14], &outerConeAngle, sizeof(float));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[15], &lightType, sizeof(int));
    memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[64], &count, sizeof(unsigned int));
    // Update buffer data
    shProgram->updateUniformBlockBufferData("sharedLights");
    testMesh->render();
}

Renderer *core::Renderer::_rdInstance = nullptr;
