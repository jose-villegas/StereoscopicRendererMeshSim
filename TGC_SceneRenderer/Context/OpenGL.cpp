#include "OpenGL.h"


#define DRAW_TEST_TRIANGLE

#ifdef DRAW_TEST_TRIANGLE
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

GLuint vArrayID;
GLuint vBuffer;
#endif // DRAW_TEST_TRIANGLE

OpenGLForm::COpenGL::COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight)
{
    // Other Class Variables
    _calcFramerate = false;
    _texCollection->Instance();
    // Create OGL Context
    _mHDC = createHandle(parentForm, iPositionX, iPositionY, iWidth, iHeight);

    if (_mHDC) {
        oglSetPixelFormat(_mHDC);
        HINSTANCE hInstance = (HINSTANCE)GetWindowLong((HWND)this->Handle.ToPointer(), GWL_HINSTANCE);

        // Check For Multisample Support
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

    if (!ogl_LoadFunctions()) {
        Utils::Logger::Write("Failed to initialize OpenGL", true, System::Drawing::Color::Red);
    }

    // Query Multisample Support
    // Setup OpenGL
    glEnable(GL_DEPTH_TEST);
    // Write Sucessfull Core Load
    System::String ^glVersionString = gcnew System::String((char *)glGetString(GL_VERSION));
    System::String ^glslVersionString = gcnew System::String((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    System::String ^oglString = L"OpenGL " + glVersionString + "s, GLSL " + glslVersionString + "\n";
    Utils::Logger::Write(oglString, true, System::Drawing::Color::Green);
    // Triangle Render Test
    glGenVertexArrays(1, &vArrayID);
    glBindVertexArray(vArrayID);
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
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
#ifdef DRAW_TEST_TRIANGLE
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void *)0           // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
#endif
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
            Utils::Logger::Write("ChoosePixelFormat Failed", true, System::Drawing::Color::Red);
            return 0;
        }
    } else {
        iPixelFormat = arbMultisampleFormat;
    }

    // make that match the device context's current pixel format
    if (SetPixelFormat(hdc, iPixelFormat, pfd) == FALSE) {
        Utils::Logger::Write("SetPixelFormat Failed", true, System::Drawing::Color::Red);
        return 0;
    }

    if ((_mHGLRC = wglCreateContext(_mHDC)) == NULL) {
        Utils::Logger::Write("wglCreateContext Failed", true, System::Drawing::Color::Red);
        return 0;
    }

    if ((wglMakeCurrent(_mHDC, _mHGLRC)) == NULL) {
        Utils::Logger::Write("wglMakeCurrent Failed", true, System::Drawing::Color::Red);
        return 0;
    }

    Utils::Logger::Write("OpenGL Context Created Successfully", true, System::Drawing::Color::Green);
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
