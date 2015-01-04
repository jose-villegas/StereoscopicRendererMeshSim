#include <windows.h>
#include "PropertiesWindow.h"
#include "Collections\SceneObjectsCollection.h"
#include "utils\ProgressiveMeshes.h"
#include "Scene\SceneObject.h"
#include "MainWindow.h"

System::Void SceneRenderer::PropertiesWindow::setActiveObjectIndex(unsigned int index)
{
    this->activeObjectIndex = index;
    this->changedActiveIndex = true;
    activeSceneObject = collections::SceneObjectsCollection::Instance()->getSceneObject(index);
    // reset components
    this->meshComponentPtr       = nullptr;
    this->lightComponentPtr      = nullptr;
    this->cameraComponentPtr     = nullptr;
    // Hide components
    this->lightControl->Visible  = false;
    this->cameraControl->Visible = false;
    this->meshControl->Visible   = false;

    if (!activeSceneObject) {
        activeSceneObject = nullptr;
        // set empty values
        this->baseControl->objectName->Text = L"Null Selection";
        this->trnfrControl->setValues(0, 0, 0, 0, 0, 0, 0, 0, 0);
        // cancel
        return;
    }

    // Update object name
    this->baseControl->objectName->Text = gcnew System::String(activeSceneObject->getBase()->objectName.c_str());
    // Update Transform accordly
    this->trnfrControl->setValues(
        (double)activeSceneObject->getBase()->transform.position.x,
        (double)activeSceneObject->getBase()->transform.position.y,
        (double)activeSceneObject->getBase()->transform.position.z,
        (double)glm::degrees(activeSceneObject->getBase()->transform.eulerAngles().x),
        (double)glm::degrees(activeSceneObject->getBase()->transform.eulerAngles().y),
        (double)glm::degrees(activeSceneObject->getBase()->transform.eulerAngles().z),
        (double)activeSceneObject->getBase()->transform.scale.x,
        (double)activeSceneObject->getBase()->transform.scale.y,
        (double)activeSceneObject->getBase()->transform.scale.z
    );
    this->trnfrControl->Refresh();
    // Set up components user controls
    std::vector<bases::BaseComponent *>::const_iterator it = activeSceneObject->getComponents().begin();
    std::vector<bases::BaseComponent *>::const_iterator ite = activeSceneObject->getComponents().end();

    // Show current object components
    for (it; it != ite; it++) {
        bases::BaseComponent *ptr = *it;

        if (dynamic_cast<scene::Mesh *>(ptr) != 0) {
            // set ptr to component
            this->meshComponentPtr = (scene::Mesh *)ptr;
            this->meshControl->Visible = true;
            unsigned int polyCount = this->meshComponentPtr->isMeshReductionEnabled() ? this->meshComponentPtr->getMeshReductor()->getActualPolyCount() : this->meshComponentPtr->getPolyCount();
            unsigned int vertexCount = this->meshComponentPtr->isMeshReductionEnabled() ? this->meshComponentPtr->getMeshReductor()->getActualVertexCount() : this->meshComponentPtr->getVertexCount();
            this->meshControl->setValues(true, this->meshComponentPtr->isMeshReductionEnabled(), polyCount, vertexCount, this->meshComponentPtr->getSubmeshesCount());
        } else if (dynamic_cast<scene::Light *>(ptr) != 0) {
            // set ptr to component
            this->lightComponentPtr = (scene::Light *)ptr;
            this->lightControl->Visible = true;
            // Create a window managed color from vector values
            System::Drawing::Color wLightColor;
            wLightColor = System::Drawing::Color::FromArgb(
                              (int)(((scene::Light *)ptr)->color.r * 255.f),
                              (int)(((scene::Light *)ptr)->color.g * 255.f),
                              (int)(((scene::Light *)ptr)->color.b * 255.f)
                          );
            // set current values to the user control
            this->lightControl->SetValues(
                wLightColor,
                ((scene::Light *)ptr)->intensity,
                ((scene::Light *)ptr)->attenuation,
                ((scene::Light *)ptr)->outerConeAngle,
                ((scene::Light *)ptr)->innerConeAngle,
                ((scene::Light *)ptr)->lightType
            );
        } else if (dynamic_cast<scene::Camera *>(ptr) != 0) {
            // set ptr to component
            this->cameraComponentPtr = (scene::Camera *)ptr;
            this->cameraControl->Visible = true;
            this->cameraControl->setValues(cameraComponentPtr->getNearClippingPlane().distance,
                                           cameraComponentPtr->getFarClippingPlane().distance,
                                           cameraComponentPtr->getFieldOfView(),
                                           cameraComponentPtr->getOrthoProjectionSize(),
                                           cameraComponentPtr->getZeroParallax(),
                                           cameraComponentPtr->getEyeSeparation(),
                                           (unsigned int)cameraComponentPtr->projectionType);
            this->cameraControl->setUpVector(cameraComponentPtr->getVectorUp().x,
                                             cameraComponentPtr->getVectorUp().y,
                                             cameraComponentPtr->getVectorUp().z);
        }
    }

    this->lightControl->Refresh();
    this->cameraControl->Refresh();
    this->meshControl->Refresh();
}

