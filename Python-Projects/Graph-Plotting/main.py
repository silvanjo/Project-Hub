import pygame
import sys
import numpy as np
import sympy as sp

pygame.init()

# Screen dimensions
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Graph Plotting Tool")

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

# Font
FONT = pygame.font.SysFont(None, 24)

# Input box class
class InputBox:
    def __init__(self, x, y, w, h, text=''):
        self.rect = pygame.Rect(x, y, w, h)
        self.color = BLACK
        self.text = text
        self.txt_surface = FONT.render(text, True, self.color)
        self.active = True  # Input box is active by default

    def handle_event(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            # Toggle active state if the input box is clicked
            if self.rect.collidepoint(event.pos):
                self.active = not self.active
            else:
                self.active = False
            self.color = (0, 0, 255) if self.active else BLACK
        if event.type == pygame.KEYDOWN:
            if self.active:
                if event.key == pygame.K_RETURN:
                    self.text = ''
                    self.update()
                elif event.key == pygame.K_BACKSPACE:
                    self.text = self.text[:-1]
                else:
                    self.text += event.unicode
                self.update()

    def update(self):
        # Re-render the text surface
        self.txt_surface = FONT.render(self.text, True, self.color)

    def draw(self, screen):
        # Blit the text surface onto the screen
        screen.blit(self.txt_surface, (self.rect.x+5, self.rect.y+5))
        # Draw the input box rectangle
        pygame.draw.rect(screen, self.color, self.rect, 2)

def parse_equation(equation_str):
    x = sp.symbols('x')
    try:
        equation = sp.sympify(equation_str)
        return sp.lambdify(x, equation, 'numpy')
    except (sp.SympifyError, TypeError):
        return None

def draw_graph(func, scale, offset):
    # Draw coordinate axes
    pygame.draw.line(screen, BLACK, (0, HEIGHT//2 + offset[1]), (WIDTH, HEIGHT//2 + offset[1]), 1)
    pygame.draw.line(screen, BLACK, (WIDTH//2 + offset[0], 0), (WIDTH//2 + offset[0], HEIGHT), 1)

    x_pixels = np.array(range(WIDTH))
    x_values = (x_pixels - WIDTH//2 - offset[0]) / scale

    try:
        y_values = func(x_values)
    except:
        y_values = np.zeros_like(x_values)

    y_pixels = HEIGHT//2 - y_values * scale + offset[1]

    points = np.column_stack((x_pixels, y_pixels))

    # Filter points that are within the screen
    points = points[(points[:,1] >= 0) & (points[:,1] <= HEIGHT)]

    if len(points) > 1:
        pygame.draw.lines(screen, (255, 0, 0), False, points, 2)

def main():
    clock = pygame.time.Clock()
    input_box = InputBox(10, 10, 300, 32)
    func = None
    scale = 50  # Pixels per unit
    offset = [0, 0]  # Panning offset

    dragging = False
    last_mouse_pos = (0, 0)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

            input_box.handle_event(event)

            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    dragging = True
                    last_mouse_pos = event.pos
                elif event.button == 4:  # Scroll up
                    scale *= 1.1
                elif event.button == 5:  # Scroll down
                    scale /= 1.1
            if event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:
                    dragging = False
            if event.type == pygame.MOUSEMOTION:
                if dragging:
                    dx = event.pos[0] - last_mouse_pos[0]
                    dy = event.pos[1] - last_mouse_pos[1]
                    offset[0] += dx
                    offset[1] += dy
                    last_mouse_pos = event.pos

        # Clear the screen
        screen.fill(WHITE)
        # Draw the input box
        input_box.draw(screen)

        # Parse the equation and update the function
        func = parse_equation(input_box.text)

        # Draw the graph if the function is valid
        if func:
            draw_graph(func, scale, offset)

        pygame.display.flip()
        clock.tick(60)

if __name__ == '__main__':
    main()

