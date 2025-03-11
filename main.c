#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILE_SIZE 20
#define MAP_WIDTH (SCREEN_WIDTH / TILE_SIZE)
#define MAP_HEIGHT (SCREEN_HEIGHT / TILE_SIZE)

typedef enum { WALL, FLOOR, START, END } Tile;
Tile dungeon[MAP_WIDTH][MAP_HEIGHT];

int playerX, playerY;

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
        if (i == 0) { // Set player start position to the first room
            startX = rx + 1;
            startY = ry + 1;
        }
    }

    dungeon[startX][startY] = START;
    dungeon[MAP_WIDTH - 3][MAP_HEIGHT - 3] = END;
    playerX = startX;
    playerY = startY;
}

void DrawDungeon() {
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            Color color = DARKGRAY;
            if (dungeon[x][y] == WALL) color = DARKGRAY;
            else if (dungeon[x][y] == FLOOR) color = WHITE;
            else if (dungeon[x][y] == START) color = GREEN;
            else if (dungeon[x][y] == END) color = RED;
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }
    DrawRectangle(playerX * TILE_SIZE, playerY * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
}

void UpdatePlayer() {
    if (IsKeyPressed(KEY_W) && dungeon[playerX][playerY - 1] != WALL) playerY--;
    if (IsKeyPressed(KEY_S) && dungeon[playerX][playerY + 1] != WALL) playerY++;
    if (IsKeyPressed(KEY_A) && dungeon[playerX - 1][playerY] != WALL) playerX--;
    if (IsKeyPressed(KEY_D) && dungeon[playerX + 1][playerY] != WALL) playerX++;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dungeon Generator");
    SetTargetFPS(60);
    srand(time(NULL));
    GenerateDungeon();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            GenerateDungeon();
        }

        UpdatePlayer();

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawDungeon();
        DrawText("Press SPACE to regenerate", 10, 10, 20, WHITE);
        DrawText("Use WASD to move", 10, 30, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
