#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/camera.h"
#include "lab_m1/Tema2/object3D.h"
#include "lab_m1/Tema2/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

// Function for checking collision between any game component.
bool CheckBorders(glm::vec3 center) {
    if (center.x <= -25 && center.x >= 25 && center.z <= -25) {
        return true;
    }

    return false;
}

bool CheckCollision(glm::vec3 center1, glm::vec3 center2, float radius1, float radius2)
{
    float distance = glm::distance(center2, center1);

    return distance < radius1 + radius2;
}

// Function which creates an opponent at random coordinates based on the plane
// quarters.
void Tema2::CreateEnemy(tank& enemy, int i)
{
    enemy.bodyName = "enemyBody" + std::to_string(i);
    enemy.turretName = "enemyTurret" + std::to_string(i);
    enemy.cannonName = "enemyCannon" + std::to_string(i);
    enemy.trackName1 = "enemyTrack1" + std::to_string(i);
    enemy.trackName2 = "enemyTack2" + std::to_string(i);
    enemy.trackAdjName1 = "enemyTrackAdj1" + std::to_string(i);
    enemy.trackAdjName2 = "enemyTrackAdj2" + std::to_string(i);

    enemy.time = 0;
    enemy.angle = 0;
    enemy.isMoving = false;
    enemy.choice = -1;
    enemy.HP = 50;

    float x_body = 0;
    float z_body = 0;

    if (rand() % 4 == 0) {
        x_body = rand() % 10 - 10;
        z_body = rand() % 10 - 10;
    }
    else if (rand() % 4 == 1) {
        x_body = rand() % 10 + 10;
        z_body = rand() % 10 + 10;
    }
    else if (rand() % 4 == 2) {
        x_body = rand() % 10 - 10;
        z_body = rand() % 10 + 10;
    }
    else {
        x_body = rand() % 10 + 10;
        z_body = rand() % 10 - 10;
    }

    enemy.bodyPos = glm::vec3(x_body, 0.25, z_body);
    enemy.turretPos = glm::vec3(x_body, 0.5, z_body);
    enemy.cannonPos = glm::vec3(x_body, 0.5, z_body - 0.2);
    enemy.trackPos1 = glm::vec3(x_body - 0.3, 0.05, z_body);
    enemy.trackPos2 = glm::vec3(x_body + 0.3, 0.05, z_body);
    enemy.trackAdjPos1 = glm::vec3(x_body - 0.3, 0.125, z_body);
    enemy.trackAdjPos2 = glm::vec3(x_body + 0.3, 0.125, z_body);

    enemy.body = object3D::CreatePyramid(enemy.bodyName, glm::vec3(), 0.8, 2, 0.7, 1.75, 0.3, glm::vec3(0, 0, 0), true);
    enemy.turret = object3D::CreatePyramid(enemy.turretName, glm::vec3(), 0.8, 1, 0.6, 0.75, 0.2, glm::vec3(0, 0.2, 0), true);
    enemy.cannon = object3D::CreateCannon(enemy.cannonName, glm::vec3(), 0.05, 1.2, 100, glm::vec3(0.5, 0.5, 0.5), true);
    enemy.track1 = object3D::CreatePyramid(enemy.trackName1, glm::vec3(), 0.2, 1.6, 0.25, 2, 0.1, glm::vec3(0.5, 0.5, 0.5), true);
    enemy.track2 = object3D::CreatePyramid(enemy.trackName2, glm::vec3(), 0.2, 1.6, 0.25, 2, 0.1, glm::vec3(0.5, 0.5, 0.5), true);
    enemy.trackAdj1 = object3D::CreatePyramid(enemy.trackAdjName1, glm::vec3(), 0.25, 2, 0.25, 2, 0.05, glm::vec3(0.5, 0.5, 0.5), true);
    enemy.trackAdj2 = object3D::CreatePyramid(enemy.trackAdjName2, glm::vec3(), 0.25, 2, 0.25, 2, 0.05, glm::vec3(0.5, 0.5, 0.5), true);

    AddMeshToList(enemy.body);
    AddMeshToList(enemy.turret);
    AddMeshToList(enemy.cannon);
    AddMeshToList(enemy.track1);
    AddMeshToList(enemy.track2);
    AddMeshToList(enemy.trackAdj1);
    AddMeshToList(enemy.trackAdj2);

    enemies.push_back(enemy);
}

