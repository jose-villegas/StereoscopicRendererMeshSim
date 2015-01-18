#pragma once
namespace bases {

    // prototype class for all objects that implement some sort of rendering method
    class Renderable {
        public:
            Renderable(void);
            virtual ~Renderable(void);
            // render base method free
            // to implement in child objects
            virtual void render() {};
            virtual void render(const float deltaTime) {};
            virtual void render(const bool debugMode) {};
            virtual void render(const bool debugMode, const int volumeType) {};
            virtual void render(const bool positions, const bool uvs, const bool normals, const bool tangents, const bool bitangents, const bool disableShaders = false) {};
    };
}

