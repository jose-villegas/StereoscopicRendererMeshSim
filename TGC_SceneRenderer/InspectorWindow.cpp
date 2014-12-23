#include "InspectorWindow.h"
#include "Collections\SceneObjectsCollection.h"
#include "Scene\SceneObject.h"


System::Void SceneRenderer::InspectorWindow::SetActiveObjectIndex(unsigned int index)
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
}

System::Void SceneRenderer::InspectorWindow::onPositionVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setPosition(this->trnfrControl->Position()->X(), this->trnfrControl->Position()->Y(), this->trnfrControl->Position()->Z());
}

System::Void SceneRenderer::InspectorWindow::onRotationVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setRotation(this->trnfrControl->Rotation()->X(), this->trnfrControl->Rotation()->Y(), this->trnfrControl->Rotation()->Z());
}

System::Void SceneRenderer::InspectorWindow::onScaleVectorChanged(System::Object ^sender, System::EventArgs ^e)
{
    if (changedActiveIndex) { changedActiveIndex = false; return; }

    // Set Transform
    activeSceneObject->getBase()->transform.setScale(this->trnfrControl->Scale()->X(), this->trnfrControl->Scale()->Y(), this->trnfrControl->Scale()->Z());
}
