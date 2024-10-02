import tkinter as tk
from tkinter import messagebox
import os

# Caesar Cipher encryption
def encrypt_password(password, shift=3):
    encrypted = ""
    for char in password:
        if char.isalpha():
            shift_by = 65 if char.isupper() else 97
            encrypted += chr((ord(char) + shift - shift_by) % 26 + shift_by)
        else:
            encrypted += char
    return encrypted

# Caesar Cipher decryption
def decrypt_password(encrypted_password, shift=3):
    decrypted = ""
    for char in encrypted_password:
        if char.isalpha():
            shift_by = 65 if char.isupper() else 97
            decrypted += chr((ord(char) - shift - shift_by) % 26 + shift_by)
        else:
            decrypted += char
    return decrypted

# Save the service name and password
def save_password():
    service = entry_service.get().strip()
    password = entry_password.get().strip()

    if not service or not password:
        messagebox.showerror("Error", "Service and password fields cannot be empty")
        return

    encrypted_password = encrypt_password(password)

    # Save the encrypted password to a file
    with open("passwords.txt", "a") as file:
        file.write(f"{service}:{encrypted_password}\n")  # No space after ":"

    entry_service.delete(0, tk.END)
    entry_password.delete(0, tk.END)

    messagebox.showinfo("Success", "Password saved successfully")

# Retrieve and display the password for a given service
def retrieve_password():
    service = entry_service.get().strip()

    if not os.path.exists("passwords.txt"):
        messagebox.showerror("Error", "No passwords stored yet.")
        return

    found = False
    with open("passwords.txt", "r") as file:
        for line in file:
            if ":" not in line:
                continue  # Skip lines without the correct format
            stored_service, stored_encrypted_password = line.strip().split(":")
            if stored_service == service:
                decrypted_password = decrypt_password(stored_encrypted_password.strip())
                messagebox.showinfo("Password Retrieved", f"Password for {service}: {decrypted_password}")
                found = True
                break

    if not found:
        messagebox.showerror("Error", f"No password found for {service}")

# Create the Tkinter window
root = tk.Tk()
root.title("Password Manager")

# Labels and entry fields
label_service = tk.Label(root, text="Service:")
label_service.pack(pady=5)
entry_service = tk.Entry(root, width=40)
entry_service.pack(pady=5)

label_password = tk.Label(root, text="Password:")
label_password.pack(pady=5)
entry_password = tk.Entry(root, show="*", width=40)
entry_password.pack(pady=5)

# Buttons to save and retrieve passwords
button_save = tk.Button(root, text="Save Password", command=save_password)
button_save.pack(pady=5)

button_retrieve = tk.Button(root, text="Retrieve Password", command=retrieve_password)
button_retrieve.pack(pady=5)

# Start the Tkinter main loop
root.mainloop()

