#!/usr/bin/env python3
import can
import time
import _thread


bus = can.ThreadSafeBus(interface='socketcan', channel='can0')


ROBOTEQ_MSGS_FREQUENCY_HZ = 200
ROVER_CAN_STREAMED_MSGS_FREQUENCY_HZ = 100
ONE_SHOT_MSGS_FREQUENCY_HZ = 1
CAN_BUS_BIT_RATE = 1000000

# CAN IDs
TPDO1 = 0x181
TPDO2 = 0x281
TPDO3 = 0x381
TPDO4 = 0x481

ARM_SET_JOINT_POSITION = 0x622
GIMBAL_SET_JOINT_POSITION = 0x6A1
SCIENCE_SET_JOINT_POSITION = 0x661

ARM_SET_CONTROL_MODE = 0x621
GIMBAL_SET_CONTROL_MODE = 0x6A0
SCIENCE_SET_CONTROL_MODE = 0x660


# roboteq streamed messages
tpdo1_msg = can.Message(arbitration_id=TPDO1, data=[0, 0, 0, 0, 0, 0, 0, 0])
tpdo2_msg = can.Message(arbitration_id=TPDO2, data=[0, 0, 0, 0, 0, 0, 0, 0])
tpdo3_msg = can.Message(arbitration_id=TPDO3, data=[0, 0, 0, 0, 0, 0, 0, 0])
tpdo4_msg = can.Message(arbitration_id=TPDO4, data=[0, 0, 0, 0, 0, 0, 0, 0])

# arm streamed messages
arm_set_joint_position_msg = can.Message(
    arbitration_id=ARM_SET_JOINT_POSITION, data=[0, 0, 0, 0, 0, 0, 0, 0])

# gimbal streamed messages
gimbal_set_joint_position_msg = can.Message(
    arbitration_id=GIMBAL_SET_JOINT_POSITION, data=[0, 0, 0, 0, 0, 0])

# science streamed messages
science_set_joint_position_msg = can.Message(
    arbitration_id=SCIENCE_SET_JOINT_POSITION, data=[0, 0, 0, 0, 0, 0, 0, 0])

# one-shots
arm_set_control_mode_msg = can.Message(
    arbitration_id=ARM_SET_CONTROL_MODE, data=[0, 0, 0])
gimbal_set_control_mode_msg = can.Message(
    arbitration_id=GIMBAL_SET_CONTROL_MODE, data=[0])
science_set_control_mode_msg = can.Message(
    arbitration_id=SCIENCE_SET_CONTROL_MODE, data=[0])

# diagnostics
num_msgs_sent_roboteq = 0
num_streamed_sent_arm = 0
num_one_shots_sent_arm = 0
num_streamed_sent_gimbal = 0
num_one_shots_sent_gimbal = 0
num_streamed_sent_science = 0
num_one_shots_sent_science = 0


def roboteq_can_traffic(args):
    global num_msgs_sent_roboteq
    while True:
        bus.send(tpdo1_msg)
        bus.send(tpdo2_msg)
        bus.send(tpdo3_msg)
        bus.send(tpdo4_msg)
        num_msgs_sent_roboteq += 4
        time.sleep(1/ROBOTEQ_MSGS_FREQUENCY_HZ)


def arm_can_traffic(args):
    global num_streamed_sent_arm
    while True:
        bus.send(arm_set_joint_position_msg)
        num_streamed_sent_arm += 1
        time.sleep(1/ROVER_CAN_STREAMED_MSGS_FREQUENCY_HZ)


def gimbal_can_traffic(args):
    global num_streamed_sent_gimbal
    while True:
        bus.send(gimbal_set_joint_position_msg)
        num_streamed_sent_gimbal += 1
        time.sleep(1/ROVER_CAN_STREAMED_MSGS_FREQUENCY_HZ)


def science_can_traffic(args):
    global num_streamed_sent_science
    while True:
        bus.send(science_set_joint_position_msg)
        num_streamed_sent_science += 1
        time.sleep(1/ROVER_CAN_STREAMED_MSGS_FREQUENCY_HZ)


def one_shots_can_traffic(args):
    global num_one_shots_sent_arm
    global num_one_shots_sent_gimbal
    global num_one_shots_sent_science
    while True:
        bus.send(arm_set_control_mode_msg)
        bus.send(gimbal_set_control_mode_msg)
        bus.send(science_set_control_mode_msg)
        num_one_shots_sent_arm += 1
        num_one_shots_sent_gimbal += 1
        num_one_shots_sent_science += 1
        time.sleep(1/ONE_SHOT_MSGS_FREQUENCY_HZ)


def output_diagnostics(args):
    global num_msgs_sent_roboteq
    global num_streamed_sent_arm
    global num_one_shots_sent_arm
    global num_streamed_sent_gimbal
    global num_one_shots_sent_gimbal
    global num_streamed_sent_science
    global num_one_shots_sent_science
    while True:
        print("ROBOTEQ - number of messages sent: " + str(num_msgs_sent_roboteq))
        print("ARM - number of streamed messages sent: " + str(num_streamed_sent_arm))
        print("ARM - number of one-shot messages sent: " + str(num_one_shots_sent_arm))
        print("GIMBAL - number of streamed messages sent: " + str(num_streamed_sent_gimbal))
        print("GIMBAL - number of one-shot messages sent: " + str(num_one_shots_sent_gimbal))
        print("SCIENCE - number of streamed messages sent: " + str(num_streamed_sent_science))
        print("SCIENCE - number of one-shot messages sent: " + str(num_one_shots_sent_science))
        print('====================')
        time.sleep(2)  # sleep 2s


try:
    _thread.start_new_thread(roboteq_can_traffic, (0,))
    _thread.start_new_thread(arm_can_traffic, (0,))
    _thread.start_new_thread(gimbal_can_traffic, (0,))
    _thread.start_new_thread(science_can_traffic, (0,))
    _thread.start_new_thread(one_shots_can_traffic, (0,))
    _thread.start_new_thread(output_diagnostics, (0,))
except Exception as e:
    print(e)

while True:
    pass
