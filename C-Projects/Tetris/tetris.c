#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 800
#define BLOCK_SIZE 40
#define ROWS (WINDOW_HEIGHT / BLOCK_SIZE)
#define COLS (WINDOW_WIDTH / BLOCK_SIZE)

// Tetromino shapes
const int shapes[7][4][4][4] = {
    // I
    {
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        }
    },
    // O
    {
        {
            {0,0,0,0},
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0}
        }
    },
    // T
    {
        {
            {0,0,0,0},
            {0,3,3,3},
            {0,0,3,0},
            {0,0,0,0}
        },
        {
            {0,0,3,0},
            {0,3,3,0},
            {0,0,3,0},
            {0,0,0,0}
        },
        {
            {0,0,3,0},
            {0,3,3,3},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,3,0},
            {0,0,3,3},
            {0,0,3,0},
            {0,0,0,0}
        }
    },
    // S
    {
        {
            {0,0,0,0},
            {0,0,4,4},
            {0,4,4,0},
            {0,0,0,0}
        },
        {
            {0,0,4,0},
            {0,0,4,4},
            {0,0,0,4},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,0,4,4},
            {0,4,4,0},
            {0,0,0,0}
        },
        {
            {0,4,0,0},
            {0,4,4,0},
            {0,0,4,0},
            {0,0,0,0}
        }
    },
    // Z
    {
        {
            {0,0,0,0},
            {0,5,5,0},
            {0,0,5,5},
            {0,0,0,0}
        },
        {
            {0,0,0,5},
            {0,0,5,5},
            {0,0,5,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,5,5,0},
            {0,0,5,5},
            {0,0,0,0}
        },
        {
            {0,0,0,5},
            {0,0,5,5},
            {0,0,5,0},
            {0,0,0,0}
        }
    },
    // J
    {
        {
            {0,0,0,0},
            {0,6,6,6},
            {0,0,0,6},
            {0,0,0,0}
        },
        {
            {0,0,6,0},
            {0,0,6,0},
            {0,6,6,0},
            {0,0,0,0}
        },
        {
            {0,6,0,0},
            {0,6,6,6},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,6,6},
            {0,0,6,0},
            {0,0,6,0},
            {0,0,0,0}
        }
    },
    // L
    {
        {
            {0,0,0,0},
            {0,7,7,7},
            {0,7,0,0},
            {0,0,0,0}
        },
        {
            {0,7,7,0},
            {0,0,7,0},
            {0,0,7,0},
            {0,0,0,0}
        },
        {
            {0,0,0,7},
            {0,7,7,7},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,7,0},
            {0,0,7,0},
            {0,0,7,7},
            {0,0,0,0}
        }
    }
};

// Colors for the tetrominoes
SDL_Color colors[] = {
    {0, 0, 0, 255},         // 0: empty
    {0, 255, 255, 255},     // 1: I - cyan
    {255, 255, 0, 255},     // 2: O - yellow
    {128, 0, 128, 255},     // 3: T - purple
    {0, 255, 0, 255},       // 4: S - green
    {255, 0, 0, 255},       // 5: Z - red
    {0, 0, 255, 255},       // 6: J - blue
    {255, 165, 0, 255}      // 7: L - orange
};

// Game state
int board[ROWS][COLS];
int currentShape, rotation;
int posX, posY;
bool gameOver = false;

// Function declarations
void initGame();
void spawnTetromino();
bool checkCollision(int dx, int dy, int dr);
void lockTetromino();
void clearLines();
void rotateTetromino(int dr);
void moveTetromino(int dx);
void drawBoard(SDL_Renderer *renderer);

int main(int argc, char *argv[]) {
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    initGame();

    Uint32 lastTick = SDL_GetTicks();
    Uint32 delay = 500; // Tetromino falls every 500ms

    while (!gameOver) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                gameOver = true;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        moveTetromino(-1);
                        break;
                    case SDLK_RIGHT:
                        moveTetromino(1);
                        break;
                    case SDLK_DOWN:
                        if (!checkCollision(0, 1, 0))
                            posY++;
                        break;
                    case SDLK_UP:
                        rotateTetromino(1);
                        break;
                    case SDLK_SPACE:
                        while (!checkCollision(0, 1, 0))
                            posY++;
                        lockTetromino();
                        break;
                }
            }
        }

        Uint32 currentTick = SDL_GetTicks();
        if (currentTick - lastTick > delay) {
            lastTick = currentTick;
            if (!checkCollision(0, 1, 0)) {
                posY++;
            } else {
                lockTetromino();
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawBoard(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void initGame() {
    // Clear the board
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = 0;

    spawnTetromino();
}

void spawnTetromino() {
    currentShape = rand() % 7;
    rotation = 0;
    posX = COLS / 2 - 2;
    posY = 0;

    if (checkCollision(0, 0, 0))
        gameOver = true;
}

bool checkCollision(int dx, int dy, int dr) {
    int newRotation = (rotation + dr) % 4;
    const int (*shape)[4] = shapes[currentShape][newRotation];

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int val = shape[y][x];
            if (val != 0) {
                int newX = posX + x + dx;
                int newY = posY + y + dy;

                if (newX < 0 || newX >= COLS || newY >= ROWS)
                    return true;
                if (newY >= 0 && board[newY][newX] != 0)
                    return true;
            }
        }
    }
    return false;
}

void lockTetromino() {
    const int (*shape)[4] = shapes[currentShape][rotation];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int val = shape[y][x];
            if (val != 0) {
                int newX = posX + x;
                int newY = posY + y;
                if (newY >= 0 && newY < ROWS && newX >= 0 && newX < COLS)
                    board[newY][newX] = val;
            }
        }
    }
    clearLines();
    spawnTetromino();
}

void clearLines() {
    for (int y = 0; y < ROWS; y++) {
        bool full = true;
        for (int x = 0; x < COLS; x++) {
            if (board[y][x] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < COLS; x++) {
                    board[yy][x] = board[yy - 1][x];
                }
            }
            for (int x = 0; x < COLS; x++) {
                board[0][x] = 0;
            }
        }
    }
}

void rotateTetromino(int dr) {
    if (!checkCollision(0, 0, dr))
        rotation = (rotation + dr) % 4;
}

void moveTetromino(int dx) {
    if (!checkCollision(dx, 0, 0))
        posX += dx;
}

void drawBoard(SDL_Renderer *renderer) {
    // Draw the board
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            SDL_Rect rect = { x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
            int val = board[y][x];
            SDL_Color color = colors[val];
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    // Draw the current tetromino
    const int (*shape)[4] = shapes[currentShape][rotation];
    SDL_Color color = colors[currentShape + 1];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int val = shape[y][x];
            if (val != 0) {
                int drawX = (posX + x) * BLOCK_SIZE;
                int drawY = (posY + y) * BLOCK_SIZE;
                SDL_Rect rect = { drawX, drawY, BLOCK_SIZE, BLOCK_SIZE };
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

