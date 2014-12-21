#pragma once
#include <windows.h>
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "../core/Renderer.h"
#include "../collections/TexturesCollection.h"
#include "LibInfo.h"
#include "Multisample/arb_multisample.h"
using namespace System::Windows::Forms;

namespace OpenGLForm {
    public ref class COpenGL : public System::Windows::Forms::NativeWindow {
        public:
            COpenGL(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight);
            System::Void restartStopwatch(System::Void);
            System::Void calculateFramerate(System::Boolean blSet) { this->_calcFramerate = blSet; }
            System::Void swapOpenGLBuffers(System::Void) { SwapBuffers(_mHDC); }
            System::Double getDeltaTime() { return _time->deltaTime(); }
            System::Double getTotalTime() { return _time->totalTime(); }
            System::Double getFrameRate() { return _fmCalc->currentFramerate(); }
            virtual System::Void render(System::Void);
            System::String ^OGL_INFO_STRING;
            System::Void clean();
            System::Boolean enableRender;

        private:
            HDC _mHDC;
            HGLRC _mHGLRC;
            PIXELFORMATDESCRIPTOR *pfd;
            System::Diagnostics::Stopwatch _stopwatch;
            utils::Time *_time;
            utils::FrameRate *_fmCalc;
            collections::TexturesCollection *_texCollection;
            System::Boolean _calcFramerate;
            core::Renderer *_oglRender;
            HDC createHandle(System::Windows::Forms::Panel ^parentForm, int iPositionX, int iPositionY, GLsizei iWidth, GLsizei iHeight);

        protected:
            ~COpenGL(System::Void);
            GLint oglSetPixelFormat(HDC hdc);
    };
}