System::Void SceneRenderer::PropertiesWindow::onPositionVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !activeSceneObject) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setPosition(
        (float)this->trnfrControl->Position()->X(),
        (float)this->trnfrControl->Position()->Y(),
        (float)this->trnfrControl->Position()->Z()
    );
}

System::Void SceneRenderer::PropertiesWindow::onRotationVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !activeSceneObject) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setRotation(
        (float)glm::radians(this->trnfrControl->Rotation()->X()),
        (float)glm::radians(this->trnfrControl->Rotation()->Y()),
        (float)glm::radians(this->trnfrControl->Rotation()->Z())
    );
}

System::Void SceneRenderer::PropertiesWindow::onScaleVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !activeSceneObject) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setScale(
        (float)this->trnfrControl->Scale()->X(),
        (float)this->trnfrControl->Scale()->Y(),
        (float)this->trnfrControl->Scale()->Z()
    );
}

System::Void SceneRenderer::PropertiesWindow::pickColorDialogPopup(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->lightControl) { return; }

    // Disable render loop for color picking dialog
    ((MainWindow ^)this->InstancedBy)->EnableRendering(false);

    if (this->colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        this->lightControl->colorPickerButton->BackColor = this->colorDialog1->Color;

        if (this->lightComponentPtr) {
            int argb[] = {(int)this->colorDialog1->Color.R, (int)this->colorDialog1->Color.G, (int)this->colorDialog1->Color.B};
            this->lightComponentPtr->setColor((unsigned int)argb[0], (unsigned int)argb[1], (unsigned int)argb[2]);
        }
    }

    ((MainWindow ^)this->InstancedBy)->EnableRendering(true);
}

System::Void SceneRenderer::PropertiesWindow::onLightAttenuationChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->lightComponentPtr) { return; }

    this->lightComponentPtr->attenuation = (float)this->lightControl->attenuationValue->Value;
}

System::Void SceneRenderer::PropertiesWindow::onLightIntensityChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->lightComponentPtr) { return; }

    this->lightComponentPtr->intensity = (float)this->lightControl->intensityValue->Value;
}

System::Void SceneRenderer::PropertiesWindow::onLightOuterAngleChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->lightComponentPtr) { return; }

    this->lightComponentPtr->outerConeAngle = (float)this->lightControl->outerAngleValue->Value;
}

System::Void SceneRenderer::PropertiesWindow::onLightInnerAngleChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->lightComponentPtr) { return; }

    this->lightComponentPtr->innerConeAngle = (float)this->lightControl->innerAngleValue->Value;
}

System::Void SceneRenderer::PropertiesWindow::onSimplMeshCheckChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->meshComponentPtr) { return; }

    if (this->meshComponentPtr->isMeshReductionEnabled()) { return; }

    this->meshComponentPtr->enableMeshReduction();
}

System::Void SceneRenderer::PropertiesWindow::onVertexCountNumericChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->meshComponentPtr) { return; }

    if (!this->meshComponentPtr->isMeshReductionEnabled()) { return; }

    this->meshComponentPtr->getMeshReductor()->reduce((unsigned int)this->meshControl->vertexNumeric->Value);
    this->meshControl->getPolyNumeric()->Value = System::Decimal(this->meshComponentPtr->getMeshReductor()->getActualPolyCount());
}

System::Void SceneRenderer::PropertiesWindow::onEyeSeparationChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->cameraComponentPtr) { return; }

    this->cameraComponentPtr->setEyeSeparation((float)this->cameraControl->separationValue->Value);
}

System::Void SceneRenderer::PropertiesWindow::onFieldOfViewChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->cameraComponentPtr) { return; }

    this->cameraComponentPtr->setFieldOfView((float)this->cameraControl->fovValue->Value);
}

System::Void SceneRenderer::PropertiesWindow::onProjectionTypeChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->cameraComponentPtr) { return; }

    scene::Camera::TypeProjection typeProj = scene::Camera::Orthographic;
    unsigned int projectionType = cameraControl->getProjectionType();

    switch (projectionType) {
        case 0: typeProj = scene::Camera::Perspective; break;

        case 1: typeProj = scene::Camera::Orthographic; break;

        case 2: typeProj = scene::Camera::Stereoscopic; break;

        default:
            break;
    }

    this->cameraComponentPtr->projectionType = typeProj;
}

System::Void SceneRenderer::PropertiesWindow::onNearDistanceChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->cameraComponentPtr) { return; }

    cameraComponentPtr->setNearClippingDistance((float)this->cameraControl->nearDistanceNumeric->Value);
}

System::Void SceneRenderer::PropertiesWindow::onFarDistanceChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->cameraComponentPtr) { return; }

    cameraComponentPtr->setFarClippingDistance((float)this->cameraControl->farDistanceNumeric->Value);
}

System::Void SceneRenderer::PropertiesWindow::onOrthoSizeChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->cameraComponentPtr) { return; }

    cameraComponentPtr->setOrthoProjectionSize((float)this->cameraControl->orthoSizeValue->Value);
}

System::Void SceneRenderer::PropertiesWindow::onZeroParallaxChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !this->cameraComponentPtr) { return; }

    cameraComponentPtr->setZeroParallax((float)this->cameraControl->zpValue->Value);
}
