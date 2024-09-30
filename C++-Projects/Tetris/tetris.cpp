#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

// Game constants
const int M = 20;
const int N = 10;
const int BLOCK_SIZE = 30;

// Field array
int field[M][N] = {0};

// Tetromino shapes
int figures[7][4] = {
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

struct Point
{
    int x, y;
} a[4], b[4];

bool check()
{
    for (int i=0;i<4;i++)
        if (a[i].x<0 || a[i].x>=N || a[i].y>=M)
            return false;
        else if (field[a[i].y][a[i].x])
            return false;
    return true;
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N*BLOCK_SIZE, M*BLOCK_SIZE), "Tetris");

    // Define colors for tetrominoes
    Color colors[8] = {
        Color::Black,      // 0 - Empty
        Color::Cyan,       // 1 - I
        Color::Red,        // 2 - Z
        Color::Green,      // 3 - S
        Color::Yellow,     // 4 - T
        Color(255, 165, 0),// 5 - L (Orange)
        Color::Blue,       // 6 - J
        Color::Magenta     // 7 - O
    };

    int dx = 0;
    bool rotate = false;
    int colorNum = 1;
    float timer = 0, delay = 0.5;

    Clock clock;

    // Generate first piece
    int n = rand() % 7;
    colorNum = n + 1;
    for (int i=0;i<4;i++)
    {
        a[i].x = figures[n][i] % 2 + N / 2 - 1;
        a[i].y = figures[n][i] / 2;
    }

    // Game loop
    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        timer += time;

        // Handle events
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Up)
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right)
                    dx = 1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        // Move
        for (int i=0;i<4;i++) { b[i] = a[i]; a[i].x += dx; }
        if (!check())
            for (int i=0;i<4;i++)
                a[i] = b[i];

        // Rotate
        if (rotate)
        {
            Point p = a[1]; // Center of rotation
            for (int i=0;i<4;i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check())
                for (int i=0;i<4;i++)
                    a[i] = b[i];
        }

        // Tick
        if (timer > delay)
        {
            for (int i=0;i<4;i++) { b[i] = a[i]; a[i].y += 1; }

            if (!check())
            {
                for (int i=0;i<4;i++)
                    field[b[i].y][b[i].x] = colorNum;

                // Generate new piece
                n = rand() % 7;
                colorNum = n + 1;
                for (int i=0;i<4;i++)
                {
                    a[i].x = figures[n][i] % 2 + N / 2 - 1;
                    a[i].y = figures[n][i] / 2;
                }

                // Check if new piece collides immediately (Game Over)
                if (!check())
                {
                    window.close();
                    break;
                }
            }

            timer = 0;
        }

        // Check lines
        int k = M - 1;
        for (int i = M - 1; i >= 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if (count < N) k--;
        }

        dx = 0; rotate = false; delay = 0.5;

        // Draw
        window.clear(Color::Black);

        // Draw field
        RectangleShape block(Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
        for (int i=0;i<M;i++)
            for (int j=0;j<N;j++)
            {
                if (field[i][j]==0) continue;
                block.setFillColor(colors[field[i][j]]);
                block.setPosition(j*BLOCK_SIZE, i*BLOCK_SIZE);
                window.draw(block);
            }

        // Draw current tetromino
        for (int i=0;i<4;i++)
        {
            block.setFillColor(colors[colorNum]);
            block.setPosition(a[i].x*BLOCK_SIZE, a[i].y*BLOCK_SIZE);
            window.draw(block);
        }

        window.display();
    }

    return 0;
}

