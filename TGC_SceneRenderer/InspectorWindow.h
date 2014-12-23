#pragma once
#include <iostream>
#include "Scene\SceneObject.h"

namespace SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for InspectorWindow
    /// </summary>
    public ref class InspectorWindow : public System::Windows::Forms::Form {
        public:
            InspectorWindow(void)
            {
                InitializeComponent();
                //
                //TODO: Add the constructor code here
                //
                this->trnfrControl->positionChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onPositionVectorChanged);
                this->trnfrControl->rotationChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onRotationVectorChanged);
                this->trnfrControl->scaleChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onScaleVectorChanged);
            }

        protected:
            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            ~InspectorWindow()
            {
                if (components) {
                    delete components;
                }
            }

        public: System::Object ^InstancedBy;
        public: System::Boolean changedActiveIndex;
        private: BaseControl::BaseControlControl  ^baseControl;
        private: System::Windows::Forms::TableLayoutPanel  ^tableLayoutPanel1;
        private: BaseControls::TransformControl  ^trnfrControl;
        private: unsigned int activeObjectIndex;
        private: scene::SceneObject *activeSceneObject;

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
                this->baseControl = (gcnew BaseControl::BaseControlControl());
                this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
                this->trnfrControl = (gcnew BaseControls::TransformControl());
                this->tableLayoutPanel1->SuspendLayout();
                this->SuspendLayout();
                //
                // baseControlControl1
                //
                this->baseControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->baseControl->Location = System::Drawing::Point(0, 0);
                this->baseControl->Name = L"baseControlControl1";
                this->baseControl->Size = System::Drawing::Size(599, 20);
                this->baseControl->TabIndex = 3;
                //
                // tableLayoutPanel1
                //
                this->tableLayoutPanel1->ColumnCount = 1;
                this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
                        100)));
                this->tableLayoutPanel1->Controls->Add(this->trnfrControl, 0, 0);
                this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->tableLayoutPanel1->Location = System::Drawing::Point(0, 20);
                this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
                this->tableLayoutPanel1->RowCount = 2;
                this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
                this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
                this->tableLayoutPanel1->Size = System::Drawing::Size(599, 242);
                this->tableLayoutPanel1->TabIndex = 4;
                //
                // transformControl1
                //
                this->trnfrControl->AutoSize = true;
                this->trnfrControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->trnfrControl->Location = System::Drawing::Point(3, 3);
                this->trnfrControl->Name = L"transformControl1";
                this->trnfrControl->Size = System::Drawing::Size(593, 106);
                this->trnfrControl->TabIndex = 0;
                //
                // InspectorWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(599, 262);
                this->Controls->Add(this->tableLayoutPanel1);
                this->Controls->Add(this->baseControl);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->Name = L"InspectorWindow";
                this->Text = L"Inspector";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &InspectorWindow::InspectorWindow_FormClosing);
                this->tableLayoutPanel1->ResumeLayout(false);
                this->tableLayoutPanel1->PerformLayout();
                this->ResumeLayout(false);
            }
            #pragma endregion
        private: System::Void onPositionVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
        private: System::Void onRotationVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
        private: System::Void onScaleVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
        public: System::Void SetActiveObjectIndex(unsigned int index);
        private: System::Void InspectorWindow_FormClosing(System::Object  ^sender, System::Windows::Forms::FormClosingEventArgs  ^e)
            {
                if (e->CloseReason == System::Windows::Forms::CloseReason::UserClosing) {
                    this->Hide();
                    e->Cancel = true;
                }
            }

    };
}
