#include "ProgressiveMeshes.h"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\constants.hpp"
#include <algorithm>
#include <iostream>
#include "..\core\Data.h"
using namespace utils;

void utils::ProgressiveMeshes::Face::replaceVertex(Vertex *oldVertex, Vertex *newVertex, const bool calcNormal /* = true */)
{
    if (!(oldVertex && newVertex) || !this->hasVertex(oldVertex) || this->hasVertex(newVertex)) { return; }

    if (oldVertex == vertices[0]) {
        vertices[0] = newVertex;
    } else if (oldVertex == vertices[1]) {
        vertices[1] = newVertex;
    } else if (oldVertex == vertices[2]) {
        vertices[2] = newVertex;
    }

    // erase in the old vertex this face association
    if (oldVertex->faces.find(this->mappingId) == newVertex->faces.end()) { return; }

    oldVertex->faces.erase(this->mappingId);

    // add to the new vertex this face as an associate
    if (newVertex->faces.find(this->mappingId) != newVertex->faces.end()) { return; }

    newVertex->faces[this->mappingId] = this;
    // remove non-neighbors
    std::for_each(this->vertices.begin(), this->vertices.end(), [oldVertex](Vertex * vertex) {
        oldVertex->removeNonNeighbor(vertex);
        vertex->removeNonNeighbor(oldVertex);
    });

    // update face vertices neighbors
    for (unsigned int i = 0; i < 3; i++) {
        if (this->vertices[i]->faces.find(this->mappingId) == this->vertices[i]->faces.end()) { return; }

        for (unsigned int j = 0; j < 3; j++) {
            if (j != i) {
                if (this->vertices[i]->neightbors.find(this->vertices[j]->mappingId) != this->vertices[i]->neightbors.end()) { break; }

                this->vertices[i]->neightbors[this->vertices[j]->mappingId] = this->vertices[j];
            }
        }
    }

    // recalculate face normal
    if (calcNormal) { this->calculateNormal(); }
}

utils::ProgressiveMeshes::Face::Face(const unsigned int id, Vertex *v1, Vertex *v2, Vertex *v3, const types::Face &nonProgFace) : types::Face(nonProgFace)
{
    this->mappingId = id;
    this->vertices[0] = v1; this->vertices[1] = v2; this->vertices[2] = v3;
    // calculate face normal
    this->calculateNormal();

    // add associate face vertices with this face
    for (unsigned int i = 0; i < 3; i++) {
        this->vertices[i]->faces[this->mappingId] = this;

        // associate vertices neighbors
        for (unsigned int j = 0; j < 3; j++) {
            if (i != j) {
                auto it = this->vertices[i]->neightbors.find(this->vertices[j]->mappingId);

                if (it == this->vertices[i]->neightbors.end()) {
                    this->vertices[i]->neightbors[this->vertices[j]->mappingId] = this->vertices[j];
                }
            }
        }
    }
}

utils::ProgressiveMeshes::Vertex::Vertex(const unsigned int id, const types::Vertex &nonProgVert) : types::Vertex(nonProgVert)
{
    this->mappingId = id;
    this->originalPlace = id;
}

void utils::ProgressiveMeshes::Face::calculateNormal()
{
    glm::vec3 v0 = this->vertices[0]->position;
    glm::vec3 v1 = this->vertices[1]->position;
    glm::vec3 v2 = this->vertices[2]->position;
    normal = (v1 - v0) * (v2 - v1);

    if (glm::length(normal) == 0) { return; }

    normal = glm::normalize(normal);
}

bool utils::ProgressiveMeshes::Face::hasVertex(types::Vertex *v)
{
    return (v == vertices[0] || v == vertices[1] || v == vertices[2]);
}

void utils::ProgressiveMeshes::Vertex::removeNonNeighbor(Vertex *v)
{
    // this vertex isn't stored
    if (this->neightbors.find(v->mappingId) == this->neightbors.end()) { return; }

    for (auto it = this->faces.begin(); it != this->faces.end(); it++) {
        if ((*it).second->hasVertex(v)) { return; }
    }

    this->neightbors.erase(v->mappingId);
}

utils::ProgressiveMeshes::Vertex::~Vertex()
{
    if (this->faces.empty()) { return; }

    for (auto it = this->neightbors.begin(); it != this->neightbors.end(); it++) {
        (*it).second->neightbors.erase(this->mappingId);
    }

    this->neightbors.clear();
}

utils::ProgressiveMeshes::Face::~Face()
{
    // remove association with this face at face vertices
    for (int i = 0; i < 3; i++) {
        if (this->vertices[i]) { this->vertices[i]->faces.erase(this->mappingId); }
    }

    // update vertices neighbors
    for (int i = 0; i < 3; i++) {
        int j = (i + 1) % 3;

        if (this->vertices[i] && this->vertices[j]) {
            this->vertices[i]->removeNonNeighbor(this->vertices[j]);
            this->vertices[j]->removeNonNeighbor(this->vertices[i]);
        }
    }
}

