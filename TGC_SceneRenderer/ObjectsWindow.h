#pragma once
#include "Collections\SceneObjectsCollection.h"
#include "Scene\SceneObject.h"
#include <iostream>

namespace SceneRenderer {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for ObjectsWindow
    /// </summary>
    public ref class ObjectsWindow : public System::Windows::Forms::Form {
        public:
            ObjectsWindow(void)
            {
                InitializeComponent();
                //
                //TODO: Add the constructor code here
                //
                std::unordered_map<unsigned int, scene::SceneObject *>::const_iterator it = collections::SceneObjectsCollection::Instance()->getSceneObjects().begin();

                for (it; it != collections::SceneObjectsCollection::Instance()->getSceneObjects().end(); ++it) {
                    objectRealIndexes.Add((*it).first);
                    this->listView1->Items->Add(gcnew System::String((*it).second->getBase()->objectName.c_str()));
                }
            }

        protected:
            /// <summary>
            /// Clean up any resources being used.
            /// </summary>
            ~ObjectsWindow()
            {
                if (components) {
                    delete components;
                }
            }

        public: unsigned int selectedIndex;
        public: System::Object ^InstancedBy;
        private: System::Collections::ArrayList objectRealIndexes;
        private: System::Windows::Forms::Panel  ^objectsWindowControlPanel;
        private: System::Windows::Forms::MenuStrip  ^createMenu;
        private: System::Windows::Forms::ToolStripMenuItem  ^createToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^searchToolStripTag;
        private: System::Windows::Forms::Panel  ^panel1;
        private: System::Windows::Forms::ToolStripTextBox  ^toolStripTextBox1;
        private: System::Windows::Forms::ToolStripMenuItem  ^cameraToolStripMenuItem;
        private: System::Windows::Forms::ToolStripSeparator  ^toolStripSeparator1;
        private: System::Windows::Forms::ToolStripMenuItem  ^directionalLightToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^spotLightToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^pointLightToolStripMenuItem;
        private: System::Windows::Forms::ToolStripSeparator  ^toolStripSeparator2;
        private: System::Windows::Forms::ToolStripMenuItem  ^cubeToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^sphereToolStripMenuItem;
        private: System::Windows::Forms::ListView  ^listView1;
        private: System::Windows::Forms::ColumnHeader  ^columnHeader1;
        private: System::Windows::Forms::ToolStripMenuItem  ^torusToolStripMenuItem;
        private: System::Windows::Forms::ToolStripMenuItem  ^cylinderToolStripMenuItem;
        private: System::ComponentModel::IContainer  ^components;

        private:
            /// <summary>
            /// Required designer variable.
            /// </summary>


            #pragma region Windows Form Designer generated code
            /// <summary>
            /// Required method for Designer support - do not modify
            /// the contents of this method with the code editor.
            /// </summary>
            void InitializeComponent(void)
            {
                this->objectsWindowControlPanel = (gcnew System::Windows::Forms::Panel());
                this->panel1 = (gcnew System::Windows::Forms::Panel());
                this->listView1 = (gcnew System::Windows::Forms::ListView());
                this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
                this->createMenu = (gcnew System::Windows::Forms::MenuStrip());
                this->createToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->cameraToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
                this->directionalLightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->spotLightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->pointLightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
                this->cubeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->sphereToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->torusToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->cylinderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->searchToolStripTag = (gcnew System::Windows::Forms::ToolStripMenuItem());
                this->toolStripTextBox1 = (gcnew System::Windows::Forms::ToolStripTextBox());
                this->objectsWindowControlPanel->SuspendLayout();
                this->panel1->SuspendLayout();
                this->createMenu->SuspendLayout();
                this->SuspendLayout();
                //
                // objectsWindowControlPanel
                //
                this->objectsWindowControlPanel->Controls->Add(this->panel1);
                this->objectsWindowControlPanel->Controls->Add(this->createMenu);
                this->objectsWindowControlPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                this->objectsWindowControlPanel->Location = System::Drawing::Point(0, 0);
                this->objectsWindowControlPanel->Name = L"objectsWindowControlPanel";
                this->objectsWindowControlPanel->Size = System::Drawing::Size(214, 316);
                this->objectsWindowControlPanel->TabIndex = 0;
                //
                // panel1
                //
                this->panel1->AutoScroll = true;
                this->panel1->Controls->Add(this->listView1);
                this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->panel1->Location = System::Drawing::Point(0, 27);
                this->panel1->Name = L"panel1";
                this->panel1->Size = System::Drawing::Size(214, 289);
                this->panel1->TabIndex = 1;
                //
                // listView1
                //
                this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader ^  >(1) {this->columnHeader1});
                this->listView1->Dock = System::Windows::Forms::DockStyle::Fill;
                this->listView1->Location = System::Drawing::Point(0, 0);
                this->listView1->MultiSelect = false;
                this->listView1->Name = L"listView1";
                this->listView1->Size = System::Drawing::Size(214, 289);
                this->listView1->TabIndex = 0;
                this->listView1->UseCompatibleStateImageBehavior = false;
                this->listView1->View = System::Windows::Forms::View::Details;
                this->listView1->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &ObjectsWindow::listView1_ItemSelectionChanged);
                this->listView1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ObjectsWindow::objectListKeyListener);
                //
                // columnHeader1
                //
                this->columnHeader1->Text = L"Object Name";
                this->columnHeader1->Width = 107;
                //
                // createMenu
                //
                this->createMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(3) {
                    this->createToolStripMenuItem,
                         this->searchToolStripTag, this->toolStripTextBox1
                });
                this->createMenu->Location = System::Drawing::Point(0, 0);
                this->createMenu->Name = L"createMenu";
                this->createMenu->Size = System::Drawing::Size(214, 27);
                this->createMenu->TabIndex = 0;
                this->createMenu->Text = L"menuStrip1";
                //
                // createToolStripMenuItem
                //
                this->createToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(10) {
                    this->cameraToolStripMenuItem,
                         this->toolStripSeparator1, this->directionalLightToolStripMenuItem, this->spotLightToolStripMenuItem, this->pointLightToolStripMenuItem,
                         this->toolStripSeparator2, this->cubeToolStripMenuItem, this->sphereToolStripMenuItem, this->torusToolStripMenuItem, this->cylinderToolStripMenuItem
                });
                this->createToolStripMenuItem->Name = L"createToolStripMenuItem";
                this->createToolStripMenuItem->Size = System::Drawing::Size(53, 23);
                this->createToolStripMenuItem->Text = L"Create";
                //
                // cameraToolStripMenuItem
                //
                this->cameraToolStripMenuItem->Name = L"cameraToolStripMenuItem";
                this->cameraToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->cameraToolStripMenuItem->Text = L"Camera";
                //
                // toolStripSeparator1
                //
                this->toolStripSeparator1->Name = L"toolStripSeparator1";
                this->toolStripSeparator1->Size = System::Drawing::Size(158, 6);
                //
                // directionalLightToolStripMenuItem
                //
                this->directionalLightToolStripMenuItem->Name = L"directionalLightToolStripMenuItem";
                this->directionalLightToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->directionalLightToolStripMenuItem->Text = L"Directional Light";
                //
                // spotLightToolStripMenuItem
                //
                this->spotLightToolStripMenuItem->Name = L"spotLightToolStripMenuItem";
                this->spotLightToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->spotLightToolStripMenuItem->Text = L"Spot Light";
                //
                // pointLightToolStripMenuItem
                //
                this->pointLightToolStripMenuItem->Name = L"pointLightToolStripMenuItem";
                this->pointLightToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->pointLightToolStripMenuItem->Text = L"Point Light";
                //
                // toolStripSeparator2
                //
                this->toolStripSeparator2->Name = L"toolStripSeparator2";
                this->toolStripSeparator2->Size = System::Drawing::Size(158, 6);
                //
                // cubeToolStripMenuItem
                //
                this->cubeToolStripMenuItem->Name = L"cubeToolStripMenuItem";
                this->cubeToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->cubeToolStripMenuItem->Text = L"Cube";
                this->cubeToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectsWindow::cubeToolStripMenuItem_Click);
                //
                // sphereToolStripMenuItem
                //
                this->sphereToolStripMenuItem->Name = L"sphereToolStripMenuItem";
                this->sphereToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->sphereToolStripMenuItem->Text = L"Sphere";
                this->sphereToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectsWindow::sphereToolStripMenuItem_Click);
                //
                // torusToolStripMenuItem
                //
                this->torusToolStripMenuItem->Name = L"torusToolStripMenuItem";
                this->torusToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->torusToolStripMenuItem->Text = L"Torus";
                this->torusToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectsWindow::torusToolStripMenuItem_Click);
                //
                // cylinderToolStripMenuItem
                //
                this->cylinderToolStripMenuItem->Name = L"cylinderToolStripMenuItem";
                this->cylinderToolStripMenuItem->Size = System::Drawing::Size(161, 22);
                this->cylinderToolStripMenuItem->Text = L"Cylinder";
                this->cylinderToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectsWindow::cylinderToolStripMenuItem_Click);
                //
                // searchToolStripTag
                //
                this->searchToolStripTag->Enabled = false;
                this->searchToolStripTag->Name = L"searchToolStripTag";
                this->searchToolStripTag->Size = System::Drawing::Size(57, 23);
                this->searchToolStripTag->Text = L"Search:";
                //
                // toolStripTextBox1
                //
                this->toolStripTextBox1->Name = L"toolStripTextBox1";
                this->toolStripTextBox1->Size = System::Drawing::Size(90, 23);
                //
                // ObjectsWindow
                //
                this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                this->ClientSize = System::Drawing::Size(214, 316);
                this->Controls->Add(this->objectsWindowControlPanel);
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
                this->MainMenuStrip = this->createMenu;
                this->Name = L"ObjectsWindow";
                this->Text = L"Objects";
                this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ObjectsWindow::ObjectsWindow_FormClosing);
                this->objectsWindowControlPanel->ResumeLayout(false);
                this->objectsWindowControlPanel->PerformLayout();
                this->panel1->ResumeLayout(false);
                this->createMenu->ResumeLayout(false);
                this->createMenu->PerformLayout();
                this->ResumeLayout(false);
            }
            #pragma endregion
        private: System::Void ObjectsWindow_FormClosing(System::Object  ^sender, System::Windows::Forms::FormClosingEventArgs  ^e)
            {
                if (e->CloseReason == System::Windows::Forms::CloseReason::UserClosing) {
                    this->Hide();
                    e->Cancel = true;
                }
            }

        private: System::Void listView1_ItemSelectionChanged(System::Object  ^sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs  ^e);

        public: void addCube()
            {
                std::string objectName = collections::SceneObjectsCollection::Instance()->addMesh(core::StoredMeshes::Cube)->base->objectName;
                this->listView1->Items->Add(gcnew System::String(objectName.c_str()));
                this->objectRealIndexes.Add(collections::SceneObjectsCollection::Instance()->getLastObjectIndex());
            }

        public: void addSphere()
            {
                std::string objectName = collections::SceneObjectsCollection::Instance()->addMesh(core::StoredMeshes::Sphere)->base->objectName;
                this->listView1->Items->Add(gcnew System::String(objectName.c_str()));
                this->objectRealIndexes.Add(collections::SceneObjectsCollection::Instance()->getLastObjectIndex());
            }

        public: void addTorus()
            {
                std::string objectName = collections::SceneObjectsCollection::Instance()->addMesh(core::StoredMeshes::Torus)->base->objectName;
                this->listView1->Items->Add(gcnew System::String(objectName.c_str()));
                this->objectRealIndexes.Add(collections::SceneObjectsCollection::Instance()->getLastObjectIndex());
            }

        public: void addCylinder()
            {
                std::string objectName = collections::SceneObjectsCollection::Instance()->addMesh(core::StoredMeshes::Cylinder)->base->objectName;
                this->listView1->Items->Add(gcnew System::String(objectName.c_str()));
                this->objectRealIndexes.Add(collections::SceneObjectsCollection::Instance()->getLastObjectIndex());
            }

        public: void addedObject()
            {
                std::string objectName = collections::SceneObjectsCollection::Instance()->getSceneObject(collections::SceneObjectsCollection::Instance()->getLastObjectIndex())->getBase()->objectName;
                this->listView1->Items->Add(gcnew System::String(objectName.c_str()));
                this->objectRealIndexes.Add(collections::SceneObjectsCollection::Instance()->getLastObjectIndex());
            }

        private: System::Void cubeToolStripMenuItem_Click(System::Object  ^sender, System::EventArgs  ^e)
            {
                addCube();
            }
        private: System::Void sphereToolStripMenuItem_Click(System::Object  ^sender, System::EventArgs  ^e)
            {
                addSphere();
            }
        private: System::Void torusToolStripMenuItem_Click(System::Object  ^sender, System::EventArgs  ^e)
            {
                addTorus();
            }
        private: System::Void cylinderToolStripMenuItem_Click(System::Object  ^sender, System::EventArgs  ^e)
            {
                addCylinder();
            }
        private: System::Void objectListKeyListener(System::Object  ^sender, System::Windows::Forms::KeyEventArgs  ^e)
            {
                if (e->KeyCode == System::Windows::Forms::Keys::Delete && this->listView1->SelectedIndices[0] != -1) {
                    unsigned int realIndex = (unsigned int)this->objectRealIndexes[this->listView1->SelectedIndices[0]];
                    collections::SceneObjectsCollection::Instance()->remove(realIndex);  // wrap around int() to create a temporary instance
                    this->objectRealIndexes.RemoveAt(this->listView1->SelectedIndices[0]);
                    this->listView1->Items->RemoveAt(this->listView1->SelectedIndices[0]);
                }
            }
    };
}
