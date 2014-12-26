// TGC_SceneRenderer.cpp : main project file.

#include "MainWindow.h"

using namespace SceneRenderer;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    // Enabling Windows XP visual effects before any controls are created
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    // Create a Console for the Program and redirect stdout to it
    AllocConsole();
    FILE *stdoutStream;
    freopen_s(&stdoutStream, "CONOUT$", "w", stdout);
    // Create the main window and run it
    MainWindow ^mnWindow = gcnew MainWindow();
    Application::Run(mnWindow);
    // Close stream
    fclose(stdoutStream);
    // Exist success
    return 0;
}