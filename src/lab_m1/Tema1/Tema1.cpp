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

    glm::mat3 initialMatrix = matrix;

    matrix *= transform2D::Translate(cx, cy);
    matrix *= transform2D::Scale(sx, sy);
    matrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes[name], shaders["VertexColor"], matrix);

    matrix = initialMatrix;
}

bool m1::Tema1::equals(const enemy& e1, const enemy& e2)
{
    return (e1.innerName == e2.innerName) && (e1.outerName == e2.outerName);
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

    isPressed = false;

    line = -1;
    color = -1;
    randomStarX = 30 * (1 + sin(M_PI / 10));
    randomStarYUp = 30 * (sqrt(1 - 4 * sin(M_PI / 10) * sin(M_PI / 10)) + sqrt(3 * sin(M_PI / 10) * sin(M_PI / 10) + 2 * sin(M_PI / 10)));
    randomStarYDown = 30 * cos(3 * M_PI / 10);
    x_inventory = 870;
    y_inventory = 580;

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

    enemyCenter[0] = 350;
    enemyCenter[1] = 205;
    enemyCenter[2] = 60;

    counterEnemies = 0;
    counterStars = 0;
    randomTime = rand() % 5 + 5;
    randomTimeStars = rand() % 10;
    hexagoneRadius = 40;
    diamondRadius = 40;
    index = 0;

    pentagoneLength = 2 * 20 * sin(M_PI / 10);
    sine = sin(M_PI / 10);
    l = 20 * sqrt(3 * sine * sine + 2 * sine) / 2;
    y = sqrt(400 - pentagoneLength * pentagoneLength);

    float x_square = 100;
    float y_square = 305;

    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0 && i > 0) {
            x_square = 100;
            y_square -= 145;
        }

        cells[i].cellColor = glm::vec3(0, 1, 0);
        cells[i].isEmpty = true;
        cells[i].time = 0;
        cells[i].squareName = "square" + std::to_string(i + 1);
        cells[i].square = object2D::CreateSquare(cells[i].squareName, glm::vec3(x_square, y_square, 0), 100, cells[i].cellColor, true);
        cells[i].scaleX = 1;
        cells[i].scaleY = 1;
        cells[i].checkScale = true;
        cells[i].diamondName = "diamond" + std::to_string(i + 1);
        AddMeshToList(cells[i].square);
        x_square += 130;
    }

    float yIn = 325;
    float yOut = 315;

    for (int i = 0; i < 3; i++) {
        enemiesSpawnIn[i] = glm::vec3(1200, yIn, 2);
        enemiesSpawnOut[i] = glm::vec3(1200, yOut, 1);
        spawnEnemy[i] = yOut;
        yIn -= 145;
        yOut -= 145;
    }

    Mesh* rectangle = object2D::CreateRectangle("rectangle", glm::vec3(10, 15, 0), 390, 60, glm::vec3(1, 0, 0), true);
    AddMeshToList(rectangle);

    x_square = 50;
    y_square = 580;
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

    starCenterDistance = 20 * sqrt(3 * sin(M_PI / 10) * sin(M_PI / 10) + 2 * sin(M_PI / 10)) / 2;

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

    j = 0;
    k = 1;
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

    for (int i = 0; i < 9; i++) {
        RenderMesh2D(meshes[cells[i].squareName], shaders["VertexColor"], modelMatrix);
    }

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

    // Generate new enemy of random color, on random line, at random time.
    if (counterEnemies > randomTime) {
        line = rand() % 3;
        color = rand() % 4;
        enemy e;
        e.line = line;
        e.innerColor = glm::vec3(0, 1, 0.75);
        e.outerColor = colors[color];
        e.index = j + 1;
        e.innerName = "enemyIn" + std::to_string(e.index);
        e.outerName = "enemyOut" + std::to_string(e.index);
        e.inner = object2D::CreateEnemy1(e.innerName, enemiesSpawnIn[line], 30, e.innerColor, true);
        e.outer = object2D::CreateEnemy1(e.outerName, enemiesSpawnOut[line], 40, e.outerColor, true);
        AddMeshToList(e.inner);
        AddMeshToList(e.outer);
        e.translateX = 100;
        e.scaleX = 1;
        e.scaleY = 1;
        e.isScaling = false;
        e.isMoving = true;
        e.lives = 3;
        e.centerY = enemyCenter[line];
        enemies.push_back(e);
        j++;

        if (line == 0) {
            firstLine.push_back(e);
        }
        else if (line == 1) {
            secondLine.push_back(e);
        }
        else if (line == 2) {
            thirdLine.push_back(e);
        }

        counterEnemies = 0;
        randomTime = rand() % 5 + 3;
    }

    // Render all the enemies int the enemies vector.
    for (auto& e : enemies) {
        if (!e.isScaling) {
            modelMatrix = glm::mat3(1);
            e.translateX -= 100 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(e.translateX, 0);

            RenderMesh2D(meshes[e.innerName], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes[e.outerName], shaders["VertexColor"], modelMatrix);
        }
    }

    // Decrement the number of lives if enemy reaches end line.
    for (auto e = enemies.begin(); e != enemies.end(); ++e) {
        if (e->translateX <= -1150) {
            meshes.erase("life" + std::to_string(lives_no--));
        }
    }

    // Remove an enemy if it gets to the border.
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](const auto& e) {
            return e.translateX <= -1150;
            }),
        enemies.end()
    );

    // Increase the time needed to launch a projectile each time a new diamond is placed.
    for (int i = 0; i < 9; i++) {
        if (!cells[i].isEmpty) {
            cells[i].time += deltaTimeSeconds;
        }
    }

    // Launch a star only when the cell is not empty, the time passed, the line is the same
    // for the enemy and for the diamond and they have the same color.
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < enemies.size(); j++) {
            if (!cells[i].isEmpty && cells[i].line == enemies[j].line && cells[i].time > 1 && cells[i].diamondColor == enemies[j].outerColor) {
                modelMatrix = glm::mat3(1);
                projectile p;
                p.name = "projectile" + std::to_string(k);
                p.color = cells[i].diamondColor;
                p.star = object2D::CreateStar(p.name, glm::vec3(x_projectile[i], y_projectile[i], 2), 20, p.color, true);
                AddMeshToList(p.star);
                p.translateX = 0;
                p.angularStep = 0;
                cells[i].projectiles.push_back(p);
                cells[i].time = 0;
                k++;
            }
        }
    }

    // Render the projectiles and remove them if they reach the end of the screen.
    for (int i = 0; i < 9; i++) {
        for (auto& p : cells[i].projectiles) {
            modelMatrix = glm::mat3(1);

            p.angularStep += 2 * deltaTimeSeconds;
            p.translateX += 400 * deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(p.translateX, 0);
            modelMatrix *= transform2D::Translate(x_projectile[i], y_projectile[i] + starCenterDistance);
            modelMatrix *= transform2D::Rotate(p.angularStep);
            modelMatrix *= transform2D::Translate(- x_projectile[i], - y_projectile[i] - starCenterDistance);

            RenderMesh2D(meshes[p.name], shaders["VertexColor"], modelMatrix);
        }

        cells[i].projectiles.erase(
            std::remove_if(cells[i].projectiles.begin(), cells[i].projectiles.end(), [](const auto& p) {
                return p.translateX >= 1100;
                }),
            cells[i].projectiles.end());
    }

    // Here I decrement the life of each enemy for whom collision with a projectile exists
    // and also the color is the same.
    for (int i = 0; i < 9; i++) {
        for (auto& en : enemies) {
            auto it1 = std::find_if(firstLine.begin(), firstLine.end(), [&](const enemy& e) {
                return equals(e, en);
                });
            auto it2 = std::find_if(secondLine.begin(), secondLine.end(), [&](const enemy& e) {
                return equals(e, en);
                });
            auto it3 = std::find_if(thirdLine.begin(), thirdLine.end(), [&](const enemy& e) {
                return equals(e, en);
                });

            if (it1 != firstLine.end()) {
                for (projectile p : cells[i].projectiles) {
                    if (CheckCollision(x_projectile[i] + p.translateX, y_projectile[i] + starCenterDistance,
                        1200 + en.translateX, spawnEnemy[0] + 40, l + y, hexagoneRadius) && p.color == en.outerColor) {
                        en.lives--;
                    }
                }

                if (en.lives == 0) {
                    break;
                }
            }

            if (it2 != secondLine.end()) {
                for (projectile p : cells[i].projectiles) {
                    if (CheckCollision(x_projectile[i] + p.translateX, y_projectile[i] + starCenterDistance,
                        1200 + en.translateX, spawnEnemy[1] + 40, l + y, hexagoneRadius) && p.color == en.outerColor) {
                        en.lives--;
                    }
                }

                if (en.lives == 0) {
                    break;
                }
            }

            if (it3 != thirdLine.end()) {
                for (projectile p : cells[i].projectiles) {
                    if (CheckCollision(x_projectile[i] + p.translateX, y_projectile[i] + starCenterDistance,
                        1200 + en.translateX, spawnEnemy[2] + 40, l + y, hexagoneRadius) && p.color == en.outerColor) {
                        en.lives--;
                    }
                }

                if (en.lives == 0) {
                    break;
                }
            }
        }
    }


    // Checking for collision by traversing all the cells and enemies and for each enemy
    // checking if it is in the "firstLine", "secondLine" or "thirdLine" vector.
    for (int i = 0; i < 9; i++) {
        for (auto& en : enemies) {
            auto it1 = std::find_if(firstLine.begin(), firstLine.end(), [&](const enemy& e) {
                return equals(e, en);
                });
            auto it2 = std::find_if(secondLine.begin(), secondLine.end(), [&](const enemy& e) {
                return equals(e, en);
                });
            auto it3 = std::find_if(thirdLine.begin(), thirdLine.end(), [&](const enemy& e) {
                return equals(e, en);
                });

            if (it1 != firstLine.end()) {
                cells[i].projectiles.erase(
                    std::remove_if(cells[i].projectiles.begin(), cells[i].projectiles.end(), [&](auto& p) {
                        return CheckCollision(
                            x_projectile[i] + p.translateX,
                            y_projectile[i] + starCenterDistance,
                            1200 + en.translateX,
                            spawnEnemy[0] + 40,
                            l + y,
                            hexagoneRadius
                        ) &&
                            p.color == en.outerColor;
                        }),
                    cells[i].projectiles.end());
            }

            if (it2 != secondLine.end()) {
                cells[i].projectiles.erase(
                    std::remove_if(cells[i].projectiles.begin(), cells[i].projectiles.end(), [&](auto& p) {
                        return CheckCollision(
                            x_projectile[i] + p.translateX,
                            y_projectile[i] + starCenterDistance,
                            1200 + en.translateX,
                            spawnEnemy[1] + 40,
                            l + y,
                            hexagoneRadius
                        ) &&
                            p.color == en.outerColor;
                        }),
                    cells[i].projectiles.end());
            }

            if (it3 != thirdLine.end()) {
                cells[i].projectiles.erase(
                    std::remove_if(cells[i].projectiles.begin(), cells[i].projectiles.end(), [&](auto& p) {
                        return CheckCollision(
                            x_projectile[i] + p.translateX,
                            y_projectile[i] + starCenterDistance,
                            1200 + en.translateX,
                            spawnEnemy[2] + 40,
                            l + y,
                            hexagoneRadius
                        ) &&
                            p.color == en.outerColor;
                        }),
                    cells[i].projectiles.end());
            }
        }
    }

    // Here I start scaling each enemies with 0 lives.
    for (auto& e : enemies) {
        if (e.lives == 0) {
            modelMatrix = glm::mat3(1);
            e.isScaling = true;

            e.inner = object2D::CreateEnemy1(e.innerName, glm::vec3(1200 + e.translateX, enemiesSpawnIn[e.line].y, 2), 30, e.innerColor, true);
            e.outer = object2D::CreateEnemy1(e.outerName, glm::vec3(1200 + e.translateX, enemiesSpawnOut[e.line].y, 1), 40, e.outerColor, true);
            AddMeshToList(e.inner);
            AddMeshToList(e.outer);

            e.scaleX -= deltaTimeSeconds;
            e.scaleY -= deltaTimeSeconds;

            modelMatrix *= transform2D::Translate(1200 + e.translateX, enemiesSpawnOut[e.line].y + 40);
            modelMatrix *= transform2D::Scale(e.scaleX, e.scaleY);
            modelMatrix *= transform2D::Translate(- 1200 - e.translateX, - enemiesSpawnOut[e.line].y - 40);
            RenderMesh2D(meshes[e.innerName], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes[e.outerName], shaders["VertexColor"], modelMatrix);
        }
    }

    // Here I remove each enemy with 0 lives and the scale factors smaller than a threshold.
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](const auto& e) {
            return e.lives == 0 && e.scaleX <= 0.1f && e.scaleY <= 0.1f;
            }),
        enemies.end());

    // Collision between enemy and diamond. For each diamond I check if its cell is not empty, because
    // this means that the diamond is already placed in the game and so I can now check each enemy and
    // see it its line is the same as the diamonds' and also if it is any collision between them.
    for (int i = 0; i < 9; i++) {
        if (!cells[i].isEmpty) {
            for (int j = 0; j < enemies.size(); j++) {
                if (enemies[j].line == cells[i].line &&
                    CheckCollision(centers[0][i], centers[1][i], 1200 + enemies[j].translateX, enemies[j].centerY, diamondRadius, hexagoneRadius)) {
                    modelMatrix = glm::mat3(1);
                    cells[i].checkScale = true;
                    cells[i].scaleX -= 2 * deltaTimeSeconds;
                    cells[i].scaleY -= 2 * deltaTimeSeconds;

                    modelMatrix *= transform2D::Translate(centers[0][i], centers[1][i]);
                    modelMatrix *= transform2D::Scale(cells[i].scaleX, cells[i].scaleY);
                    modelMatrix *= transform2D::Translate(-centers[0][i], -centers[1][i]);
                    RenderMesh2D(meshes[cells[i].diamondName], shaders["VertexColor"], modelMatrix);

                    if (cells[i].scaleX <= 0 && cells[i].scaleY <= 0) {
                        cells[i].scaleX = 1;
                        cells[i].scaleY = 1;
                        cells[i].isEmpty = true;
                        cells[i].checkScale = false;
                        meshes.erase(cells[i].diamondName);
                    }
                }
            }
        }
    }
    

    if (lives_no == 0) {
        exit(0);
    }

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
        DisplayDiamond(cells[i].diamondName, checkButtonPressRight[i], cells[i].checkScale, cells[i].scaleX,
            cells[i].scaleY, deltaTimeSeconds, centers[0][i], centers[1][i], modelMatrix, meshes, shaders);
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

            if (mouseX >= x1 && mouseX <= x2 && mouseY >= 720 - y2 && mouseY <= 720 - y1 && cells[i].isEmpty) {
                if (cells[i].scaleX <= 0 && cells[i].scaleY <= 0) {
                    meshes.erase(cells[i].diamondName);
                    cells[i].scaleX = 1;
                    cells[i].scaleY = 1;
                }

                if (y1 == 305) {
                    cells[i].line = 0;
                    for (int j = 0; j < 4; j++) {
                        if (inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            cells[i].diamondColor = colors[j];
                            cells[i].diamond = object2D::CreateDiamond(cells[i].diamondName, glm::vec3((x1 + x2) / 2, y1 + 5, 1), 90, 40, 20, 0.8f, colors[j], true);
                            AddMeshToList(cells[i].diamond);
                            cells[i].isEmpty = false;
                            break;
                        }
                    }
                }
                else if (y1 == 160) {
                    cells[i].line = 1;
                    for (int j = 0; j < 4; j++) {
                        if (inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            cells[i].diamondColor = colors[j];
                            cells[i].diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1), 90, 40, 20, 0.8f, colors[j], true);
                            AddMeshToList(cells[i].diamond);
                            cells[i].isEmpty = false;
                            break;
                        }
                    }
                }
                else if (y1 == 15) {
                    cells[i].line = 2;
                    for (int j = 0; j < 4; j++) {
                        if (inventory.size() >= boxes[j].stars_no) {
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            cells[i].diamondColor = colors[j];
                            cells[i].diamond = object2D::CreateDiamond("diamond" + std::to_string(i + 1), glm::vec3((x1 + x2) / 2, y1 + 5, 1), 90, 40, 20, 0.8f, colors[j], true);
                            AddMeshToList(cells[i].diamond);
                            cells[i].isEmpty = false;
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
            randomTimeStars = rand() % 5 + 2;
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

            if (mouseX >= x1 && mouseX <= x2 && mouseY >= 720 - y2 && mouseY <= 720 - y1 && !cells[i].isEmpty) {
                checkButtonPressRight[i] = true;
                cells[i].checkScale = true;
                cells[i].isEmpty = true;
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
            index = i;
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
        isDragged[index] = false;

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

            if (mouseX >= x1 && mouseX <= x2 && mouseY >= 720 - y2 && mouseY <= 720 - y1 && cells[i].isEmpty) {
                if (cells[i].scaleX <= 0 && cells[i].scaleY <= 0) {
                    meshes.erase(cells[i].diamondName);
                    cells[i].scaleX = 1;
                    cells[i].scaleY = 1;
                }

                if (y1 == 305) {
                    cells[i].line = 0;
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            cells[i].diamondColor = colors[index];
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            cells[i].diamond = object2D::CreateDiamond(cells[i].diamondName, glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, cells[i].diamondColor, true);
                            AddMeshToList(cells[i].diamond);
                            cells[i].isEmpty = false;
                            break;
                        }
                    }
                }
                else if (y1 == 160) {
                    cells[i].line = 1;
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            cells[i].diamondColor = colors[index];
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            cells[i].diamond = object2D::CreateDiamond(cells[i].diamondName, glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, cells[i].diamondColor, true);
                            AddMeshToList(cells[i].diamond);
                            cells[i].isEmpty = false;
                            break;
                        }
                    }
                }
                else if (y1 == 15) {
                    cells[i].line = 2;
                    for (int j = 0; j < 4; j++) {
                        if (draggedColor == colors[j] && inventory.size() >= boxes[j].stars_no) {
                            cells[i].diamondColor = colors[index];
                            int cnt = boxes[j].stars_no;
                            while (cnt > 0) {
                                inventory.pop();
                                meshes.erase("resource" + std::to_string(inventory.size() + 1));
                                cnt--;
                                x_inventory -= 40;
                            }
                            cells[i].diamond = object2D::CreateDiamond(cells[i].diamondName, glm::vec3((x1 + x2) / 2, y1 + 5, 1),
                                90, 40, 20, 0.8f, cells[i].diamondColor, true);
                            AddMeshToList(cells[i].diamond);
                            cells[i].isEmpty = false;
                            break;
                        }
                    }
                }
            }

             x1 += 130;
             x2 += 130;
        }

        meshes.erase("dragged" + std::to_string(index + 1));
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
