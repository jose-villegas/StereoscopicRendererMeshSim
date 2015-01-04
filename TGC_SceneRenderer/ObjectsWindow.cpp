#include "ObjectsWindow.h"
#include "MainWindow.h"

System::Void SceneRenderer::ObjectsWindow::listView1_ItemSelectionChanged(System::Object ^sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs ^e)
{
    if (e->ItemIndex == selectedIndex) { return; }

    selectedIndex = e->ItemIndex;
    ((MainWindow ^)this->InstancedBy)->GetInspectorWindow()->setActiveObjectIndex((unsigned int)this->objectRealIndexes[selectedIndex]);
}

System::Void SceneRenderer::ObjectsWindow::objectListKeyListener(System::Object ^sender, System::Windows::Forms::KeyEventArgs ^e)
{
    if (e->KeyCode == System::Windows::Forms::Keys::Delete && this->listView1->SelectedIndices[0] != -1) {
        unsigned int realIndex = (unsigned int)this->objectRealIndexes[this->listView1->SelectedIndices[0]];
        collections::SceneObjectsCollection::Instance()->remove(realIndex);  // wrap around int() to create a temporary instance
        this->objectRealIndexes.RemoveAt(this->listView1->SelectedIndices[0]);
        this->listView1->Items->RemoveAt(this->listView1->SelectedIndices[0]);
        ((MainWindow ^)this->InstancedBy)->GetInspectorWindow()->setActiveObjectIndex(-1);
    }
}
