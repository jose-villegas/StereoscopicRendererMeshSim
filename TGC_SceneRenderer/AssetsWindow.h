#pragma once

namespace TGC_SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for AssetsWindow
    /// </summary>
    public ref class AssetsWindow : public System::Windows::Forms::Form {
        public:
            AssetsWindow(void)
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
            ~AssetsWindow()
            {
                if (components) {
                    delete components;
                }
            }

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
                this->SuspendLayout();
                //
                // AssetsWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(284, 262);
                this->Name = L"AssetsWindow";
                this->Text = L"Assets";
                this->ResumeLayout(false);
            }
            #pragma endregion
    };
}
