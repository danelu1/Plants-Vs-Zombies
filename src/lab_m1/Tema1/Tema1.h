#pragma once

#include "components/simple_scene.h"
#include <stack>
#include <queue>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        typedef struct enemy {
            Mesh* inner;
            Mesh* outer;
            glm::vec3 innerColor;
            glm::vec3 outerColor;
            std::string innerName;
            std::string outerName;
            bool isMoving;
            bool isScaling;
            float scaleX;
            float scaleY;
            float translateX;
            int lives;
            int line;
            int index;
            bool checkScale;
            float centerY;
        } enemy;

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
        bool equals(const enemy& e1, const enemy& e2);

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;

        float scaleX[9];
        float scaleY[9];
        float scaleEnemy[3];
        float scaleEnemyX[3];
        float scaleEnemyY[3];

        float translateX[9];
        float translateY[9];
        float tx[9][5]; 
        float ty;

        bool checkScale[9];
        bool checkButtonPressRight[9];

        float angularStep[9][5];

        // TODO(student): If you need any other class variables, define them here.
        Mesh* diamond;
        Mesh* projectiles[9][5];

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
        float counterProjectiles[9];
        int randomTime;
        int randomTimeStars;
        int randomTimeProjectiles;

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
            glm::vec3 color;
        } box;

        typedef struct projectile {
            Mesh* star;
            glm::vec3 color;
            std::string name;
            float translateX;
            float angularStep;
        } projectile;

        typedef struct cell {
            Mesh* square;
            Mesh* diamond;
            glm::vec3 cellColor;
            glm::vec3 diamondColor;
            bool isEmpty;
            std::string diamondName;
            std::string squareName;
            std::vector<projectile> projectiles;
            float scaleX;
            float scaleY;
            float time;
            bool checkScale;
            int line;
        } cell;

        box boxes[4];
        cell cells[9];
        std::vector<enemy> enemies;
        enemy e;
        int j;

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

        int enemyLives[3];
        float distance[3];
        bool scaleOut[3];
        bool scaleIn[3];

        float pentagoneLength;
        float sine;
        float l;
        float y;

        glm::vec3 cellColor[9];
        std::queue<glm::vec3> lineColors[3];
        int lastProjectile;
        bool isMovingProjectile[9][5];

        glm::vec3 enemiesSpawnIn[3];
        glm::vec3 enemiesSpawnOut[3];

        float enemyCenter[3];
        int k;
        float spawnEnemy[3];
        std::vector<enemy> firstLine;
        std::vector<enemy> secondLine;
        std::vector<enemy> thirdLine;
    };
}   // namespace m1
