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
                    // returns true on old vertex faces erase
                    // meaning previous iterators are invalid
                    bool replaceVertex(std::unordered_map<unsigned int, utils::ProgressiveMeshes::Face *>::iterator &it, Vertex *oldVertex, Vertex *newVertex, const bool calcNormal = true);
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
                    std::unordered_map<unsigned int, Vertex * > neightbors;
                    std::unordered_map<unsigned int, Face * > faces;
                    void removeNonNeighbor(Vertex *v);
            };

            std::map<unsigned int, Vertex * > progVertices;
            std::map<unsigned int, Face * > progFaces;

            float edgeCollapseCost(Vertex *u, Vertex *v);
            void edgeCostAtVertex(Vertex *v);
            // updates u and v neighbors and faces after
            // collapsing the uv edge
            void collapse(Vertex *u, Vertex *v);
            // iterates through all the class vertices returns
            // the one with the minimumn collapse cost
            Vertex *minimumCostEdge();
            // stores normal mesh values into progressive mesh data
            void copyVertices(const std::vector<types::Vertex> &vertices);
            // stores normal mesh values into progressive mesh data
            void copyFaces(const std::vector<types::Face> &faces);
            // deletes a face at iterator <it>, also deletes its vertices
            // and neighbors references to this face returns true if the
            // iterator was invalidated during erase, in which case <it>
            // becomes the <it> next to the deleted element (valid), used
            // in collapse() function
            bool removeProgFace(std::unordered_map<unsigned int, utils::ProgressiveMeshes::Face * >::iterator &it, Vertex *src);

        public:

            ProgressiveMeshes() : minOptimalVertexCount(-1) {};

            int minOptimalVertexCount;
            std::vector<int> candidatesMap;
            std::vector<int> permutations;

            void generateProgressiveMesh(const std::vector<types::Vertex> &vertices, const std::vector<types::Face> &faces);
    };
}

