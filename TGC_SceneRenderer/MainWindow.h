#pragma once
#include "Context/OpenGL.h"

namespace TGC_SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace OpenGLForm;

    /// <summary>
    /// Summary for Form1
    /// </summary>
    public ref class MainWindow : public System::Windows::Forms::Form {
        public:
            MainWindow(void)
            {
                InitializeComponent();
                //
                //TODO: Add the constructor code here
                OpenGL = gcnew COpenGL(this->OpenGLRenderPanel, 0, 0,
                                       this->OpenGLRenderPanel->ClientSize.Width,
                                       this->OpenGLRenderPanel->ClientSize.Height);
                //
            }

        protected:
            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            ~MainWindow()
            {
                if (components) {
                    delete components;
                }
            }

        private:
            /// <summary>
            /// Required designer variable.
            /// </summary>
            OpenGLForm::COpenGL ^OpenGL;
        private: System::Windows::Forms::Panel  ^OpenGLRenderPanel;




            System::ComponentModel::Container ^components;

            #pragma region Windows Form Designer generated code
            /// <summary>
            /// Required method for Designer support - do not modify
            /// the contents of this method with the code editor.
            /// </summary>
            void InitializeComponent(void)
            {
                this->OpenGLRenderPanel = (gcnew System::Windows::Forms::Panel());
                this->SuspendLayout();
                //
                // panel1
                //
                this->OpenGLRenderPanel->Location = System::Drawing::Point(454, 172);
                this->OpenGLRenderPanel->Name = L"panel1";
                this->OpenGLRenderPanel->Size = System::Drawing::Size(200, 100);
                this->OpenGLRenderPanel->TabIndex = 0;
                //
                // MainWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(1226, 565);
                this->Controls->Add(this->OpenGLRenderPanel);
                this->Name = L"MainWindow";
                this->Text = L"TCG - SCENE";
                this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::timer1_Tick);
                this->ResumeLayout(false);
            }
            #pragma endregion
        private: System::Void timer1_Tick(System::Object  ^sender, System::Windows::Forms::PaintEventArgs  ^e)
            {
                UNREFERENCED_PARAMETER(sender);
                UNREFERENCED_PARAMETER(e);
                OpenGL->Render();
                OpenGL->SwapOpenGLBuffers();
            }
    };
}

