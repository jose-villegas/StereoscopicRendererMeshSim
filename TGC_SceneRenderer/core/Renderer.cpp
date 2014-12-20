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
    this->_texCollection = collections::Textures::Instance();
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
    //shProgram->addUniformBlock("sharedLights");
    //unfrBlockInfo = shProgram->getUniformBlock("sharedLights");
    //unfrBlockInfo->indices = new GLuint[65];
    //unfrBlockInfo->offset = new GLint[65];
    //const char *names[] = {
    //    "sharedLights.source[0].position",
    //    "sharedLights.source[0].direction",
    //    "sharedLights.source[0].color",
    //    "sharedLights.source[0].intensity",
    //    "sharedLights.source[0].attenuation",
    //    "sharedLights.source[0].innerConeAngle",
    //    "sharedLights.source[0].outerConeAngle",
    //    "sharedLights.source[0].lightType",
    //    "sharedLights.source[1].position",
    //    "sharedLights.source[1].direction",
    //    "sharedLights.source[1].color",
    //    "sharedLights.source[1].intensity",
    //    "sharedLights.source[1].attenuation",
    //    "sharedLights.source[1].innerConeAngle",
    //    "sharedLights.source[1].outerConeAngle",
    //    "sharedLights.source[1].lightType",
    //    "sharedLights.source[2].position",
    //    "sharedLights.source[2].direction",
    //    "sharedLights.source[2].color",
    //    "sharedLights.source[2].intensity",
    //    "sharedLights.source[2].attenuation",
    //    "sharedLights.source[2].innerConeAngle",
    //    "sharedLights.source[2].outerConeAngle",
    //    "sharedLights.source[2].lightType",
    //    "sharedLights.source[3].position",
    //    "sharedLights.source[3].direction",
    //    "sharedLights.source[3].color",
    //    "sharedLights.source[3].intensity",
    //    "sharedLights.source[3].attenuation",
    //    "sharedLights.source[3].innerConeAngle",
    //    "sharedLights.source[3].outerConeAngle",
    //    "sharedLights.source[3].lightType",
    //    "sharedLights.source[4].position",
    //    "sharedLights.source[4].direction",
    //    "sharedLights.source[4].color",
    //    "sharedLights.source[4].intensity",
    //    "sharedLights.source[4].attenuation",
    //    "sharedLights.source[4].innerConeAngle",
    //    "sharedLights.source[4].outerConeAngle",
    //    "sharedLights.source[4].lightType",
    //    "sharedLights.source[5].position",
    //    "sharedLights.source[5].direction",
    //    "sharedLights.source[5].color",
    //    "sharedLights.source[5].intensity",
    //    "sharedLights.source[5].attenuation",
    //    "sharedLights.source[5].innerConeAngle",
    //    "sharedLights.source[5].outerConeAngle",
    //    "sharedLights.source[5].lightType",
    //    "sharedLights.source[6].position",
    //    "sharedLights.source[6].direction",
    //    "sharedLights.source[6].color",
    //    "sharedLights.source[6].intensity",
    //    "sharedLights.source[6].attenuation",
    //    "sharedLights.source[6].innerConeAngle",
    //    "sharedLights.source[6].outerConeAngle",
    //    "sharedLights.source[6].lightType",
    //    "sharedLights.source[7].position",
    //    "sharedLights.source[7].direction",
    //    "sharedLights.source[7].color",
    //    "sharedLights.source[7].intensity",
    //    "sharedLights.source[7].attenuation",
    //    "sharedLights.source[7].innerConeAngle",
    //    "sharedLights.source[7].outerConeAngle",
    //    "sharedLights.source[7].lightType",
    //    "sharedLights.count"
    //};
    //shProgram->use();
    //shProgram->setUniformBlockInfoIndexAndOffset("sharedLights", unfrBlockInfo, names, 65);
    //shProgram->bindUniformBlock("sharedLights");
    //// Copy values to buffer object memory addresses
    //float intensity = 0.5, attenuation = 2.0, innerConeAngle = 0.0, outerConeAngle = 1.0;
    //int lightType = 1; unsigned int count = 1;
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[0], glm::value_ptr(glm::vec3(0.0, 0.0, -3.0)), sizeof(glm::vec3)); // position
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[1], glm::value_ptr(glm::vec3(0.0, 0.0, -1.0)), sizeof(glm::vec3)); // direction
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[2], glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)), sizeof(glm::vec3)); // color
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[3], &intensity, sizeof(float));
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[4], &attenuation, sizeof(float));
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[5], &innerConeAngle, sizeof(float));
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[6], &outerConeAngle, sizeof(float));
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[7], &lightType, sizeof(int));
    //memcpy(unfrBlockInfo->dataPointer + unfrBlockInfo->offset[64], &count, sizeof(unsigned int));
    //// Update buffer data
    //shProgram->updateUniformBlockBufferData("sharedLights");
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
    model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -5.0f)) * glm::rotate<float>(time->totalTime() * 15.0f, glm::vec3(0.30, 0.60, 0.90));
    // set matrices
    _elementalMatrices->setModelMatrix(model);
    _elementalMatrices->setViewMatrix(view);
    _elementalMatrices->setProjectionMatrix(projection);
    _elementalMatrices->calculateMatrices();
    //shProgram->setUniform("inputMatrices.modelViewProjection", modelViewProjection);
    //shProgram->setUniform("inputMatrices.view", view);
    //shProgram->setUniform("inputMatrices.modelView", modelView);
    //shProgram->setUniform("inputMatrices.normal", normalMatrix);
    //shProgram->setUniform("light[0].color", glm::vec3(1.0, 1.0, 1.0));
    //shProgram->setUniform("light[0].position", glm::vec3(0.0, 0.0, -3.0));
    //shProgram->setUniform("light[0].intensity", 0.5f);
    //shProgram->setUniform("light[0].attenuation", 2.0f);
    shProgram->setUniform("material.specular", glm::vec3(0.5, 0.5, 0.5));
    shProgram->setUniform("material.diffuse", glm::vec3(0.7, 0.7, 0.7));
    shProgram->setUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
    shProgram->setUniform("material.shininess", 20.0f);
    shProgram->setUniform("lightsCount", 1);
    _elementalMatrices->setUniforms();
    testMesh->render();
}

Renderer *core::Renderer::_rdInstance = nullptr;
