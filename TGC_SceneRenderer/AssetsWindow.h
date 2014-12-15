#pragma once
#include "AssetBoxControl.h"

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
                flowLayoutPanel1->Controls->Add(gcnew AssetBoxControl());
                flowLayoutPanel1->Controls->Add(gcnew AssetBoxControl());
                flowLayoutPanel1->Controls->Add(gcnew AssetBoxControl());
                flowLayoutPanel1->Controls->Add(gcnew AssetBoxControl());
                flowLayoutPanel1->Controls->Add(gcnew AssetBoxControl());
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
        private: System::Windows::Forms::TabControl  ^tabControl1;
        protected:
        private: System::Windows::Forms::TabPage  ^tabPage1;
        private: System::Windows::Forms::TabPage  ^tabPage2;
        private: System::Windows::Forms::Panel  ^panel1;
        private: System::Windows::Forms::TabPage  ^tabPage3;
        private: System::Windows::Forms::FlowLayoutPanel  ^flowLayoutPanel2;
        private: System::Windows::Forms::FlowLayoutPanel  ^flowLayoutPanel1;
        private: System::Windows::Forms::FlowLayoutPanel  ^flowLayoutPanel3;

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
                this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
                this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
                this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
                this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
                this->flowLayoutPanel2 = (gcnew System::Windows::Forms::FlowLayoutPanel());
                this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
                this->flowLayoutPanel3 = (gcnew System::Windows::Forms::FlowLayoutPanel());
                this->panel1 = (gcnew System::Windows::Forms::Panel());
                this->tabControl1->SuspendLayout();
                this->tabPage1->SuspendLayout();
                this->tabPage2->SuspendLayout();
                this->tabPage3->SuspendLayout();
                this->panel1->SuspendLayout();
                this->SuspendLayout();
                //
                // tabControl1
                //
                this->tabControl1->Controls->Add(this->tabPage1);
                this->tabControl1->Controls->Add(this->tabPage2);
                this->tabControl1->Controls->Add(this->tabPage3);
                this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->tabControl1->Location = System::Drawing::Point(0, 0);
                this->tabControl1->Name = L"tabControl1";
                this->tabControl1->SelectedIndex = 0;
                this->tabControl1->Size = System::Drawing::Size(284, 366);
                this->tabControl1->TabIndex = 0;
                //
                // tabPage1
                //
                this->tabPage1->Controls->Add(this->flowLayoutPanel1);
                this->tabPage1->Location = System::Drawing::Point(4, 22);
                this->tabPage1->Name = L"tabPage1";
                this->tabPage1->Padding = System::Windows::Forms::Padding(3);
                this->tabPage1->Size = System::Drawing::Size(276, 340);
                this->tabPage1->TabIndex = 0;
                this->tabPage1->Text = L"Materials";
                this->tabPage1->UseVisualStyleBackColor = true;
                //
                // flowLayoutPanel1
                //
                this->flowLayoutPanel1->AutoScroll = true;
                this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->flowLayoutPanel1->Location = System::Drawing::Point(3, 3);
                this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
                this->flowLayoutPanel1->Size = System::Drawing::Size(270, 334);
                this->flowLayoutPanel1->TabIndex = 1;
                //
                // tabPage2
                //
                this->tabPage2->Controls->Add(this->flowLayoutPanel2);
                this->tabPage2->Location = System::Drawing::Point(4, 22);
                this->tabPage2->Name = L"tabPage2";
                this->tabPage2->Padding = System::Windows::Forms::Padding(3);
                this->tabPage2->Size = System::Drawing::Size(276, 340);
                this->tabPage2->TabIndex = 1;
                this->tabPage2->Text = L"Models";
                this->tabPage2->UseVisualStyleBackColor = true;
                //
                // flowLayoutPanel2
                //
                this->flowLayoutPanel2->AutoScroll = true;
                this->flowLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
                this->flowLayoutPanel2->Location = System::Drawing::Point(3, 3);
                this->flowLayoutPanel2->Name = L"flowLayoutPanel2";
                this->flowLayoutPanel2->Size = System::Drawing::Size(270, 334);
                this->flowLayoutPanel2->TabIndex = 0;
                //
                // tabPage3
                //
                this->tabPage3->Controls->Add(this->flowLayoutPanel3);
                this->tabPage3->Location = System::Drawing::Point(4, 22);
                this->tabPage3->Name = L"tabPage3";
                this->tabPage3->Padding = System::Windows::Forms::Padding(3);
                this->tabPage3->Size = System::Drawing::Size(276, 340);
                this->tabPage3->TabIndex = 2;
                this->tabPage3->Text = L"Textures";
                this->tabPage3->UseVisualStyleBackColor = true;
                //
                // flowLayoutPanel3
                //
                this->flowLayoutPanel3->AutoScroll = true;
                this->flowLayoutPanel3->Dock = System::Windows::Forms::DockStyle::Fill;
                this->flowLayoutPanel3->Location = System::Drawing::Point(3, 3);
                this->flowLayoutPanel3->Name = L"flowLayoutPanel3";
                this->flowLayoutPanel3->Size = System::Drawing::Size(270, 334);
                this->flowLayoutPanel3->TabIndex = 1;
                //
                // panel1
                //
                this->panel1->Controls->Add(this->tabControl1);
                this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->panel1->Location = System::Drawing::Point(0, 0);
                this->panel1->Name = L"panel1";
                this->panel1->Size = System::Drawing::Size(284, 366);
                this->panel1->TabIndex = 1;
                //
                // AssetsWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(284, 366);
                this->ControlBox = false;
                this->Controls->Add(this->panel1);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->Name = L"AssetsWindow";
                this->Text = L"Assets";
                this->tabControl1->ResumeLayout(false);
                this->tabPage1->ResumeLayout(false);
                this->tabPage2->ResumeLayout(false);
                this->tabPage3->ResumeLayout(false);
                this->panel1->ResumeLayout(false);
                this->ResumeLayout(false);
            }
            #pragma endregion
    };
}
