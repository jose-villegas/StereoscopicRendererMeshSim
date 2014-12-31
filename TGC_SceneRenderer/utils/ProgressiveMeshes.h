#pragma once
#include "..\types\Vertex.h"
#include "..\types\Face.h"
#include <unordered_map>
#include <map>
#include <set>

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

                    struct VertexPtrCostComp {
                        bool operator()(const Vertex *lhs, const Vertex *rhs) const  { return lhs->collapseCost < rhs->collapseCost; }
                    };

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
            // maps vertex correctly with collpase order
            unsigned int mapVertexCollapse(const unsigned int a, const unsigned int b);

        public:

            ProgressiveMeshes() : levelOfDetailBase(0.5f), morphingFactor(1.0f), vertexCount(-1) {};

            float morphingFactor;
            float levelOfDetailBase;
            unsigned int vertexCount;
            unsigned int polyCount;
            std::vector<int> candidatesMap;
            std::vector<int> permutations;

            // generates progmeshes data structures (permutations and candidatesMap)
            // based on a initial mesh mesh model and progressive collapse iterations
            void generateProgressiveMesh(const std::vector<types::Vertex> &vertices, const std::vector<types::Face> &faces);
            // reorder vertices indices and faces input based on
            // permutation order for future prog meshes iteratios
            void permuteVertices(std::vector<types::Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<types::Face> &faces);
            // outputs reordered and resized vertices, faces and indices structure based on vertexCount, vertices,
            // indices and faces need to be reorder previously by permuteVertices, final output is the simplified mesh data
            struct ReducedMesh {
                std::vector<types::Vertex> vertices;
                std::vector<unsigned int> indices;
                std::vector<types::Face> faces;
            };
            ReducedMesh *reorderVertices(const std::vector<types::Vertex> &vertices,
                                         const std::vector<unsigned int> &indices,
                                         const std::vector<types::Face> &faces,
                                         const int vertexCount);

    };
}

