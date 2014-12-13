#pragma once
namespace ECollections {

    ref class SceneObjects {
        public:
            // Creates Unique Static Instace
            static SceneObjects ^Instance();
            SceneObjects(void);
            ~SceneObjects();
            // Binding List for ObjectsWindow
            System::ComponentModel::BindingList<System::String ^> ^objectsWindowBindingList;

        private:
            // Unique Static Instance
            static SceneObjects ^_eInstance;
            System::Collections::Hashtable ^_sceneInstancedObjects;
    };

}
