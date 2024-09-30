#include <SDL2/SDL.h>
#include <stdbool.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Player constants
#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 50
#define PLAYER_SPEED 6
#define JUMP_SPEED -12
#define GRAVITY 0.5

// Platform structure
typedef struct {
    SDL_Rect rect;
} Platform;

// Function prototypes
bool init();
void close_SDL();
bool check_collision(SDL_Rect a, SDL_Rect b);

// Global variables
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    bool quit = false;
    SDL_Event e;
    SDL_Rect player = {50, SCREEN_HEIGHT - PLAYER_HEIGHT - 10, PLAYER_WIDTH, PLAYER_HEIGHT};
    float velocityX = 0;
    float velocityY = 0;

    // Platforms
    Platform platforms[4];
    platforms[0].rect = (SDL_Rect){0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40};
    platforms[1].rect = (SDL_Rect){200, 450, 100, 20};
    platforms[2].rect = (SDL_Rect){400, 350, 100, 20};
    platforms[3].rect = (SDL_Rect){600, 250, 100, 20};

    bool keys[SDL_NUM_SCANCODES] = {false};

    // Game loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_KEYDOWN) {
                keys[e.key.keysym.scancode] = true;
            } else if (e.type == SDL_KEYUP) {
                keys[e.key.keysym.scancode] = false;
            }
        }

        // Movement
        velocityX = 0;
        if (keys[SDL_SCANCODE_LEFT])
            velocityX = -PLAYER_SPEED;
        if (keys[SDL_SCANCODE_RIGHT])
            velocityX = PLAYER_SPEED;
        if (keys[SDL_SCANCODE_SPACE]) {
            // Check if on ground or on a platform
            SDL_Rect tempRect = player;
            tempRect.y += 2;
            bool onGround = false;
            for (int i = 0; i < 4; i++) {
                if (check_collision(tempRect, platforms[i].rect)) {
                    onGround = true;
                    break;
                }
            }
            if (onGround || player.y + player.h >= SCREEN_HEIGHT) {
                velocityY = JUMP_SPEED;
            }
        }

        // Apply gravity
        velocityY += GRAVITY;

        // Update player position
        player.x += (int)velocityX;

        // Collision detection on X-axis
        for (int i = 0; i < 4; i++) {
            if (check_collision(player, platforms[i].rect)) {
                if (velocityX > 0)
                    player.x = platforms[i].rect.x - player.w;
                else if (velocityX < 0)
                    player.x = platforms[i].rect.x + platforms[i].rect.w;
            }
        }

        player.y += (int)velocityY;

        // Collision detection on Y-axis
        for (int i = 0; i < 4; i++) {
            if (check_collision(player, platforms[i].rect)) {
                if (velocityY > 0) {
                    player.y = platforms[i].rect.y - player.h;
                    velocityY = 0;
                } else if (velocityY < 0) {
                    player.y = platforms[i].rect.y + platforms[i].rect.h;
                    velocityY = 0;
                }
            }
        }

        // Keep player within screen bounds
        if (player.x < 0)
            player.x = 0;
        if (player.x + player.w > SCREEN_WIDTH)
            player.x = SCREEN_WIDTH - player.w;
        if (player.y + player.h > SCREEN_HEIGHT) {
            player.y = SCREEN_HEIGHT - player.h;
            velocityY = 0;
        }

        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(gRenderer);

        // Draw platforms
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Black platforms
        for (int i = 0; i < 4; i++) {
            SDL_RenderFillRect(gRenderer, &platforms[i].rect);
        }

        // Draw player
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255); // Blue player
        SDL_RenderFillRect(gRenderer, &player);

        // Update screen
        SDL_RenderPresent(gRenderer);

        // Delay to limit frame rate
        SDL_Delay(16); // Approximately 60 FPS
    }

    // Clean up
    close_SDL();
    return 0;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Simple Platformer",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);

    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(gWindow);
        return false;
    }

    return true;
}

void close_SDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool check_collision(SDL_Rect a, SDL_Rect b) {
    // The sides of the rectangles
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    // Check for overlap
    if (bottomA <= topB)
        return false;
    if (topA >= bottomB)
        return false;
    if (rightA <= leftB)
        return false;
    if (leftA >= rightB)
        return false;

    // Collision detected
    return true;
}

