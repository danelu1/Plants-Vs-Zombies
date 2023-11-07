#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

bool CheckCollision(float x1, float y1, float x2, float y2, float radius1, float radius2) {
    glm::vec3 p1 = glm::vec3(x1, y1, 1);
    glm::vec3 p2 = glm::vec3(x2, y2, 1);

    float distance = glm::distance(p1, p2);

    if (distance < radius1 + radius2) {
        return true;
    }

    return false;
}

void Tema1::DisplayDiamond(const std::string& name,
    bool& checkB,
    bool& checkS,
    float& sx,
    float& sy,
    float time,
    float cx,
    float cy,
    glm::mat3& matrix,
    std::unordered_map<std::string, Mesh*> meshes,
    std::unordered_map<std::string, Shader*> shaders)
{
    if (checkS) {
        sx -= 2 * time;
        sy -= 2 * time;

        if (sx <= 0 && sy <= 0) {
            checkS = false;
        }
    }

    glm::mat3 initialMatrix = matrix; // Store the initial matrix

    matrix *= transform2D::Translate(cx, cy);
    matrix *= transform2D::Scale(sx, sy);
    matrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes[name], shaders["VertexColor"], matrix);

    matrix = initialMatrix;
}

void m1::Tema1::CreateNewEnemy(const std::string name1,
    const std::string name2,
    Mesh* enemies[],
    glm::vec3 position1,
    glm::vec3 position2,
    int index,
    int& color,
    glm::vec3& enemyColor,
    glm::mat3 matrix,
    std::unordered_map<std::string, Mesh*>& meshes,
    std::unordered_map<std::string, Shader*> shaders)
{
    if (color == 0) {
        enemyColor = colors[0];
    }
    else if (color == 1) {
        enemyColor = colors[1];
    }
    else if (color == 2) {
        enemyColor = colors[2];
    }
    else if (color == 3) {
        enemyColor = colors[3];
    }

    meshes.erase(name1);
    meshes.erase(name2);
    enemies[index] = object2D::CreateEnemy1(name1, position1, 50, enemyColor, true);
    enemies[index + 9] = object2D::CreateEnemy1(name2, position2, 40, glm::vec3(0, 1, 0.75), true);
    AddMeshToList(enemies[index]);
    AddMeshToList(enemies[index + 9]);
    RenderMesh2D(meshes[name1], shaders["VertexColor"], matrix);
    RenderMesh2D(meshes[name2], shaders["VertexColor"], matrix);
}


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    sx = 1;
    sy = 1;
    check = true;
    isPressed = false;
    ty = 0;
    angularStep = 0;

    for (int i = 0; i < 9; i++) {
        tx[i] = 0;
    }

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    for (int i = 0; i < 9; i++) {
        translateX[i] = 100;
        translateY[i] = 0;
    }

    line = -1;
    color = -1;
    randomStarX = 30 * (1 + sin(M_PI / 10));
    randomStarYUp = 30 * (sqrt(1 - 4 * sin(M_PI / 10) * sin(M_PI / 10)) + sqrt(3 * sin(M_PI / 10) * sin(M_PI / 10) + 2 * sin(M_PI / 10)));
    randomStarYDown = 30 * cos(3 * M_PI / 10);
    x_inventory = 870;
    y_inventory = 580;

    for (int i = 0; i < 9; i++) {
        scaleX[i] = 1;
        scaleY[i] = 1;
    }

    for (int i = 0; i < 9; i++) {
        isEmptyCell[i] = true;
    }

    for (int i = 0; i < 9; i++) {
        isMoving[i] = false;
    }

    for (int i = 0; i < 9; i++) {
        checkScale[i] = true;
    }

    for (int i = 0; i < 9; i++) {
        checkButtonPressRight[i] = false;
    }

    for (int i = 0; i < 3; i++) {
        randomCoordinates[i] = glm::vec3(rand() % 1100 + 10, rand() % 620 + 10, 3);
    }

    for (int i = 0; i < 3; i++) {
        flag[i] = true;
    }

    colors[0] = glm::vec3(1, 0.5, 0); // orange
    colors[1] = glm::vec3(0, 0, 1); // blue
    colors[2] = glm::vec3(1, 1, 0); // yellow
    colors[3] = glm::vec3(0.5, 0, 0.5); // purple

    counterEnemies = 0;
    counterStars = 0;
    counterProjectiles = 0;
    randomTime = rand() % 10;
    randomTimeStars = rand() % 10;
    hexagoneRadius = 50;
    diamondRadius = 40;

    Mesh* square1 = object2D::CreateSquare("square1", glm::vec3(100, 15, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", glm::vec3(230, 15, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", glm::vec3(360, 15, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquare("square4", glm::vec3(100, 160, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square4);

    Mesh* square5 = object2D::CreateSquare("square5", glm::vec3(230, 160, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square5);

    Mesh* square6 = object2D::CreateSquare("square6", glm::vec3(360, 160, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square6);

    Mesh* square7 = object2D::CreateSquare("square7", glm::vec3(100, 305, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square7);

    Mesh* square8 = object2D::CreateSquare("square8", glm::vec3(230, 305, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square8);

    Mesh* square9 = object2D::CreateSquare("square9", glm::vec3(360, 305, 0), squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square9);

    Mesh* rectangle = object2D::CreateRectangle("rectangle", glm::vec3(10, 15, 0), 390, 60, glm::vec3(1, 0, 0), true);
    AddMeshToList(rectangle);

    float x_square = 50;
    float y_square = 580;
    float x_diamond = 110;
    float y_diamond = 600;
    float x_stars = 70;
    float y_stars = 550;

    boxes[0].stars_no = 1;
    boxes[1].stars_no = 2;
    boxes[2].stars_no = 2;
    boxes[3].stars_no = 3;

    int count = 1;

    for (int i = 0; i < 4; i++) {
        boxes[i].squareName = "box" + std::to_string(i + 1);
        boxes[i].diamondName = "item" + std::to_string(i + 1);
        boxes[i].square = object2D::CreateSquare(boxes[i].squareName, glm::vec3(x_square, y_square, 0), 120, colors[i], false);
        boxes[i].diamond = object2D::CreateDiamond(boxes[i].diamondName, glm::vec3(x_diamond, y_diamond, 0), 90, 40, 20, 0.8, colors[i], true);

        for (int j = 0; j < boxes[i].stars_no; j++) {
            boxes[i].starsNames.push_back("starItem" + std::to_string(count));
            boxes[i].stars.push_back(object2D::CreateStar(boxes[i].starsNames[j], glm::vec3(x_stars, y_stars, 0), 15, glm::vec3(0.5, 0.5, 0.5), true));
            AddMeshToList(boxes[i].stars[j]);
            count++;
            x_stars += 40;
        }

        x_square += 200;
        x_diamond += 200;
        x_stars = 70 + 200 * (i + 1);

        AddMeshToList(boxes[i].square);
        AddMeshToList(boxes[i].diamond);
    }

    lives_no = 3;

    float x_life = 850;
    float y_life = 610;

    for (int i = 0; i < 3; i++) {
        lives[i] = object2D::CreateSquare("life" + std::to_string(i + 1), glm::vec3(x_life, y_life, 0), 80, glm::vec3(1, 0, 0), true);
        AddMeshToList(lives[i]);

        x_life += 120;
    }

    starsOnScreen = 3;

    float x = 150;
    float y = 355;

    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0 && i > 0) {
            x = 150;
            y -= 145;
        }

        centers[0][i] = x;
        centers[1][i] = y;
        x += 130;
    }

    starCenterDistance = 15 * sqrt(3 * sin(M_PI / 10) * sin(M_PI / 10) + 2 * sin(M_PI / 10)) / 2;

    x = 200;
    y = 345;
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0 && i > 0) {
            x = 200;
            y -= 145;
        }

        x_projectile[i] = x;
        y_projectile[i] = y;
        x += 130;
    }

    for (int i = 0; i < 4; i++) {
        isDragged[i] = false;
    }
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    modelMatrix = glm::mat3(1);

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square5"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square6"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square7"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square8"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["square9"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

    for (int i = 0; i < 4; i++) {
        RenderMesh2D(meshes[boxes[i].squareName], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes[boxes[i].diamondName], shaders["VertexColor"], modelMatrix);
    }

    for (int j = 1; j < 9; j++) {
        RenderMesh2D(meshes["starItem" + std::to_string(j)], shaders["VertexColor"], modelMatrix);
    }

    for (int i = 0; i < 3; i++) {
        RenderMesh2D(meshes["life" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
    }

    counterEnemies += deltaTimeSeconds;
    counterStars += deltaTimeSeconds;

    if (counterEnemies > randomTime) {
        for (int i = 0; i < 3; i++) {
            if (!isMoving[i]) {
                line = rand() % 3;
                color = rand() % 4;
                isMoving[i] = true;

                modelMatrix = glm::mat3(1);

                modelMatrix *= transform2D::Translate(translateX[i], translateY[i]);

                if (line == 0) {
                    CreateNewEnemy("enemyOut" + std::to_string(i + 1), "enemyIn" + std::to_string(i + 1), enemies, glm::vec3(1200, 310, 1),
                        glm::vec3(1200, 320, 2), i, color, enemyColor1, modelMatrix, meshes, shaders);
                }
                else if (line == 1) {
                    CreateNewEnemy("enemyOut" + std::to_string(i + 1), "enemyIn" + std::to_string(i + 1), enemies, glm::vec3(1200, 165, 1),
                        glm::vec3(1200, 175, 2), i, color, enemyColor2, modelMatrix, meshes, shaders);
                }
                else if (line == 2) {
                    CreateNewEnemy("enemyOut" + std::to_string(i + 1), "enemyIn" + std::to_string(i + 1), enemies, glm::vec3(1200, 20, 1),
                        glm::vec3(1200, 30, 2), i, color, enemyColor3, modelMatrix, meshes, shaders);
                }

                break;
            }
        }
        counterEnemies = 0;
        randomTime = rand() % 10 + 3;
    }

    // Here I keep translating the hexagon until it reaches the end line
    // When the end line is reached, I decrement the number of lives.
    for (int i = 0; i < 3; i++) {
        if (isMoving[i]) {
            modelMatrix = glm::mat3(1);
            translateX[i] -= 100 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(translateX[i], translateY[i]);

            RenderMesh2D(meshes["enemyOut" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes["enemyIn" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

            if (translateX[i] <= -1150) {
                translateX[i] = 100;
                isMoving[i] = false;
                meshes.erase("life" + std::to_string(lives_no--));
                cout << i << endl;
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        modelMatrix = glm::mat3(1);
        if (!isEmptyCell[i] && isMoving[i]) {
            if (line == 0) {
                projectiles[i] = object2D::CreateStar("projectile" + std::to_string(i + 1), glm::vec3(x_projectile[i], y_projectile[i], 2), 15, draggedColor, true);
                AddMeshToList(projectiles[i]);

                angularStep += 5 * deltaTimeSeconds;
                tx[i] += 400 * deltaTimeSeconds;
                ty = 0;

                modelMatrix *= transform2D::Translate(tx[i], ty);
                modelMatrix *= transform2D::Translate(x_projectile[i], y_projectile[i] + starCenterDistance);
                modelMatrix *= transform2D::Rotate(angularStep);
                modelMatrix *= transform2D::Translate(- x_projectile[i], - y_projectile[i] - starCenterDistance);

                RenderMesh2D(meshes["projectile" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

                if (tx[i] >= 1100) {
                    tx[i] = 0;
                    angularStep = 0;
                }

                float pentagoneLength = 2 * 15 * sin(M_PI / 10);
                float sine = sin(M_PI / 10);
                float l = 15 * sqrt(3 * sine * sine + 2 * sine) / 2;
                float y = sqrt(225 - pentagoneLength * pentagoneLength);

                if (CheckCollision(x_projectile[i] + tx[i], y_projectile[i] + starCenterDistance, 1200 + translateX[i], 360, l + y, hexagoneRadius)) {
                    tx[i] == 0;
                    meshes.erase("projectile" + std::to_string(i + 1));
                }
            }
            else if (line == 1) {
                projectiles[i] = object2D::CreateStar("projectile" + std::to_string(i + 1), glm::vec3(x_projectile[i], y_projectile[i], 2), 15, draggedColor, true);
                AddMeshToList(projectiles[i]);

                angularStep += 5 * deltaTimeSeconds;
                tx[i] += 400 * deltaTimeSeconds;
                ty = 0;

                modelMatrix *= transform2D::Translate(tx[i], ty);
                modelMatrix *= transform2D::Translate(x_projectile[i], y_projectile[i] + starCenterDistance);
                modelMatrix *= transform2D::Rotate(angularStep);
                modelMatrix *= transform2D::Translate(-x_projectile[i], -y_projectile[i] - starCenterDistance);

                RenderMesh2D(meshes["projectile" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

                if (tx[i] >= 1100) {
                    tx[i] = 0;
                    angularStep = 0;
                }

                float pentagoneLength = 2 * 15 * sin(M_PI / 10);
                float sine = sin(M_PI / 10);
                float l = 15 * sqrt(3 * sine * sine + 2 * sine) / 2;
                float y = sqrt(225 - pentagoneLength * pentagoneLength);

                if (CheckCollision(x_projectile[i] + tx[i], y_projectile[i] + starCenterDistance, 1200 + translateX[i], 215, l + y, hexagoneRadius)) {
                    tx[i] == 0;
                    meshes.erase("projectile" + std::to_string(i + 1));
                }
            }
            else if (line == 2) {
                projectiles[i] = object2D::CreateStar("projectile" + std::to_string(i + 1), glm::vec3(x_projectile[i], y_projectile[i], 2), 15, draggedColor, true);
                AddMeshToList(projectiles[i]);

                angularStep += 5 * deltaTimeSeconds;
                tx[i] += 400 * deltaTimeSeconds;
                ty = 0;

                modelMatrix *= transform2D::Translate(tx[i], ty);
                modelMatrix *= transform2D::Translate(x_projectile[i], y_projectile[i] + starCenterDistance);
                modelMatrix *= transform2D::Rotate(angularStep);
                modelMatrix *= transform2D::Translate(-x_projectile[i], -y_projectile[i] - starCenterDistance);

                RenderMesh2D(meshes["projectile" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

                if (tx[i] >= 1100) {
                    tx[i] = 0;
                    angularStep = 0;
                }

                float pentagoneLength = 2 * 15 * sin(M_PI / 10);
                float sine = sin(M_PI / 10);
                float l = 15 * sqrt(3 * sine * sine + 2 * sine) / 2;
                float y = sqrt(225 - pentagoneLength * pentagoneLength);

                if (CheckCollision(x_projectile[i] + tx[i], y_projectile[i] + starCenterDistance, 1200 + translateX[i], 70, l + y, hexagoneRadius)) {
                    tx[i] == 0;
                    meshes.erase("projectile" + std::to_string(i + 1));
                }
            }
        }
    }

    if (line == 0) {
        if ((CheckCollision(150, 355, 1200 + translateX[0], 360, diamondRadius, hexagoneRadius) ||
            CheckCollision(150, 355, 1200 + translateX[1], 360, diamondRadius, hexagoneRadius) ||
            CheckCollision(150, 355, 1200 + translateX[2], 360, diamondRadius, hexagoneRadius))
            && !isEmptyCell[0]) {
            modelMatrix = glm::mat3(1);
            checkScale[0] = true;
            sx -= 2 * deltaTimeSeconds;
            sy -= 2 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(150, 355);
            modelMatrix *= transform2D::Scale(sx, sy);
            modelMatrix *= transform2D::Translate(-150, -355);
            RenderMesh2D(meshes["diamond" + std::to_string(0 + 1)], shaders["VertexColor"], modelMatrix);

            if (sx <= 0 && sy <= 0) {
                sx = 1;
                sy = 1;
                isEmptyCell[0] = true;
                checkScale[0] = false;
                meshes.erase("diamond" + std::to_string(0 + 1));
            }
        }
    }

    /*for (int i = 0; i < 3; i++) {
        if (CheckCollision(centers[0][i], centers[1][i], 1200 + translateX[i], 360, diamondRadius, hexagoneRadius) && !isEmptyCell[i]) {
            modelMatrix = glm::mat3(1);
            checkScale[i] = true;
            sx -= 2 * deltaTimeSeconds;
            sy -= 2 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(centers[0][i], centers[1][i]);
            modelMatrix *= transform2D::Scale(sx, sy);
            modelMatrix *= transform2D::Translate(-centers[0][i], -centers[1][i]);
            RenderMesh2D(meshes["diamond" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

            if (sx <= 0 && sy <= 0) {
                sx = 1;
                sy = 1;
                isEmptyCell[i] = true;
                checkScale[i] = false;
                disappear[i] = false;
                meshes.erase("diamond" + std::to_string(i + 1));
            }
        }
    }
    for (int i = 3; i < 6; i++) {
        if (CheckCollision(centers[0][i], centers[1][i], 1200 + translateX[i], 215, diamondRadius, hexagoneRadius) && !isEmptyCell[i]) {
            modelMatrix = glm::mat3(1);
            checkScale[i] = true;
            sx -= 2 * deltaTimeSeconds;
            sy -= 2 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(centers[0][i], centers[1][i]);
            modelMatrix *= transform2D::Scale(sx, sy);
            modelMatrix *= transform2D::Translate(-centers[0][i], -centers[1][i]);
            RenderMesh2D(meshes["diamond" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

            if (sx <= 0 && sy <= 0) {
                sx = 1;
                sy = 1;
                isEmptyCell[i] = true;
                checkScale[i] = false;
                disappear[i] = false;
                meshes.erase("diamond" + std::to_string(i + 1));
            }
        }
    }
    for (int i = 6; i < 9; i++) {
        if (CheckCollision(centers[0][i], centers[1][i], 1200 + translateX[i], 70, diamondRadius, hexagoneRadius) && !isEmptyCell[i]) {
            modelMatrix = glm::mat3(1);
            checkScale[i] = true;
            sx -= 2 * deltaTimeSeconds;
            sy -= 2 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(centers[0][i], centers[1][i]);
            modelMatrix *= transform2D::Scale(sx, sy);
            modelMatrix *= transform2D::Translate(-centers[0][i], -centers[1][i]);
            RenderMesh2D(meshes["diamond" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

            if (sx <= 0 && sy <= 0) {
                sx = 1;
                sy = 1;
                isEmptyCell[i] = true;
                checkScale[i] = false;
                disappear[i] = false;
                meshes.erase("diamond" + std::to_string(i + 1));
            }
        }
    }*/

    //if (lives_no == 0) {
    //    exit(0);
    //}

    // Here I draw stars on screen based on the random time passed and the fact that a star
    // should not be drawn anymore if I clicked on it.
    if (counterStars > randomTimeStars) {
        for (int i = 0; i < 3; i++) {
            if (flag[i]) {
                modelMatrix = glm::mat3(1);
                randomStars[i] = object2D::CreateStar("randomStar" + std::to_string(i + 1), randomCoordinates[i], 30, glm::vec3(1, 0.753f, 0.796f), true);
                AddMeshToList(randomStars[i]);
                RenderMesh2D(meshes["randomStar" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    modelMatrix = glm::mat3(1);

    // Here I draw the inventory containing the number of stars collected until the actual moment.
    for (int i = 0; i < inventory.size(); i++) {
        RenderMesh2D(meshes["resource" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
    }

    for (int i = 0; i < 9; i++) {
        modelMatrix = glm::mat3(1);
        DisplayDiamond("diamond" + std::to_string(i + 1), checkButtonPressRight[i], checkScale[i], scaleX[i],
            scaleY[i], deltaTimeSeconds, centers[0][i], centers[1][i], modelMatrix, meshes, shaders);
    }

    for (int i = 0; i < 4; i++) {
        if (isDragged[i]) {
            modelMatrix = glm::mat3(1);
            RenderMesh2D(meshes["dragged" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
        }
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1) {
        float x1 = 100;
        float y1 = 305;
        float x2 = 200;
        float y2 = 405;

        for (int i = 0; i < 9; i++) {
            if (i % 3 == 0 && i > 0) {
                x1 = 100;
                x2 = 200;

                y1 -= 145;
                y2 -= 145;
            }

            if (mouseX >= x1 && mouseX <= x2 && mouseY >= 720 - y2 && mouseY <= 720 - y1 && isEmptyCell[i]) {
                if (scaleX[i] <= 0 && scaleY[i] <= 0) {
                    meshes.erase("diamond" + std::to_string(i + 1));
                    scaleX[i] = 1;
                    scaleY[i] = 1;
                }

                if (y1 == 305) {
                    for (int j = 0; j < 4; j++) {
                        if (enemyColor1 == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1), 90, 40, 20, 0.8f, enemyColor1, true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
                else if (y1 == 160) {
                    for (int j = 0; j < 4; j++) {
                        if (enemyColor2 == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1), 90, 40, 20, 0.8f, enemyColor2, true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
                else if (y1 == 15) {
                    for (int j = 0; j < 4; j++) {
                        if (enemyColor3 == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1), 90, 40, 20, 0.8f, enemyColor3, true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
            }
            x1 += 130;
            x2 += 130;
        }


        for (int i = 0; i < 3; i++) {
            float x_left = randomCoordinates[i].x - randomStarX;
            float x_right = randomCoordinates[i].x + randomStarX;
            float y_up = 720 - (randomCoordinates[i].y + randomStarYUp);
            float y_down = 720 - (randomCoordinates[i].y - randomStarYDown);

            if (mouseX >= x_left && mouseX <= x_right && mouseY >= y_up && mouseY <= y_down) {
                flag[i] = false;
                meshes.erase("randomStar" + std::to_string(i + 1));
                starsOnScreen--;

                if (inventory.size() <= 9) {
                    Mesh* resource = object2D::CreateStar("resource" + std::to_string(inventory.size() + 1), glm::vec3(x_inventory, y_inventory, 0), 15, glm::vec3(0.5, 0.5, 0.5), true);
                    inventory.push(resource);
                    AddMeshToList(resource);
                    x_inventory += 40;
                }

                break;
            }
        }

        if (starsOnScreen == 0) {
            counterStars = 0;
            randomTimeStars = rand() % 10;
            starsOnScreen = 3;
            for (int i = 0; i < 3; i++) {
                flag[i] = true;
                randomCoordinates[i] = glm::vec3(rand() % 1100 + 10, rand() % 620 + 10, 3);
            }
        }

        x1 = 90;
        x2 = 150;
        y1 = 600;
        y2 = 690;

        for (int i = 0; i < 4; i++) {
            if (mouseX >= x1 && mouseX <= x2 && mouseY >= 720 - y2 && mouseY <= 720 - y1) {
                isDragged[i] = true;
                isPressed = true;

                break;
            }

            x1 += 200;
            x2 += 200;
        }
    }

    if (button == 2) {
        int x1 = 130;
        int y1 = 310;
        int x2 = 185;
        int y2 = 400;

        for (int i = 0; i < 9; i++) {
            if (i % 3 == 0 && i > 0) {
                x1 = 130;
                x2 = 185;

                y1 -= 155;
                y2 -= 155;
            }

            if (mouseX >= x1 && mouseX <= x2 && mouseY >= 720 - y2 && mouseY <= 720 - y1 && !isEmptyCell[i]) {
                checkButtonPressRight[i] = true;
                checkScale[i] = true;
                isEmptyCell[i] = true;
            }
            x1 += 130;
            x2 += 130;
        }
    }
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    for (int i = 0; i < 4; i++) {
        if (isDragged[i] && isPressed) {
            diamond = object2D::CreateDiamond("dragged" + std::to_string(i + 1), glm::vec3(mouseX, 720 - mouseY, 1), 90, 40, 20, 0.8, colors[i], true);
            AddMeshToList(diamond);
            index = i + 1;
            draggedColor = colors[i];

            break;
        }
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (button == 1) {
        isPressed = false;
        isDragged[index - 1] = false;

        float x1 = 100;
        float y1 = 305;
        float x2 = 200;
        float y2 = 405;

        for (int i = 0; i < 9; i++) {
            if (i % 3 == 0 && i > 0) {
                x1 = 100;
                x2 = 200;

                y1 -= 145;
                y2 -= 145;
            }

            if (mouseX >= x1 && mouseX <= x2 && mouseY >= 720 - y2 && mouseY <= 720 - y1 && isEmptyCell[i]) {
                if (scaleX[i] <= 0 && scaleY[i] <= 0) {
                    meshes.erase("diamond" + std::to_string(i + 1));
                    scaleX[i] = 1;
                    scaleY[i] = 1;
                }

                if (y1 == 305) {
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, draggedColor, true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
                else if (y1 == 160) {
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, draggedColor, true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
                else if (y1 == 15) {
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, draggedColor, true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
            }

             x1 += 130;
             x2 += 130;
        }

        meshes.erase("dragged" + std::to_string(index));
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}