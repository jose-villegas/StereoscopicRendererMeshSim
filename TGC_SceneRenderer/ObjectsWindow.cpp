#include "ObjectsWindow.h"
#include "MainWindow.h"

System::Void SceneRenderer::ObjectsWindow::listView1_ItemSelectionChanged(System::Object ^sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs ^e)
{
    selectedIndex = this->listView1->SelectedIndices[0];
    ((MainWindow ^)this->InstancedBy)->GetInspectorWindow()->setActiveObjectIndex((unsigned int)this->objectRealIndexes[selectedIndex]);
}
