#include "Matrices.h"
using namespace core;

Matrices::Matrices(void)
{
    view = modelView = model = modelViewProjection = projection = glm::mat4(1.0f);
    normal = glm::mat3(1.0f);
}
