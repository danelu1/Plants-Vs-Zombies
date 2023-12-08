#include "lab_m1/Tema2/object3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object3D::CreatePyramid(const std::string& name,
    glm::vec3 leftBottomCorner,
    float bigBase1,
    float bigBase2,
    float smallBase1,
    float smallBase2,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(- bigBase1 / 2, - height / 2, bigBase2 / 2), color),
        VertexFormat(corner + glm::vec3(bigBase1 / 2, - height / 2, bigBase2 / 2), color),
        VertexFormat(corner + glm::vec3(bigBase1 / 2, -height / 2, - bigBase2 / 2), color),
        VertexFormat(corner + glm::vec3(- bigBase1 / 2, -height / 2, - bigBase2 / 2), color),
        VertexFormat(corner + glm::vec3(- smallBase1 / 2, height / 2, smallBase2 / 2), color),
        VertexFormat(corner + glm::vec3(smallBase1 / 2, height / 2, smallBase2 / 2), color),
        VertexFormat(corner + glm::vec3(smallBase1 / 2, height / 2, - smallBase2 / 2), color),
        VertexFormat(corner + glm::vec3(- smallBase1 / 2, height / 2, - smallBase2 / 2), color)
    };

    Mesh* pyramid = new Mesh(name);

    if (!fill) {
        pyramid->SetDrawMode(GL_LINE_LOOP);
    }

    std::vector<unsigned int> indices =
    {
        0, 1, 5,
        0, 5, 4,
        1, 2, 6,
        1, 6, 5,
        2, 3, 7,
        2, 7, 6,
        3, 0, 4,
        3, 4, 7,
        0, 4, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };

    pyramid->InitFromData(vertices, indices);
    return pyramid;
}