// Function for moving enemies according to the random type of movement
// chosen(forward, backward, left, right) for a random time.
void m1::Tema2::MoveEnemy(tank& enemy,
    int choice,
    float randomTime,
    float deltaTime,
    std::unordered_map<std::string, Mesh *> meshes,
    std::unordered_map<std::string, Shader *> shaders)
{
    if (choice == 0) {
        TranslateEnemyFront(enemy, deltaTime, meshes, shaders);
    }
    else if (choice == 1) {
        TranslateEnemyBack(enemy, deltaTime, meshes, shaders);
    }
    else if (choice == 2) {
        RotateEnemyLeft(enemy, deltaTime, meshes, shaders);
    }
    else if (choice == 3) {
        RotateEnemyRight(enemy, deltaTime, meshes, shaders);
    }
}

// Function for moving the enemies forward.
void m1::Tema2::TranslateEnemyFront(tank& enemy,
    float deltaTime,
    std::unordered_map<std::string, Mesh*> meshes,
    std::unordered_map<std::string, Shader*> shaders)
{
    enemy.bodyPos.x += 2 * deltaTime * sin(enemy.angle);
    enemy.cannonPos.x += 2 * deltaTime * sin(enemy.angle);
    enemy.turretPos.x += 2 * deltaTime * sin(enemy.angle);
    enemy.trackAdjPos1.x += 2 * deltaTime * sin(enemy.angle);
    enemy.trackAdjPos2.x += 2 * deltaTime * sin(enemy.angle);
    enemy.trackPos1.x += 2 * deltaTime * sin(enemy.angle);
    enemy.trackPos2.x += 2 * deltaTime * sin(enemy.angle);

    enemy.bodyPos.z -= 2 * deltaTime * cos(enemy.angle);
    enemy.cannonPos.z -= 2 * deltaTime * cos(enemy.angle);
    enemy.turretPos.z -= 2 * deltaTime * cos(enemy.angle);
    enemy.trackAdjPos1.z -= 2 * deltaTime * cos(enemy.angle);
    enemy.trackAdjPos2.z -= 2 * deltaTime * cos(enemy.angle);
    enemy.trackPos1.z -= 2 * deltaTime * cos(enemy.angle);
    enemy.trackPos2.z -= 2 * deltaTime * cos(enemy.angle);

    RenderEnemyMoving(enemy, meshes, shaders, modelMatrix);
}

// Function for moving the enemies backward.
void m1::Tema2::TranslateEnemyBack(tank& enemy,
    float deltaTime,
    std::unordered_map<std::string, Mesh*> meshes,
    std::unordered_map<std::string, Shader*> shaders)
{
    enemy.bodyPos.x -= 2 * deltaTime * sin(enemy.angle);
    enemy.cannonPos.x -= 2 * deltaTime * sin(enemy.angle);
    enemy.turretPos.x -= 2 * deltaTime * sin(enemy.angle);
    enemy.trackAdjPos1.x -= 2 * deltaTime * sin(enemy.angle);
    enemy.trackAdjPos2.x -= 2 * deltaTime * sin(enemy.angle);
    enemy.trackPos1.x -= 2 * deltaTime * sin(enemy.angle);
    enemy.trackPos2.x -= 2 * deltaTime * sin(enemy.angle);

    enemy.bodyPos.z += 2 * deltaTime * cos(enemy.angle);
    enemy.cannonPos.z += 2 * deltaTime * cos(enemy.angle);
    enemy.turretPos.z += 2 * deltaTime * cos(enemy.angle);
    enemy.trackAdjPos1.z += 2 * deltaTime * cos(enemy.angle);
    enemy.trackAdjPos2.z += 2 * deltaTime * cos(enemy.angle);
    enemy.trackPos1.z += 2 * deltaTime * cos(enemy.angle);
    enemy.trackPos2.z += 2 * deltaTime * cos(enemy.angle);

    RenderEnemyMoving(enemy, meshes, shaders, modelMatrix);
}

// Function for rotating the enemies to the left.
void m1::Tema2::RotateEnemyLeft(tank& enemy,
    float deltaTime,
    std::unordered_map<std::string, Mesh*> meshes,
    std::unordered_map<std::string, Shader*> shaders)
{
    enemy.angle -= 2 * deltaTime;
    RenderEnemyMoving(enemy, meshes, shaders, modelMatrix);
}

