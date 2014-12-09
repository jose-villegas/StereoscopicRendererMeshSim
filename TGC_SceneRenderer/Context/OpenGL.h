#pragma once
#include <GL/gl3w.h>
#include "../Utils/Logger.h"
#include "../Utils/FrameRate.h"
using namespace System::Windows::Forms;

namespace OpenGLForm {
    public ref class COpenGL : public System::Windows::Forms::NativeWindow {
        public:
            COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight);
            System::Void RestartStopwatch(System::Void);
            System::Void SwapOpenGLBuffers(System::Void) { SwapBuffers(m_hDC); }
            System::Double getDeltaTime() { return deltaTime; }
            System::Double getTotalTime() { return totalTime; }
            System::Double getFrameRate() { return frameRate; }
            virtual System::Void Render(System::Void);

        private:
            HDC m_hDC;
            HGLRC m_hglrc;
            System::Diagnostics::Stopwatch stopWatch;
            System::Double deltaTime;
            System::Double totalTime;
            System::Double frameRate;
            Utils::FrameRate fmCalc;

        protected:
            ~COpenGL(System::Void) { this->DestroyHandle(); }
            GLint oglSetPixelFormat(HDC hdc);
    };
}