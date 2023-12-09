#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        typedef struct projectile {
            Mesh* sphere;
            std::string name;
            glm::vec3 pos;
            bool isMoving;
            float angularStep;
        } projectile;

        typedef struct tank {
            Mesh* body;
            Mesh* cannon;
            Mesh* turret;
            Mesh* track1;
            Mesh* track2;
            Mesh* trackAdj1;
            Mesh* trackAdj2;

            std::string bodyName;
            std::string cannonName;
            std::string turretName;
            std::string trackName1;
            std::string trackName2;
            std::string trackAdjName1;
            std::string trackAdjName2;

            glm::vec3 bodyPos;
            glm::vec3 cannonPos;
            glm::vec3 turretPos;
            glm::vec3 trackPos1;
            glm::vec3 trackPos2;
            glm::vec3 trackAdjPos1;
            glm::vec3 trackAdjPos2;

            float time;
            float angle;
            bool isMoving;
            int choice;

            std::vector<projectile> projectiles;
        } tank;

        typedef struct building {
            Mesh* body;
            glm::vec3 pos;
            std::string name;
            float length;
            float height;
            float width;
        } building;

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void CreateEnemy(tank& enemy, int i);
        void MoveEnemy(tank& enemy,
            int choice,
            float randomTime,
            float deltaTime,
            std::unordered_map<std::string, Mesh *> meshes,
            std::unordered_map<std::string, Shader *> shaders);
        void TranslateEnemyFront(tank& enemy,
            float deltaTime,
            std::unordered_map<std::string, Mesh*> meshes,
            std::unordered_map<std::string, Shader*> shaders);
        void TranslateEnemyBack(tank& enemy,
            float deltaTime,
            std::unordered_map<std::string, Mesh*> meshes,
            std::unordered_map<std::string, Shader*> shaders);
        void RotateEnemyLeft(tank& enemy,
            float deltaTime,
            std::unordered_map<std::string, Mesh*> meshes,
            std::unordered_map<std::string, Shader*> shaders);
        void RotateEnemyRight(tank& enemy,
            float deltaTime,
            std::unordered_map<std::string, Mesh*> meshes,
            std::unordered_map<std::string, Shader*> shaders);
        void RenderEnemyMoving(tank& enemy,
            std::unordered_map<std::string, Mesh*> meshes,
            std::unordered_map<std::string, Shader*> shaders,
            glm::mat4& modelMatrix);
        void CreateBuilding(building& building, int i);

    protected:
        implemented::Camera1* camera;
        implemented::Camera1* aux;
        glm::vec3 auxpos;
        glm::vec3 auxcenter;
        glm::vec3 auxup;
        glm::vec3 auxright;
        glm::vec3 auxforward;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        glm::mat4 modelMatrix;

        // TODO(student): If you need any other class variables, define them here.
        bool projection;

        //float right, left, bottom, top, fov;
        //float zFar;
        //float zNear;

        tank player;
        std::vector<tank> enemies;
        std::vector<building> buildings;

        float angularStep = 0;

        float counterProjectile = 0;
        float time = 0;
        int i = 1;
        int j = 0;
        bool isPressed = false;

        float x_projectile = 0;
        float y_projectile = 0;
        float z_projectile = 0;

        bool hasMoved = false;

        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;

        int auxx = 0;
        glm::vec3 pos;
        float randomTime = 0;
        int choice = 0;

        Mesh* surface;
    };
}   // namespace m1
