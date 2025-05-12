import cv2
import numpy as np
import math
import serial

import struct


def serial_init():
    global ser
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
两个包头    一个长度    三个short   一个校验    一个flag    两个包尾
'''
# 这是发送数据包
def serial_out(left_speed, right_speed, angle,Flag):
    # 左轮速度，右轮速度，角度，语音标志位

    bytesdata = struct.pack('<3h', int(left_speed), int(right_speed), int(angle))

    # 这里就是合体，打包成一个数据包
    combined = b'\x55' + b'\xaa'+ b'\x07' + bytesdata + Flag + b'\x00' +b'\x0d'+ b'\x0a'


    # 这里就是发送数据包 
    write_len = ser.write(combined)
    print('hex值为：', combined)
    print('字节串长度为：', write_len)



# 这是接收数据
# length：接收数据长度
def serial_in():
    # ----读取串口数据-----------------------------------
    try:
        # ???
        count = ser.inWaiting()
        if count > 0:
            # 初始化数据
            Read_buffer = []
            # 接收数据至缓存区
            Read_buffer=serial.read(13)         # 读取 13 个字节        # 左轮速度，右轮速度，角度，语音标志位

            if len(Read_buffer) != 13:
                print("数据包长度错误")
                return -1
            else:
                if Read_buffer[0] != 0x55 or Read_buffer[1] != 0xaa or Read_buffer[11] != 0x0d or Read_buffer[12] != 0x0a:
                    print("数据包头尾错误")
                    return -1
                else:
                    # 0~12
                    # 0,1包头   2长度   3~8数据     9flag   10校验位      11,12包尾 9
                    speed_angel = struct.unpack('<3h', Read_buffer[3:9])
                    Flag = Read_buffer[9:10]

                    return speed_angel
                    # 这里是将数据包解析成左轮速度，右轮速度，角度，语音标志位
    except KeyboardInterrupt:
        if serial != None:
            print("close serial port")
            serial.close()
