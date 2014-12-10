#include "OpenGL.h"

OpenGLForm::COpenGL::COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight)
{
    CreateParams ^cp = gcnew CreateParams;
    // Set the position on the form
    cp->X = iPositionX;
    cp->Y = iPositionY;
    cp->Height = iHeight;
    cp->Width = iWidth;
    // Specify the form as the parent.
    cp->Parent = parentForm->Handle;
    // Create as a child of the specified parent and make OpenGL compliant (no clipping)
    cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    // Create the actual window
    this->CreateHandle(cp);
    _mHDC = GetDC((HWND)this->Handle.ToPointer());

    if (_mHDC) {
        oglSetPixelFormat(_mHDC);
    }

    if (gl3wInit()) {
        MessageBox::Show("Failed to initialize OpenGL");
    }

    // Min Version
    if (!gl3wIsSupported(3, 2)) {
        MessageBox::Show("OpenGL 3.2 not supported");
    }

    System::String ^glVersionString = gcnew System::String((char *)glGetString(GL_VERSION));
    System::String ^glslVersionString = gcnew System::String((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    System::String ^oglString = L"OpenGL " + glVersionString + "s, GLSL " + glslVersionString + "\n";
    Utils::Logger::Write(oglString, true, System::Drawing::Color::Green);
    // Other Class Variables
    _calcFramerate = false;
    _texCollection->Inst();
}

System::Void OpenGLForm::COpenGL::restartStopwatch(System::Void)
{
    _deltaTime = _stopwatch.Elapsed.TotalSeconds;
    _totalTime += _deltaTime;
    _stopwatch.Restart();

    if (_calcFramerate) { _framerate = _fmCalc.Calculate(_deltaTime); }
}

System::Void OpenGLForm::COpenGL::render(System::Void)
{
    // Clear the color and depth buffers.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLint OpenGLForm::COpenGL::oglSetPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd
        1,                                // version number
        PFD_DRAW_TO_WINDOW |              // support window
        PFD_SUPPORT_OPENGL |              // support OpenGL
        PFD_DOUBLEBUFFER,                 // double buffered
        PFD_TYPE_RGBA,                    // RGBA type
        24,                               // 24-bit color depth
        0, 0, 0, 0, 0, 0,                 // color bits ignored
        0,                                // no alpha buffer
        0,                                // shift bit ignored
        0,                                // no accumulation buffer
        0, 0, 0, 0,                       // accum bits ignored
        32,                               // 32-bit z-buffer
        0,                                // no stencil buffer
        0,                                // no auxiliary buffer
        PFD_MAIN_PLANE,                   // main layer
        0,                                // reserved
        0, 0, 0                           // layer masks ignored
    };
    GLint  iPixelFormat;

    // get the device context's best, available pixel format match
    if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0) {
        MessageBox::Show("ChoosePixelFormat Failed");
        return 0;
    }

    // make that match the device context's current pixel format
    if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE) {
        MessageBox::Show("SetPixelFormat Failed");
        return 0;
    }

    if ((_mHGLRC = wglCreateContext(_mHDC)) == NULL) {
        MessageBox::Show("wglCreateContext Failed");
        return 0;
    }

    if ((wglMakeCurrent(_mHDC, _mHGLRC)) == NULL) {
        MessageBox::Show("wglMakeCurrent Failed");
        return 0;
    }

    return 1;
}
