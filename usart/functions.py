import cv2
import numpy as np
import math
import serial
import serial.tools.list_ports
import struct



def serial_init():
    global ser
    ser = serial.Serial('/dev/ttyS1', 9600, timeout=1)
    if ser.isOpen():
        print(ser.name, ' Open Successed!')
    else:
        print('Open Failed!')

def serial_out(x, y, z, color):
    # struct.pack 应该是把这四个float值都结算为十六进制的值然后排列成包
    bytesdata = struct.pack('<4f', float(x), float(y), float(z), float(color))

    # 这里就是合体，打包成一个数据包
    combined = b'\xFF' + bytesdata + b'\xFe'

    # 这里就是发送数据包 
    write_len = ser.write(combined)
    print('hex值为：', combined)
    print('字节串长度为：', write_len)

# def serial_in(length):
#     try:
#         recieved_data = ser.read(length)
#         # print(recieved_data)
#         if recieved_data[0] == 255 :

#             # print('get a requisition')
#             return 1
#         elif recieved_data[0] == 254:
#             print("被中断了！！！！！！！！！！！！！！！！！！")
#             return 2
#         else:
#             print(recieved_data)
#             return False
#     except:
#         print('not recieved any data')
#         return False

def serial_in(length):
    data = ser.read(length)
    return data

def searchposition(camera, color):
    center = []
    while 1:
        _, frame = camera.read()
        cv2.imwrite('0.jpg', frame)
        # 尝试获取坐标，失败打印未找到
        try:
            cl = get_color(frame, color)
            rst = pre_process(cl)
            dots = findcorner(rst)
            X, Y, Z = get_xyz(dots, 23)
            # center.append([X, Y, Z])
            # print(f"方块中心与相机距离：X={X:.2f}cm, Y={Y:.2f}cm, Z={Z:.2f}cm")
            x, y, z = xyz_transaction(float(X), float(Y), float(Z))
            # x, y, z = tran1(float(X), float(Y), float(Z))
            center.append([x, y, z])
            print(f"方块中心相对机械臂：X={x:.2f}mm, Y={y:.2f}mm, Z={z:.2f}mm ")
            #for i in dots:  # 在原图上画多边形的端点
                #result2 = cv2.circle(frame, i[0], 3, (0, 0, 255), -1)
        except:
            print('cube not found')
        #cv2.imshow('frame', frame)

        # 返回处理好的数据
        # cv2.waitKey(2)
        if len(center) > 30:
            break
    center = np.array(center, dtype=np.float32)
    x, y, z = np.median(center, axis=0)
    return x, y, z