// Function for rotating the enemies to the right.
void m1::Tema2::RotateEnemyRight(tank& enemy,
    float deltaTime,
    std::unordered_map<std::string, Mesh*> meshes,
    std::unordered_map<std::string, Shader*> shaders)
{
    enemy.angle += 2 * deltaTime;
    RenderEnemyMoving(enemy, meshes, shaders, modelMatrix);
}

// Function for rendering the enemies while they are moving.
void m1::Tema2::RenderEnemyMoving(tank& enemy,
    std::unordered_map<std::string, Mesh*> meshes,
    std::unordered_map<std::string, Shader*> shaders,
    glm::mat4& modelMatrix)
{
    glm::mat4 initialMatrix = modelMatrix;

    modelMatrix *= transform3D::Translate(enemy.bodyPos.x, enemy.bodyPos.y, enemy.bodyPos.z);
    modelMatrix *= transform3D::RotateOY(enemy.angle);
    RenderSimpleMesh(meshes[enemy.bodyName], shaders["CustomShader"], modelMatrix, enemy.HP, 1);

    modelMatrix = initialMatrix;

    modelMatrix *= transform3D::Translate(enemy.turretPos.x, enemy.turretPos.y, enemy.turretPos.z);
    modelMatrix *= transform3D::RotateOY(enemy.angle);
    RenderSimpleMesh(meshes[enemy.turretName], shaders["CustomShader"], modelMatrix, enemy.HP, 2);

    modelMatrix = initialMatrix;

    modelMatrix *= transform3D::Translate(enemy.trackPos1.x, enemy.trackPos1.y, enemy.trackPos1.z);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(enemy.angle);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    RenderSimpleMesh(meshes[enemy.trackName1], shaders["CustomShader"], modelMatrix, enemy.HP, 3);

    modelMatrix = initialMatrix;

    modelMatrix *= transform3D::Translate(enemy.trackPos2.x, enemy.trackPos2.y, enemy.trackPos2.z);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(enemy.angle);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    RenderSimpleMesh(meshes[enemy.trackName2], shaders["CustomShader"], modelMatrix, enemy.HP, 3);

    modelMatrix = initialMatrix;

    modelMatrix *= transform3D::Translate(enemy.trackAdjPos1.x, enemy.trackAdjPos1.y, enemy.trackAdjPos1.z);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(enemy.angle);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    RenderSimpleMesh(meshes[enemy.trackAdjName1], shaders["CustomShader"], modelMatrix, enemy.HP, 3);

    modelMatrix = initialMatrix;

    modelMatrix *= transform3D::Translate(enemy.trackAdjPos2.x, enemy.trackAdjPos2.y, enemy.trackAdjPos2.z);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(enemy.angle);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    RenderSimpleMesh(meshes[enemy.trackAdjName2], shaders["CustomShader"], modelMatrix, enemy.HP, 3);

    modelMatrix = initialMatrix;

    modelMatrix *= transform3D::Translate(enemy.cannonPos.x, enemy.cannonPos.y, enemy.cannonPos.z);
    modelMatrix *= transform3D::Translate(0, 0, 0.2);
    modelMatrix *= transform3D::RotateOY(enemy.angle);
    modelMatrix *= transform3D::Translate(0, 0, -0.2);
    RenderSimpleMesh(meshes[enemy.cannonName], shaders["CustomShader"], modelMatrix, enemy.HP, 3);

    modelMatrix = initialMatrix;
}

