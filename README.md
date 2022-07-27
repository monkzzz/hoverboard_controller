# Hoverboard Controller

A controller for inline skates made with hoverboard wheels.

The controller is based on a ttgo esp32 with screen and programmed with arduino C language.

It receives input by a Nintendo Nunchuck by direct connection or a PS3 controller via bluetooth.

The controller interfaces by UART with the GD32 microcontroller, present in the hoverboard, changing their speed, steer and lights.

There is also information the controller receives from the hoverboard like battery voltage.

This project is in a very early stage and is being developed slowly as the hoverboard GD32 code is also being developed.

Extra safety measures like a deadman's trigger, a simulated electronic gear shift and a obstacle detector is or will be implemented, but for now is not 100% safe or viable
