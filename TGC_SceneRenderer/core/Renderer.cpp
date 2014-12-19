#include "Renderer.h"
using namespace core;

// Testing
types::ShaderProgram *shProgram;
scene::Mesh *testMesh;
scene::Camera *cam;
glm::mat4 modelViewProjection, view, projection, model;
glm::mat4 modelView;
glm::mat4 normalMatrix;
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
    modelView = view * model;
    modelViewProjection = projection * modelView;
    normalMatrix = glm::inverseTranspose(modelView); // Remember to just use modelview if orthographic
    //shProgram->setUniform("inputMatrices.modelViewProjection", modelViewProjection);
    //shProgram->setUniform("inputMatrices.view", view);
    //shProgram->setUniform("inputMatrices.modelView", modelView);
    //shProgram->setUniform("inputMatrices.normal", normalMatrix);
    shProgram->setUniform("light[0].color", glm::vec3(1.0, 1.0, 1.0));
    shProgram->setUniform("light[0].position", glm::vec3(0.0, 0.0, -3.0));
    shProgram->setUniform("light[0].intensity", 0.5f);
    shProgram->setUniform("light[0].attenuation", 2.0f);
    shProgram->setUniform("material.specular", glm::vec3(0.5, 0.5, 0.5));
    shProgram->setUniform("material.diffuse", glm::vec3(0.7, 0.7, 0.7));
    shProgram->setUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
    shProgram->setUniform("material.shininess", 20.0f);
    shProgram->setUniform("lightsCount", 1);
    // Query for the offsets of each block variable
    const GLchar *names[] = {
        "input.modelViewProjection", "input.modelView",
        "input.model", "input.view",
        "input.projection", "input.normal"
    };
    types::ShaderProgram::UniformBlock *uniformBlock =  shProgram->getUniformBlock("input");
    GLuint *indices = new GLuint[6];
    GLint *offset = new GLint[6];
    shProgram->bindUniformBlock("input");
    shProgram->getUniformBlockIndexAndOffset("input", names, &indices, &offset, 6);
    memcpy(uniformBlock->dataPointer + offset[0], glm::value_ptr(modelViewProjection), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[1], glm::value_ptr(modelView), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[2], glm::value_ptr(model), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[3], glm::value_ptr(view), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[4], glm::value_ptr(projection), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[5], glm::value_ptr(normalMatrix), sizeof(glm::mat4));
    shProgram->updateUniformBlockBufferData("input");
    testMesh->render();
}

Renderer *core::Renderer::_rdInstance = nullptr;
