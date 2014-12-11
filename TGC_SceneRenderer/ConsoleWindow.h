#pragma once
#include "Utils\Logger.h"

namespace TGC_SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for ConsoleWindow
    /// </summary>
    public ref class ConsoleWindow : public System::Windows::Forms::Form {
        public:
            ConsoleWindow(void)
            {
                InitializeComponent();
                //
                //TODO: Add the constructor code here
                //
                // Bind Logger.h Log with Console Window List
                listBox1->DataSource = Utils::Logger::GetLog();
            }

        protected:
            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            ~ConsoleWindow()
            {
                if (components) {
                    delete components;
                }
            }
        private: System::Windows::Forms::ListBox  ^logListbox;
        protected:

        protected:




        protected:

        private:
            /// <summary>
            /// Required designer variable.
            /// </summary>
            System::ComponentModel::Container ^components;

            #pragma region Windows Form Designer generated code
            /// <summary>
            /// Required method for Designer support - do not modify
            /// the contents of this method with the code editor.
            /// </summary>
            void InitializeComponent(void)
            {
                this->logListbox = (gcnew System::Windows::Forms::ListBox());
                this->SuspendLayout();
                //
                // logListbox
                //
                this->logListbox->Dock = System::Windows::Forms::DockStyle::Fill;
                this->logListbox->FormattingEnabled = true;
                this->logListbox->Location = System::Drawing::Point(0, 0);
                this->logListbox->Name = L"logListbox";
                this->logListbox->ScrollAlwaysVisible = true;
                this->logListbox->Size = System::Drawing::Size(484, 216);
                this->logListbox->TabIndex = 1;
                //
                // ConsoleWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(484, 216);
                this->Controls->Add(this->logListbox);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->Name = L"ConsoleWindow";
                this->ShowInTaskbar = false;
                this->Text = L"Console";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ConsoleWindow::ConsoleWindow_FormClosing);
                this->ResumeLayout(false);
            }
            #pragma endregion
        private: System::Void ConsoleWindow_FormClosing(System::Object  ^sender, System::Windows::Forms::FormClosingEventArgs  ^e)
            {
                if (e->CloseReason == System::Windows::Forms::CloseReason::UserClosing) {
                    this->Hide();
                    e->Cancel = true;
                }
            }
    };
}
