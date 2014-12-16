#include "OpenGL.h"


OpenGLForm::COpenGL::COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight)
{
    // Create OGL Context
    _mHDC = createHandle(parentForm, iPositionX, iPositionY, iWidth, iHeight);

    if (_mHDC) {
        oglSetPixelFormat(_mHDC);
        HINSTANCE hInstance = (HINSTANCE)GetWindowLong((HWND)this->Handle.ToPointer(), GWL_HINSTANCE);

        // Query Multisample Support
        if (!arbMultisampleSupported && CHECK_FOR_MULTISAMPLE) {
            if (InitMultisample(hInstance, (HWND)this->Handle.ToPointer(), *pfd)) {
                // Destroy Temporary Context
                DestroyWindow((HWND)this->Handle.ToPointer());
                // Create new Context with MultiSample Support
                _mHDC = createHandle(parentForm, iPositionX, iPositionY, iWidth, iHeight);;
                oglSetPixelFormat(_mHDC);
            }
        }
    }

    // Create Renderer (Core::Renderer is in charge of rendering all scene objects and misc)
    _oglRender = new Core::Renderer();

    if (!_oglRender->load()) {
        std::cout << "Failed to initialize OpenGL" << std::endl;
    }

    // Write Library Loading / Current Instance Info
    LibInfo::Write();
    // Setup OGL Flags
    _oglRender->setup();
    // Query Current OGL Context Info
    OGL_INFO_STRING = LibInfo::OGL_INFO_STRING;
    // Other Class Variables Instancing
    _fmCalc = Utils::FrameRate::Instance();
    _time = Utils::Time::Instance();
    _texCollection = ECollections::Textures::Instance();
    _calcFramerate = true; // Default Don't
}

System::Void OpenGLForm::COpenGL::restartStopwatch(System::Void)
{
    this->_time->deltaTime(_stopwatch.Elapsed.TotalSeconds);
    this->_time->totalTime(this->_time->totalTime() + this->_time->deltaTime());
    _stopwatch.Restart();

    if (_calcFramerate) { _fmCalc->calculate(this->_time->deltaTime()); }
}

System::Void OpenGLForm::COpenGL::render(System::Void)
{
    _oglRender->loop();
}

GLint OpenGLForm::COpenGL::oglSetPixelFormat(HDC hdc)
{
    if (!pfd) {
        PIXELFORMATDESCRIPTOR temporalPFD = {
            sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd
            1,                                // version number
            PFD_DRAW_TO_WINDOW |              // support window
            PFD_SUPPORT_OPENGL |              // support OpenGL
            PFD_DOUBLEBUFFER,                 // double buffered
            PFD_TYPE_RGBA,                    // RGBA type
            32,                               // 32-bit color depth
            0, 0, 0, 0, 0, 0,                 // color bits ignored
            0,                                // no alpha buffer
            0,                                // shift bit ignored
            0,                                // no accumulation buffer
            0, 0, 0, 0,                       // accum bits ignored
            32,                               // 32-bit z-buffer
            8,                                // 8-bit stencil buffer
            0,                                // no auxiliary buffer
            PFD_MAIN_PLANE,                   // main layer
            0,                                // reserved
            0, 0, 0                           // layer masks ignored
        };
        pfd = new PIXELFORMATDESCRIPTOR(temporalPFD);
    }

    GLint  iPixelFormat;

    // First Pass Before Querying Multisample support
    // so it actually creates a temporary OGL Context
    if (!arbMultisampleSupported) {
        // get the device context's best, available pixel format match
        if ((iPixelFormat = ChoosePixelFormat(hdc, pfd)) == 0) {
            std::cout << "ChoosePixelFormat Failed" << std::endl;
            return 0;
        }
    } else {
        iPixelFormat = arbMultisampleFormat;
    }

    // make that match the device context's current pixel format
    if (SetPixelFormat(hdc, iPixelFormat, pfd) == FALSE) {
        std::cout << "SetPixelFormat Failed" << std::endl;
        return 0;
    }

    if ((_mHGLRC = wglCreateContext(_mHDC)) == NULL) {
        std::cout << "wglCreateContext Failed" << std::endl;
        return 0;
    }

    if ((wglMakeCurrent(_mHDC, _mHGLRC)) == NULL) {
        std::cout << "wglMakeCurrent Failed" << std::endl;
        return 0;
    }

    return 1;
}

HDC OpenGLForm::COpenGL::createHandle(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth,
                                      GLsizei iHeight)
{
    // Create Windows Handler Again But Now With MultiSample Enabled
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
    // Return Created Handle
    return GetDC((HWND)this->Handle.ToPointer());
}

OpenGLForm::COpenGL::~COpenGL(System::Void)
{
    _texCollection->unloadAllTextures();
    delete _texCollection;
    delete _fmCalc;
    delete _time;
    this->DestroyHandle();
}
