#pragma once
#include <windows.h>
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "../core/Renderer.h"
#include "../collections/TexturesCollection.h"
#include "LibInfo.h"
#include "Multisample/arb_multisample.h"
using namespace System::Windows::Forms;

namespace OGLContext {

    enum RenderMode {
        Points,
        Wireframe,
        Textured
    };

    public ref class COpenGL : public System::Windows::Forms::NativeWindow {
        public:

            COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight);
            System::Void restartStopwatch(System::Void);
            System::Void calculateFramerate(System::Boolean blSet) { this->calcFramerate = blSet; }
            System::Void swapOpenGLBuffers(System::Void) { SwapBuffers(mHDC); }
            System::Double getDeltaTime() { return time->deltaTime; }
            System::Double getTotalTime() { return time->totalTime; }
            System::Double getFrameRate() { return fmCalc->getCurrentFramerate(); }
            virtual System::Void render(System::Void);
            System::String ^OGL_INFO_STRING;
            System::Void clean();
            System::Boolean enableRender;

            void resizeRenderingViewPort(System::Int32 width, System::Int32 height);
            void renderMode(RenderMode mode);

        private:
            HDC mHDC;
            HGLRC mHGLRC;
            PIXELFORMATDESCRIPTOR *pfd;
            System::Diagnostics::Stopwatch stopwatch;
            utils::Time *time;
            utils::FrameRate *fmCalc;
            System::Boolean calcFramerate;
            core::Renderer *oglRender;
            HDC createHandle(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight);

        protected:
            ~COpenGL(System::Void);
            GLint oglSetPixelFormat(HDC hdc);
    };
}