#include "ProgressiveMeshes.h"
#include "glm\gtc\quaternion.hpp"
#include <algorithm>
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
    if (oldVertex->faces.find(this->id) == newVertex->faces.end()) { return; }

    oldVertex->faces.erase(this->id);

    // add to the new vertex this face as an associate
    if (newVertex->faces.find(this->id) != newVertex->faces.end()) { return; }

    newVertex->faces[this->id] = this;
    // remove non-neighbors
    std::for_each(this->vertices.begin(), this->vertices.end(), [oldVertex](Vertex * vertex) {
        oldVertex->removeNonNeighbor(vertex);
        vertex->removeNonNeighbor(oldVertex);
    });

    // update face vertices neighbors
    for (int i = 0; i < this->vertices.size(); i++) {
        if (this->vertices[i]->faces.find(this->id) == this->vertices[i]->faces.end()) { return; }

        for (int j = 0; j < this->vertices.size(); j++) {
            if (j != i) {
                if (this->vertices[i]->neightbors.find(this->vertices[j]->id) != this->vertices[i]->neightbors.end()) { break; }

                this->vertices[i]->neightbors[this->vertices[j]->id] = this->vertices[j];
            }
        }
    }

    // recalculate face normal
    if (calcNormal) { this->calculateNormal(); }
}

utils::ProgressiveMeshes::Face::Face(const unsigned int id, Vertex *v1, Vertex *v2, Vertex *v3)
{
    this->id = id;
    this->vertices[0] = v1; this->vertices[1] = v2; this->vertices[2] = v3;
}

void utils::ProgressiveMeshes::Vertex::removeNonNeighbor(Vertex *v)
{
    // this vertex isn't stored
    if (this->neightbors.find(v->id) == this->neightbors.end()) { return; }

    for (auto it = this->faces.begin(); it != this->faces.end(); it++) {
        if ((*it).second->hasVertex(v)) { return; }
    }

    this->neightbors.erase(v->id);
}

utils::ProgressiveMeshes::Vertex::Vertex(const unsigned int id, const types::Vertex &nonProgVert) : types::Vertex(nonProgVert)
{
    this->id = id;
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

void utils::ProgressiveMeshes::collapse(Vertex *u, Vertex *v)
{
    if (!v) { delete u; return; }

    std::unordered_map<unsigned int, Vertex *> tmpNeighbors = u->neightbors;

    // delete triangles on edge of uv
    for (auto it = u->faces.rbegin(); it != u->faces.rend(); it++) {
        if ((*it).second->hasVertex(v)) {
            delete(*it).second;
        }
    }

    // update rest of the triangles accordily to deleted triangle
    for (auto it = u->faces.rbegin(); it != u->faces.rend(); it++) {
        (*it).second->replaceVertex(u, v);
    }

    delete u;

    // recalculate edge cost with new neighbors
    for (auto it = tmpNeighbors.begin(); it != tmpNeighbors.end(); it++) {
        edgeCostAtVertex((*it).second);
    }
}

utils::ProgressiveMeshes::Vertex *utils::ProgressiveMeshes::minimumCostEdge()
{
    Vertex *minCostVertex = vertices[0];

    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        if ((*it)->collapseCost < minCostVertex->collapseCost) {
            minCostVertex = *it;
        }
    }

    return minCostVertex;
}

void utils::ProgressiveMeshes::copyVertices(const std::vector<types::Vertex *> &vertices)
{
    int index = 0;

    for (std::vector<types::Vertex *>::const_iterator it = vertices.begin(); it < vertices.end(); it++, index++) {
        this->vertices.push_back(new Vertex(index, *(*it)));
    }
}

void utils::ProgressiveMeshes::copyFaces(const std::vector<types::Face *> &faces)
{
    int index = 0;

    for (std::vector<types::Face *>::const_iterator it = faces.begin(); it < faces.end(); it++, index++) {
        Face *newFace = new Face(index, vertices[(*it)->indices[0]], vertices[(*it)->indices[1]], vertices[(*it)->indices[2]]);
    }
}

void utils::ProgressiveMeshes::generateProgressiveMesh(
    const std::vector<types::Vertex *> &vertices,
    const std::vector<types::Face *> &faces,
    std::vector<int> &outMap,
    std::vector<int> &outPermutation
)
{
    copyVertices(vertices); copyFaces(faces);
}
