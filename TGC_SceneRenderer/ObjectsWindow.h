#pragma once

namespace TGC_SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for ObjectsWindow
    /// </summary>
    public ref class ObjectsWindow : public System::Windows::Forms::Form {
        public:
            ObjectsWindow(void)
            {
                InitializeComponent();
                //
                //TODO: Add the constructor code here
                //
            }

        protected:
            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            ~ObjectsWindow()
            {
                if (components) {
                    delete components;
                }
            }
        private: System::Windows::Forms::MenuStrip  ^createMenu;
        protected:

        protected:
        private: System::Windows::Forms::ToolStripMenuItem  ^createToolStripMenuItem;
        private: System::Windows::Forms::Panel  ^sceneObjectsPanel;


        private: System::Windows::Forms::Label  ^searchTextBoxLabel;
        private: System::Windows::Forms::TextBox  ^searchTextBoxInput;




        protected:
        private: System::ComponentModel::IContainer  ^components;

        private:
            /// <summary>
            /// Required designer variable.
            /// </summary>


            #pragma region Windows Form Designer generated code
            /// <summary>
            /// Required method for Designer support - do not modify
            /// the contents of this method with the code editor.
            /// </summary>
            void InitializeComponent(void)
            {
                this->createMenu = (gcnew System::Windows::Forms::MenuStrip());
                this->createToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->sceneObjectsPanel = (gcnew System::Windows::Forms::Panel());
                this->searchTextBoxLabel = (gcnew System::Windows::Forms::Label());
                this->searchTextBoxInput = (gcnew System::Windows::Forms::TextBox());
                this->createMenu->SuspendLayout();
                this->SuspendLayout();
                //
                // createMenu
                //
                this->createMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(1) {this->createToolStripMenuItem});
                this->createMenu->Location = System::Drawing::Point(0, 0);
                this->createMenu->Name = L"createMenu";
                this->createMenu->Size = System::Drawing::Size(214, 24);
                this->createMenu->TabIndex = 0;
                this->createMenu->Text = L"menuStrip1";
                //
                // createToolStripMenuItem
                //
                this->createToolStripMenuItem->Name = L"createToolStripMenuItem";
                this->createToolStripMenuItem->Size = System::Drawing::Size(53, 20);
                this->createToolStripMenuItem->Text = L"Create";
                //
                // sceneObjectsPanel
                //
                this->sceneObjectsPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                this->sceneObjectsPanel->Location = System::Drawing::Point(0, 24);
                this->sceneObjectsPanel->Name = L"sceneObjectsPanel";
                this->sceneObjectsPanel->Size = System::Drawing::Size(214, 292);
                this->sceneObjectsPanel->TabIndex = 1;
                //
                // searchTextBoxLabel
                //
                this->searchTextBoxLabel->AutoSize = true;
                this->searchTextBoxLabel->Location = System::Drawing::Point(61, 4);
                this->searchTextBoxLabel->Name = L"searchTextBoxLabel";
                this->searchTextBoxLabel->Size = System::Drawing::Size(44, 13);
                this->searchTextBoxLabel->TabIndex = 2;
                this->searchTextBoxLabel->Text = L"Search:";
                //
                // searchTextBoxInput
                //
                this->searchTextBoxInput->Location = System::Drawing::Point(111, 1);
                this->searchTextBoxInput->Name = L"searchTextBoxInput";
                this->searchTextBoxInput->Size = System::Drawing::Size(100, 20);
                this->searchTextBoxInput->TabIndex = 3;
                //
                // ObjectsWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(214, 316);
                this->Controls->Add(this->searchTextBoxLabel);
                this->Controls->Add(this->searchTextBoxInput);
                this->Controls->Add(this->sceneObjectsPanel);
                this->Controls->Add(this->createMenu);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->MainMenuStrip = this->createMenu;
                this->Name = L"ObjectsWindow";
                this->Text = L"Objects";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ObjectsWindow::ObjectsWindow_FormClosing);
                this->createMenu->ResumeLayout(false);
                this->createMenu->PerformLayout();
                this->ResumeLayout(false);
                this->PerformLayout();
            }
            #pragma endregion
        private: System::Void ObjectsWindow_FormClosing(System::Object  ^sender, System::Windows::Forms::FormClosingEventArgs  ^e)
            {
                if (e->CloseReason == System::Windows::Forms::CloseReason::UserClosing) {
                    this->Hide();
                    e->Cancel = true;
                }
            }
    };
}
