#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object3D
{
    // Create square with given bottom left corner, length and color

    Mesh* CreatePyramid(const std::string& name,
        glm::vec3 leftBottomCorner,
        float bigBase1,
        float bigBase2,
        float smallBase1,
        float smallBase2,
        float height,
        glm::vec3 color,
        bool fill = false);

    inline Mesh* CreateCannon(const std::string& name,
        glm::vec3 center,
        float radius,
        float height,
        int n,
        glm::vec3 color,
        bool fill = false) 
    {
        std::vector<VertexFormat> vertices;

        for (int i = 0; i < n; i++) {
            float theta = 2 * M_PI * (float)i / (float)n;
            glm::vec3 position1 = glm::vec3(radius * cos(theta), radius * sin(theta), 0);
            vertices.push_back(VertexFormat(center + position1, color));

            glm::vec3 position2 = glm::vec3(radius * cos(theta), radius * sin(theta), -height);
            vertices.push_back(VertexFormat(center + position2, color));
        }

        Mesh* cannon = new Mesh(name);

        std::vector<unsigned int> indices;

        for (int i = 0; i < n; i++) {
            if (i == n - 1) {
                indices.push_back(n - 1);
                indices.push_back(0);
                indices.push_back(n);

                indices.push_back(n - 1);
                indices.push_back(n);
                indices.push_back(2 * n - 1);
            }
            else {
                indices.push_back(i);
                indices.push_back(i + 1);
                indices.push_back(i + n + 1);

                indices.push_back(i);
                indices.push_back(i + n + 1);
                indices.push_back(i + n);
            }
        }

        cannon->InitFromData(vertices, indices);

        return cannon;
    }
}