// Function for creating a building at random coordinates(a building may be spawn
// anywhere, because in case of collision the tanks are being moved outside it).
void m1::Tema2::CreateBuilding(building& building, int i)
{
    building.name = "building" + std::to_string(i);
    building.height = rand() % 4 + 2;
    building.length = rand() % 2 + 3;
    building.width = rand() % 2 + 2;
    int c = rand() % 4;

    if (c == 0) {
        building.pos = glm::vec3(rand() % 10 - 10, building.height / 2 - 0.5, rand() % 10 - 10);
    }
    else if (c == 1) {
        building.pos = glm::vec3(rand() % 10 + 10, building.height / 2 - 0.5, rand() % 10 + 10);
    }
    else if (c == 2) {
        building.pos = glm::vec3(rand() % 10 - 10, building.height / 2 - 0.5, rand() % 10 + 10);
    }
    else if (c == 3) {
        building.pos = glm::vec3(rand() % 10 + 10, building.height / 2 - 0.5, rand() % 10 - 10);
    }

    building.body = object3D::CreatePyramid(building.name, building.pos, building.length,
        building.width, building.length, building.width, building.height, glm::vec3(0.2), true);

    AddMeshToList(building.body);

    buildings.push_back(building);
}


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;

    right = 10;
    left = 0.01;
    bottom = 0.01;
    top = 10;
    fov = 50;

    camera = new implemented::Camera1();
    camera->Set(glm::vec3(0.4f, 0.25f, 4.f), glm::vec3(0.4f, 0.25f, 0), glm::vec3(0, 1, 0));
    aux = new implemented::Camera1();
    aux->Set(glm::vec3(0.4f, 0.25, 4.f), glm::vec3(0.4f, 0.25, 0), glm::vec3(0, 1, 0));

    camera->RotateThirdPerson_OX(RADIANS(-15));

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    projection = true;

    player.bodyPos = glm::vec3(0.4, 0.25, 1);
    player.trackPos1 = glm::vec3(0.1, 0.05, 1);
    player.trackPos2 = glm::vec3(0.7, 0.05, 1);
    player.trackAdjPos1 = glm::vec3(0.1, 0.125, 1);
    player.trackAdjPos2 = glm::vec3(0.7, 0.125, 1);
    player.turretPos = glm::vec3(0.4, 0.5, 1);
    player.cannonPos = glm::vec3(0.4, 0.5, 0.8);

    player.angle = 0;
    player.angularStep = 0;

    player.body = object3D::CreatePyramid("playerBody", glm::vec3(), 0.8, 2, 0.7, 1.75, 0.3, glm::vec3(0, 0, 0), true);
    AddMeshToList(player.body);

    player.track1 = object3D::CreatePyramid("playerTrack1", glm::vec3(), 0.2, 1.6, 0.25, 2, 0.1, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(player.track1);

    player.track2 = object3D::CreatePyramid("playerTrack2", glm::vec3(), 0.2, 1.6, 0.25, 2, 0.1, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(player.track2);

    player.trackAdj1 = object3D::CreatePyramid("playerTrackAdj1", glm::vec3(), 0.25, 2, 0.25, 2, 0.05, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(player.trackAdj1);

    player.trackAdj2 = object3D::CreatePyramid("playerTrackAdj2", glm::vec3(), 0.25, 2, 0.25, 2, 0.05, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(player.trackAdj2);

    player.turret = object3D::CreatePyramid("playerTurret", glm::vec3(), 0.8, 1, 0.6, 0.75, 0.2, glm::vec3(0, 0.2, 0), true);
    AddMeshToList(player.turret);

    player.cannon = object3D::CreateCannon("playerCannon", glm::vec3(), 0.05, 1.2, 100, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(player.cannon);

    Mesh* s = new Mesh("sphere");
    s->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[s->GetMeshID()] = s;
    
    player.bodyName = "playerBody";
    player.cannonName = "playerCannon";
    player.trackAdjName1 = "playerTrackAdj1";
    player.trackAdjName2 = "playerTrackAdj2";
    player.turretName = "playerTurret";
    player.trackName1 = "playerTrack1";
    player.trackName2 = "playerTrack2";

    tank e;

    for (int j = 0; j < 10; j++) {
        CreateEnemy(e, j + 1);
    }

    surface = object3D::CreatePyramid("surface", glm::vec3(), 50, 50, 50, 50, 0.05, glm::vec3(0, 1, 0), true);
    AddMeshToList(surface);

    building b;

    for (int i = 0; i < 15; i++) {
        CreateBuilding(b, i + 1);
    }

    Shader* shader = new Shader("CustomShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    window->DisablePointer();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    modelMatrix = glm::mat4(1.0f);
    RenderMesh(meshes["surface"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix *= transform3D::RotateOY(player.angle);
    RenderMesh(meshes["playerBody"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= transform3D::Translate(player.turretPos.x, player.turretPos.y, player.turretPos.z);
    modelMatrix *= transform3D::RotateOY(player.angularStep);
    RenderMesh(meshes["playerTurret"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= transform3D::Translate(player.trackPos1.x, player.trackPos1.y, player.trackPos1.z);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(player.angle);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    RenderMesh(meshes["playerTrack1"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= transform3D::Translate(player.trackPos2.x, player.trackPos2.y, player.trackPos2.z);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(player.angle);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    RenderMesh(meshes["playerTrack2"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= transform3D::Translate(player.trackAdjPos1.x, player.trackAdjPos1.y, player.trackAdjPos1.z);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(player.angle);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    RenderMesh(meshes["playerTrackAdj1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= transform3D::Translate(player.trackAdjPos2.x, player.trackAdjPos2.y, player.trackAdjPos2.z);
    modelMatrix *= transform3D::Translate(-0.3, 0, 0);
    modelMatrix *= transform3D::RotateOY(player.angle);
    modelMatrix *= transform3D::Translate(0.3, 0, 0);
    RenderMesh(meshes["playerTrackAdj2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix *= transform3D::Translate(player.cannonPos.x, player.cannonPos.y, player.cannonPos.z);
    modelMatrix *= transform3D::Translate(0, 0, 0.2);
    modelMatrix *= transform3D::RotateOY(player.angularStep);
    modelMatrix *= transform3D::Translate(0, 0, -0.2);
    RenderMesh(meshes["playerCannon"], shaders["VertexColor"], modelMatrix);

    counterProjectile += deltaTimeSeconds;
    
    // Render the buildings on the map.
    for (auto b : buildings) {
        modelMatrix = glm::mat4(1.0f);
        RenderMesh(meshes[b.name], shaders["VertexColor"], modelMatrix);
    }

    // Checking the collision between the enemies and the projectiles.
    for (auto b : buildings) {
        player.projectiles.erase(
            std::remove_if(player.projectiles.begin(), player.projectiles.end(), [&](const auto p) {
                    glm::vec3 bPos = glm::vec3(b.pos.x, b.pos.y - b.height / 2 + 0.75, b.pos.z);
                    return CheckCollision(p.pos, bPos, 0.1, b.length / 2);
                }),
            player.projectiles.end()
        );
    }

    // Checking the collision between the tanks and the buildings by using
    // the coordinates of the tank and the coordinates of the building, but
    // with the same local y-axis.
    for (auto b : buildings) {
        glm::vec3 bPos = glm::vec3(b.pos.x, b.pos.y - b.height / 2 + 0.75, b.pos.z);
        if (CheckCollision(player.bodyPos, bPos, 1, b.length / 2)) {
            glm::vec3 diff = player.bodyPos - bPos;
            float positive = abs(b.length / 2 + 1 - glm::distance(player.bodyPos, bPos));
            glm::vec3 p = positive * glm::normalize(diff);

            player.bodyPos += 0.5f * p;
            camera->position += 0.5f * p;
            player.turretPos += 0.5f * p;
            player.cannonPos += 0.5f * p;
            player.trackPos1 += 0.5f * p;
            player.trackPos2 += 0.5f * p;
            player.trackAdjPos1 += 0.5f * p;
            player.trackAdjPos2 += 0.5f * p;
        }

        for (auto& e : enemies) {
            if (CheckCollision(e.bodyPos, bPos, 1, b.length / 2)) {
                glm::vec3 diff1 = e.bodyPos - bPos;
                float positive1 = abs(b.length / 2 + 1 - glm::distance(e.bodyPos, bPos));
                glm::vec3 p1 = positive1 * glm::normalize(diff1);

                e.bodyPos += 0.5f * p1;
                e.turretPos += 0.5f * p1;
                e.cannonPos += 0.5f * p1;
                e.trackPos1 += 0.5f * p1;
                e.trackPos2 += 0.5f * p1;
                e.trackAdjPos1 += 0.5f * p1;
                e.trackAdjPos2 += 0.5f * p1;
            }
        }
    }

    // Making the projectiles move along the cannon axis.
    for (auto& p : player.projectiles) {
        if (p.isMoving) {
            p.pos.z -= 5 * deltaTimeSeconds * cos(p.angularStep);
            p.pos.x += 5 * deltaTimeSeconds * sin(p.angularStep);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(p.pos.x, p.pos.y, p.pos.z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderMesh(meshes[p.name], shaders["VertexColor"], modelMatrix);
        }
    }

    // Collision between a projectile and an enemy.
    for (auto& e : enemies) {
        for (auto p : player.projectiles) {
            if (CheckCollision(p.pos, e.bodyPos, 0.1, 1)) {
                e.HP -= 10.0f;
            }
        }

        player.projectiles.erase(
            std::remove_if(player.projectiles.begin(), player.projectiles.end(), [&](const auto& p) {
                return abs(p.pos.x) >= 40
                    || abs(p.pos.z) >= 40
                    || CheckCollision(p.pos, e.bodyPos, 0.1, 1);
                }),
            player.projectiles.end()
        );
    }

    for (auto e : enemies) {
        if (e.HP <= 0) {
            score += 100;
        }
    }

    // Here I remove ech enemy with HP smaller than 0.
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](const auto e) {
            return e.HP <= 0;
            }),
        enemies.end()
    );

    // Check the collision player-enemy, enemy-enemy and moving them backwards
    // to get them out of the collision phase.
    for (auto& e : enemies) {
        if (CheckCollision(player.bodyPos, e.bodyPos, 1, 1)) {
            glm::vec3 diff = e.bodyPos - player.bodyPos;
            float positive = abs(2 - glm::distance(e.bodyPos, player.bodyPos));
            glm::vec3 p = positive * glm::normalize(diff);

            player.bodyPos += -0.5f * p;
            e.bodyPos += 0.5f * p;
            camera->position += -0.5f * p;
            
            e.turretPos += 0.5f * p;
            player.turretPos += -0.5f * p;

            player.cannonPos += -0.5f * p;
            e.cannonPos += 0.5f * p;

            player.trackPos1 += -0.5f * p;
            e.trackPos1 += 0.5f * p;

            player.trackPos2 += -0.5f * p;
            e.trackPos2 += 0.5f * p;

            player.trackAdjPos1 += -0.5f * p;
            e.trackAdjPos1 += 0.5f * p;

            player.trackAdjPos2 += -0.5f * p;
            e.trackAdjPos2 += 0.5f * p;
        }

        for (auto& e1 : enemies) {
            if (CheckCollision(e.bodyPos, e1.bodyPos, 1, 1)) {
                glm::vec3 diff1 = e.bodyPos - player.bodyPos;
                float positive1 = abs(2 - glm::distance(e.bodyPos, e1.bodyPos));
                glm::vec3 p1 = positive1 * glm::normalize(diff1);

                e.bodyPos += -0.5f * p1;
                e1.bodyPos += 0.5f * p1;

                e.turretPos += -0.5f * p1;
                e1.turretPos += 0.5f * p1;

                e.cannonPos += -0.5f * p1;
                e1.cannonPos += 0.5f * p1;

                e.trackPos1 += -0.5f * p1;
                e1.trackPos1 += 0.5f * p1;

                e.trackPos2 += -0.5f * p1;
                e1.trackPos2 += 0.5f * p1;

                e.trackAdjPos1 += -0.5f * p1;
                e1.trackAdjPos1 += 0.5f * p1;

                e.trackAdjPos2 += -0.5f * p1;
                e1.trackAdjPos2 += 0.5f * p1;
            }
        }
    }

    // Here I update the time of the enemies to make them move
    // in a different way.
    for (auto& e : enemies) {
        e.time += deltaTimeSeconds;
        if (e.time > randomTime) {
            e.isMoving = true;
            e.choice = rand() % 4;
            e.time = 0;
            randomTime = rand() % 4 + 2;
        }
    }

    // Here I move the enemies.
    for (auto& e : enemies) {
        if (e.isMoving) {
            modelMatrix = glm::mat4(1);
            MoveEnemy(e, e.choice, randomTime, deltaTimeSeconds, meshes, shaders);
        }
    }

    modelMatrix = glm::mat4(1);

    finishTime += deltaTimeSeconds;

    // Here the game finishes after 2 minutes passed since the beggining.
    if (finishTime > 120) {
        waitTime += deltaTimeSeconds;
        if (waitTime > 10) {
            cout << score << endl;
            exit(0);
        }
    }
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;
    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (finishTime < 120) {
        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

        } else {
            if (window->KeyHold(GLFW_KEY_W)) {
                player.bodyPos.z -= 2 * deltaTime * cos(player.angle);
                player.trackPos1.z -= 2 * deltaTime * cos(player.angle);
                player.trackPos2.z -= 2 * deltaTime * cos(player.angle);
                player.trackAdjPos1.z -= 2 * deltaTime * cos(player.angle);
                player.trackAdjPos2.z -= 2 * deltaTime * cos(player.angle);
                player.turretPos.z -= 2 * deltaTime * cos(player.angle);
                player.cannonPos.z -= 2 * deltaTime * cos(player.angle);


                player.bodyPos.x += 2 * deltaTime * sin(player.angle);
                player.trackPos1.x += 2 * deltaTime * sin(player.angle);
                player.trackPos2.x += 2 * deltaTime * sin(player.angle);
                player.trackAdjPos1.x += 2 * deltaTime * sin(player.angle);
                player.trackAdjPos2.x += 2 * deltaTime * sin(player.angle);
                player.turretPos.x += 2 * deltaTime * sin(player.angle);
                player.cannonPos.x += 2 * deltaTime * sin(player.angle);

                camera->MoveForward(2 * deltaTime);
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                player.bodyPos.z += 2 * deltaTime * cos(player.angle);
                player.trackPos1.z += 2 * deltaTime * cos(player.angle);
                player.trackPos2.z += 2 * deltaTime * cos(player.angle);
                player.trackAdjPos1.z += 2 * deltaTime * cos(player.angle);
                player.trackAdjPos2.z += 2 * deltaTime * cos(player.angle);
                player.turretPos.z += 2 * deltaTime * cos(player.angle);
                player.cannonPos.z += 2 * deltaTime * cos(player.angle);

                player.bodyPos.x -= 2 * deltaTime * sin(player.angle);
                player.trackPos1.x -= 2 * deltaTime * sin(player.angle);
                player.trackPos2.x -= 2 * deltaTime * sin(player.angle);
                player.trackAdjPos1.x -= 2 * deltaTime * sin(player.angle);
                player.trackAdjPos2.x -= 2 * deltaTime * sin(player.angle);
                player.turretPos.x -= 2 * deltaTime * sin(player.angle);
                player.cannonPos.x -= 2 * deltaTime * sin(player.angle);

                camera->MoveForward(-2 * deltaTime);
            }

            if (window->KeyHold(GLFW_KEY_A)) {
                player.angle -= 2 * deltaTime;
                player.angularStep -= 2 * deltaTime;
                camera->RotateThirdPerson_OY(2 * deltaTime);
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                player.angle += 2 * deltaTime;
                player.angularStep += 2 * deltaTime;
                camera->RotateThirdPerson_OY(-2 * deltaTime);
            }
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (!renderCameraTarget) {
            renderCameraTarget = true;
            aux->Set(camera->position, glm::vec3(0.4f, 0.25, 0), camera->up);
            aux->position = camera->position;
            aux->up = camera->up;
            aux->right = camera->right;
            aux->forward = camera->forward;
        }

        camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
        camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
    }
    else {
        player.angularStep += deltaX * 0.002f;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1 && counterProjectile > 1) {
        counterProjectile = 0;
        projectile p;

        p.angularStep = player.angularStep;

        float x = player.turretPos.x + 1.4 * sin(p.angularStep);
        float y = player.turretPos.y;
        float z = player.turretPos.z - 1.4 * cos(p.angularStep);

        p.name = "sphere" + std::to_string(i++);
        p.isMoving = false;

        p.sphere = new Mesh(p.name);
        p.sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[p.sphere->GetMeshID()] = p.sphere;

        p.pos = glm::vec3(x, y, z);

        player.projectiles.push_back(p);

        AddMeshToList(p.sphere);

        if (player.projectiles.size() > 0) {
            player.projectiles[player.projectiles.size() - 1].isMoving = true;
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (button == 2) {
        camera->Set(aux->position, glm::vec3(0.4f, 0.25, 0), aux->up);
        camera->right = aux->right;
        camera->forward = aux->forward;
        camera->position = aux->position;
        camera->up = aux->up;
        renderCameraTarget = false;
    }
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, float life, int component)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    int modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // TODO(student): Get shader location for uniform mat4 "View"
    int viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");
    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    int projectionLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint lifeLocation = glGetUniformLocation(shader->GetProgramID(), "lifeLocation");
    glUniform1f(lifeLocation, life);

    GLint componentLocation = glGetUniformLocation(shader->GetProgramID(), "componentLocation");
    glUniform1i(componentLocation, component);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