float utils::ProgressiveMeshes::edgeCollapseCost(Vertex *u, Vertex *v)
{
    float edgeLength, curvature = 0.f;
    edgeLength = glm::length(v->position - u->position);
    // find faces associated with uv edge
    std::unordered_map<unsigned int, Face *> sides;

    for (auto it = u->faces.begin(); it != u->faces.end(); it++) {
        if ((*it).second->hasVertex(v)) { sides[(*it).first] = (*it).second; }
    }

    // use face facing most away from associated faces
    for (auto it = u->faces.begin(); it != u->faces.end(); it++) {
        float minCurvature = 1.f;

        // determine the minimum normal curvature between faces
        for (auto itSides = sides.begin(); itSides != sides.end(); itSides++) {
            float dotProduct = glm::dot((*it).second->normal, (*itSides).second->normal);
            minCurvature = glm::min(minCurvature, (1.f - dotProduct) / 2.f);
        }

        curvature = glm::max(curvature, minCurvature);
    }

    return edgeLength * curvature;
}

void utils::ProgressiveMeshes::edgeCostAtVertex(Vertex *v)
{
    if (v->neightbors.empty()) {
        v->collapseCandidate = nullptr;
        v->collapseCost = 0.01f;
        return;
    }

    v->collapseCost = std::numeric_limits<float>::infinity();
    v->collapseCandidate = nullptr;

    // search for the least cost edge
    for (auto it = v->neightbors.begin(); it != v->neightbors.end(); it++) {
        float currentCost = edgeCollapseCost(v, (*it).second);

        if (currentCost < v->collapseCost) {
            v->collapseCandidate = (*it).second;
            v->collapseCost = currentCost;
        }
    }
}

void utils::ProgressiveMeshes::removeVertex(Vertex *v)
{
    int mappingId = v->mappingId;
    this->progVertices.erase(mappingId);
}

void utils::ProgressiveMeshes::removeFace(Face *f)
{
    int mappingId = f->mappingId;
    this->progFaces.erase(mappingId);
}


void utils::ProgressiveMeshes::collapse(Vertex *u, Vertex *v)
{
    if (!v) { removeVertex(u); return; }

    std::unordered_map<unsigned int, Vertex *> tmpNeighbors = u->neightbors;

    // delete triangles on edge of uv
    for (auto it = u->faces.rbegin(); it != u->faces.rend(); ++it) {
        if ((*it).second->hasVertex(v)) {
            removeFace((*it).second);
        }
    }

    // update rest of the triangles accordily to deleted triangle
    for (auto it = u->faces.begin(); it != u->faces.end(); it++) {
        (*it).second->replaceVertex(u, v);
    }

    removeVertex(u);

    // recalculate edge cost with new neighbors
    for (auto it = tmpNeighbors.begin(); it != tmpNeighbors.end(); it++) {
        edgeCostAtVertex((*it).second);
    }
}

utils::ProgressiveMeshes::Vertex *utils::ProgressiveMeshes::minimumCostEdge()
{
    Vertex *minCostVertex = (*(progVertices.begin())).second;
    // the find lowest cost vertex to be deleted meaning
    // this vertex will affect less the final model
    auto it = std::find_if(progVertices.begin(), progVertices.end(),
    [&minCostVertex](const std::pair<unsigned int, Vertex *> &index) {
        // found smaller cost so swap vertex candidate
        if (index.second->collapseCost < minCostVertex->collapseCost) {
            minCostVertex = index.second;

            // early loop break if the collapse cost is zero or really close to zero
            if (minCostVertex->collapseCost > -glm::epsilon<float>() && minCostVertex->collapseCost < glm::epsilon<float>()) {
                return true;
            }
        }

        return false;
    });

    if (this->minOptimalVertexCount < 0 && it == progVertices.end()) { this->minOptimalVertexCount = this->progVertices.size(); }

    return minCostVertex;
}

void utils::ProgressiveMeshes::copyVertices(const std::vector<types::Vertex> &vertices)
{
    int index = 0;

    for (std::vector<types::Vertex>::const_iterator it = vertices.begin(); it < vertices.end(); it++, index++) {
        this->progVertices[index] = new Vertex(index, *it);
    }
}

void utils::ProgressiveMeshes::copyFaces(const std::vector<types::Face> &faces)
{
    int index = 0;

    for (std::vector<types::Face>::const_iterator it = faces.begin(); it < faces.end(); it++, index++) {
        Face *newFace = new Face(index, progVertices[(*it).indices[0]], progVertices[(*it).indices[1]], progVertices[(*it).indices[2]], *it);
        this->progFaces[index] = newFace;
    }
}

void utils::ProgressiveMeshes::generateProgressiveMesh(const std::vector<types::Vertex> &vertices, const std::vector<types::Face> &faces)
{
    copyVertices(vertices); copyFaces(faces);
    // allocate space for output
    this->candidatesMap.resize(this->progVertices.size());
    this->permutations.resize(this->progVertices.size());

    for (auto it = this->progVertices.begin(); it != this->progVertices.end(); it++) {
        edgeCostAtVertex((*it).second);
    }

    while (!this->progVertices.empty()) {
        Vertex *mnimum = minimumCostEdge();
        // track of the collapse vertices order
        this->permutations[mnimum->mappingId] = this->progVertices.size() - 1;
        // track of the collapse candidate
        this->candidatesMap[this->progVertices.size() - 1] = mnimum->collapseCandidate != nullptr ? mnimum->collapseCandidate->mappingId : -1;
        this->collapse(mnimum, mnimum->collapseCandidate);
    }
}
