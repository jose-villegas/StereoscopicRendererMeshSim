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
        private: System::Windows::Forms::Panel  ^objectsWindowControlPanel;
        protected:

        protected:

        protected:



        private: System::Windows::Forms::MenuStrip  ^createMenu;
        private: System::Windows::Forms::ToolStripMenuItem  ^createToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^searchToolStripTag;

        private: System::Windows::Forms::Panel  ^panel1;
        private: System::Windows::Forms::ToolStripTextBox  ^toolStripTextBox1;








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
                this->objectsWindowControlPanel = (gcnew System::Windows::Forms::Panel());
                this->panel1 = (gcnew System::Windows::Forms::Panel());
                this->createMenu = (gcnew System::Windows::Forms::MenuStrip());
                this->createToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->searchToolStripTag = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->toolStripTextBox1 = (gcnew System::Windows::Forms::ToolStripTextBox());
                this->objectsWindowControlPanel->SuspendLayout();
                this->createMenu->SuspendLayout();
                this->SuspendLayout();
                //
                // objectsWindowControlPanel
                //
                this->objectsWindowControlPanel->Controls->Add(this->panel1);
                this->objectsWindowControlPanel->Controls->Add(this->createMenu);
                this->objectsWindowControlPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                this->objectsWindowControlPanel->Location = System::Drawing::Point(0, 0);
                this->objectsWindowControlPanel->Name = L"objectsWindowControlPanel";
                this->objectsWindowControlPanel->Size = System::Drawing::Size(214, 316);
                this->objectsWindowControlPanel->TabIndex = 0;
                //
                // panel1
                //
                this->panel1->AutoScroll = true;
                this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->panel1->Location = System::Drawing::Point(0, 27);
                this->panel1->Name = L"panel1";
                this->panel1->Size = System::Drawing::Size(214, 289);
                this->panel1->TabIndex = 1;
                //
                // createMenu
                //
                this->createMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(3) {
                    this->createToolStripMenuItem,
                         this->searchToolStripTag, this->toolStripTextBox1
                });
                this->createMenu->Location = System::Drawing::Point(0, 0);
                this->createMenu->Name = L"createMenu";
                this->createMenu->Size = System::Drawing::Size(214, 27);
                this->createMenu->TabIndex = 0;
                this->createMenu->Text = L"menuStrip1";
                //
                // createToolStripMenuItem
                //
                this->createToolStripMenuItem->Name = L"createToolStripMenuItem";
                this->createToolStripMenuItem->Size = System::Drawing::Size(53, 23);
                this->createToolStripMenuItem->Text = L"Create";
                //
                // searchToolStripTag
                //
                this->searchToolStripTag->Name = L"searchToolStripTag";
                this->searchToolStripTag->Size = System::Drawing::Size(57, 23);
                this->searchToolStripTag->Text = L"Search:";
                //
                // toolStripTextBox1
                //
                this->toolStripTextBox1->Name = L"toolStripTextBox1";
                this->toolStripTextBox1->Size = System::Drawing::Size(90, 23);
                //
                // ObjectsWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(214, 316);
                this->Controls->Add(this->objectsWindowControlPanel);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->MainMenuStrip = this->createMenu;
                this->Name = L"ObjectsWindow";
                this->Text = L"Objects";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ObjectsWindow::ObjectsWindow_FormClosing);
                this->objectsWindowControlPanel->ResumeLayout(false);
                this->objectsWindowControlPanel->PerformLayout();
                this->createMenu->ResumeLayout(false);
                this->createMenu->PerformLayout();
                this->ResumeLayout(false);
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
