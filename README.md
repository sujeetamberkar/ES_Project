# ES_Project
This repository contains an embedded C program for the LPC1768 microcontroller, which demonstrates Nokia keypad-style text input using a 4x4 matrix keypad and displays the decoded text on an LCD screen. The code is designed to scan the keypad for input, store the pressed keys in a queue, and then convert the multi-press Nokia-style input into readable text, which is then displayed on the LCD.

# Features
    4x4 matrix keypad input
    Nokia-style multi-press text input decoding
    Queue implementation for storing pressed keys
    LCD screen integration for displaying the decoded text
    GPIO pin configuration for keypad input and LCD output
# Configuration
  Keyboard:
    2.10-2.13
    1.23--1.26
  LCD display:
    0.23-0.28
    
# Prerequisites
    LPC1768 microcontroller
    4x4 matrix keypad
    LCD screen (compatible with the provided code)
    Keil uVision IDE (or any other suitable ARM Cortex-M3 compatible IDE)
  
# Installation
      1)Clone the repository to your local machine.
            git clone https://github.com/your_username/nokia-keypad-lpc1768.git

      2)Open the project in Keil uVision IDE (or your preferred ARM Cortex-M3 compatible IDE).

      3)Configure your LPC1768 microcontroller settings as per your development board.

      4)Connect the 4x4 matrix keypad and LCD screen to the appropriate GPIO pins on the LPC1768 microcontroller, as specified in the code.

      5)Build the project and flash the compiled binary to your LPC1768 microcontroller.

      6)Run the program and start inputting text using the Nokia-style keypad input

