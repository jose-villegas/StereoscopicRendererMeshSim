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
                    std::array <Vertex *, 3> vertices;
                    unsigned int id;
                    void replaceVertex(Vertex *oldVertex, Vertex *newVertex, const bool calcNormal = true);
            };

            class Vertex : public types::Vertex {
                public:
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

        public:
    };
}

