#pragma once
#include <unordered_map>
#include "..\Scene\BaseObject.h"
namespace ECollections {

    ref class SceneObjects {
        public:
            static SceneObjects ^Instance();
            SceneObjects(void);
            ~SceneObjects();
            // Binding List for ObjectsWindow
            System::ComponentModel::BindingList<System::String ^> ^objectsWindowBindingList;

        private:
            static SceneObjects ^_eInstance;
            System::Collections::Hashtable ^_sceneInstancedObjects;
    };

}
