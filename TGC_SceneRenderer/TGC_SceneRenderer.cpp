// TGC_SceneRenderer.cpp : main project file.

#include "MainWindow.h"

using namespace TGC_SceneRenderer;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    // Enabling Windows XP visual effects before any controls are created
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    // Create a Console for the Program and redirect stdout to it
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    // Create the main window and run it
    MainWindow ^mnWindow = gcnew MainWindow();
    Application::Run(mnWindow);
    return 0;
}
