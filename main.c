#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define TILE_SIZE 20
#define MAP_WIDTH (SCREEN_WIDTH / TILE_SIZE)
#define MAP_HEIGHT (SCREEN_HEIGHT / TILE_SIZE)

typedef enum { WALL, FLOOR, START, END } Tile;
typedef enum { MENU, GAME } GameState;

typedef enum { START_GAME, QUIT_GAME } MenuOption;

Tile dungeon[MAP_WIDTH][MAP_HEIGHT];
GameState gameState = MENU;
MenuOption selectedOption = START_GAME;

int playerX, playerY;
int endX, endY;
int moveCounter = 0;

void GenerateRoom(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            if (i > 0 && i < MAP_WIDTH - 1 && j > 0 && j < MAP_HEIGHT - 1) {
                dungeon[i][j] = FLOOR;
            }
        }
    }
}

void GenerateCorridor(int x1, int y1, int x2, int y2) {
    while (x1 != x2) {
        dungeon[x1][y1] = FLOOR;
        x1 += (x2 > x1) ? 1 : -1;
    }
    while (y1 != y2) {
        dungeon[x1][y1] = FLOOR;
        y1 += (y2 > y1) ? 1 : -1;
    }
}

void GenerateDungeon() {
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            dungeon[x][y] = WALL;
        }
    }

    int roomCount = 5 + rand() % 5;
    int prevX = 0, prevY = 0;
    int startX = 2, startY = 2;

    for (int i = 0; i < roomCount; i++) {
        int rw = 3 + rand() % 6;
        int rh = 3 + rand() % 6;
        int rx = rand() % (MAP_WIDTH - rw - 1) + 1;
        int ry = rand() % (MAP_HEIGHT - rh - 1) + 1;
        GenerateRoom(rx, ry, rw, rh);
        if (i > 0) {
            GenerateCorridor(prevX, prevY, rx, ry);
        }
        prevX = rx;
        prevY = ry;
        if (i == 0) {
            startX = rx + 1;
            startY = ry + 1;
        }
    }

    dungeon[startX][startY] = START;
    playerX = startX;
    playerY = startY;

    endX = prevX + 1;
    endY = prevY + 1;
    dungeon[endX][endY] = END;
    GenerateCorridor(startX, startY, endX, endY);
}

void DrawDungeon() {
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            Color color = DARKGRAY;
            if (dungeon[x][y] == WALL) color = DARKGRAY;
            else if (dungeon[x][y] == FLOOR) color = WHITE;
            else if (dungeon[x][y] == START) color = GREEN;
            else if (dungeon[x][y] == END) color = GREEN;
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }
    DrawRectangle(playerX * TILE_SIZE, playerY * TILE_SIZE, TILE_SIZE, TILE_SIZE, ORANGE);
}

void UpdatePlayer() {
    moveCounter++;
    if (moveCounter % 2 == 0) {
        if (IsKeyDown(KEY_W) && dungeon[playerX][playerY - 1] != WALL) playerY--;
        if (IsKeyDown(KEY_S) && dungeon[playerX][playerY + 1] != WALL) playerY++;
        if (IsKeyDown(KEY_A) && dungeon[playerX - 1][playerY] != WALL) playerX--;
        if (IsKeyDown(KEY_D) && dungeon[playerX + 1][playerY] != WALL) playerX++;
    }

    if (dungeon[playerX][playerY] == END || IsKeyPressed(KEY_SPACE)) {
        GenerateDungeon();
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dungeon Generator");
    SetTargetFPS(60);
    srand(time(NULL));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (gameState == MENU) {
            int titleWidth = MeasureText("Endless Dungeon", 40);
            int startWidth = MeasureText("Start", 20);
            int quitWidth = MeasureText("Quit Game", 20);
            int creditWidth = MeasureText("Credit: Mathias Novakovic", 15);
            DrawText("Endless Dungeon", (SCREEN_WIDTH - titleWidth) / 2, SCREEN_HEIGHT / 3, 40, WHITE);
            DrawText("Credit: Mathias Novakovic", (SCREEN_WIDTH - creditWidth) / 2, SCREEN_HEIGHT - 50, 15, WHITE);
            DrawText("Start", (SCREEN_WIDTH - startWidth) / 2, SCREEN_HEIGHT / 2, 20, (selectedOption == START_GAME) ? YELLOW : WHITE);
            DrawText("Quit Game", (SCREEN_WIDTH - quitWidth) / 2, SCREEN_HEIGHT / 2 + 40, 20, (selectedOption == QUIT_GAME) ? YELLOW : WHITE);

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
                selectedOption = (selectedOption == START_GAME) ? QUIT_GAME : START_GAME;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (selectedOption == START_GAME) {
                    gameState = GAME;
                    GenerateDungeon();
                } else {
                    break;
                }
            }
        } else if (gameState == GAME) {
            UpdatePlayer();
            DrawDungeon();
            int textWidth1 = MeasureText("Reach the goal to reach the next floor", 20);
            int textWidth2 = MeasureText("Press SPACE to generate new map", 20);
            int textWidth3 = MeasureText("Use WASD to move", 20);
            DrawText("Reach the goal to reach the next floor", (SCREEN_WIDTH - textWidth1) / 2, 10, 20, WHITE);
            DrawText("Press SPACE to generate new map", (SCREEN_WIDTH - textWidth2) / 2, 35, 20, WHITE);
            DrawText("Use WASD to move", (SCREEN_WIDTH - textWidth3) / 2, 60, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
