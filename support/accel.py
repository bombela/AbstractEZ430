#!/usr/bin/python

import serial
import array

def startAccessPoint():
    return array.array('B', [0xFF, 0x07, 0x03]).tostring()

def accDataRequest():
    return array.array('B', [0xFF, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00]).tostring()

#Open COM port 6 (check your system info to see which port
#yours is actually on.)
#argments are 5 (COM6), 115200 (bit rate), and timeout is set so
#the serial read function won't loop forever.
ser = serial.Serial("/dev/ttyACM1", 115200, timeout=1)

#Start access point
ser.write(startAccessPoint())


while True:
    #Send request for acceleration data
    ser.write(accDataRequest())
    accel = ser.read(7)

    
    if ord(accel[0]) != 0 and ord(accel[1]) != 0 and ord(accel[2]) != 0:
         print "x: " + str(ord(accel[0])) + " y: " + str(ord(accel[1])) + " z: " + str(ord(accel[2]))
    
    
ser.close()

