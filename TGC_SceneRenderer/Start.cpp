// TGC_SceneRenderer.cpp : main project file.

#include "MainWindow.h"

// #define COUT_TO_FILE

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
#ifdef COUT_TO_FILE
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
#endif
    // Create the main window and run it
    MainWindow ^mnWindow = gcnew MainWindow();
    Application::Run(mnWindow);
    // Close stream
    fclose(stdoutStream);
    // Exist success
    return 0;
}