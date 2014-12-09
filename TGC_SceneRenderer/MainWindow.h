#pragma once
#include "Context/OpenGL.h"
#include "Utils/Logger.h"

namespace TGC_SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace OpenGLForm;
    using namespace Utils;

    /// <summary>
    /// Summary for Form1
    /// </summary>
    public ref class MainWindow : public System::Windows::Forms::Form {
        public:
            MainWindow(void)
            {
                InitializeComponent();
                // Pass Tool Label To Logger
                Utils::Logger::SetStatusLabel(this->StatusStripLabel);
                // Setup OpenGL Render Context Inside a Panel
                System::Windows::Forms::Panel ^oglRenderPanel = this->OpenGLRenderPanel;
                OpenGL = gcnew COpenGL(oglRenderPanel, 0, 0, oglRenderPanel->ClientSize.Width, oglRenderPanel->ClientSize.Height);
                // Add OpenGL Info to Form Title
                this->Text += " (" + Utils::Logger::GetLastLog() + ")";
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
        private: OpenGLForm::COpenGL ^OpenGL;
        private: System::Windows::Forms::Panel  ^OpenGLRenderPanel;
        private: System::Windows::Forms::MenuStrip  ^topMenuBar;



        private: System::Windows::Forms::ToolStripMenuItem  ^fileToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^newSceneToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^openSceneToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^saveSceneToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^exitToolStripMenuItem;
        private: System::Windows::Forms::ToolStripSeparator  ^toolStripSeparator1;
        private: System::Windows::Forms::ToolStripMenuItem  ^saveSceneAsToolStripMenuItem;
        private: System::Windows::Forms::ToolStripSeparator  ^toolStripSeparator2;
        private: System::Windows::Forms::ToolStripMenuItem  ^editToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^modelsToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^importModelToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^sceneObjectToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^createEmptyToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^windowToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^aboutToolStripMenuItem;
        private: System::Windows::Forms::OpenFileDialog  ^assetImportFileDialog;
        private: System::Windows::Forms::StatusStrip  ^StatusStrip;

        private: System::Windows::Forms::ToolStripStatusLabel  ^StatusStripLabel;
        private: System::Windows::Forms::ToolStripMenuItem  ^consoleToolStripMenuItem;









            System::ComponentModel::Container ^components;

            #pragma region Windows Form Designer generated code
            /// <summary>
            /// Required method for Designer support - do not modify
            /// the contents of this method with the code editor.
            /// </summary>
            void InitializeComponent(void)
            {
                this->OpenGLRenderPanel = (gcnew System::Windows::Forms::Panel());
                this->topMenuBar = (gcnew System::Windows::Forms::MenuStrip());
                this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->newSceneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->openSceneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
                this->saveSceneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->saveSceneAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
                this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->modelsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->importModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->sceneObjectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->createEmptyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->windowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->consoleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->assetImportFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
                this->StatusStrip = (gcnew System::Windows::Forms::StatusStrip());
                this->StatusStripLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
                this->topMenuBar->SuspendLayout();
                this->StatusStrip->SuspendLayout();
                this->SuspendLayout();
                //
                // OpenGLRenderPanel
                //
                this->OpenGLRenderPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                this->OpenGLRenderPanel->Location = System::Drawing::Point(0, 24);
                this->OpenGLRenderPanel->Name = L"OpenGLRenderPanel";
                this->OpenGLRenderPanel->Size = System::Drawing::Size(1226, 541);
                this->OpenGLRenderPanel->TabIndex = 0;
                this->OpenGLRenderPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::timer1_Tick);
                //
                // topMenuBar
                //
                this->topMenuBar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(6) {
                    this->fileToolStripMenuItem,
                         this->editToolStripMenuItem, this->modelsToolStripMenuItem, this->sceneObjectToolStripMenuItem, this->windowToolStripMenuItem,
                         this->aboutToolStripMenuItem
                });
                this->topMenuBar->Location = System::Drawing::Point(0, 0);
                this->topMenuBar->Name = L"topMenuBar";
                this->topMenuBar->Size = System::Drawing::Size(1226, 24);
                this->topMenuBar->TabIndex = 1;
                this->topMenuBar->Text = L"topMenu";
                //
                // fileToolStripMenuItem
                //
                this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(7) {
                    this->newSceneToolStripMenuItem,
                         this->openSceneToolStripMenuItem, this->toolStripSeparator1, this->saveSceneToolStripMenuItem, this->saveSceneAsToolStripMenuItem,
                         this->toolStripSeparator2, this->exitToolStripMenuItem
                });
                this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
                this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
                this->fileToolStripMenuItem->Text = L"File";
                //
                // newSceneToolStripMenuItem
                //
                this->newSceneToolStripMenuItem->Name = L"newSceneToolStripMenuItem";
                this->newSceneToolStripMenuItem->Size = System::Drawing::Size(157, 22);
                this->newSceneToolStripMenuItem->Text = L"New Scene";
                //
                // openSceneToolStripMenuItem
                //
                this->openSceneToolStripMenuItem->Name = L"openSceneToolStripMenuItem";
                this->openSceneToolStripMenuItem->Size = System::Drawing::Size(157, 22);
                this->openSceneToolStripMenuItem->Text = L"Open Scene";
                //
                // toolStripSeparator1
                //
                this->toolStripSeparator1->Name = L"toolStripSeparator1";
                this->toolStripSeparator1->Size = System::Drawing::Size(154, 6);
                //
                // saveSceneToolStripMenuItem
                //
                this->saveSceneToolStripMenuItem->Name = L"saveSceneToolStripMenuItem";
                this->saveSceneToolStripMenuItem->Size = System::Drawing::Size(157, 22);
                this->saveSceneToolStripMenuItem->Text = L"Save Scene";
                //
                // saveSceneAsToolStripMenuItem
                //
                this->saveSceneAsToolStripMenuItem->Name = L"saveSceneAsToolStripMenuItem";
                this->saveSceneAsToolStripMenuItem->Size = System::Drawing::Size(157, 22);
                this->saveSceneAsToolStripMenuItem->Text = L"Save Scene As...";
                //
                // toolStripSeparator2
                //
                this->toolStripSeparator2->Name = L"toolStripSeparator2";
                this->toolStripSeparator2->Size = System::Drawing::Size(154, 6);
                //
                // exitToolStripMenuItem
                //
                this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
                this->exitToolStripMenuItem->ShowShortcutKeys = false;
                this->exitToolStripMenuItem->Size = System::Drawing::Size(157, 22);
                this->exitToolStripMenuItem->Text = L"Exit";
                //
                // editToolStripMenuItem
                //
                this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
                this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
                this->editToolStripMenuItem->Text = L"Edit";
                //
                // modelsToolStripMenuItem
                //
                this->modelsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(1) {this->importModelToolStripMenuItem});
                this->modelsToolStripMenuItem->Name = L"modelsToolStripMenuItem";
                this->modelsToolStripMenuItem->Size = System::Drawing::Size(52, 20);
                this->modelsToolStripMenuItem->Text = L"Assets";
                //
                // importModelToolStripMenuItem
                //
                this->importModelToolStripMenuItem->Name = L"importModelToolStripMenuItem";
                this->importModelToolStripMenuItem->Size = System::Drawing::Size(168, 22);
                this->importModelToolStripMenuItem->Text = L"Import New Asset";
                //
                // sceneObjectToolStripMenuItem
                //
                this->sceneObjectToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(1) {this->createEmptyToolStripMenuItem});
                this->sceneObjectToolStripMenuItem->Name = L"sceneObjectToolStripMenuItem";
                this->sceneObjectToolStripMenuItem->Size = System::Drawing::Size(88, 20);
                this->sceneObjectToolStripMenuItem->Text = L"Scene Object";
                //
                // createEmptyToolStripMenuItem
                //
                this->createEmptyToolStripMenuItem->Name = L"createEmptyToolStripMenuItem";
                this->createEmptyToolStripMenuItem->Size = System::Drawing::Size(145, 22);
                this->createEmptyToolStripMenuItem->Text = L"Create Empty";
                //
                // windowToolStripMenuItem
                //
                this->windowToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(1) {this->consoleToolStripMenuItem});
                this->windowToolStripMenuItem->Name = L"windowToolStripMenuItem";
                this->windowToolStripMenuItem->Size = System::Drawing::Size(63, 20);
                this->windowToolStripMenuItem->Text = L"Window";
                //
                // consoleToolStripMenuItem
                //
                this->consoleToolStripMenuItem->Name = L"consoleToolStripMenuItem";
                this->consoleToolStripMenuItem->Size = System::Drawing::Size(117, 22);
                this->consoleToolStripMenuItem->Text = L"Console";
                //
                // aboutToolStripMenuItem
                //
                this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
                this->aboutToolStripMenuItem->Size = System::Drawing::Size(52, 20);
                this->aboutToolStripMenuItem->Text = L"About";
                //
                // assetImportFileDialog
                //
                this->assetImportFileDialog->FileName = L"assetImportFileDialog";
                //
                // StatusStrip
                //
                this->StatusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(1) {this->StatusStripLabel});
                this->StatusStrip->Location = System::Drawing::Point(0, 543);
                this->StatusStrip->Name = L"StatusStrip";
                this->StatusStrip->Size = System::Drawing::Size(1226, 22);
                this->StatusStrip->TabIndex = 2;
                this->StatusStrip->Text = L"statusStrip1";
                //
                // StatusStripLabel
                //
                this->StatusStripLabel->AutoSize = false;
                this->StatusStripLabel->Name = L"StatusStripLabel";
                this->StatusStripLabel->Size = System::Drawing::Size(500, 17);
                this->StatusStripLabel->Text = L"Welcome";
                this->StatusStripLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
                //
                // MainWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(1226, 565);
                this->Controls->Add(this->StatusStrip);
                this->Controls->Add(this->OpenGLRenderPanel);
                this->Controls->Add(this->topMenuBar);
                this->MainMenuStrip = this->topMenuBar;
                this->Name = L"MainWindow";
                this->Text = L"TCG - SCENE";
                this->topMenuBar->ResumeLayout(false);
                this->topMenuBar->PerformLayout();
                this->StatusStrip->ResumeLayout(false);
                this->StatusStrip->PerformLayout();
                this->ResumeLayout(false);
                this->PerformLayout();
            }
            #pragma endregion
        private: System::Void timer1_Tick(System::Object  ^sender, System::Windows::Forms::PaintEventArgs  ^e)
            {
                OpenGL->RestartStopwatch();
                UNREFERENCED_PARAMETER(sender);
                UNREFERENCED_PARAMETER(e);
                OpenGL->Render();
                OpenGL->SwapOpenGLBuffers();
                ((System::Windows::Forms::Panel ^)sender)->Invalidate();
            }

    };
}

