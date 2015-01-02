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
    public ref class PropertiesWindow : public System::Windows::Forms::Form {


        public:
            PropertiesWindow(void)
            {
                InitializeComponent();
                // Set Delegated Events from Transform Control
                this->trnfrControl->positionChanged += gcnew TransformValuesChangedHandler(this, &PropertiesWindow::onPositionVectorChanged);
                this->trnfrControl->rotationChanged += gcnew TransformValuesChangedHandler(this, &PropertiesWindow::onRotationVectorChanged);
                this->trnfrControl->scaleChanged += gcnew TransformValuesChangedHandler(this, &PropertiesWindow::onScaleVectorChanged);
                // Set light control color picking event
                this->lightControl->colorPickerButton->Click += gcnew System::EventHandler(this, &PropertiesWindow::pickColorDialogPopup);
                // Set delegated events from light control
                this->lightControl->attenuationChangedDelegatedEvent += gcnew AttenuationChangedDelegateEvent(this, &PropertiesWindow::onLightAttenuationChanged);
                this->lightControl->intensityChangedDelegatedEvent += gcnew IntensityChangedDelegateEvent(this, &PropertiesWindow::onLightIntensityChanged);
                this->lightControl->outerAngleChangedDelegatedEvent += gcnew OuterAngleChangedDelegateEvent(this, &PropertiesWindow::onLightOuterAngleChanged);
                this->lightControl->innerAngleChangedDelegatedEvent += gcnew InnerAngleChangedDelegateEvent(this, &PropertiesWindow::onLightInnerAngleChanged);
                // Mesh Control Events
                this->meshControl->renderSimplChangedHandler += gcnew RenderSimplChanged(this, &PropertiesWindow::onSimplMeshCheckChanged);
                this->meshControl->vertexNumeric->ValueChanged += gcnew System::EventHandler(this, &PropertiesWindow::onVertexCountNumericChanged);
                // Construct unmanaged vectors
                this->lightComponentPtr  = nullptr;
                this->cameraComponentPtr =  nullptr;
                this->meshComponentPtr   =  nullptr;
                // Speed params
                this->trnfrControl->positionSpeed(0.01);
                this->trnfrControl->rotationSpeed(0.33);
                this->trnfrControl->scaleSpeed(0.01);
            }

        public: System::Void setActiveObjectIndex(unsigned int index);

        private:

            unsigned int activeObjectIndex;
            scene::SceneObject *activeSceneObject;
            scene::Light  *lightComponentPtr;
            scene::Camera *cameraComponentPtr;
            scene::Mesh *meshComponentPtr;
            CameraControl::CameraControlControl  ^cameraControl;
            MeshControl::MeshControlControl  ^meshControl;

            // Transform Control Events
            System::Void onPositionVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onRotationVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onScaleVectorChanged(System::Object  ^sender, System::EventArgs  ^e);
            // Light Control Events
            System::Void onLightAttenuationChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onLightIntensityChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onLightOuterAngleChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onLightInnerAngleChanged(System::Object  ^sender, System::EventArgs  ^e);
            // Mesh control events
            System::Void onSimplMeshCheckChanged(System::Object  ^sender, System::EventArgs  ^e);
            System::Void onVertexCountNumericChanged(System::Object  ^sender, System::EventArgs  ^e);

        protected:
            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            ~PropertiesWindow()
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
                this->trnfrControl = (gcnew BaseControls::TransformControl());
                this->componentPanel = (gcnew System::Windows::Forms::Panel());
                this->meshControl = (gcnew MeshControl::MeshControlControl());
                this->cameraControl = (gcnew CameraControl::CameraControlControl());
                this->lightControl = (gcnew BaseControls::LightControlControl());
                this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
                this->baseControl = (gcnew BaseControl::BaseControlControl());
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
                this->tableLayoutPanel1->Size = System::Drawing::Size(384, 392);
                this->tableLayoutPanel1->TabIndex = 4;
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
                // componentPanel
                //
                this->componentPanel->AutoScroll = true;
                this->componentPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
                this->componentPanel->Controls->Add(this->meshControl);
                this->componentPanel->Controls->Add(this->cameraControl);
                this->componentPanel->Controls->Add(this->lightControl);
                this->componentPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                this->componentPanel->Location = System::Drawing::Point(0, 112);
                this->componentPanel->Margin = System::Windows::Forms::Padding(0);
                this->componentPanel->Name = L"componentPanel";
                this->componentPanel->Padding = System::Windows::Forms::Padding(5);
                this->componentPanel->Size = System::Drawing::Size(384, 280);
                this->componentPanel->TabIndex = 1;
                //
                // meshControl
                //
                this->meshControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->meshControl->Location = System::Drawing::Point(5, 415);
                this->meshControl->Name = L"meshControl";
                this->meshControl->Size = System::Drawing::Size(353, 136);
                this->meshControl->TabIndex = 2;
                this->meshControl->Visible = false;
                //
                // cameraControl
                //
                this->cameraControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->cameraControl->Location = System::Drawing::Point(5, 208);
                this->cameraControl->Name = L"cameraControl";
                this->cameraControl->Size = System::Drawing::Size(353, 207);
                this->cameraControl->TabIndex = 1;
                this->cameraControl->Visible = false;
                //
                // lightControl
                //
                this->lightControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->lightControl->Location = System::Drawing::Point(5, 5);
                this->lightControl->Name = L"lightControl";
                this->lightControl->Size = System::Drawing::Size(353, 203);
                this->lightControl->TabIndex = 0;
                this->lightControl->Visible = false;
                //
                // colorDialog1
                //
                this->colorDialog1->AnyColor = true;
                this->colorDialog1->ShowHelp = true;
                //
                // baseControl
                //
                this->baseControl->Dock = System::Windows::Forms::DockStyle::Top;
                this->baseControl->Location = System::Drawing::Point(0, 0);
                this->baseControl->Name = L"baseControl";
                this->baseControl->Size = System::Drawing::Size(384, 20);
                this->baseControl->TabIndex = 3;
                //
                // PropertiesWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(384, 412);
                this->Controls->Add(this->tableLayoutPanel1);
                this->Controls->Add(this->baseControl);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->Name = L"PropertiesWindow";
                this->Text = L"Properties";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &PropertiesWindow::InspectorWindow_FormClosing);
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
