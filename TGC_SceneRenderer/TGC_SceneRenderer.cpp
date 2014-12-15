// TGC_SceneRenderer.cpp : main project file.

#include "MainWindow.h"

using namespace TGC_SceneRenderer;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    // Enabling Windows XP visual effects before any controls are created
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    // Redirect cout stream to file
    std::ofstream out("utils_logger.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    // Create the main window and run it
    MainWindow ^mnWindow = gcnew MainWindow();
    Application::Run(mnWindow);
    return 0;
}
