#pragma once
#include "..\types\Vertex.h"
#include "..\types\Face.h"
#include <unordered_map>

namespace utils {

    class ProgressiveMeshes {
        private:

            class Face;
            class Vertex;

            class Face : public types::Face {
                public:
                    Face() {};
                    Face(const unsigned int id, Vertex *v1, Vertex *v2, Vertex *v3);

                    std::array <Vertex *, 3> vertices;
                    unsigned int id;
                    void replaceVertex(Vertex *oldVertex, Vertex *newVertex, const bool calcNormal = true);
            };

            class Vertex : public types::Vertex {
                public:
                    Vertex(const types::Vertex &nonProgVert) : types::Vertex(nonProgVert) {};
                    Vertex(const unsigned int id, const types::Vertex &nonProgVert);

                    unsigned int id;
                    unsigned int originalPlace;
                    float collapseCost;
                    Vertex *collapseCandidate;
                    std::unordered_map<unsigned int, Vertex *> neightbors;
                    std::unordered_map<unsigned int, Face *> faces;
                    void removeNonNeighbor(Vertex *v);
            };

            float edgeCollapseCost(Vertex *u, Vertex *v);
            void edgeCostAtVertex(Vertex *v);
            void collapse(Vertex *u, Vertex *v);
            Vertex *minimumCostEdge();

            void copyVertices(const std::vector<types::Vertex *> &vertices);
            void copyFaces(const std::vector<types::Face *> &faces);

        public:

            std::vector<Vertex *> vertices;
            std::vector<Face *> faces;

            void generateProgressiveMesh(
                const std::vector<types::Vertex *> &vertices,
                const std::vector<types::Face *> &faces,
                std::vector<int> &outMap,
                std::vector<int> &outPermutation
            );
    };
}

