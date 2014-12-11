#pragma once
#include "GL/gl_core_4_4.h"
#include "GL/wglext.h"
#include <windows.h>
#include "../Utils/Logger.h"
#include "../Utils/FrameRate.h"
#include "../Collections/TextureCollection.h"
using namespace System::Windows::Forms;

namespace OpenGLForm {
    public ref class COpenGL : public System::Windows::Forms::NativeWindow {
        public:
            COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight);
            System::Void restartStopwatch(System::Void);
            System::Void calculateFramerate(System::Boolean blSet) { this->_calcFramerate = blSet; }
            System::Void swapOpenGLBuffers(System::Void) { SwapBuffers(_mHDC); }
            System::Double getDeltaTime() { return _deltaTime; }
            System::Double getTotalTime() { return _totalTime; }
            System::Double getFrameRate() { return _framerate; }
            System::Boolean wglIsExtensionSupported(const char *extension);
            virtual System::Void render(System::Void);

        private:
            HDC _mHDC;
            HGLRC _mHGLRC;
            System::Diagnostics::Stopwatch _stopwatch;
            System::Double _deltaTime;
            System::Double _totalTime;
            System::Double _framerate;
            Utils::FrameRate _fmCalc;
            TextureCollection *_texCollection;
            System::Boolean _calcFramerate;

        protected:
            ~COpenGL(System::Void) { this->DestroyHandle(); }
            GLint oglSetPixelFormat(HDC hdc);
    };
}