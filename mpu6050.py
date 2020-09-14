import smbus
import math
import socket
import struct


# Power management registers
power_mgmt_1 = 0x6b
power_mgmt_2 = 0x6c

def read_byte(adr):
    return bus.read_byte_data(address, adr)

def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val


bus = smbus.SMBus(1) 
address = 0x68       

# Now wake the 6050 up as it starts in sleep mode
bus.write_byte_data(address, power_mgmt_1, 0)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('192.168.100.5', 48050))

while True:
    gyro_xout = round(read_word_2c(0x43) / 131.0, 15)
    gyro_yout = round(read_word_2c(0x45) / 131.0, 15)
    gyro_zout = round(read_word_2c(0x47) / 131.0, 15)

    accel_xout = round(read_word_2c(0x3b) / 16384.0, 15)
    accel_yout = round(read_word_2c(0x3d) / 16384.0, 15)
    accel_zout = round(read_word_2c(0x3f) / 16384.0, 15)

    sock.sendall(struct.pack('dddddd', gyro_xout, gyro_yout, gyro_zout, accel_xout, accel_yout, accel_zout))
