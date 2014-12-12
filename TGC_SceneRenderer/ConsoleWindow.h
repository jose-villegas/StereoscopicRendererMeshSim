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
        private: System::Windows::Forms::ListBox  ^listBox1;
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
                this->listBox1 = (gcnew System::Windows::Forms::ListBox());
                this->SuspendLayout();
                this->listBox1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->listBox1->DrawMode = System::Windows::Forms::DrawMode::OwnerDrawFixed;
                this->listBox1->FormattingEnabled = true;
                this->listBox1->Items->AddRange(gcnew cli::array< System::Object ^  >(2) {L"asd", L"asd"});
                this->listBox1->Location = System::Drawing::Point(0, 0);
                this->listBox1->Name = L"listBox1";
                this->listBox1->Size = System::Drawing::Size(484, 216);
                this->listBox1->TabIndex = 0;
                this->listBox1->DrawItem += gcnew System::Windows::Forms::DrawItemEventHandler(this, &ConsoleWindow::listBox1_DrawItem);
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(484, 216);
                this->Controls->Add(this->listBox1);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->Name = L"ConsoleWindow";
                this->ShowInTaskbar = false;
                this->Text = L"Console";
                this->TopMost = true;
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
        private: System::Void listBox1_DrawItem(System::Object  ^sender, System::Windows::Forms::DrawItemEventArgs  ^e)
            {
                e->DrawBackground();
                System::Windows::Forms::Label ^currentLabel = (System::Windows::Forms::Label ^)((System::Windows::Forms::ListBox ^)sender)->Items[e->Index];
                System::Drawing::SolidBrush ^currentBrush = gcnew System::Drawing::SolidBrush(currentLabel->ForeColor);
                e->Graphics->DrawString(currentLabel->Text, e->Font, currentBrush, e->Bounds, System::Drawing::StringFormat::GenericDefault);
                e->DrawFocusRectangle();
            }
    };
}
