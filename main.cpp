#include <cstdlib>
#include <ctime>
#include <raylib.h>
#include <string>

using namespace std;

const int CELL_SIZE = 20;
const int GRID_SIZE = 30; // Number of cells in each row/column
const int PADDING = 20;   // Padding around the grid
const int SCREEN_SIZE = CELL_SIZE * GRID_SIZE + 2 * PADDING; // Window size
const int INITIAL_SNAKE_LENGTH = 4;
const float MOVE_INTERVAL = 0.2f; // Move every 0.2 seconds

struct SnakeNode {
  int x;
  int y;
  SnakeNode *next = nullptr;
};

SnakeNode *head = nullptr;
int directionX = 1; // Initial direction will be set randomly
int directionY = 0;
float lastMoveTime = 0;
int score = 0;

struct Apple {
  int x;
  int y;
} apple;

void SpawnApple() {
  apple.x = rand() % GRID_SIZE;
  apple.y = rand() % GRID_SIZE;
}

void DrawGrid() {
  const Color GRID_COLOR = {50, 50, 50, 255}; // Dark gray color for the grid

  for (int x = 0; x <= GRID_SIZE; x++) {
    DrawLine(PADDING + x * CELL_SIZE, PADDING, PADDING + x * CELL_SIZE,
             PADDING + GRID_SIZE * CELL_SIZE, GRID_COLOR);
  }

  for (int y = 0; y <= GRID_SIZE; y++) {
    DrawLine(PADDING, PADDING + y * CELL_SIZE, PADDING + GRID_SIZE * CELL_SIZE,
             PADDING + y * CELL_SIZE, GRID_COLOR);
  }
}

void DrawCells() {
  BeginDrawing();
  ClearBackground(BLACK);

  DrawGrid();

  SnakeNode *cur = head;
  while (cur != nullptr) {
    DrawRectangle(PADDING + cur->x * CELL_SIZE, PADDING + cur->y * CELL_SIZE,
                  CELL_SIZE, CELL_SIZE, WHITE);
    cur = cur->next;
  }

  DrawRectangle(PADDING + apple.x * CELL_SIZE, PADDING + apple.y * CELL_SIZE,
                CELL_SIZE, CELL_SIZE, RED);

  DrawText(("Score: " + to_string(score)).c_str(), PADDING, PADDING / 2, 20,
           WHITE);

  EndDrawing();
}

void SetRandomDirection() {
  int randomDir = rand() % 4;
  switch (randomDir) {
  case 0: // Right
    directionX = 1;
    directionY = 0;
    break;
  case 1: // Left
    directionX = -1;
    directionY = 0;
    break;
  case 2: // Down
    directionX = 0;
    directionY = 1;
    break;
  case 3: // Up
    directionX = 0;
    directionY = -1;
    break;
  }
}

void ResetSnake() {
  while (head != nullptr) {
    SnakeNode *temp = head;
    head = head->next;
    delete temp;
  }

  int startX = GRID_SIZE / 2;
  int startY = GRID_SIZE / 2;

  for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
    SnakeNode *newNode = new SnakeNode();
    newNode->x = startX;
    newNode->y = startY;
    newNode->next = head;
    head = newNode;
  }

  SetRandomDirection();
  score = 0;
  SpawnApple();
  lastMoveTime = GetTime();
}

bool CheckCollision() {
  // Check if snake hit the edge
  if (head->x < 0 || head->x >= GRID_SIZE || head->y < 0 ||
      head->y >= GRID_SIZE) {
    return true;
  }

  // Check if snake hit itself
  SnakeNode *current = head->next;
  while (current != nullptr) {
    if (head->x == current->x && head->y == current->y) {
      return true;
    }
    current = current->next;
  }

  return false;
}

void MoveSnake() {
  // Create new head
  SnakeNode *newHead = new SnakeNode();
  newHead->x = head->x + directionX;
  newHead->y = head->y + directionY;
  newHead->next = head;
  head = newHead;

  if (head->x == apple.x && head->y == apple.y) {
    score++;
    SpawnApple();
  } else {
    // Remove tail
    SnakeNode *current = head;
    while (current->next->next != nullptr) {
      current = current->next;
    }
    delete current->next;
    current->next = nullptr;
  }

  if (CheckCollision()) {
    ResetSnake();
  }
}

void HandleInput() {
  if (IsKeyPressed(KEY_RIGHT) && directionX == 0) {
    directionX = 1;
    directionY = 0;
  }
  if (IsKeyPressed(KEY_LEFT) && directionX == 0) {
    directionX = -1;
    directionY = 0;
  }
  if (IsKeyPressed(KEY_DOWN) && directionY == 0) {
    directionX = 0;
    directionY = 1;
  }
  if (IsKeyPressed(KEY_UP) && directionY == 0) {
    directionX = 0;
    directionY = -1;
  }
}

void PlaySnake() {
  while (!WindowShouldClose()) {
    HandleInput();

    float currentTime = GetTime();
    if (currentTime - lastMoveTime >= MOVE_INTERVAL) {
      MoveSnake();
      lastMoveTime = currentTime;
    }

    DrawCells();
  }
}

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));
  InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Snek");
  SetTargetFPS(60);
  ResetSnake();
  PlaySnake();
  CloseWindow();

  while (head != nullptr) {
    SnakeNode *temp = head;
    head = head->next;
    delete temp;
  }

  return 0;
}
