#include "LoadingWindow.h"
#include "MainWindow.h"


System::Void SceneRenderer::LoadingWindow::LoadingWindow_Paint(System::Object ^sender, System::Windows::Forms::PaintEventArgs ^e)
{
    if (((SceneRenderer::MainWindow ^)this->mainWindow)->renderingEnabled) { this->Close(); }

    this->Refresh();
}
