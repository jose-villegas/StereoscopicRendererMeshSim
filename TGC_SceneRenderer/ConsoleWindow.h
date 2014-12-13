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
                // loggerListBox->Enabled = false;
                loggerListBox->DataSource = Utils::Logger::GetLog();
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

        private: System::Windows::Forms::Panel  ^consoleWindowControlPanel;
        private: System::Windows::Forms::ListBox  ^loggerListBox;

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
                this->loggerListBox = (gcnew System::Windows::Forms::ListBox());
                this->consoleWindowControlPanel = (gcnew System::Windows::Forms::Panel());
                this->consoleWindowControlPanel->SuspendLayout();
                this->SuspendLayout();
                //
                // loggerListBox
                //
                this->loggerListBox->Dock = System::Windows::Forms::DockStyle::Fill;
                this->loggerListBox->DrawMode = System::Windows::Forms::DrawMode::OwnerDrawFixed;
                this->loggerListBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
                                             System::Drawing::GraphicsUnit::Point,
                                             static_cast<System::Byte>(0)));
                this->loggerListBox->FormattingEnabled = true;
                this->loggerListBox->HorizontalScrollbar = true;
                this->loggerListBox->Items->AddRange(gcnew cli::array< System::Object ^  >(2) {L"asd", L"asd"});
                this->loggerListBox->Location = System::Drawing::Point(0, 0);
                this->loggerListBox->Name = L"loggerListBox";
                this->loggerListBox->Size = System::Drawing::Size(484, 216);
                this->loggerListBox->TabIndex = 0;
                this->loggerListBox->DrawItem += gcnew System::Windows::Forms::DrawItemEventHandler(this, &ConsoleWindow::listBox1_DrawItem);
                //
                // consoleWindowControlPanel
                //
                this->consoleWindowControlPanel->Controls->Add(this->loggerListBox);
                this->consoleWindowControlPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                this->consoleWindowControlPanel->Location = System::Drawing::Point(0, 0);
                this->consoleWindowControlPanel->Name = L"consoleWindowControlPanel";
                this->consoleWindowControlPanel->Size = System::Drawing::Size(484, 216);
                this->consoleWindowControlPanel->TabIndex = 1;
                //
                // ConsoleWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(484, 216);
                this->Controls->Add(this->consoleWindowControlPanel);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->Name = L"ConsoleWindow";
                this->ShowInTaskbar = false;
                this->Text = L"Console";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ConsoleWindow::ConsoleWindow_FormClosing);
                this->consoleWindowControlPanel->ResumeLayout(false);
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
