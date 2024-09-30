import pygame
import sys

# Initialize Pygame
pygame.init()

# Screen dimensions
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

# Create the display
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Simple Platformer")

# Define constants
GRAVITY = 0.5

# Colors
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
BLACK = (0, 0, 0)

# Clock
clock = pygame.time.Clock()

# Player class
class Player(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.image = pygame.Surface((30, 50))
        self.image.fill(BLUE)
        self.rect = self.image.get_rect()
        self.rect.x = 50
        self.rect.y = SCREEN_HEIGHT - self.rect.height - 10
        self.change_x = 0
        self.change_y = 0
        self.jump_speed = -12

    def update(self, platforms):
        self.gravity()
        self.rect.x += self.change_x

        # Check for collisions on the x-axis
        collisions = pygame.sprite.spritecollide(self, platforms, False)
        for platform in collisions:
            if self.change_x > 0:
                self.rect.right = platform.rect.left
            elif self.change_x < 0:
                self.rect.left = platform.rect.right

        self.rect.y += self.change_y

        # Check for collisions on the y-axis
        collisions = pygame.sprite.spritecollide(self, platforms, False)
        for platform in collisions:
            if self.change_y > 0:
                self.rect.bottom = platform.rect.top
                self.change_y = 0
            elif self.change_y < 0:
                self.rect.top = platform.rect.bottom
                self.change_y = 0

    def gravity(self):
        self.change_y += GRAVITY
        if self.rect.y >= SCREEN_HEIGHT - self.rect.height and self.change_y >= 0:
            self.change_y = 0
            self.rect.y = SCREEN_HEIGHT - self.rect.height

    def jump(self, platforms):
        # Check if player is on a platform
        self.rect.y += 2
        platform_hit_list = pygame.sprite.spritecollide(self, platforms, False)
        self.rect.y -= 2

        if len(platform_hit_list) > 0 or self.rect.bottom >= SCREEN_HEIGHT:
            self.change_y = self.jump_speed

    def go_left(self):
        self.change_x = -6

    def go_right(self):
        self.change_x = 6

    def stop(self):
        self.change_x = 0

# Platform class
class Platform(pygame.sprite.Sprite):
    def __init__(self, x, y, width, height):
        super().__init__()
        self.image = pygame.Surface([width, height])
        self.image.fill(BLACK)
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y

# Create platforms
platform_list = pygame.sprite.Group()

platforms = [
    Platform(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40),
    Platform(200, 450, 100, 20),
    Platform(400, 350, 100, 20),
    Platform(600, 250, 100, 20)
]

for platform in platforms:
    platform_list.add(platform)

# Create player
player = Player()
all_sprites = pygame.sprite.Group()
all_sprites.add(player)
all_sprites.add(platform_list)

# Game loop
running = True
while running:
    clock.tick(60)  # Limit to 60 frames per second

    # Event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                player.go_left()
            if event.key == pygame.K_RIGHT:
                player.go_right()
            if event.key == pygame.K_SPACE:
                player.jump(platform_list)

        if event.type == pygame.KEYUP:
            if (event.key == pygame.K_LEFT and player.change_x < 0) or \
               (event.key == pygame.K_RIGHT and player.change_x > 0):
                player.stop()

    # Update
    player.update(platform_list)

    # Draw
    screen.fill(WHITE)
    all_sprites.draw(screen)

    # Flip display
    pygame.display.flip()

pygame.quit()
sys.exit()

