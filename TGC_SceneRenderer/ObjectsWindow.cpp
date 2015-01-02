#include "ObjectsWindow.h"
#include "MainWindow.h"

System::Void SceneRenderer::ObjectsWindow::listView1_ItemSelectionChanged(System::Object ^sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs ^e)
{
    if (e->ItemIndex == selectedIndex) { return; }

    selectedIndex = e->ItemIndex;
    ((MainWindow ^)this->InstancedBy)->GetInspectorWindow()->setActiveObjectIndex((unsigned int)this->objectRealIndexes[selectedIndex]);
}
