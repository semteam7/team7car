import serial
#import vehiclecontrol_pb2
import sys

#speed, acceleration, steeringWheelAngle, brakeLights,
# flashingLightsLeft, flashingLightsRight

ser = serial.Serial('COM1', 9600, timeout=.1)
ser.write('testing this stuff>')
# def testsend():
#     VC = vehiclecontrol_pb2.VehicleCommand()
#     VC.speed = "1"
#     VC.acceleration = "1"
#     VC.steeringWheelAngle = "0"
#     VC.brakeLights = "0"
#     VC.flashingLightsRight = "0"
#     VC.flashingLightsLeft = "0"
#
#     print(VC.SerializeToString());
print("Hey")
