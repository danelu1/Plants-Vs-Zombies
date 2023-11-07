#include "lab_m1/Tema1/object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* rectangle = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateDiamond(
    const std::string& name,
    glm::vec3 bottomCorner,
    float diag1,
    float diag2,
    float length,
    float k,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = bottomCorner;

    std::vector<VertexFormat> vertices =
    {
        /*VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(diag2 / 2, diag1 / 2, 0), color),
        VertexFormat(corner + glm::vec3(0, diag1, 0), color),
        VertexFormat(corner + glm::vec3(- diag2 / 2, diag1 / 2, 0), color)*/
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(k * diag2 / 2, k * diag1 / 2, 0), color),
        VertexFormat(corner + glm::vec3(k * diag2 / 2 + length, k * diag1 / 2, 0), color),
        VertexFormat(corner + glm::vec3(k * diag2 / 2 + length, k * diag1 / 2 + (1 - k) * diag1, 0), color),
        VertexFormat(corner + glm::vec3(k * diag2 / 2, k * diag1 / 2 + (1 - k) * diag1, 0), color),
        VertexFormat(corner + glm::vec3(0, diag1, 0), color),
        VertexFormat(corner + glm::vec3(- diag2 / 2, diag1 / 2, 0), color)
    };

    Mesh* diamond = new Mesh(name);

    std::vector<unsigned int> indices
    {
        0, 5, 6,
        0, 1, 4,
        0, 4, 5,
        1, 3, 4,
        1, 2, 3
    };

    if (!fill) {
        diamond->SetDrawMode(GL_LINE_LOOP);
    }

    diamond->InitFromData(vertices, indices);
    return diamond;
}

Mesh* object2D::CreateEnemy(const std::string &name,
    glm::vec3 bottomCorner,
    float length,
    float k,
    glm::vec3 externColor,
    glm::vec3 internColor,
    bool fill)
{
    glm::vec3 corner1 = bottomCorner;

    float length1 = (1 - k) * length;
    glm::vec3 corner2 = corner1 + glm::vec3(0, k * length, 0);

    glm::vec3 color1 = externColor;
    glm::vec3 color2 = internColor;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner1, color1),
        VertexFormat(corner1 + glm::vec3(length * sqrt(3) / 2, length / 2, 0), color1),
        VertexFormat(corner1 + glm::vec3(length * sqrt(3) / 2, 3 * length / 2, 0), color1),
        VertexFormat(corner1 + glm::vec3(0, 2 * length, 0), color1),
        VertexFormat(corner1 + glm::vec3(- length * sqrt(3) / 2, 3 * length / 2, 0), color1),
        VertexFormat(corner1 + glm::vec3(- length * sqrt(3) / 2, length / 2, 0), color1),

        VertexFormat(corner2, color2),
        VertexFormat(corner2 + glm::vec3(length1 * sqrt(3) / 2, length1 / 2, 1), color2),
        VertexFormat(corner2 + glm::vec3(length1 * sqrt(3) / 2, 3 * length1 / 2, 1), color2),
        VertexFormat(corner2 + glm::vec3(0, 2 * length1, 0), color2),
        VertexFormat(corner2 + glm::vec3(- length1 * sqrt(3) / 2, 3 * length1 / 2, 1), color2),
        VertexFormat(corner2 + glm::vec3(- length1 * sqrt(3) / 2, length1 / 2, 1), color2)
    };

    Mesh* enemy = new Mesh(name);

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,

        6, 7, 8,
        6, 8, 9,
        6, 9, 10,
        6, 10, 11
    };

    if (!fill) {
        enemy->SetDrawMode(GL_LINE_LOOP);
    }

    enemy->InitFromData(vertices, indices);
    return enemy;
}

Mesh* object2D::CreateStar(const std::string &name,
    glm::vec3 bottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = bottomCorner;

    float bigLengthOut = 2 * length * sin(3 * M_PI / 10);
    float y = length * cos(3 * M_PI / 10);
    float pentagoneLength = 2 * length * sin(M_PI / 10);
    float bigLengthIn = 2 * pentagoneLength * sin(3 * M_PI / 10);
    float sine = sin(M_PI / 10);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(bigLengthOut / 2, - y, 0), color),
        VertexFormat(corner + glm::vec3(bigLengthIn / 2, pentagoneLength * cos(3 * M_PI / 10), 0), color),
        VertexFormat(corner + glm::vec3(length + pentagoneLength / 2, length * sqrt(3 * sine * sine + 2 * sine), 0), color),
        VertexFormat(corner + glm::vec3(pentagoneLength / 2, length * sqrt(3 * sine * sine + 2 * sine), 0), color),
        VertexFormat(corner + glm::vec3(0, sqrt(length * length - pentagoneLength * pentagoneLength) 
        + length * sqrt(3 * sine * sine + 2 * sine), 0), color),
        VertexFormat(corner + glm::vec3(- pentagoneLength / 2, length * sqrt(3 * sine * sine + 2 * sine), 0), color),
        VertexFormat(corner + glm::vec3(- length - pentagoneLength / 2, length * sqrt(3 * sine * sine + 2 * sine), 0), color),
        VertexFormat(corner + glm::vec3(- bigLengthIn / 2, pentagoneLength * cos(3 * M_PI / 10), 0), color),
        VertexFormat(corner + glm::vec3(- bigLengthOut / 2, -y, 0), color)
    };

    Mesh* star = new Mesh(name);

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        2, 3, 4,
        4, 5, 6,
        6, 7, 8,
        8, 9, 0,

        0, 2, 4,
        0, 4, 6,
        0, 6, 8
    };

    if (!fill)
    {
        star->SetDrawMode(GL_LINE_LOOP);
    }

    star->InitFromData(vertices, indices);

    return star;
}

Mesh* object2D::CreateEnemy1(const std::string& name,
    glm::vec3 bottomCorner,
    float length,
    glm::vec3 externColor,
    bool fill)
{
    glm::vec3 corner1 = bottomCorner;

    glm::vec3 color1 = externColor;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner1, color1),
        VertexFormat(corner1 + glm::vec3(length * sqrt(3) / 2, length / 2, 0), color1),
        VertexFormat(corner1 + glm::vec3(length * sqrt(3) / 2, 3 * length / 2, 0), color1),
        VertexFormat(corner1 + glm::vec3(0, 2 * length, 0), color1),
        VertexFormat(corner1 + glm::vec3(-length * sqrt(3) / 2, 3 * length / 2, 0), color1),
        VertexFormat(corner1 + glm::vec3(-length * sqrt(3) / 2, length / 2, 0), color1),
    };

    Mesh* enemy = new Mesh(name);

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
    };

    if (!fill) {
        enemy->SetDrawMode(GL_LINE_LOOP);
    }

    enemy->InitFromData(vertices, indices);
    return enemy;
}