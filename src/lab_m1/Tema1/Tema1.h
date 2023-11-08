#pragma once

#include "components/simple_scene.h"
#include <stack>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void DisplayDiamond(const std::string& name,
            bool& checkB,
            bool& checkS,
            float& sx,
            float& sy,
            float time,
            float cx,
            float cy,
            glm::mat3& matrix,
            std::unordered_map<std::string, Mesh*> meshes,
            std::unordered_map<std::string, Shader*> shaders);
        void CreateNewEnemy(const std::string name1,
            const std::string name2,
            Mesh* enemies[],
            glm::vec3 position1,
            glm::vec3 position2,
            int index,
            int& color,
            glm::vec3& enemyColor,
            glm::mat3 matrix,
            std::unordered_map<std::string, Mesh*>& meshes,
            std::unordered_map<std::string, Shader*> shaders);

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;

        float scaleX[9];
        float scaleY[9];

        float translateX[9];
        float translateY[9];
        float tx[9]; 
        float ty;

        bool checkScale[9];
        bool checkButtonPressRight[9];

        float angularStep;

        // TODO(student): If you need any other class variables, define them here.
        Mesh* diamond;
        Mesh* enemies[6];
        Mesh* projectiles[9];

        bool isEmptyCell[9];
        bool isMoving[9];

        glm::vec3 enemyColor1;
        glm::vec3 enemyColor2;
        glm::vec3 enemyColor3;
        glm::vec3 draggedColor;
        
        int line;
        int color;

        float counterEnemies;
        float counterStars;
        float counterProjectiles;
        int randomTime;
        int randomTimeStars;

        float sx;
        float sy;
        bool check;

        typedef struct box {
            std::string squareName;
            std::string diamondName;
            std::vector<std::string> starsNames;
            int stars_no;
            Mesh* square;
            Mesh* diamond;
            std::vector<Mesh *> stars;
        } box;

        box boxes[4];

        glm::vec3 colors[4];

        Mesh* lives[3];
        int lives_no;

        std::stack<Mesh *> inventory;
        Mesh* randomStars[3];
        glm::vec3 randomCoordinates[3];
        
        float randomStarX;
        float randomStarYUp;
        float randomStarYDown;
        float starCenterDistance;
        int starsOnScreen;
        
        bool flag[3];
        
        int x_inventory;
        int y_inventory;
        
        float centers[2][9];
        float hexagoneRadius;
        float diamondRadius;

        float x_projectile[9];
        float y_projectile[9];

        bool isDragged[4];
        bool isPressed;
        int index;

        std::unordered_map<int, int> map;
    };
}   // namespace m1
