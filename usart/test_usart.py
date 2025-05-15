import cv2
import numpy as np
import math
import serial
import serial.tools.list_ports
import struct

# 串口初始化
def serial_init():
    # 全局变量
    global ser
    # 波特率9600
    ser = serial.Serial('/dev/ttyS1', 9600, timeout=1)
    if ser.isOpen():
        print(ser.name, ' Open Successed!')
    else:
        print('Open Failed!')


"""
包头 55 aa
包尾 0d 0a

发送端：
"""

'''
发送left_speed, right_speed, angle
以short类型发送
13个字节
数据包格式：55 aa 00 00 00 00 00 00 00 00 0d 0a
'''
# 这是发送数据包
def serial_out(left_speed, right_speed, angle):
    # struct.pack 应该是把这四个float值都结算为十六进制的值然后排列成包
    bytesdata = struct.pack('<4f', )

    # 这里就是合体，打包成一个数据包
    combined = b'\x55' + b'\xaa'+ bytesdata + b'\x0d'+ b'\x0a'


    # 这里就是发送数据包 
    write_len = ser.write(combined)
    print('hex值为：', combined)
    print('字节串长度为：', write_len)



# 这是接收数据
def serial_in(length):
    data = ser.read(length)
    return data

# ----读取串口数据-----------------------------------
try:
    # ???
    count = serial.inWaiting()
    if count > 0:
        # 初始化数据
        Read_buffer = []
        # 接收数据至缓存区
        Read_buffer=serial.read(16)         # 我们需要读取的是40个寄存器数据，即40个字节
        # Read_data()						# 前面两行可以注释，换成后面这个函数
        # 将 16 个字节解析为 4 个 float
    try:
        floats = struct.unpack('<4f', Read_buffer)  # '<4f' 表示小端序的 4 个 float
        print("解析后的 float 数值:", floats)
    except struct.error as e:
        print("解析失败:", e)
except KeyboardInterrupt:
     if serial != None:
         print("close serial port")
         serial.close()

