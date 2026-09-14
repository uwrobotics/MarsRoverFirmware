Servo training

Build either app from the repository root

```sh
make APP=tutorial-servo-pot-control TARGET=UWRT_NUCLEO
make APP=tutorial-servo-can-control TARGET=UWRT_NUCLEO
```

Both apps output a 50 Hz servo signal on PA_1
The potentiometer app reads PA_0 and maps the normalized input directly to a pulse width from 1 to 2 ms

TutorialServo accepts a configurable travel range and pulse limits
Positions outside the travel range are clamped and nonfinite positions are ignored
The initial position is zero degrees

CAN commands

Use a 500 kbps CAN bus with a transceiver connected to CAN_RX on PA_11 and CAN_TX on PA_12
The command is a standard data frame with ID 0x100 and exactly four payload bytes
The payload is a little endian IEEE 754 float giving a percentage from 0 to 100
Other IDs and formats and invalid percentages are ignored

After configuring a CAN interface on Linux the following commands request zero and half and full travel

```sh
cansend can0 100#00000000
cansend can0 100#00004842
cansend can0 100#0000C842
```

Hardware verification is still pending
The commands above are test instructions and have not been run on a connected servo
