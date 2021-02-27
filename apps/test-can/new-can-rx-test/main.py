# import the library
import can
import time

# create a bus instance
# many other interfaces are supported as well (see below)
bus = can.Bus(interface='socketcan',
              channel='can0',
              receive_own_messages=False)

# send a message
message = can.Message(arbitration_id=0x123, is_extended_id=False,
                      data=[0xAA])

start = time.time()
for i in range(1000):
    bus.send(message,timeout=0.2)
    time.sleep(0.001)

print("--- %s seconds ---" % (time.time() - start))

print("done")
