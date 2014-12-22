#pragma once
#include <iostream>

namespace TGC_SceneRenderer {

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
                this->vector3DControl2->valuesChangedEvent += gcnew ValuesChanged(this, &InspectorWindow::onVector3DUserControlChanged);
                this->transformControl1->positionChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onPositionVectorChanged);
                this->transformControl1->rotationChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onRotationVectorChanged);
                this->transformControl1->scaleChanged += gcnew TransformValuesChangedHandler(this, &InspectorWindow::onScaleVectorChanged);
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

        private: BaseControls::TransformControl  ^transformControl1;

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
                this->transformControl1 = (gcnew BaseControls::TransformControl());
                this->SuspendLayout();
                //
                // transformControl1
                //
                this->transformControl1->Location = System::Drawing::Point(12, 12);
                this->transformControl1->Name = L"transformControl1";
                this->transformControl1->Size = System::Drawing::Size(297, 96);
                this->transformControl1->TabIndex = 2;
                //
                // InspectorWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(470, 262);
                this->Controls->Add(this->transformControl1);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
                this->Name = L"InspectorWindow";
                this->Text = L"Inspector";
                this->ResumeLayout(false);
            }
            #pragma endregion
        private: System::Void onPositionVectorChanged(System::Object  ^sender, System::EventArgs  ^e)
            {
                std::cout << this->transformControl1->Position()->X() << " ," <<  this->transformControl1->Position()->Y() << " ," <<  this->transformControl1->Position()->Z() << "\n";
            }

        private: System::Void onRotationVectorChanged(System::Object  ^sender, System::EventArgs  ^e)
            {
                std::cout << this->transformControl1->Rotation()->X() << " ," <<  this->transformControl1->Rotation()->Y() << " ," <<  this->transformControl1->Rotation()->Z() << "\n";
            }

        private: System::Void onScaleVectorChanged(System::Object  ^sender, System::EventArgs  ^e)
            {
                std::cout << this->transformControl1->Scale()->X() << " ," <<  this->transformControl1->Scale()->Y() << " ," <<  this->transformControl1->Scale()->Z() << "\n";
            }




        public: System::Void onVector3DUserControlChanged(System::Object  ^sender, System::EventArgs  ^e)
            {
                std::cout << this->vector3DControl2->X() << " ," << this->vector3DControl2->Y() << " ," << this->vector3DControl2->Z() << "\n";
            }


    };
}
