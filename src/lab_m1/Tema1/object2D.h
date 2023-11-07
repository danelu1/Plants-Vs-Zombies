#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);
    Mesh* CreateDiamond(const  std::string &name, glm::vec3 bottomCorner, float diag1, float diag2, float length, float k, glm::vec3 color, bool fill = false);
    Mesh* CreateEnemy(const std::string& name, glm::vec3 bottomCorner, float length, float k, glm::vec3 externColor, glm::vec3 internColor, bool fill = false);
    Mesh* CreateStar(const std::string& name, glm::vec3 bottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateEnemy1(const std::string& name, glm::vec3 bottomCorner, float length, glm::vec3 externColor, bool fill = false);
}
