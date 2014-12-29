#include <windows.h>
#include "PropertiesWindow.h"
#include "Collections\SceneObjectsCollection.h"
#include "Scene\SceneObject.h"
#include "MainWindow.h"

System::Void SceneRenderer::PropertiesWindow::setActiveObjectIndex(unsigned int index)
{
    this->activeObjectIndex = index;
    this->changedActiveIndex = true;
    activeSceneObject = collections::SceneObjectsCollection::Instance()->getSceneObject(index);
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
    // Hide components
    this->lightControl->Visible = false;
    this->cameraControl->Visible = false;

    // Show current object components
    for (it; it != ite; it++) {
        bases::BaseComponent *ptr = *it;

        if (dynamic_cast<scene::Mesh *>(ptr) != 0) {
            // set ptr to component
            this->_meshComponentPtr = (scene::Mesh *)ptr;
        } else if (dynamic_cast<scene::Light *>(ptr) != 0) {
            // set ptr to component
            this->_lightComponentPtr = (scene::Light *)ptr;
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
            this->_cameraComponentPtr = (scene::Camera *)ptr;
            this->cameraControl->Visible = true;
        }
    }
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
    // Disable render loop for color picking dialog
    ((MainWindow ^)this->InstancedBy)->EnableRendering(false);

    if (this->colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        this->lightControl->colorPickerButton->BackColor = this->colorDialog1->Color;

        if (this->_lightComponentPtr) {
            int argb[] = {(int)this->colorDialog1->Color.R, (int)this->colorDialog1->Color.G, (int)this->colorDialog1->Color.B};
            this->_lightComponentPtr->setColor((unsigned int)argb[0], (unsigned int)argb[1], (unsigned int)argb[2]);
        }
    }

    ((MainWindow ^)this->InstancedBy)->EnableRendering(true);
}

System::Void SceneRenderer::PropertiesWindow::onLightAttenuationChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (this->_lightComponentPtr) {
        this->_lightComponentPtr->attenuation = (float)this->lightControl->attenuationValue->Value;
    }
}

System::Void SceneRenderer::PropertiesWindow::onLightIntensityChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (this->_lightComponentPtr) {
        this->_lightComponentPtr->intensity = (float)this->lightControl->intensityValue->Value;
    }
}

System::Void SceneRenderer::PropertiesWindow::onLightOuterAngleChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (this->_lightComponentPtr) {
        this->_lightComponentPtr->outerConeAngle = (float)this->lightControl->outerAngleValue->Value;
    }
}

System::Void SceneRenderer::PropertiesWindow::onLightInnerAngleChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (this->_lightComponentPtr) {
        this->_lightComponentPtr->innerConeAngle = (float)this->lightControl->innerAngleValue->Value;
    }
}
