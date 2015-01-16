#pragma once

namespace SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for LoadingWindow
    /// </summary>
    public ref class LoadingWindow : public System::Windows::Forms::Form {
        public:
            LoadingWindow(void)
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
            ~LoadingWindow()
            {
                if (components) {
                    delete components;
                }
            }
        private: System::Windows::Forms::ProgressBar  ^progressBar1;
        public: System::Windows::Forms::Form ^mainWindow;
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
                this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
                this->SuspendLayout();
                //
                // progressBar1
                //
                this->progressBar1->Dock = System::Windows::Forms::DockStyle::Top;
                this->progressBar1->Location = System::Drawing::Point(0, 0);
                this->progressBar1->Name = L"progressBar1";
                this->progressBar1->Size = System::Drawing::Size(284, 23);
                this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
                this->progressBar1->TabIndex = 0;
                //
                // LoadingWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->AutoSize = true;
                this->ClientSize = System::Drawing::Size(284, 23);
                this->ControlBox = false;
                this->Controls->Add(this->progressBar1);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
                this->MaximizeBox = false;
                this->MinimizeBox = false;
                this->Name = L"LoadingWindow";
                this->ShowIcon = false;
                this->ShowInTaskbar = false;
                this->Text = L"Loading...";
                this->TopMost = true;
                this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &LoadingWindow::LoadingWindow_Paint);
                this->ResumeLayout(false);
            }
            #pragma endregion
        private: System::Void LoadingWindow_Paint(System::Object  ^sender, System::Windows::Forms::PaintEventArgs  ^e);
    };
}
