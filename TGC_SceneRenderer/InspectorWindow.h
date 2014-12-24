#pragma once
#include <iostream>
#include "Scene\SceneObject.h"
#include "Scene\Camera.h"
#include "Scene\Light.h"
#include "Scene\Mesh.h"

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
        private:
            scene::Light  *_lightComponentPtr;
            scene::Camera *_cameraComponentPtr;
            scene::Mesh *_meshComponentPtr;

            // Transform Control Events
            System::Void onPositionVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onRotationVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onScaleVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
            // Light Control Events
            System::Void onLightAttenuationChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onLightIntensityChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onLightOuterAngleChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onLightInnerAngleChanged(System::Object  ^sender, System::EventArgs  ^e);

        public:
            InspectorWindow(void)
            {
                InitializeComponent();
                // Set Delegated Events from Transform Control
                this->trnfrControl->positionChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onPositionVectorChanged);
                this->trnfrControl->rotationChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onRotationVectorChanged);
                this->trnfrControl->scaleChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onScaleVectorChanged);
                // Set light control color picking event
                this->lightControl->colorPickerButton->Click += gcnew System::EventHandler(this, &InspectorWindow::pickColorDialogPopup);
                // Set delegated events from light control
                this->lightControl->attenuationChangedDelegatedEvent += gcnew AttenuationChangedDelegateEvent(this, &InspectorWindow::onLightAttenuationChanged);
                this->lightControl->intensityChangedDelegatedEvent += gcnew IntensityChangedDelegateEvent(this, &InspectorWindow::onLightIntensityChanged);
                this->lightControl->outerAngleChangedDelegatedEvent += gcnew OuterAngleChangedDelegateEvent(this, &InspectorWindow::onLightOuterAngleChanged);
                this->lightControl->innerAngleChangedDelegatedEvent += gcnew InnerAngleChangedDelegateEvent(this, &InspectorWindow::onLightInnerAngleChanged);
                // Construct unmanaged vectors
                this->_lightComponentPtr  = nullptr;
                this->_cameraComponentPtr =  nullptr;
                this->_meshComponentPtr   =  nullptr;
            }

        public: System::Void setActiveObjectIndex(unsigned int index);

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
            // Object Components

        private: System::Void pickColorDialogPopup(System::Object  ^sender, System::EventArgs  ^e);

        public: System::Object ^InstancedBy;
        public: System::Boolean changedActiveIndex;
        private: BaseControl::BaseControlControl  ^baseControl;
        private: System::Windows::Forms::TableLayoutPanel  ^tableLayoutPanel1;
        private: BaseControls::TransformControl  ^trnfrControl;
        private: unsigned int activeObjectIndex;
        private: scene::SceneObject *activeSceneObject;
        private: System::Windows::Forms::Panel  ^componentPanel;
        private: System::Windows::Forms::ColorDialog  ^colorDialog1;
        private: BaseControls::LightControlControl  ^lightControl;

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
                this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
                this->componentPanel = (gcnew System::Windows::Forms::Panel());
                this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
                this->trnfrControl = (gcnew BaseControls::TransformControl());
                this->baseControl = (gcnew BaseControl::BaseControlControl());
                this->lightControl = (gcnew BaseControls::LightControlControl());
                this->tableLayoutPanel1->SuspendLayout();
                this->componentPanel->SuspendLayout();
                this->SuspendLayout();
                //
                // tableLayoutPanel1
                //
                this->tableLayoutPanel1->ColumnCount = 1;
                this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
                        100)));
                this->tableLayoutPanel1->Controls->Add(this->trnfrControl, 0, 0);
                this->tableLayoutPanel1->Controls->Add(this->componentPanel, 0, 1);
                this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->tableLayoutPanel1->Location = System::Drawing::Point(0, 20);
                this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
                this->tableLayoutPanel1->RowCount = 2;
                this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
                this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
                this->tableLayoutPanel1->Size = System::Drawing::Size(384, 334);
                this->tableLayoutPanel1->TabIndex = 4;
                //
                // componentPanel
                //
                this->componentPanel->AutoScroll = true;
                this->componentPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
                this->componentPanel->Controls->Add(this->lightControl);
                this->componentPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                this->componentPanel->Location = System::Drawing::Point(0, 112);
                this->componentPanel->Margin = System::Windows::Forms::Padding(0);
                this->componentPanel->Name = L"componentPanel";
                this->componentPanel->Padding = System::Windows::Forms::Padding(5);
                this->componentPanel->Size = System::Drawing::Size(384, 222);
                this->componentPanel->TabIndex = 1;
                //
                // colorDialog1
                //
                this->colorDialog1->AnyColor = true;
                this->colorDialog1->ShowHelp = true;
                //
                // trnfrControl
                //
                this->trnfrControl->AutoSize = true;
                this->trnfrControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->trnfrControl->Location = System::Drawing::Point(3, 3);
                this->trnfrControl->Name = L"trnfrControl";
                this->trnfrControl->Size = System::Drawing::Size(378, 106);
                this->trnfrControl->TabIndex = 0;
                //
                // baseControl
                //
                this->baseControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->baseControl->Location = System::Drawing::Point(0, 0);
                this->baseControl->Name = L"baseControl";
                this->baseControl->Size = System::Drawing::Size(384, 20);
                this->baseControl->TabIndex = 3;
                //
                // lightControlControl1
                //
                this->lightControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->lightControl->Location = System::Drawing::Point(5, 5);
                this->lightControl->Name = L"lightControlControl1";
                this->lightControl->Size = System::Drawing::Size(370, 203);
                this->lightControl->TabIndex = 0;
                this->lightControl->Visible = false;
                //
                // InspectorWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(384, 354);
                this->Controls->Add(this->tableLayoutPanel1);
                this->Controls->Add(this->baseControl);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->Name = L"InspectorWindow";
                this->Text = L"Inspector";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &InspectorWindow::InspectorWindow_FormClosing);
                this->tableLayoutPanel1->ResumeLayout(false);
                this->tableLayoutPanel1->PerformLayout();
                this->componentPanel->ResumeLayout(false);
                this->ResumeLayout(false);
            }
            #pragma endregion

        private: System::Void InspectorWindow_FormClosing(System::Object  ^sender, System::Windows::Forms::FormClosingEventArgs  ^e)
            {
                if (e->CloseReason == System::Windows::Forms::CloseReason::UserClosing) {
                    this->Hide();
                    e->Cancel = true;
                }
            }
    };
}