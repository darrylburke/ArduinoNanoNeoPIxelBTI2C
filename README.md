# ArduinoNanoNeoPIxelBTI2C
Project to drive an Arduino Nano with Neopixels using Serial / Bluetooth / I2C

The Arduino Nano is set to read commands via the SERIAL in (TTY / Bluetooth) or via the I2C.  all commands are the same regardless of method of input.

<h1> Arduino Nano</h1>

if the demo pin is set to "HIGH" the nano will go into an automated "Demo" mode and not read any serial input.

Serial commands are seperated by a "|" character

```
Commands are:

Read:[?|]
 Usage: End all Commands with '|' 
 ? - Show Usage (This list)
 ARRGGBB - Set all pixels to the same RGB values
 A000000 - Set all pixels to Off
 COP - Police Lights Mode
 COPCAR - Police Car Lights Mode
 WORM - Worm Mode
 CYCLONE - Cyclone Lights Mode
 STROBE - Strobe Lights Mode
 RAINBOWS - Rainbow Lights Mode - Individual Pixels
 RAINBOW - Rainbow Lights Mode - all Pixels
 FIRE - Fire Lights Mode
 LDDDDPSPERRGGBB[DDDDPSPERRGGBB] - Set Pixel Mode (You can stack multiple calls in a row using the "|" delimiter
  Groups of 10 Digits:
  DDDD = Miliseconds to delay before execution
  PS = Start Pixel
  PE = End Pixel
  RR = Red Color
  GG = Green Color
  BB = Blue Color

Eg.  L10000102ffffff|50000809aaaaaa|
```

<H1> Raspberry Pi </h1>
THe 


./i2cstring RAINBOWS\|

