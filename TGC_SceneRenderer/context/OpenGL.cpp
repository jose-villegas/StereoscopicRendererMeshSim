#include "OpenGL.h"

OGLContext::COpenGL::COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight)
{
    enableRender = true;
    // Create OGL Context
    mHDC = createHandle(parentForm, iPositionX, iPositionY, iWidth, iHeight);

    if (mHDC) {
        oglSetPixelFormat(mHDC);
        HINSTANCE hInstance = (HINSTANCE)GetWindowLong((HWND)this->Handle.ToPointer(), GWL_HINSTANCE);

        // Query Multisample Support
        if (!arbMultisampleSupported && CHECK_FOR_MULTISAMPLE) {
            if (InitMultisample(hInstance, (HWND)this->Handle.ToPointer(), *pfd)) {
                // Destroy Temporary Context
                DestroyWindow((HWND)this->Handle.ToPointer());
                // Create new Context with MultiSample Support
                mHDC = createHandle(parentForm, iPositionX, iPositionY, iWidth, iHeight);;
                oglSetPixelFormat(mHDC);
            }
        }
    }

    // Create Renderer (Core::Renderer is in charge of rendering all scene objects and misc)
    oglRender = core::Renderer::Instance();

    if (!oglRender->load()) {
        std::cout << "Failed to initialize OpenGL" << std::endl;
        delete oglRender;
        enableRender = false;
        oglRender = nullptr;
    }

    // Write Library Loading / Current Instance Info
    LibInfo::Write();
    // Setup render instance
    oglRender->setup();
    oglRender->viewport(parentForm->Width, parentForm->Height);
    // Query Current OGL Context Info
    OGL_INFO_STRING = LibInfo::OGL_INFO_STRING;
    // Other Class Variables Instancing
    fmCalc = utils::FrameRate::Instance();
    time = utils::Time::Instance();
    calcFramerate = true; // Default Don't
}

System::Void OGLContext::COpenGL::restartStopwatch(System::Void)
{
    this->time->deltaTime = stopwatch.Elapsed.TotalSeconds;
    this->time->totalTime += this->time->deltaTime;

    if (calcFramerate) { fmCalc->calculate(this->time->deltaTime); }

    stopwatch.Restart();
}

System::Void OGLContext::COpenGL::render(System::Void)
{
    if (enableRender) { oglRender->loop(); }
}

GLint OGLContext::COpenGL::oglSetPixelFormat(HDC hdc)
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

    if ((mHGLRC = wglCreateContext(mHDC)) == NULL) {
        std::cout << "wglCreateContext Failed" << std::endl;
        return 0;
    }

    if ((wglMakeCurrent(mHDC, mHGLRC)) == NULL) {
        std::cout << "wglMakeCurrent Failed" << std::endl;
        return 0;
    }

    return 1;
}

HDC OGLContext::COpenGL::createHandle(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth,
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

OGLContext::COpenGL::~COpenGL(System::Void)
{
    enableRender = false;
    delete oglRender;
    this->DestroyHandle();
}

System::Void OGLContext::COpenGL::clean()
{
}

void OGLContext::COpenGL::resizeRenderingViewPort(System::Int32 width, System::Int32 height)
{
    // Resize context handler
    SetWindowPos((HWND)this->Handle.ToPointer(), HWND_BOTTOM, 0, 0, width, height, SWP_NOMOVE);
    // Resize rendering viewport
    this->oglRender->viewport(int(width), int(height));
}

void OGLContext::COpenGL::renderMode(RenderMode mode)
{
    if (!oglRender) { return; }

    switch (mode) {
        case OGLContext::Points:
            this->oglRender->polygonModel(core::Renderer::Points);
            break;

        case OGLContext::Wireframe:
            this->oglRender->polygonModel(core::Renderer::Wireframe);
            break;

        case OGLContext::Textured:
            this->oglRender->polygonModel(core::Renderer::Textured);
            break;

        default:
            break;
    }
}

