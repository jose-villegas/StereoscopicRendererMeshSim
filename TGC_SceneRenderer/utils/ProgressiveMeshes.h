#pragma once
#include "..\types\Vertex.h"
#include "..\types\Face.h"
#include <map>
#include <unordered_map>

namespace utils {

    class ProgressiveMeshes {
        private:

            class Face;
            class Vertex;

            class Face : public types::Face {
                public:
                    // instances new face and associate vertex neighbors and vertex's faces with this instance
                    Face(const unsigned int id, Vertex *v1, Vertex *v2, Vertex *v3, const types::Face &nonProgFace);
                    ~Face();

                    std::array <Vertex *, 3> vertices;
                    unsigned int mappingId;
                    void replaceVertex(Vertex *oldVertex, Vertex *newVertex, const bool calcNormal = true);
                    void calculateNormal();
                    bool hasVertex(types::Vertex *v);
            };

            class Vertex : public types::Vertex {
                public:
                    // instances new vertex
                    Vertex(const unsigned int id, const types::Vertex &nonProgVert);
                    // deletes vertex and neighbor associations with this vertex
                    ~Vertex();

                    unsigned int mappingId;
                    unsigned int originalPlace;
                    float collapseCost;
                    Vertex *collapseCandidate;
                    std::unordered_map<unsigned int, Vertex *> neightbors;
                    std::unordered_map<unsigned int, Face *> faces;
                    void removeNonNeighbor(Vertex *v);
            };

            std::map<unsigned int, Vertex *> progVertices;
            std::map<unsigned int, Face *> progFaces;

            float edgeCollapseCost(Vertex *u, Vertex *v);
            void edgeCostAtVertex(Vertex *v);
            void collapse(Vertex *u, Vertex *v);
            Vertex *minimumCostEdge();

            void removeVertex(Vertex *v);
            void removeFace(Face *f);
            void copyVertices(const std::vector<types::Vertex> &vertices);
            void copyFaces(const std::vector<types::Face> &faces);

        public:

            ProgressiveMeshes() : minOptimalVertexCount(-1) {};

            int minOptimalVertexCount;
            std::vector<int> candidatesMap;
            std::vector<int> permutations;

            void generateProgressiveMesh(const std::vector<types::Vertex> &vertices, const std::vector<types::Face> &faces);
    };
}

