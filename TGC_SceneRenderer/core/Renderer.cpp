#include "Renderer.h"

#include "../collections/SceneObjectsCollection.h"
#include "../collections/stored/StoredShaders.h"
#include "../scene/Camera.h"
#include "../scene/Mesh.h"
#include "../scene/Light.h"
#include "../types/ShaderProgram.h"
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "Data.h"

using namespace core;

Renderer::Renderer(void)
{
    // Get Tools / Collections Instances
    this->_frameRate    = utils::FrameRate::Instance();
    this->_time         = utils::Time::Instance();
    this->_lights       = collections::LightsCollection::Instance();
    this->_meshes		= collections::MeshesCollection::Instance();
    this->_cameras		= collections::CamerasCollection::Instance();
    this->_sceneObjects = collections::SceneObjectsCollection::Instance();
}

Renderer *core::Renderer::Instance()
{
    if (!_rdInstance) {
        _rdInstance = new Renderer();
    }

    return _rdInstance;
}

int core::Renderer::load()
{
    return ogl_LoadFunctions();
}

void core::Renderer::setup()
{
    // Initialize Engine Data
    core::ShadersData::Initialize();
    // Setup OpenGL Flags
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Load member classes
    this->_elementalMatrices = new Matrices();
    // Get a default stored shader
    types::ShaderProgram *shProgram = collections::stored::StoredShaders::getDefaultShader(core::StoredShaders::Diffuse);
    // Initial camera setup
    this->_activeCamera = this->_sceneObjects->addCamera();
    this->_activeCamera->projectionType = scene::Camera::Perspective;
    this->_activeCamera->base->transform.position.z += 5;
    collections::CamerasCollection::Instance()->setActiveCamera(0);
    // set elemental  matrices data info ubo
    this->_elementalMatrices->setShaderProgram(shProgram);
    this->_elementalMatrices->setUniformBlockInfo();
    // Add test objects
    this->_sceneObjects->addMesh(StoredMeshes::Torus);
    // Initial Light UBO setup
    this->_lights->setShaderProgram(shProgram);
    this->_lights->setUniformBlockInfo();
    // Add lights to scene objects
    this->_sceneObjects->addLight(scene::Light::Point);
    this->_lights->getLight(0)->base->transform.setPosition(3.0, 0.0, 3.0);
    this->_lights->getLight(0)->setColor(1.0f, 1.0f, 1.0f);
    this->_lights->getLight(0)->intensity = 1.0f;
}

void core::Renderer::loop()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set control variables
    this->_activeCamera = this->_cameras->getActiveCamera();
    // set lights uniform block data
    this->_lights->setUniformBlock();
    // set elemetal matricse with the active camera info, these matrices stay the same for all models
    this->_elementalMatrices->setViewMatrix(this->_activeCamera->getViewMatrix());
    this->_elementalMatrices->setProjectionMatrix(this->_activeCamera->getProjectionTypeMatrix());

    for (unsigned int i = 0; i < this->_meshes->meshCount(); i++) {
        // update the model matrix per model
        this->_elementalMatrices->setModelMatrix(this->_meshes->getMesh(i)->base->transform.getModelMatrix());
        // recalculate modelview, modelviewprojection and normal matrices with the current matrices
        this->_elementalMatrices->calculateMatrices();
        // update matrices uniform block data
        this->_elementalMatrices->setUniformBlock();
        // render the current mesh
        this->_meshes->getMesh(i)->render();
    }
}

void core::Renderer::viewport(const unsigned int &width, const unsigned int &height)
{
    glViewport(0, 0, width, height);
    this->_cameras->getActiveCamera()->setAspectRatio((float)width / (float)height);
}

void core::Renderer::wireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void core::Renderer::filledMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Renderer *core::Renderer::_rdInstance = nullptr;
