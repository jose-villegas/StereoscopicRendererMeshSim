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
#include "../collections/MeshesCollection.h"

using namespace core;

Renderer::Renderer(void)
{
    // Get Tools / Collections Instances
    this->framerate     = utils::FrameRate::Instance();
    this->time          = utils::Time::Instance();
    this->lights        = collections::LightsCollection::Instance();
    this->meshes		= collections::MeshesCollection::Instance();
    this->cameras		= collections::CamerasCollection::Instance();
    this->sceneObjects  = collections::SceneObjectsCollection::Instance();
    this->textures		= collections::TexturesCollection::Instance();
}

Renderer *core::Renderer::Instance()
{
    if (!instance) {
        instance = new Renderer();
    }

    return instance;
}

int core::Renderer::load()
{
    return ogl_LoadFunctions();
}

void core::Renderer::setup()
{
    // Initialize Engine Data
    core::Data::Initialize();
    // Setup OpenGL Flags
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_TEXTURE);
    // Load member classes
    this->matrices = new Matrices();
    // Get a default stored shader
    types::ShaderProgram *shProgram = collections::stored::StoredShaders::getDefaultShader(core::StoredShaders::Diffuse);
    // Initial camera setup
    this->activeCamera = this->sceneObjects->addCamera();
    this->activeCamera->base->transform.position.z += 5;
    collections::CamerasCollection::Instance()->setActiveCamera(0);
    // set elemental  matrices data info ubo
    this->matrices->setShaderProgram(shProgram);
    this->matrices->setUniformBlockInfo();
    // Initial Light UBO setup
    this->lights->setShaderProgram(shProgram);
    this->lights->setUniformBlockInfo();
    // Add lights to scene objects
    this->sceneObjects->addLight(scene::Light::Point);
    this->lights->getLight(0)->base->transform.setPosition(0.0, 0.0, 3.0);
    // Add test objects
    this->sceneObjects->addMesh(StoredMeshes::Sphere);
}

void core::Renderer::loop()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->activeCamera = this->cameras->getActiveCamera();

    if (!this->activeCamera) { return; }

    // call active camera with this class collection
    // camera will draw depending on its parameters
    // and the current loaded data in renderer collections
    this->activeCamera->renderFromPOV(this);
}

void core::Renderer::viewport(const unsigned int &width, const unsigned int &height)
{
    glViewport(0, 0, width, height);
    this->cameras->getActiveCamera()->setAspectRatio((float)width / (float)height);
}

void core::Renderer::polygonModel(Modes mode)
{
    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void core::Renderer::unload()
{
    core::Data::Clear();
    delete this->matrices;
}

core::Renderer::~Renderer()
{
    unload();
}

Renderer *core::Renderer::instance = nullptr;
