#include <windows.h>
#include "PropertiesWindow.h"
#include "Collections\SceneObjectsCollection.h"
#include "Scene\SceneObject.h"
#include "MainWindow.h"

const double PI  = 3.141592653589793238463;

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / PI)

System::Void SceneRenderer::PropertiesWindow::setActiveObjectIndex(unsigned int index)
{
    this->activeObjectIndex = index;
    this->changedActiveIndex = true;
    activeSceneObject = collections::SceneObjectsCollection::Instance()->getSceneObject(index);
    // Update object name
    this->baseControl->objectName->Text = gcnew System::String(activeSceneObject->getBase()->objectName.c_str());
    // Update Transform accordly
    this->trnfrControl->Position(
        activeSceneObject->getBase()->transform.position.x,
        activeSceneObject->getBase()->transform.position.y,
        activeSceneObject->getBase()->transform.position.z
    );
    this->trnfrControl->Rotation(
        activeSceneObject->getBase()->transform.eulerAngles().x,
        activeSceneObject->getBase()->transform.eulerAngles().y,
        activeSceneObject->getBase()->transform.eulerAngles().z
    );
    this->trnfrControl->Scale(
        activeSceneObject->getBase()->transform.scale.x,
        activeSceneObject->getBase()->transform.scale.y,
        activeSceneObject->getBase()->transform.scale.z
    );
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
                              ((scene::Light *)ptr)->color.r * 255,
                              ((scene::Light *)ptr)->color.g * 255,
                              ((scene::Light *)ptr)->color.b * 255
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
        this->trnfrControl->Position()->X(),
        this->trnfrControl->Position()->Y(),
        this->trnfrControl->Position()->Z()
    );
}

System::Void SceneRenderer::PropertiesWindow::onRotationVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !activeSceneObject) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setRotation(
        degreesToRadians(this->trnfrControl->Rotation()->X()),
        degreesToRadians(this->trnfrControl->Rotation()->Y()),
        degreesToRadians(this->trnfrControl->Rotation()->Z())
    );
}

System::Void SceneRenderer::PropertiesWindow::onScaleVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex || !activeSceneObject) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setScale(
        this->trnfrControl->Scale()->X(),
        this->trnfrControl->Scale()->Y(),
        this->trnfrControl->Scale()->Z()
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
