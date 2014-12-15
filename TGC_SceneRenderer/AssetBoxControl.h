#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

enum ASSET_TYPE {
    MATERIALS,
    MODELS,
    TEXTURES
};

namespace TGC_SceneRenderer {

    /// <summary>
    /// Summary for AssetBoxControl
    /// </summary>
    public ref class AssetBoxControl : public System::Windows::Forms::UserControl {
        public:
            AssetBoxControl(void)
            {
                InitializeComponent();
                //
                //TODO: Add the constructor code here
                //
            }

            AssetBoxControl(ASSET_TYPE assetType)
            {
                this->assetType = assetType;
                AssetBoxControl();
            }

        protected:
            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            ~AssetBoxControl()
            {
                if (components) {
                    delete components;
                }
            }
        private: System::Windows::Forms::PictureBox  ^pictureBox1;
        private: System::Windows::Forms::Label  ^label1;
        public: ASSET_TYPE assetType;

        private :
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
                this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
                this->label1 = (gcnew System::Windows::Forms::Label());
                (cli::safe_cast<System::ComponentModel::ISupportInitialize ^  >(this->pictureBox1))->BeginInit();
                this->SuspendLayout();
                //
                // pictureBox1
                //
                this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Top;
                this->pictureBox1->Location = System::Drawing::Point(0, 0);
                this->pictureBox1->Name = L"pictureBox1";
                this->pictureBox1->Size = System::Drawing::Size(65, 65);
                this->pictureBox1->TabIndex = 0;
                this->pictureBox1->TabStop = false;
                this->pictureBox1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &AssetBoxControl::pictureBox1_MouseClick);
                //
                // label1
                //
                this->label1->Dock = System::Windows::Forms::DockStyle::Top;
                this->label1->Location = System::Drawing::Point(0, 65);
                this->label1->Name = L"label1";
                this->label1->Size = System::Drawing::Size(65, 23);
                this->label1->TabIndex = 1;
                this->label1->Text = L"label1";
                this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
                this->label1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &AssetBoxControl::pictureBox1_MouseClick);
                //
                // AssetBoxControl
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->Controls->Add(this->label1);
                this->Controls->Add(this->pictureBox1);
                this->Name = L"AssetBoxControl";
                this->Size = System::Drawing::Size(65, 88);
                (cli::safe_cast<System::ComponentModel::ISupportInitialize ^  >(this->pictureBox1))->EndInit();
                this->ResumeLayout(false);
            }
            #pragma endregion

        private: System::Void AssetBoxControl::pictureBox1_MouseClick(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e)
            {
                for each(Object ^ var in((FlowLayoutPanel ^)this->Parent)->Controls) {
                    ((AssetBoxControl ^)var)->label1->BackColor = System::Drawing::Color::White;
                }

                this->label1->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
            }
    };
}
