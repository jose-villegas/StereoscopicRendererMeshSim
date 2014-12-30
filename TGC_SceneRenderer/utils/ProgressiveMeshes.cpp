#include "ProgressiveMeshes.h"
#include "..\core\Data.h"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\constants.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
using namespace utils;

bool utils::ProgressiveMeshes::Face::replaceVertex(std::unordered_map<unsigned int, utils::ProgressiveMeshes::Face *>::iterator &it, Vertex *oldVertex, Vertex *newVertex,
        const bool calcNormal /* = true */)
{
    if (!(oldVertex && newVertex) || !this->hasVertex(oldVertex) || this->hasVertex(newVertex)) { return false; }

    if (oldVertex == vertices[0]) {
        vertices[0] = newVertex;
    } else if (oldVertex == vertices[1]) {
        vertices[1] = newVertex;
    } else if (oldVertex == vertices[2]) {
        vertices[2] = newVertex;
    }

    // erase in the old vertex this face association
    it = oldVertex->faces.erase(oldVertex->faces.find(this->mappingId));
    // add to the new vertex this face as an associate
    newVertex->faces[this->mappingId] = this;
    // remove non-neighbors
    std::for_each(this->vertices.begin(), this->vertices.end(), [oldVertex](Vertex * vertex) {
        oldVertex->removeNonNeighbor(vertex);
        vertex->removeNonNeighbor(oldVertex);
    });

    // update face vertices neighbors
    for (unsigned int i = 0; i < 3; i++) {
        if (this->vertices[i]->faces.find(this->mappingId) == this->vertices[i]->faces.end()) { return true; }

        for (unsigned int j = 0; j < 3; j++) {
            if (j != i) {
                //if (this->vertices[i]->neightbors.find(this->vertices[j]->mappingId) == this->vertices[i]->neightbors.end())
                this->vertices[i]->neightbors[this->vertices[j]->mappingId] = this->vertices[j];
            }
        }
    }

    // recalculate face normal
    if (calcNormal) { this->calculateNormal(); }

    return true;
}

