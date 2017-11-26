This driver allows you to setup Spaceball either as a 6-axis joystick (via vJoy: http://vjoystick.sourceforge.net/site/ ) or as a mouse.

Usage:
driver.exe [settings file]



settings.txt:

(integer) - vjoy device id or 0 to disable joystick support
(string) - COM port name connected to Spaceball
(integer) (hex string)... - Initialization string sent to Spaceball  ( old protocol spec: http://spacemice.org/pdf/SpaceBall_2003-3003_Protocol.pdf )

(36 integers) - Linear input sensitivity matrix

(6 integers) - Offset for each axis

(12 strings) - Key names to which 12 buttons are bound to

(6 integers) - Mouse mode for each axis:
0 - disabled
1 - horizontal mouse movement
2 - vertical mouse movement
3 - horizontal mouse wheel
4 - mouse wheel
