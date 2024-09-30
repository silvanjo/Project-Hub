import pygame
import time
from pygame.locals import *

pygame.init()

# Global constants
WIDTH, HEIGHT = 540, 600  # Window size
GRID_SIZE = 9             # Sudoku grid size
CELL_SIZE = WIDTH // GRID_SIZE
FPS = 60                  # Frames per second

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GRAY = (128, 128, 128)
BLUE = (0, 0, 255)
RED = (255, 0, 0)

# Fonts
FONT = pygame.font.SysFont("comicsans", 40)
SMALL_FONT = pygame.font.SysFont("comicsans", 20)

class Cell:
    def __init__(self, value, row, col, width, height):
        self.value = value  # Actual value of the cell
        self.temp = 0       # Temporary value (while the user is entering)
        self.row = row
        self.col = col
        self.width = width
        self.height = height
        self.selected = False

    def draw(self, win):
        x = self.col * self.width
        y = self.row * self.height

        if self.value != 0:
            # Draw permanent number (black)
            text = FONT.render(str(self.value), True, BLACK)
            win.blit(text, (x + (self.width - text.get_width()) / 2,
                            y + (self.height - text.get_height()) / 2))
        elif self.temp != 0:
            # Draw temporary number (gray)
            text = SMALL_FONT.render(str(self.temp), True, GRAY)
            win.blit(text, (x + 5, y + 5))

        if self.selected:
            pygame.draw.rect(win, RED, (x, y, self.width, self.height), 3)

    def set_value(self, val):
        self.value = val

    def set_temp(self, val):
        self.temp = val

class Grid:
    def __init__(self, rows, cols, width, height, board):
        self.rows = rows
        self.cols = cols
        self.cells = [[Cell(board[i][j], i, j, width / cols, height / rows)
                       for j in range(cols)] for i in range(rows)]
        self.width = width
        self.height = height
        self.selected = None
        self.board = board

    def draw(self, win):
        # Draw grid lines
        win.fill(WHITE)
        for i in range(self.rows + 1):
            line_width = 3 if i % 3 == 0 else 1
            # Draw horizontal lines
            pygame.draw.line(win, BLACK, (0, i * CELL_SIZE),
                             (self.width, i * CELL_SIZE), line_width)
            # Draw vertical lines
            pygame.draw.line(win, BLACK, (i * CELL_SIZE, 0),
                             (i * CELL_SIZE, self.height), line_width)

        # Draw cells
        for row in self.cells:
            for cell in row:
                cell.draw(win)

    def select(self, row, col):
        # Deselect all cells
        for i in range(self.rows):
            for j in range(self.cols):
                self.cells[i][j].selected = False

        # Select the new cell
        self.cells[row][col].selected = True
        self.selected = (row, col)

    def click(self, pos):
        """
        Returns the position in the grid based on the mouse position
        """
        if pos[0] < self.width and pos[1] < self.height:
            x = pos[0] // CELL_SIZE
            y = pos[1] // CELL_SIZE
            return (int(y), int(x))
        else:
            return None

    def clear(self):
        row, col = self.selected
        if self.cells[row][col].value == 0:
            self.cells[row][col].temp = 0

    def place(self, val):
        row, col = self.selected
        if self.cells[row][col].value == 0:
            self.cells[row][col].set_value(val)
            self.board[row][col] = val

            if valid(self.board, val, (row, col)) and self.solve():
                return True
            else:
                self.cells[row][col].set_value(0)
                self.cells[row][col].set_temp(0)
                self.board[row][col] = 0
                return False

    def sketch(self, val):
        row, col = self.selected
        self.cells[row][col].set_temp(val)

    def is_finished(self):
        for row in self.cells:
            for cell in row:
                if cell.value == 0:
                    return False
        return True

    def solve(self):
        find = find_empty(self.board)
        if not find:
            return True
        else:
            row, col = find

        for i in range(1, 10):
            if valid(self.board, i, (row, col)):
                self.board[row][col] = i

                if self.solve():
                    return True

                self.board[row][col] = 0

        return False

    def solve_gui(self, win):
        self.draw(win)
        pygame.display.update()
        time.sleep(0.05)

        find = find_empty(self.board)
        if not find:
            return True
        else:
            row, col = find

        for i in range(1, 10):
            if valid(self.board, i, (row, col)):
                self.board[row][col] = i
                self.cells[row][col].set_value(i)
                self.cells[row][col].draw(win)
                pygame.display.update()
                time.sleep(0.05)

                if self.solve_gui(win):
                    return True

                self.board[row][col] = 0
                self.cells[row][col].set_value(0)
                self.cells[row][col].draw(win)
                pygame.display.update()
                time.sleep(0.05)

        return False

