from __future__ import print_function

from inputs import get_key
import can
import struct
import time
import threading
import xbox

turnTableMotion = 0
shoulderMotion  = 0
elbowMotion     = 0

scalingFactor = 0.5

def getInput():
    
    global turnTableMotion
    global shoulderMotion
    global elbowMotion

    global scalingFactor

    joy = xbox.Joystick()

    while 1:

        turnTableMotion = fmtFloat(joy.leftX())
        shoulderMotion = fmtFloat(joy.leftY())
        elbowMotion = fmtFloat(joy.rightY())

        # events = get_key()
        # if events:
        #         for event in events:
        #             # print(event.ev_type, event.code, event.state)

        #             scaledInput = min(1, event.state) * scalingFactor

        #             if (event.ev_type == 'Key'):

        #                 if (event.code == 'KEY_A'):
        #                     turnTableMotion = -scaledInput
        #                 elif (event.code == 'KEY_D'):
        #                     turnTableMotion = +scaledInput

        #                 elif (event.code == 'KEY_W'):
        #                     shoulderMotion = +scaledInput
        #                 elif (event.code == 'KEY_S'):
        #                     shoulderMotion = -scaledInput

        #                 elif (event.code == 'KEY_UP'):
        #                     elbowMotion = -scaledInput
        #                 elif (event.code == 'KEY_DOWN'):
        #                     elbowMotion = +scaledInput



def main():

    global turnTableMotion
    global shoulderMotion
    global elbowMotion
    global getInput

    getInput = threading.Thread(target=getInput);
    getInput.daemon = True;
    getInput.start()

    bus = can.interface.Bus(bustype='socketcan', channel='can0', bitrate=500000)

    while 1:
        
        turnTableMotionMessage = can.Message(arbitration_id=0x301, 
                                             data=bytearray(struct.pack("f", turnTableMotion)),
                                             is_extended_id=False)
        
        shoulderMotionMessage = can.Message(arbitration_id=0x303, 
                                            data=bytearray(struct.pack("f", shoulderMotion)),
                                            is_extended_id=False)

        elbowMotionMessage = can.Message(arbitration_id=0x305, 
                                         data=bytearray(struct.pack("f", elbowMotion)),
                                         is_extended_id=False)

        try:
            bus.send(turnTableMotionMessage)
            time.sleep(0.02);
            bus.send(shoulderMotionMessage)
            time.sleep(0.02);
            bus.send(elbowMotionMessage)
            time.sleep(0.02);            
        except can.CanError:
            print("Message NOT sent")

        # print("TT: %f\n" % turnTableMotion);
        # print("SH: %f\n" % shoulderMotion);
        # print("EL: %f\n" % elbowMotion);

if __name__ == "__main__":
    main()
