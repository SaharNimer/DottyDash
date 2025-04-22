#DottyDash 
An Interactive Morse Code Translator using Arduino, LEDs, LCD, and Web Interface

 Overview
DottyDash is a bidirectional Morse code translator built with Arduino. It supports:
- Text-to-Morse with LED output
- Morse-to-Text input via push button
- Real-time LCD display of Morse and decoded text
- Web Serial interface for browser-based interaction

Features
- Push-button Morse input (short press = dot, long = dash)
- LED blinking for visual Morse output
- LCD shows both Morse and alphabet output
- Reset via button or `!RESET` command
- Web UI for serial communication (HTML + JS)

Components
- Arduino Uno
- 16x2 LCD (I2C)
- 2 LEDs (Red, Green)
- 2 Push Buttons
- 220Ω resistors
- Breadboard + wires