def valid(board, num, pos):
    # Check row
    for i in range(len(board[0])):
        if board[pos[0]][i] == num and pos[1] != i:
            return False

    # Check column
    for i in range(len(board)):
        if board[i][pos[1]] == num and pos[0] != i:
            return False

    # Check box
    box_x = pos[1] // 3
    box_y = pos[0] // 3

    for i in range(box_y * 3, box_y * 3 + 3):
        for j in range(box_x * 3, box_x * 3 + 3):
            if board[i][j] == num and (i, j) != pos:
                return False

    return True

def find_empty(board):
    for i in range(len(board)):
        for j in range(len(board[0])):
            if board[i][j] == 0:
                return (i, j)  # row, col

    return None

def redraw_window(win, board, time, strikes):
    win.fill(WHITE)
    # Draw time
    text = SMALL_FONT.render("Time: " + format_time(time), True, BLACK)
    win.blit(text, (WIDTH - 160, HEIGHT - 50))
    # Draw strikes
    text = SMALL_FONT.render("X " * strikes, True, RED)
    win.blit(text, (20, HEIGHT - 50))
    # Draw grid and board
    board.draw(win)

def format_time(secs):
    sec = secs % 60
    minute = secs // 60
    return f"{minute:02}:{sec:02}"

def main():
    win = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Sudoku Solver")
    board = [
        [7, 8, 0, 4, 0, 0, 1, 2, 0],
        [6, 0, 0, 0, 7, 5, 0, 0, 9],
        [0, 0, 0, 6, 0, 1, 0, 7, 8],
        [0, 0, 7, 0, 4, 0, 2, 6, 0],
        [0, 0, 1, 0, 5, 0, 9, 3, 0],
        [9, 0, 4, 0, 6, 0, 0, 0, 5],
        [0, 7, 0, 3, 0, 0, 0, 1, 2],
        [1, 2, 0, 0, 0, 7, 4, 0, 0],
        [0, 4, 9, 2, 0, 6, 0, 0, 7]
    ]
    key = None
    run = True
    start = time.time()
    strikes = 0
    grid = Grid(GRID_SIZE, GRID_SIZE, WIDTH, HEIGHT - 60, board)

    while run:
        play_time = round(time.time() - start)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
                pygame.quit()
                return

            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                clicked = grid.click(pos)
                if clicked:
                    grid.select(clicked[0], clicked[1])
                    key = None

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_1 or event.key == pygame.K_KP1:
                    key = 1
                if event.key == pygame.K_2 or event.key == pygame.K_KP2:
                    key = 2
                if event.key == pygame.K_3 or event.key == pygame.K_KP3:
                    key = 3
                if event.key == pygame.K_4 or event.key == pygame.K_KP4:
                    key = 4
                if event.key == pygame.K_5 or event.key == pygame.K_KP5:
                    key = 5
                if event.key == pygame.K_6 or event.key == pygame.K_KP6:
                    key = 6
                if event.key == pygame.K_7 or event.key == pygame.K_KP7:
                    key = 7
                if event.key == pygame.K_8 or event.key == pygame.K_KP8:
                    key = 8
                if event.key == pygame.K_9 or event.key == pygame.K_KP9:
                    key = 9
                if event.key == pygame.K_DELETE or event.key == pygame.K_BACKSPACE:
                    grid.clear()
                    key = None
                if event.key == pygame.K_RETURN:
                    i, j = grid.selected if grid.selected else (None, None)
                    if i is not None and grid.cells[i][j].temp != 0:
                        if grid.place(grid.cells[i][j].temp):
                            print("Success")
                        else:
                            print("Wrong")
                            strikes += 1
                        key = None

                        if grid.is_finished():
                            print("Game over")
                            run = False
                if event.key == pygame.K_SPACE:
                    grid.solve_gui(win)

        if grid.selected and key is not None:
            grid.sketch(key)

        redraw_window(win, grid, play_time, strikes)
        pygame.display.update()

if __name__ == "__main__":
    main()