utils::ProgressiveMeshes::Face::Face(const unsigned int id, Vertex *v1, Vertex *v2, Vertex *v3, const types::Face &nonProgFace) : types::Face(nonProgFace)
{
    this->mappingId = id;
    this->vertices[0] = v1;
    this->vertices[1] = v2;
    this->vertices[2] = v3;
    // calculate face normal
    this->calculateNormal();

    // add associate face vertices with this face
    for (int i = 0; i < 3; i++) {
        this->vertices[i]->faces[this->mappingId] = this;

        // associate vertices neighbors
        for (int j = 0; j < 3; j++) {
            if (i != j) { this->vertices[i]->neightbors[this->vertices[j]->mappingId] = this->vertices[j]; }
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

    for (auto it = this->faces.begin(); it != this->faces.end(); ++it) {
        if ((*it).second->hasVertex(v)) { return; }
    }

    this->neightbors.erase(v->mappingId);
}

utils::ProgressiveMeshes::Vertex::~Vertex()
{
    if (this->faces.empty()) { return; }

    while (!this->neightbors.empty()) {
        (*this->neightbors.begin()).second->neightbors.erase(this->mappingId);
        this->neightbors.erase(this->neightbors.begin());
    }
}

utils::ProgressiveMeshes::Face::~Face()
{
    this->vertices[0] = nullptr; this->vertices[1] = nullptr; this->vertices[2] = nullptr;
}

float utils::ProgressiveMeshes::edgeCollapseCost(Vertex *u, Vertex *v)
{
    float edgeLength, curvature = 0.f;
    edgeLength = glm::length(v->position - u->position);
    // find faces associated with uv edge
    std::unordered_map<unsigned int, Face *> sides;

    for (auto it = u->faces.begin(); it != u->faces.end(); ++it) {
        if ((*it).second->hasVertex(v)) { sides[(*it).first] = (*it).second; }
    }

    // use face facing most away from associated faces
    for (auto it = u->faces.begin(); it != u->faces.end(); ++it) {
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
    for (auto it = v->neightbors.begin(); it != v->neightbors.end(); ++it) {
        float currentCost = edgeCollapseCost(v, (*it).second);

        if (currentCost < v->collapseCost) {
            v->collapseCandidate = (*it).second;
            v->collapseCost = currentCost;
        }
    }
}

bool  utils::ProgressiveMeshes::removeProgFace(std::unordered_map<unsigned int, utils::ProgressiveMeshes::Face *>::iterator &it, Vertex *src)
{
    Face *ptr = (*it).second;
    bool iteratorInvalidated = false;

    // remove association with this face at face vertices
    for (int i = 0; i < 3; i++) {
        // if we found this vertice is the src we need to replace
        // the iterator, erase operation invalidates all class iterators
        if (!ptr->vertices[i]) { continue; }

        auto faceIt  = ptr->vertices[i]->faces.find(ptr->mappingId);

        if (faceIt == ptr->vertices[i]->faces.end()) { continue; }

        if (ptr->vertices[i] && ptr->vertices[i] == src) {
            it = ptr->vertices[i]->faces.erase(faceIt);
            iteratorInvalidated = true;
        } else if (ptr->vertices[i]) {
            ptr->vertices[i]->faces.erase(faceIt); // else just erase normally
        }
    }

    // update vertices neighbors
    for (int i = 0; i < 3; i++) {
        int j = (i + 1) % 3;

        if (ptr->vertices[i] && ptr->vertices[j]) {
            ptr->vertices[i]->removeNonNeighbor(ptr->vertices[j]);
            ptr->vertices[j]->removeNonNeighbor(ptr->vertices[i]);
        }
    }

    // delete face reserved memory
    delete ptr;
    // delete face reference from
    // progressivemeshes class collection
    this->progFaces.erase(ptr->mappingId);
    // return iterator status
    return iteratorInvalidated;
}

void utils::ProgressiveMeshes::collapse(Vertex *u, Vertex *v)
{
    if (!v) {
        this->progVertices.erase(u->mappingId);
        delete u;;
        return;
    }

    std::unordered_map<unsigned int, Vertex *> tmpNeighbors = u->neightbors;
    // deletes the face triangle from the progmesh
    // and updates vertices and neighbors accordly
    auto it = u->faces.begin();

    while (it != u->faces.end()) {
        if ((*it).second->hasVertex(v)) {
            removeProgFace(it, u) ? it : ++it;
        } else { ++it; }
    }

    // update rest of the triangles and vertices
    // accordily to deleted triangle
    it = u->faces.begin();

    while (it != u->faces.end()) {
        (*it).second->replaceVertex(it, u, v) ?  it : ++it;
    }

    this->progVertices.erase(u->mappingId);
    delete u;

    // recalculate edge cost with new neighbors
    for (auto it = tmpNeighbors.begin(); it != tmpNeighbors.end(); ++it) {
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
    return minCostVertex;
}

void utils::ProgressiveMeshes::copyVertices(const std::vector<types::Vertex> &vertices)
{
    int index = 0;

    for (std::vector<types::Vertex>::const_iterator it = vertices.begin(); it < vertices.end(); ++it) {
        this->progVertices[index++] = new Vertex(index, *it); // starts from 1
    }
}

void utils::ProgressiveMeshes::copyFaces(const std::vector<types::Face> &faces)
{
    int index = 0;

    for (std::vector<types::Face>::const_iterator it = faces.begin(); it < faces.end(); ++it) {
        this->progFaces[index++] = new Face(index, progVertices[(*it).indices[0]], progVertices[(*it).indices[1]], progVertices[(*it).indices[2]], *it);;
    }
}

void utils::ProgressiveMeshes::generateProgressiveMesh(const std::vector<types::Vertex> &vertices, const std::vector<types::Face> &faces)
{
    vertexCount = vertices.size();
    polyCount = faces.size();
    // copy regular mesh values to prog mesh structures
    copyVertices(vertices); copyFaces(faces);
    // allocate space for output
    this->candidatesMap.resize(this->progVertices.size());
    this->permutations.resize(this->progVertices.size());

    for (auto it = this->progVertices.begin(); it != this->progVertices.end(); ++it) {
        edgeCostAtVertex((*it).second);
    }

    while (!this->progVertices.empty()) {
        Vertex *mnimum = minimumCostEdge();
        // track of the collapse vertices order
        this->permutations[mnimum->mappingId] = this->progVertices.size() - 1;
        // track of the collapse candidate
        this->candidatesMap[this->progVertices.size() - 1] = mnimum->collapseCandidate != nullptr ? mnimum->collapseCandidate->mappingId : -1;
        // collapse with candidate and reorganize progressivemesh
        this->collapse(mnimum, mnimum->collapseCandidate);
    }

    for (auto it = this->candidatesMap.begin(); it != this->candidatesMap.end(); ++it) {
        (*it) = (*it) == -1 ? 0 : this->permutations[(*it)];
    }

    std::cout << ";";
}

void utils::ProgressiveMeshes::permuteVertices(std::vector<types::Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<types::Face> &faces)
{
    if (permutations.empty()) { return; }

    std::vector<types::Vertex> tmpVerts = vertices;

    // reorder vertices by permutation
    for (unsigned int i = 0; i < vertices.size(); i++) {
        vertices[permutations[i]] = tmpVerts[i];
    }

    // update face indices respectively
    for (unsigned int i = 0; i < faces.size(); i++) {
        for (int j = 0; j < 3; j++) {
            faces[i].indices[j] = permutations[faces[i].indices[j]];
            faces[i].vertices[j] = &vertices[faces[i].indices[j]];
            indices[i * 3 + j] = faces[i].indices[j];
        }
    }
}

unsigned int utils::ProgressiveMeshes::mapVertexCollapse(const int a, const int b)
{
    if (b <= 0) { return 0; }

    unsigned int result = a;

    while (result >= b) {
        result = candidatesMap[result];
    }

    return result;
}

void utils::ProgressiveMeshes::reorderVertices(std::vector<types::Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<types::Face> &faces, const int vertexCount)
{
    if (vertexCount <= 0) { return; }

    std::vector<types::Vertex> verticesResult(vertexCount);
    std::vector<types::Face> facesResult;
    std::vector<unsigned int> indicesResult;
    int point[3], qpoint[3];
    types::Vertex vertex[3];

    for (int i = 0; i < faces.size(); i++) {
        for (int j = 0; j < 3; j++) {
            point[j] = mapVertexCollapse(faces[i].indices[j], vertexCount);
        }

        if (point[0] == point[1] || point[1] == point[2] || point[2] == point[0]) { continue; }

        facesResult.push_back(faces[i]);

        for (int j = 0; j < 3; j++) {
            vertex[j] = vertices[point[j]];
            indicesResult.push_back(faces[i].indices[j]);
            verticesResult[point[j]] = vertex[j];
        }
    }

    vertices.clear();
    faces.clear();
    indices.clear();
    vertices = verticesResult;
    faces = facesResult;
    indices = indicesResult;
}
