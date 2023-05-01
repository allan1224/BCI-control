import socket
import pyautogui
import tkinter as tk
import threading
import time

# Set up UDP socket
UDP_IP = "0.0.0.0"
UDP_PORT = 1234
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# Define GUI functions
def click_button():
    pyautogui.click()

def type_string():
    string_to_type = type_entry.get()
    pyautogui.typewrite(string_to_type)

# Create GUI window
window = tk.Tk()
window.title("UDP Controller")

# Add text entry to GUI for keyboard typing
type_entry = tk.Entry(window)
type_entry.pack()

# Add typing button to GUI
type_button = tk.Button(window, text="Type", command=type_string)
type_button.pack()

# Define main loop function
def main_loop():
    # Listen for UDP packets from any IP address
    while True:
        data, addr = sock.recvfrom(1024)
        print("Received data from {}: {}".format(addr[0], data))

        # Simulate left mouse click
        if data == b"left_click":
            time.sleep(3)
            pyautogui.click()

        # Simulate keyboard typing
        elif data.startswith(b"type:"):
            string_to_type = data[5:].decode('utf-8')
            time.sleep(3)
            pyautogui.typewrite(string_to_type)

        # Update GUI
        window.update()

# Add mouse click button to GUI
click_button = tk.Button(window, text="Click", command=click_button)
click_button.pack()

# Create and start main loop thread
t = threading.Thread(target=main_loop)
t.start()

# Run GUI
window.mainloop()

