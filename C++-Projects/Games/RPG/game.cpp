// main.cpp

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Player constants
const float PLAYER_SPEED = 200.0f;

// Tile constants
const int TILE_SIZE = 32;

// Function prototypes
bool loadTileset(sf::Texture& tileset);
void handleInput(sf::Event& event, bool& running, sf::Vector2f& movement);
void updatePlayer(sf::Sprite& player, sf::Vector2f& movement, float deltaTime);
void loadMap(std::vector<sf::RectangleShape>& walls);

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Simple 2D RPG");

    // Set the frame rate limit
    window.setFramerateLimit(60);

    // Load the player texture
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.jpeg")) {
        std::cerr << "Error loading player texture!" << std::endl;
        return -1;
    }

    // Create the player sprite
    sf::Sprite player(playerTexture);
    player.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

    // Load the tileset texture
    sf::Texture tilesetTexture;
    if (!loadTileset(tilesetTexture)) {
        return -1;
    }

    // Create a vector to store wall rectangles
    std::vector<sf::RectangleShape> walls;
    loadMap(walls);

    // Game loop variables
    bool running = true;
    sf::Clock clock;

    while (running) {
        sf::Event event;
        sf::Vector2f movement(0.0f, 0.0f);
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        while (window.pollEvent(event)) {
            handleInput(event, running, movement);
        }

        // Handle continuous input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            movement.x -= PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            movement.x += PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            movement.y -= PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            movement.y += PLAYER_SPEED;
        }

        // Update player position
        updatePlayer(player, movement, deltaTime);

        // Clear screen
        window.clear(sf::Color::Black);

        // Draw walls
        for (auto& wall : walls) {
            window.draw(wall);
        }

        // Draw the player
        window.draw(player);

        // Update the window
        window.display();
    }

    return 0;
}

bool loadTileset(sf::Texture& tileset) {
    if (!tileset.loadFromFile("tileset.jpeg")) {
        std::cerr << "Error loading tileset texture!" << std::endl;
        return false;
    }
    return true;
}

void handleInput(sf::Event& event, bool& running, sf::Vector2f& movement) {
    if (event.type == sf::Event::Closed) {
        running = false;
    }
}

void updatePlayer(sf::Sprite& player, sf::Vector2f& movement, float deltaTime) {
    player.move(movement * deltaTime);
}

void loadMap(std::vector<sf::RectangleShape>& walls) {
    // For simplicity, we will create a simple map with walls at the borders
    sf::RectangleShape wall;
    wall.setFillColor(sf::Color::White);

    // Top wall
    wall.setSize(sf::Vector2f(SCREEN_WIDTH, TILE_SIZE));
    wall.setPosition(0, 0);
    walls.push_back(wall);

    // Bottom wall
    wall.setPosition(0, SCREEN_HEIGHT - TILE_SIZE);
    walls.push_back(wall);

    // Left wall
    wall.setSize(sf::Vector2f(TILE_SIZE, SCREEN_HEIGHT));
    wall.setPosition(0, 0);
    walls.push_back(wall);

    // Right wall
    wall.setPosition(SCREEN_WIDTH - TILE_SIZE, 0);
    walls.push_back(wall);

    // Add more walls or obstacles as needed
}

