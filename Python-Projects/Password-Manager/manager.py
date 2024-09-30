import pygame
import sys
import os
import pickle
from cryptography.fernet import Fernet

# Initialize PyGame
pygame.init()

# Set up display
WIDTH, HEIGHT = 600, 400
window = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('PyGame Password Manager')

# Fonts
FONT = pygame.font.SysFont('arial', 20)
FONT_LARGE = pygame.font.SysFont('arial', 30)

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

# Constants
DATA_FILE = 'passwords.dat'
KEY_FILE = 'secret.key'

# Global Variables
current_screen = 'menu'
input_boxes = []
passwords = {}
selected_account = None

# Load or generate encryption key
def load_key():
    if os.path.exists(KEY_FILE):
        with open(KEY_FILE, 'rb') as key_file:
            key = key_file.read()
    else:
        key = Fernet.generate_key()
        with open(KEY_FILE, 'wb') as key_file:
            key_file.write(key)
    return key

# Initialize encryption key
key = load_key()
fernet = Fernet(key)

# Load passwords
def load_passwords():
    global passwords
    if os.path.exists(DATA_FILE):
        with open(DATA_FILE, 'rb') as file:
            encrypted_data = file.read()
            if encrypted_data:
                decrypted_data = fernet.decrypt(encrypted_data)
                passwords = pickle.loads(decrypted_data)
    else:
        passwords = {}

# Save passwords
def save_passwords():
    with open(DATA_FILE, 'wb') as file:
        data = pickle.dumps(passwords)
        encrypted_data = fernet.encrypt(data)
        file.write(encrypted_data)

# InputBox class for text input
class InputBox:
    def __init__(self, x, y, w, h, text='', is_password=False):
        self.rect = pygame.Rect(x, y, w, h)
        self.color = BLACK
        self.text = text
        self.txt_surface = FONT.render(text, True, self.color)
        self.active = False
        self.is_password = is_password

    def handle_event(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            # Toggle active state if the user clicked on the input box
            if self.rect.collidepoint(event.pos):
                self.active = not self.active
            else:
                self.active = False
            self.color = BLACK if self.active else BLACK
        if event.type == pygame.KEYDOWN:
            if self.active:
                if event.key == pygame.K_RETURN:
                    pass  # Handle Enter key if needed
                elif event.key == pygame.K_BACKSPACE:
                    self.text = self.text[:-1]
                else:
                    self.text += event.unicode
                # Re-render the text
                display_text = '*' * len(self.text) if self.is_password else self.text
                self.txt_surface = FONT.render(display_text, True, self.color)

    def draw(self, window):
        # Blit the text
        window.blit(self.txt_surface, (self.rect.x+5, self.rect.y+5))
        # Blit the rect
        pygame.draw.rect(window, self.color, self.rect, 2)

# GUI Functions
def draw_menu():
    window.fill(WHITE)
    title = FONT_LARGE.render('Password Manager', True, BLACK)
    window.blit(title, (WIDTH//2 - title.get_width()//2, 50))

    buttons = ['Add Password', 'Retrieve Password', 'Update Password', 'Delete Password', 'List Accounts', 'Exit']
    for i, text in enumerate(buttons):
        btn_rect = pygame.Rect(WIDTH//2 - 100, 120 + i*40, 200, 30)
        pygame.draw.rect(window, BLACK, btn_rect, 2)
        btn_text = FONT.render(text, True, BLACK)
        window.blit(btn_text, (btn_rect.x + 10, btn_rect.y + 5))

def draw_add_password():
    window.fill(WHITE)
    title = FONT_LARGE.render('Add Password', True, BLACK)
    window.blit(title, (WIDTH//2 - title.get_width()//2, 50))
    for box in input_boxes:
        box.draw(window)
    save_btn = pygame.Rect(WIDTH//2 - 50, 250, 100, 30)
    pygame.draw.rect(window, BLACK, save_btn, 2)
    btn_text = FONT.render('Save', True, BLACK)
    window.blit(btn_text, (save_btn.x + 25, save_btn.y + 5))

def draw_message(message):
    window.fill(WHITE)
    msg_text = FONT.render(message, True, BLACK)
    window.blit(msg_text, (WIDTH//2 - msg_text.get_width()//2, HEIGHT//2))
    pygame.display.flip()
    pygame.time.delay(2000)

def draw_list_accounts():
    window.fill(WHITE)
    title = FONT_LARGE.render('Stored Accounts', True, BLACK)
    window.blit(title, (WIDTH//2 - title.get_width()//2, 50))
    y_offset = 100
    for account in passwords.keys():
        acc_text = FONT.render('- ' + account, True, BLACK)
        window.blit(acc_text, (50, y_offset))
        y_offset += 30

# Event Handlers
def handle_menu_events(event):
    global current_screen
    if event.type == pygame.MOUSEBUTTONDOWN:
        x, y = event.pos
        for i in range(6):
            btn_rect = pygame.Rect(WIDTH//2 - 100, 120 + i*40, 200, 30)
            if btn_rect.collidepoint(event.pos):
                if i == 0:
                    current_screen = 'add_password'
                elif i == 1:
                    current_screen = 'retrieve_password'
                elif i == 2:
                    current_screen = 'update_password'
                elif i == 3:
                    current_screen = 'delete_password'
                elif i == 4:
                    current_screen = 'list_accounts'
                elif i == 5:
                    pygame.quit()
                    sys.exit()

def handle_add_password_events(event):
    global current_screen
    for box in input_boxes:
        box.handle_event(event)
    if event.type == pygame.MOUSEBUTTONDOWN:
        save_btn = pygame.Rect(WIDTH//2 - 50, 250, 100, 30)
        if save_btn.collidepoint(event.pos):
            account = input_boxes[0].text.strip()
            password = input_boxes[1].text.strip()
            if account and password:
                passwords[account] = password
                save_passwords()
                draw_message('Password Saved Successfully!')
                current_screen = 'menu'
            else:
                draw_message('Please fill in all fields.')

def handle_list_accounts_events(event):
    global current_screen
    if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
        current_screen = 'menu'

# Main Loop
def main():
    global input_boxes, current_screen
    load_passwords()
    clock = pygame.time.Clock()
    running = True

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                save_passwords()
                running = False

            if current_screen == 'menu':
                handle_menu_events(event)
            elif current_screen == 'add_password':
                handle_add_password_events(event)
            elif current_screen == 'list_accounts':
                handle_list_accounts_events(event)
            # Add more event handlers for other screens

        if current_screen == 'menu':
            draw_menu()
        elif current_screen == 'add_password':
            if not input_boxes:
                input_boxes = [
                    InputBox(WIDTH//2 - 100, 120, 200, 30, 'Account Name'),
                    InputBox(WIDTH//2 - 100, 170, 200, 30, 'Password', is_password=True)
                ]
            draw_add_password()
        elif current_screen == 'list_accounts':
            draw_list_accounts()
        # Add more drawing functions for other screens

        pygame.display.flip()
        clock.tick(30)

    pygame.quit()

if __name__ == '__main__':
    main()

