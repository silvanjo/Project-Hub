import pygame
import random

# Initialize Pygame
pygame.init()

# Screen dimensions
WIDTH, HEIGHT = 300, 600
BLOCK_SIZE = 30  # Size of the blocks
cols = WIDTH // BLOCK_SIZE
rows = HEIGHT // BLOCK_SIZE

# Set up the display
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Tetris')

# Define shapes and colors
SHAPES = [
    [['.....',
      '.....',
      '..0..',
      '..0..',
      '..0..'],

     ['.....',
      '..000',
      '.....',
      '.....',
      '.....']],

    [['.....',
      '.....',
      '..00.',
      '..00.',
      '.....']],

    [['.....',
      '.....',
      '..0..',
      '..0..',
      '..00.'],

     ['.....',
      '..000',
      '..0..',
      '.....',
      '.....']],

    [['.....',
      '.....',
      '..0..',
      '..0..',
      '.00..'],

     ['.....',
      '..000',
      '...0.',
      '.....',
      '.....']],

    [['.....',
      '.....',
      '..00.',
      '.00..',
      '.....']],

    [['.....',
      '.....',
      '.00..',
      '..00.',
      '.....']],
]

SHAPE_COLORS = [
    (0, 255, 255),
    (255, 255, 0),
    (128, 0, 128),
    (0, 255, 0),
    (255, 165, 0),
    (255, 0, 0),
    (0, 0, 255)
]

class Piece:
    def __init__(self, x, y, shape):
        self.x = x
        self.y = y
        self.shape = shape
        self.color = SHAPE_COLORS[SHAPES.index(shape)]
        self.rotation = 0

def create_grid(locked_positions={}):
    grid = [[(0,0,0) for _ in range(cols)] for _ in range(rows)]
    for i in range(rows):
        for j in range(cols):
            if (j, i) in locked_positions:
                c = locked_positions[(j, i)]
                grid[i][j] = c
    return grid

def convert_shape_format(piece):
    positions = []
    format = piece.shape[piece.rotation % len(piece.shape)]
    for i, line in enumerate(format):
        row = list(line)
        for j, column in enumerate(row):
            if column == '0':
                positions.append((piece.x + j - 2, piece.y + i - 4))
    return positions

def valid_space(piece, grid):
    accepted_positions = [[(j, i) for j in range(cols) if grid[i][j] == (0,0,0)] for i in range(rows)]
    accepted_positions = [j for sub in accepted_positions for j in sub]
    formatted = convert_shape_format(piece)
    for pos in formatted:
        if pos not in accepted_positions:
            if pos[1] > -1:
                return False
    return True

def check_lost(positions):
    for pos in positions:
        x, y = pos
        if y < 1:
            return True
    return False

def get_shape():
    return Piece(cols // 2, 0, random.choice(SHAPES))

def draw_grid(surface, grid):
    for i in range(rows):
        pygame.draw.line(surface, (128,128,128), (0, i*BLOCK_SIZE), (WIDTH, i*BLOCK_SIZE))
        for j in range(cols):
            pygame.draw.line(surface, (128,128,128), (j*BLOCK_SIZE, 0), (j*BLOCK_SIZE, HEIGHT))

def clear_rows(grid, locked):
    inc = 0
    for i in range(len(grid)-1,-1,-1):
        row = grid[i]
        if (0,0,0) not in row:
            inc += 1
            ind = i
            for j in range(len(row)):
                try:
                    del locked[(j, i)]
                except:
                    continue
    if inc > 0:
        for key in sorted(list(locked), key=lambda x:x[1])[::-1]:
            x, y = key
            if y < ind:
                newKey = (x, y + inc)
                locked[newKey] = locked.pop(key)

def draw_next_shape(piece, surface):
    font = pygame.font.SysFont('comicsans', 30)
    label = font.render('Next Shape:', 1, (255,255,255))
    sx = WIDTH + 50
    sy = HEIGHT // 2 - 100
    format = piece.shape[piece.rotation % len(piece.shape)]
    for i, line in enumerate(format):
        row = list(line)
        for j, column in enumerate(row):
            if column == '0':
                pygame.draw.rect(surface, piece.color, (sx + j*BLOCK_SIZE, sy + i*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE), 0)
    surface.blit(label, (sx + 10, sy - 30))

def draw_window(surface, grid, score=0):
    surface.fill((0,0,0))
    font = pygame.font.SysFont('comicsans', 60)
    label = font.render('Tetris', 1, (255,255,255))
    surface.blit(label, (WIDTH // 2 - label.get_width() // 2, 30))
    # Draw current score
    font = pygame.font.SysFont('comicsans', 30)
    label = font.render('Score: ' + str(score), 1, (255,255,255))
    surface.blit(label, (WIDTH - label.get_width() - 10, HEIGHT - 30))
    for i in range(rows):
        for j in range(cols):
            pygame.draw.rect(surface, grid[i][j], (j*BLOCK_SIZE, i*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE), 0)
    draw_grid(surface, grid)

def main():
    global grid
    locked_positions = {}
    grid = create_grid(locked_positions)
    change_piece = False
    run = True
    current_piece = get_shape()
    next_piece = get_shape()
    clock = pygame.time.Clock()
    fall_time = 0
    fall_speed = 0.27
    level_time = 0
    score = 0
    while run:
        grid = create_grid(locked_positions)
        fall_time += clock.get_rawtime()
        level_time += clock.get_rawtime()
        clock.tick()
        if level_time/1000 > 5:
            level_time = 0
            if fall_speed > 0.12:
                fall_speed -= 0.005
        if fall_time/1000 > fall_speed:
            fall_time = 0
            current_piece.y +=1
            if not(valid_space(current_piece, grid)) and current_piece.y > 0:
                current_piece.y -=1
                change_piece = True
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
                pygame.display.quit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    current_piece.x -=1
                    if not(valid_space(current_piece, grid)):
                        current_piece.x +=1
                elif event.key == pygame.K_RIGHT:
                    current_piece.x +=1
                    if not(valid_space(current_piece, grid)):
                        current_piece.x -=1
                elif event.key == pygame.K_DOWN:
                    current_piece.y +=1
                    if not(valid_space(current_piece, grid)):
                        current_piece.y -=1
                elif event.key == pygame.K_UP:
                    current_piece.rotation +=1
                    if not(valid_space(current_piece, grid)):
                        current_piece.rotation -=1
        shape_pos = convert_shape_format(current_piece)
        for i in range(len(shape_pos)):
            x, y = shape_pos[i]
            if y > -1:
                grid[y][x] = current_piece.color
        if change_piece:
            for pos in shape_pos:
                p = (pos[0], pos[1])
                locked_positions[p] = current_piece.color
            current_piece = next_piece
            next_piece = get_shape()
            change_piece = False
            score += 10
            clear_rows(grid, locked_positions)
        draw_window(screen, grid, score)
        draw_next_shape(next_piece, screen)
        pygame.display.update()
        if check_lost(locked_positions):
            run = False
    pygame.display.quit()

def main_menu():
    run = True
    while run:
        screen.fill((0,0,0))
        font = pygame.font.SysFont('comicsans', 60)
        label = font.render('Press Any Key', 1, (255,255,255))
        screen.blit(label, (WIDTH // 2 - label.get_width() // 2, HEIGHT // 2 - label.get_height() // 2))
        pygame.display.update()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.KEYDOWN:
                main()
    pygame.quit()

if __name__ == '__main__':
    main_menu()

