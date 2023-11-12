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
bool contains(std::queue<glm::vec3> colors[], glm::vec3 color, int line) {
    std::queue<glm::vec3> q = colors[line];
    while (!q.empty()) {
        if (q.front() == color) {
            return true;
        }
        q.pop();
    }

    return false;
}

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

    glm::mat3 initialMatrix = matrix;

    matrix *= transform2D::Translate(cx, cy);
    matrix *= transform2D::Scale(sx, sy);
    matrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes[name], shaders["VertexColor"], matrix);

    matrix = initialMatrix;
}

void Tema1::CreateNewEnemy(const std::string name1,
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
    enemies[index + 3] = object2D::CreateEnemy1(name2, position2, 40, glm::vec3(0, 1, 0.75), true);
    AddMeshToList(enemies[index]);
    AddMeshToList(enemies[index + 3]);
    RenderMesh2D(meshes[name1], shaders["VertexColor"], matrix);
    RenderMesh2D(meshes[name2], shaders["VertexColor"], matrix);
}

void Tema1::KillEnemy(const std::string name1,
    const std::string name2,
    float time,
    int line,
    float x,
    glm::vec3 color,
    int* enemyLives,
    std::unordered_map<int, int> map,
    float* translateX,
    float* tx,
    float* scaleEnemyX,
    float* scaleEnemyY,
    bool* isMoving,
    glm::mat3& modelMatrix,
    std::unordered_map<std::string, Mesh*>& meshes,
    std::unordered_map<std::string, Shader*> shaders)
{
    if (enemyLives[0] >= 3 && map[0] == line) {
        glm::mat3 matrix = modelMatrix;

        scaleOut[0] = true;
        scaleIn[0] = true;

        meshes.erase(name1 + std::to_string(1));
        meshes.erase(name2 + std::to_string(1));
        enemies[0] = object2D::CreateEnemy1(name1 + std::to_string(1), glm::vec3(distance[0], x, 1), 50, color, true);
        enemies[3] = object2D::CreateEnemy1(name2 + std::to_string(1), glm::vec3(distance[0], x + 10, 2), 40, glm::vec3(0, 1, 0.75), true);
        AddMeshToList(enemies[0]);
        AddMeshToList(enemies[3]);

        scaleEnemyX[0] -= time;
        scaleEnemyY[0] -= time;

        modelMatrix *= transform2D::Translate(distance[0], x + 50);
        modelMatrix *= transform2D::Scale(scaleEnemyX[0], scaleEnemyY[0]);
        modelMatrix *= transform2D::Translate(-distance[0], -(x + 50));
        RenderMesh2D(meshes[name1 + std::to_string(1)], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes[name2 + std::to_string(1)], shaders["VertexColor"], modelMatrix);

        if (scaleEnemyX[0] <= 0 && scaleEnemyY[0] <= 0) {
            if (!lineColors[line].empty()) {
                lineColors[line].pop();
            }
            translateX[0] = 100;
            enemyLives[0] = 0;
            isMoving[0] = false;
            scaleEnemyX[0] = 1;
            scaleEnemyY[0] = 1;
            meshes.erase(name1 + std::to_string(1));
            meshes.erase(name2 + std::to_string(1));
            tx[3 * line] = 0;
            tx[3 * line + 1] = 0;
            tx[3 * line + 2] = 0;
            scaleOut[0] = false;
            scaleIn[0] = false;
            cout << enemyLives[0] << endl;
        }
        modelMatrix = matrix;
    }
    if (enemyLives[1] >= 3 && map[1] == line) {
        glm::mat3 matrix = modelMatrix;

        scaleOut[1] = true;
        scaleIn[1] = true;

        meshes.erase(name1 + std::to_string(2));
        meshes.erase(name2 + std::to_string(2));
        enemies[1] = object2D::CreateEnemy1(name1 + std::to_string(2), glm::vec3(distance[1], x, 1), 50, color, true);
        enemies[4] = object2D::CreateEnemy1(name2 + std::to_string(2), glm::vec3(distance[1], x + 10, 2), 40, glm::vec3(0, 1, 0.75), true);
        AddMeshToList(enemies[1]);
        AddMeshToList(enemies[4]);

        scaleEnemyX[1] -= time;
        scaleEnemyY[1] -= time;

        modelMatrix *= transform2D::Translate(distance[1], x + 50);
        modelMatrix *= transform2D::Scale(scaleEnemyX[1], scaleEnemyY[1]);
        modelMatrix *= transform2D::Translate(-distance[1], -(x + 50));
        RenderMesh2D(meshes[name1 + std::to_string(2)], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes[name2 + std::to_string(2)], shaders["VertexColor"], modelMatrix);

        if (scaleEnemyX[1] <= 0 && scaleEnemyY[1] <= 0) {
            if (!lineColors[line].empty()) {
                lineColors[line].pop();
            }
            translateX[1] = 100;
            enemyLives[1] = 0;
            isMoving[1] = false;
            scaleEnemyX[1] = 1;
            scaleEnemyY[1] = 1;
            meshes.erase(name1 + std::to_string(2));
            meshes.erase(name2 + std::to_string(2));
            tx[3 * line] = 0;
            tx[3 * line + 1] = 0;
            tx[3 * line + 2] = 0;
            scaleOut[1] = false;
            scaleIn[1] = false;
            cout << enemyLives[1] << endl;
        }
        modelMatrix = matrix;
    }
    if (enemyLives[2] >= 3 && map[2] == line) {
        glm::mat3 matrix = modelMatrix;

        scaleOut[2] = true;
        scaleIn[2] = true;

        meshes.erase(name1 + std::to_string(3));
        meshes.erase(name2 + std::to_string(3));
        enemies[2] = object2D::CreateEnemy1(name1 + std::to_string(3), glm::vec3(distance[2], x, 1), 50, color, true);
        enemies[5] = object2D::CreateEnemy1(name2 + std::to_string(3), glm::vec3(distance[2], x + 10, 2), 40, glm::vec3(0, 1, 0.75), true);
        AddMeshToList(enemies[2]);
        AddMeshToList(enemies[5]);

        scaleEnemyX[2] -= time;
        scaleEnemyY[2] -= time;

        modelMatrix *= transform2D::Translate(distance[2], x + 50);
        modelMatrix *= transform2D::Scale(scaleEnemyX[2], scaleEnemyY[2]);
        modelMatrix *= transform2D::Translate(-distance[2], -(x + 50));
        RenderMesh2D(meshes[name1 + std::to_string(3)], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes[name2 + std::to_string(3)], shaders["VertexColor"], modelMatrix);

        if (scaleEnemyX[2] <= 0 && scaleEnemyY[2] <= 0) {
            if (!lineColors[line].empty()) {
                lineColors[line].pop();
            }
            translateX[2] = 100;
            enemyLives[2] = 0;
            isMoving[2] = false;
            scaleEnemyX[2] = 1;
            scaleEnemyY[2] = 1;
            meshes.erase(name1 + std::to_string(3));
            meshes.erase(name2 + std::to_string(3));
            tx[3 * line] = 0;
            tx[3 * line + 1] = 0;
            tx[3 * line + 2] = 0;
            scaleOut[2] = false;
            scaleIn[2] = false;
            cout << enemyLives[2] << endl;
        }
        modelMatrix = matrix;
    }
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
    
    for (int i = 0; i < 9; i++) {
        angularStep[i] = 0;
    }

    for (int i = 0; i < 9; i++) {
        tx[i] = 0;
    }

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    for (int i = 0; i < 3; i++) {
        translateX[i] = 100;
        translateY[i] = 0;
    }

    for (int i = 0; i < 3; i++) {
        scaleEnemyX[i] = 1;
        scaleEnemyY[i] = 1;
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

    for (int i = 0; i < 3; i++) {
        distance[i] = 0;
    }

    colors[0] = glm::vec3(1, 0.5, 0); // orange
    colors[1] = glm::vec3(0, 0, 1); // blue
    colors[2] = glm::vec3(1, 1, 0); // yellow
    colors[3] = glm::vec3(0.5, 0, 0.5); // purple

    counterEnemies = 0;
    counterStars = 0;
    counterProjectiles = 0;
    randomTime = rand() % 5 + 5;
    randomTimeStars = rand() % 10;
    hexagoneRadius = 50;
    diamondRadius = 40;

    pentagoneLength = 2 * 15 * sin(M_PI / 10);
    sine = sin(M_PI / 10);
    l = 15 * sqrt(3 * sine * sine + 2 * sine) / 2;
    y = sqrt(225 - pentagoneLength * pentagoneLength);

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

    for (int i = 0; i < 3; i++) {
        enemyLives[i] = 0;
    }

    for (int i = 0; i < 3; i++) {
        scaleOut[i] = false;
    }

    for (int i = 0; i < 3; i++) {
        scaleIn[i] = false;
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
                        glm::vec3(1200, 320, 2), line, color, enemyColor1, modelMatrix, meshes, shaders);
                    lineColors[0].push(enemyColor1);
                    cout << "Enemy spawned " << i << endl;
                    map[i] = line;
                }
                else if (line == 1) {
                    CreateNewEnemy("enemyOut" + std::to_string(i + 1), "enemyIn" + std::to_string(i + 1), enemies, glm::vec3(1200, 165, 1),
                        glm::vec3(1200, 175, 2), line, color, enemyColor2, modelMatrix, meshes, shaders);
                    lineColors[1].push(enemyColor2);
                    cout << "Enemy spawned " << i << endl;;
                    map[i] = line;
                }
                else if (line == 2) {
                    CreateNewEnemy("enemyOut" + std::to_string(i + 1), "enemyIn" + std::to_string(i + 1), enemies, glm::vec3(1200, 20, 1),
                        glm::vec3(1200, 30, 2), line, color, enemyColor3, modelMatrix, meshes, shaders);
                    lineColors[2].push(enemyColor3);
                    cout << "Enemy spawned " << i << endl;
                    map[i] = line;
                }

                break;
            }
        }
        counterEnemies = 0;
        randomTime = rand() % 10 + 1;
    }

    // Here I keep translating the hexagon until it reaches the end line
    // When the end line is reached, I decrement the number of lives.
    for (int i = 0; i < 3; i++) {
        if (isMoving[i] && !scaleOut[i] && !scaleIn[i]) {
            modelMatrix = glm::mat3(1);
            translateX[i] -= 100 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(translateX[i], translateY[i]);

            RenderMesh2D(meshes["enemyOut" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes["enemyIn" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);

            if (translateX[i] <= -1150) {
                if (map[i] == 0) {
                    lineColors[0].pop();
                }
                else if (map[i] == 1) {
                    lineColors[1].pop();
                }
                else if (map[i] == 2) {
                    lineColors[2].pop();
                }
                translateX[i] = 100;
                isMoving[i] = false;
                meshes.erase("life" + std::to_string(lives_no--));
            }
        }
        else if (isMoving[i] && scaleOut[i] && scaleIn[i]) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(translateX[i], translateY[i]);
            RenderMesh2D(meshes["enemyOut" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes["enemyIn" + std::to_string(i + 1)], shaders["VertexColor"], modelMatrix);
        }
    }

    counterProjectiles += deltaTimeSeconds;

    /*
        Launching stars and checking for collision between them and the hexagons
    */
    if (((isMoving[0] && map[0] == 0) ||
        (isMoving[1] && map[1] == 0) ||
        (isMoving[2] && map[2] == 0)) &&
        !isEmptyCell[0] && contains(lineColors, cellColor[0], 0)) {
        modelMatrix = glm::mat3(1);

        if (counterProjectiles > 2) {
            meshes.erase("projectile1");
            projectiles[0] = object2D::CreateStar("projectile1", glm::vec3(x_projectile[0], y_projectile[0], 2), 15, cellColor[0], true);
            AddMeshToList(projectiles[0]);
            counterProjectiles = 0;
        }

        angularStep[0] += 2 * deltaTimeSeconds;
        tx[0] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[0], ty);
        modelMatrix *= transform2D::Translate(x_projectile[0], y_projectile[0] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[0]);
        modelMatrix *= transform2D::Translate(-x_projectile[0], -y_projectile[0] - starCenterDistance);

        RenderMesh2D(meshes["projectile1"], shaders["VertexColor"], modelMatrix);

        if (tx[0] >= 1100) {
            tx[0] = 0;
            angularStep[0] = 0;
        }

        if ((map[0] == 0 && CheckCollision(x_projectile[0] + tx[0], y_projectile[0] + starCenterDistance, 1200 + translateX[0], 360, l + y, hexagoneRadius)) ||
            (map[1] == 0 && CheckCollision(x_projectile[0] + tx[0], y_projectile[0] + starCenterDistance, 1200 + translateX[1], 360, l + y, hexagoneRadius)) ||
            (map[2] == 0 && CheckCollision(x_projectile[0] + tx[0], y_projectile[0] + starCenterDistance, 1200 + translateX[2], 360, l + y, hexagoneRadius))) {
            tx[0] = 0;
            meshes.erase("projectile1");
            if (map[0] == 0) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 0) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 0) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 0, 310, enemyColor1, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 0) ||
        (isMoving[1] && map[1] == 0) ||
        (isMoving[2] && map[2] == 0)) &&
        !isEmptyCell[1] && contains(lineColors, cellColor[1], 0)) {
        modelMatrix = glm::mat3(1);

        projectiles[1] = object2D::CreateStar("projectile2", glm::vec3(x_projectile[1], y_projectile[1], 2), 15, cellColor[1], true);
        AddMeshToList(projectiles[1]);

        angularStep[1] += 2 * deltaTimeSeconds;
        tx[1] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[1], ty);
        modelMatrix *= transform2D::Translate(x_projectile[1], y_projectile[1] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[1]);
        modelMatrix *= transform2D::Translate(-x_projectile[1], -y_projectile[1] - starCenterDistance);

        RenderMesh2D(meshes["projectile2"], shaders["VertexColor"], modelMatrix);

        if (tx[1] >= 1100) {
            tx[1] = 0;
            angularStep[1] = 0;
        }

        if ((map[0] == 0 && CheckCollision(x_projectile[1] + tx[1], y_projectile[1] + starCenterDistance, 1200 + translateX[0], 360, l + y, hexagoneRadius)) ||
            (map[1] == 0 && CheckCollision(x_projectile[1] + tx[1], y_projectile[1] + starCenterDistance, 1200 + translateX[1], 360, l + y, hexagoneRadius)) ||
            (map[2] == 0 && CheckCollision(x_projectile[1] + tx[1], y_projectile[1] + starCenterDistance, 1200 + translateX[2], 360, l + y, hexagoneRadius))) {
            tx[1] = 0;
            meshes.erase("projectile2");
            if (map[0] == 0) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 0) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 0) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 0, 310, enemyColor1, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 0) ||
        (isMoving[1] && map[1] == 0) ||
        (isMoving[2] && map[2] == 0)) &&
        !isEmptyCell[2] && contains(lineColors, cellColor[2], 0)) {
        modelMatrix = glm::mat3(1);

        projectiles[2] = object2D::CreateStar("projectile3", glm::vec3(x_projectile[2], y_projectile[2], 2), 15, cellColor[2], true);
        AddMeshToList(projectiles[2]);

        angularStep[2] += 2 * deltaTimeSeconds;
        tx[2] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[2], ty);
        modelMatrix *= transform2D::Translate(x_projectile[2], y_projectile[2] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[2]);
        modelMatrix *= transform2D::Translate(-x_projectile[2], -y_projectile[2] - starCenterDistance);

        RenderMesh2D(meshes["projectile3"], shaders["VertexColor"], modelMatrix);

        if (tx[2] >= 1100) {
            tx[2] = 0;
            angularStep[2] = 0;
        }

        if ((map[0] == 0 && CheckCollision(x_projectile[2] + tx[2], y_projectile[2] + starCenterDistance, 1200 + translateX[0], 360, l + y, hexagoneRadius)) ||
            (map[1] == 0 && CheckCollision(x_projectile[2] + tx[2], y_projectile[2] + starCenterDistance, 1200 + translateX[1], 360, l + y, hexagoneRadius)) ||
            (map[2] == 0 && CheckCollision(x_projectile[2] + tx[2], y_projectile[2] + starCenterDistance, 1200 + translateX[2], 360, l + y, hexagoneRadius))) {
            tx[2] = 0;
            meshes.erase("projectile3");
            if (map[0] == 0) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 0) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 0) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 0, 310, enemyColor1, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 1) ||
        (isMoving[1] && map[1] == 1) ||
        (isMoving[2] && map[2] == 1)) &&
        !isEmptyCell[3] && contains(lineColors, cellColor[3], 1)) {
        modelMatrix = glm::mat3(1);

        projectiles[3] = object2D::CreateStar("projectile4", glm::vec3(x_projectile[3], y_projectile[3], 2), 15, cellColor[3], true);
        AddMeshToList(projectiles[3]);

        angularStep[3] += 2 * deltaTimeSeconds;
        tx[3] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[3], ty);
        modelMatrix *= transform2D::Translate(x_projectile[3], y_projectile[3] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[3]);
        modelMatrix *= transform2D::Translate(-x_projectile[3], -y_projectile[3] - starCenterDistance);

        RenderMesh2D(meshes["projectile4"], shaders["VertexColor"], modelMatrix);

        if (tx[3] >= 1100) {
            tx[3] = 0;
            angularStep[3] = 0;
        }

        if ((map[0] == 1 && CheckCollision(x_projectile[3] + tx[3], y_projectile[3] + starCenterDistance, 1200 + translateX[0], 215, l + y, hexagoneRadius)) ||
            (map[1] == 1 && CheckCollision(x_projectile[3] + tx[3], y_projectile[3] + starCenterDistance, 1200 + translateX[1], 215, l + y, hexagoneRadius)) ||
            (map[2] == 1 && CheckCollision(x_projectile[3] + tx[3], y_projectile[3] + starCenterDistance, 1200 + translateX[2], 215, l + y, hexagoneRadius))) {
            tx[3] = 0;
            meshes.erase("projectile4");
            if (map[0] == 1) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 1) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 1) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 1, 165, enemyColor2, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 1) ||
        (isMoving[1] && map[1] == 1) ||
        (isMoving[2] && map[2] == 1)) &&
        !isEmptyCell[4] && contains(lineColors, cellColor[4], 1)) {
        modelMatrix = glm::mat3(1);

        projectiles[4] = object2D::CreateStar("projectile5", glm::vec3(x_projectile[4], y_projectile[4], 2), 15, cellColor[4], true);
        AddMeshToList(projectiles[4]);

        angularStep[4] += 2 * deltaTimeSeconds;
        tx[4] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[4], ty);
        modelMatrix *= transform2D::Translate(x_projectile[4], y_projectile[4] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[4]);
        modelMatrix *= transform2D::Translate(-x_projectile[4], -y_projectile[4] - starCenterDistance);

        RenderMesh2D(meshes["projectile5"], shaders["VertexColor"], modelMatrix);

        if (tx[4] >= 1100) {
            tx[4] = 0;
            angularStep[4] = 0;
        }

        if ((map[0] == 1 && CheckCollision(x_projectile[4] + tx[4], y_projectile[4] + starCenterDistance, 1200 + translateX[0], 215, l + y, hexagoneRadius)) ||
            (map[1] == 1 && CheckCollision(x_projectile[4] + tx[4], y_projectile[4] + starCenterDistance, 1200 + translateX[1], 215, l + y, hexagoneRadius)) ||
            (map[2] == 1 && CheckCollision(x_projectile[4] + tx[4], y_projectile[4] + starCenterDistance, 1200 + translateX[2], 215, l + y, hexagoneRadius))) {
            tx[4] = 0;
            meshes.erase("projectile5");
            if (map[0] == 1) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 1) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 1) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 1, 165, enemyColor2, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 1) ||
        (isMoving[1] && map[1] == 1) ||
        (isMoving[2] && map[2] == 1)) &&
        !isEmptyCell[5] && contains(lineColors, cellColor[5], 1)) {
        modelMatrix = glm::mat3(1);

        projectiles[5] = object2D::CreateStar("projectile6", glm::vec3(x_projectile[5], y_projectile[5], 2), 15, cellColor[5], true);
        AddMeshToList(projectiles[5]);

        angularStep[5] += 2 * deltaTimeSeconds;
        tx[5] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[5], ty);
        modelMatrix *= transform2D::Translate(x_projectile[5], y_projectile[5] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[5]);
        modelMatrix *= transform2D::Translate(-x_projectile[5], -y_projectile[5] - starCenterDistance);

        RenderMesh2D(meshes["projectile6"], shaders["VertexColor"], modelMatrix);

        if (tx[5] >= 1100) {
            tx[5] = 0;
            angularStep[5] = 0;
        }

        if ((map[0] == 1 && CheckCollision(x_projectile[5] + tx[5], y_projectile[5] + starCenterDistance, 1200 + translateX[0], 215, l + y, hexagoneRadius)) ||
            (map[1] == 1 && CheckCollision(x_projectile[5] + tx[5], y_projectile[5] + starCenterDistance, 1200 + translateX[1], 215, l + y, hexagoneRadius)) ||
            (map[2] == 1 && CheckCollision(x_projectile[5] + tx[5], y_projectile[5] + starCenterDistance, 1200 + translateX[2], 215, l + y, hexagoneRadius))) {
            tx[5] = 0;
            meshes.erase("projectile6");
            if (map[0] == 1) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 1) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 1) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 1, 165, enemyColor2, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 2) ||
        (isMoving[1] && map[1] == 2) ||
        (isMoving[2] && map[2] == 2)) &&
        !isEmptyCell[6] && contains(lineColors, cellColor[6], 2)) {
        modelMatrix = glm::mat3(1);

        projectiles[6] = object2D::CreateStar("projectile7", glm::vec3(x_projectile[6], y_projectile[6], 2), 15, cellColor[6], true);
        AddMeshToList(projectiles[6]);

        angularStep[6] += 2 * deltaTimeSeconds;
        tx[6] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[6], ty);
        modelMatrix *= transform2D::Translate(x_projectile[6], y_projectile[6] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[6]);
        modelMatrix *= transform2D::Translate(-x_projectile[6], -y_projectile[6] - starCenterDistance);

        RenderMesh2D(meshes["projectile7"], shaders["VertexColor"], modelMatrix);

        if (tx[6] >= 1100) {
            tx[6] = 0;
            angularStep[6] = 0;
        }

        if ((map[0] == 2 && CheckCollision(x_projectile[6] + tx[6], y_projectile[6] + starCenterDistance, 1200 + translateX[0], 70, l + y, hexagoneRadius)) ||
            (map[1] == 2 && CheckCollision(x_projectile[6] + tx[6], y_projectile[6] + starCenterDistance, 1200 + translateX[1], 70, l + y, hexagoneRadius)) ||
            (map[2] == 2 && CheckCollision(x_projectile[6] + tx[6], y_projectile[6] + starCenterDistance, 1200 + translateX[2], 70, l + y, hexagoneRadius))) {
            tx[6] = 0;
            meshes.erase("projectile7");
            if (map[0] == 2) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 2) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 2) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 2, 20, enemyColor3, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 2) ||
        (isMoving[1] && map[1] == 2) ||
        (isMoving[2] && map[2] == 2)) &&
        !isEmptyCell[7] && contains(lineColors, cellColor[7], 2)) {
        modelMatrix = glm::mat3(1);

        projectiles[7] = object2D::CreateStar("projectile8", glm::vec3(x_projectile[7], y_projectile[7], 2), 15, cellColor[7], true);
        AddMeshToList(projectiles[7]);

        angularStep[7] += 2 * deltaTimeSeconds;
        tx[7] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[7], ty);
        modelMatrix *= transform2D::Translate(x_projectile[7], y_projectile[7] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[7]);
        modelMatrix *= transform2D::Translate(-x_projectile[7], -y_projectile[7] - starCenterDistance);

        RenderMesh2D(meshes["projectile8"], shaders["VertexColor"], modelMatrix);

        if (tx[7] >= 1100) {
            tx[7] = 0;
            angularStep[7] = 0;
        }

        if ((map[0] == 2 && CheckCollision(x_projectile[7] + tx[7], y_projectile[7] + starCenterDistance, 1200 + translateX[0], 70, l + y, hexagoneRadius)) ||
            (map[1] == 2 && CheckCollision(x_projectile[7] + tx[7], y_projectile[7] + starCenterDistance, 1200 + translateX[1], 70, l + y, hexagoneRadius)) ||
            (map[2] == 2 && CheckCollision(x_projectile[7] + tx[7], y_projectile[7] + starCenterDistance, 1200 + translateX[2], 70, l + y, hexagoneRadius))) {
            tx[7] = 0;
            meshes.erase("projectile8");
            if (map[0] == 2) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 2) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 2) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 2, 20, enemyColor3, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }
    if (((isMoving[0] && map[0] == 2) ||
        (isMoving[1] && map[1] == 2) ||
        (isMoving[2] && map[2] == 2)) &&
        !isEmptyCell[8] && contains(lineColors, cellColor[8], 2)) {
        modelMatrix = glm::mat3(1);

        projectiles[8] = object2D::CreateStar("projectile9", glm::vec3(x_projectile[8], y_projectile[8], 2), 15, cellColor[8], true);
        AddMeshToList(projectiles[8]);

        angularStep[8] += 2 * deltaTimeSeconds;
        tx[8] += 400 * deltaTimeSeconds;
        ty = 0;

        modelMatrix *= transform2D::Translate(tx[8], ty);
        modelMatrix *= transform2D::Translate(x_projectile[8], y_projectile[8] + starCenterDistance);
        modelMatrix *= transform2D::Rotate(angularStep[8]);
        modelMatrix *= transform2D::Translate(-x_projectile[8], -y_projectile[8] - starCenterDistance);

        RenderMesh2D(meshes["projectile9"], shaders["VertexColor"], modelMatrix);

        if (tx[8] >= 1100) {
            tx[8] = 0;
            angularStep[8] = 0;
        }

        if ((map[0] == 2 && CheckCollision(x_projectile[8] + tx[8], y_projectile[8] + starCenterDistance, 1200 + translateX[0], 70, l + y, hexagoneRadius)) ||
            (map[1] == 2 && CheckCollision(x_projectile[8] + tx[8], y_projectile[8] + starCenterDistance, 1200 + translateX[1], 70, l + y, hexagoneRadius)) ||
            (map[2] == 2 && CheckCollision(x_projectile[8] + tx[8], y_projectile[8] + starCenterDistance, 1200 + translateX[2], 70, l + y, hexagoneRadius))) {
            tx[8] = 0;
            meshes.erase("projectile9");
            if (map[0] == 2) {
                distance[0] = 1200 + translateX[0];
                enemyLives[0]++;
            }
            if (map[1] == 2) {
                distance[1] = 1200 + translateX[1];
                enemyLives[1]++;
            }
            if (map[2] == 2) {
                distance[2] = 1200 + translateX[2];
                enemyLives[2]++;
            }
        }

        modelMatrix = glm::mat3(1);

        KillEnemy("enemyOut", "enemyIn", deltaTimeSeconds, 2, 20, enemyColor3, enemyLives, map,
            translateX, tx, scaleEnemyX, scaleEnemyY, isMoving, modelMatrix, meshes, shaders);
    }


    /*
        Checking for collision between the diamonds and the hexagons.
    */
    if ((CheckCollision(150, 355, 1200 + translateX[0], 360, diamondRadius, hexagoneRadius) && map[0] == 0)
        || (CheckCollision(150, 355, 1200 + translateX[1], 360, diamondRadius, hexagoneRadius) && map[1] == 0)
        || (CheckCollision(150, 355, 1200 + translateX[2], 360, diamondRadius, hexagoneRadius) && map[2] == 0)
        && !isEmptyCell[0]) {
        modelMatrix = glm::mat3(1);
        checkScale[0] = true;
        scaleX[0] -= 2 * deltaTimeSeconds;
        scaleY[0] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(150, 355);
        modelMatrix *= transform2D::Scale(scaleX[0], scaleY[0]);
        modelMatrix *= transform2D::Translate(-150, -355);
        RenderMesh2D(meshes["diamond" + std::to_string(0 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[0] <= 0 && scaleY[0] <= 0) {
            scaleX[0] = 1;
            scaleY[0] = 1;
            isEmptyCell[0] = true;
            checkScale[0] = false;
            meshes.erase("diamond" + std::to_string(0 + 1));
        }
    }
    if ((CheckCollision(280, 355, 1200 + translateX[0], 360, diamondRadius, hexagoneRadius) && map[0] == 0)
        || (CheckCollision(280, 355, 1200 + translateX[1], 360, diamondRadius, hexagoneRadius) && map[1] == 0)
        || (CheckCollision(280, 355, 1200 + translateX[2], 360, diamondRadius, hexagoneRadius) && map[2] == 0)
        && !isEmptyCell[1]) {
        modelMatrix = glm::mat3(1);
        checkScale[1] = true;
        scaleX[1] -= 2 * deltaTimeSeconds;
        scaleY[1] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(280, 355);
        modelMatrix *= transform2D::Scale(scaleX[1], scaleY[1]);
        modelMatrix *= transform2D::Translate(-280, -355);
        RenderMesh2D(meshes["diamond" + std::to_string(1 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[1] <= 0 && scaleY[1] <= 0) {
            scaleX[1] = 1;
            scaleY[1] = 1;
            isEmptyCell[1] = true;
            checkScale[1] = false;
            meshes.erase("diamond" + std::to_string(1 + 1));
        }
    }
    if ((CheckCollision(410, 355, 1200 + translateX[0], 360, diamondRadius, hexagoneRadius) && map[0] == 0)
        || (CheckCollision(410, 355, 1200 + translateX[1], 360, diamondRadius, hexagoneRadius) && map[1] == 0)
        || (CheckCollision(410, 355, 1200 + translateX[2], 360, diamondRadius, hexagoneRadius) && map[2] == 0)
        && !isEmptyCell[2]) {
        modelMatrix = glm::mat3(1);
        checkScale[2] = true;
        scaleX[2] -= 2 * deltaTimeSeconds;
        scaleY[2] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(410, 355);
        modelMatrix *= transform2D::Scale(scaleX[2], scaleY[2]);
        modelMatrix *= transform2D::Translate(-410, -355);
        RenderMesh2D(meshes["diamond" + std::to_string(2 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[2] <= 0 && scaleY[2] <= 0) {
            scaleX[2] = 1;
            scaleY[2] = 1;
            isEmptyCell[2] = true;
            checkScale[2] = false;
            meshes.erase("diamond" + std::to_string(2 + 1));
        }
    }
    if ((CheckCollision(150, 210, 1200 + translateX[0], 215, diamondRadius, hexagoneRadius) && map[0] == 1)
        || (CheckCollision(150, 210, 1200 + translateX[1], 215, diamondRadius, hexagoneRadius) && map[1] == 1)
        || (CheckCollision(150, 210, 1200 + translateX[2], 215, diamondRadius, hexagoneRadius) && map[2] == 1)
        && !isEmptyCell[3]) {
        modelMatrix = glm::mat3(1);
        checkScale[3] = true;
        scaleX[3] -= 2 * deltaTimeSeconds;
        scaleY[3] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(150, 210);
        modelMatrix *= transform2D::Scale(scaleX[3], scaleY[3]);
        modelMatrix *= transform2D::Translate(-150, -210);
        RenderMesh2D(meshes["diamond" + std::to_string(3 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[3] <= 0 && scaleY[3] <= 0) {
            scaleX[3] = 1;
            scaleY[3] = 1;
            isEmptyCell[3] = true;
            checkScale[3] = false;
            meshes.erase("diamond" + std::to_string(3 + 1));
        }
    }
    if ((CheckCollision(280, 210, 1200 + translateX[0], 215, diamondRadius, hexagoneRadius) && map[0] == 1)
        || (CheckCollision(280, 210, 1200 + translateX[1], 215, diamondRadius, hexagoneRadius) && map[1] == 1)
        || (CheckCollision(280, 210, 1200 + translateX[2], 215, diamondRadius, hexagoneRadius) && map[2] == 1)
        && !isEmptyCell[4]) {
        modelMatrix = glm::mat3(1);
        checkScale[4] = true;
        scaleX[4] -= 2 * deltaTimeSeconds;
        scaleY[4] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(280, 210);
        modelMatrix *= transform2D::Scale(scaleX[4], scaleY[4]);
        modelMatrix *= transform2D::Translate(-280, -210);
        RenderMesh2D(meshes["diamond" + std::to_string(4 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[4] <= 0 && scaleY[4] <= 0) {
            scaleX[4] = 1;
            scaleY[4] = 1;
            isEmptyCell[4] = true;
            checkScale[4] = false;
            meshes.erase("diamond" + std::to_string(4 + 1));
        }
    }
    if ((CheckCollision(410, 210, 1200 + translateX[0], 215, diamondRadius, hexagoneRadius) && map[0] == 1)
        || (CheckCollision(410, 210, 1200 + translateX[1], 215, diamondRadius, hexagoneRadius) && map[1] == 1)
        || (CheckCollision(410, 210, 1200 + translateX[2], 215, diamondRadius, hexagoneRadius) && map[2] == 1)
        && !isEmptyCell[5]) {
        modelMatrix = glm::mat3(1);
        checkScale[5] = true;
        scaleX[5] -= 2 * deltaTimeSeconds;
        scaleY[5] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(410, 210);
        modelMatrix *= transform2D::Scale(scaleX[5], scaleY[5]);
        modelMatrix *= transform2D::Translate(-410, -210);
        RenderMesh2D(meshes["diamond" + std::to_string(5 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[5] <= 0 && scaleY[5] <= 0) {
            scaleX[5] = 1;
            scaleY[5] = 1;
            isEmptyCell[5] = true;
            checkScale[5] = false;
            meshes.erase("diamond" + std::to_string(5 + 1));
        }
    }
    if ((CheckCollision(150, 65, 1200 + translateX[0], 70, diamondRadius, hexagoneRadius) && map[0] == 2)
        || (CheckCollision(150, 65, 1200 + translateX[1], 70, diamondRadius, hexagoneRadius) && map[1] == 2)
        || (CheckCollision(150, 65, 1200 + translateX[2], 70, diamondRadius, hexagoneRadius) && map[2] == 2)
        && !isEmptyCell[6]) {
        modelMatrix = glm::mat3(1);
        checkScale[6] = true;
        scaleX[6] -= 2 * deltaTimeSeconds;
        scaleY[6] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(150, 65);
        modelMatrix *= transform2D::Scale(scaleX[6], scaleY[6]);
        modelMatrix *= transform2D::Translate(-150, -65);
        RenderMesh2D(meshes["diamond" + std::to_string(6 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[6] <= 0 && scaleY[6] <= 0) {
            scaleX[6] = 1;
            scaleY[6] = 1;
            isEmptyCell[6] = true;
            checkScale[6] = false;
            meshes.erase("diamond" + std::to_string(6 + 1));
        }
    }
    if ((CheckCollision(280, 65, 1200 + translateX[0], 70, diamondRadius, hexagoneRadius) && map[0] == 2)
        || (CheckCollision(280, 65, 1200 + translateX[1], 70, diamondRadius, hexagoneRadius) && map[1] == 2)
        || (CheckCollision(280, 65, 1200 + translateX[2], 70, diamondRadius, hexagoneRadius) && map[2] == 2)
        && !isEmptyCell[7]) {
        modelMatrix = glm::mat3(1);
        checkScale[7] = true;
        scaleX[7] -= 2 * deltaTimeSeconds;
        scaleY[7] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(280, 65);
        modelMatrix *= transform2D::Scale(scaleX[7], scaleY[7]);
        modelMatrix *= transform2D::Translate(-280, -65);
        RenderMesh2D(meshes["diamond" + std::to_string(7 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[7] <= 0 && scaleY[7] <= 0) {
            scaleX[7] = 1;
            scaleY[7] = 1;
            isEmptyCell[7] = true;
            checkScale[7] = false;
            meshes.erase("diamond" + std::to_string(7 + 1));
        }
    }
    if ((CheckCollision(410, 65, 1200 + translateX[0], 70, diamondRadius, hexagoneRadius) && map[0] == 2)
        || (CheckCollision(410, 65, 1200 + translateX[1], 70, diamondRadius, hexagoneRadius) && map[1] == 2)
        || (CheckCollision(410, 65, 1200 + translateX[2], 70, diamondRadius, hexagoneRadius) && map[2] == 2)
        && !isEmptyCell[8]) {
        modelMatrix = glm::mat3(1);
        checkScale[8] = true;
        scaleX[8] -= 2 * deltaTimeSeconds;
        scaleY[8] -= 2 * deltaTimeSeconds;

        modelMatrix *= transform2D::Translate(410, 65);
        modelMatrix *= transform2D::Scale(scaleX[8], scaleY[8]);
        modelMatrix *= transform2D::Translate(-410, -65);
        RenderMesh2D(meshes["diamond" + std::to_string(8 + 1)], shaders["VertexColor"], modelMatrix);

        if (scaleX[8] <= 0 && scaleY[8] <= 0) {
            scaleX[8] = 1;
            scaleY[8] = 1;
            isEmptyCell[8] = true;
            checkScale[8] = false;
            meshes.erase("diamond" + std::to_string(8 + 1));
        }
    }
    

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

    // This is for displaying the touretes.
    for (int i = 0; i < 9; i++) {
        modelMatrix = glm::mat3(1);
        DisplayDiamond("diamond" + std::to_string(i + 1), checkButtonPressRight[i], checkScale[i], scaleX[i],
            scaleY[i], deltaTimeSeconds, centers[0][i], centers[1][i], modelMatrix, meshes, shaders);
    }

    // This is for drag and drop.
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
                            cellColor[i] = draggedColor;
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, cellColor[i], true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
                else if (y1 == 160) {
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            cellColor[i] = draggedColor;
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, cellColor[i], true);
                            AddMeshToList(diamond);
                            isEmptyCell[i] = false;
                            break;
                        }
                    }
                }
                else if (y1 == 15) {
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            cellColor[i] = draggedColor;
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, cellColor[i], true);
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
