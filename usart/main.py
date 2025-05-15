import cv2
import numpy as np
import functions as f
import time
import serial
import serial.tools.list_ports


blue = [[90, 130], [200, 255], [0, 255]]
green = [[50, 100], [150, 255], [120, 255]]
orange = [[0, 18] , [100, 255], [214, 255]]
colors = [green, orange]
f.serial_init()
camera = cv2.VideoCapture(9)
# img = cv2.imread('0.jpg')
while 1:

    rcv = f.serial_in(1) # 带STM32时的代码

    if rcv != b'':
        if rcv[0] == 255:
            print('get a reqisition')
            # 识别颜色
            for i in range(20):

                _, frame = camera.read()

            ind = f.knowcolor(frame, colors[0])
            color = colors[ind]

            rcv = f.serial_in(1)
            while 1:




                try:
                    cl = f.get_color(frame, color)
                    rst = f.pre_process(cl)
                    dots = f.findcorner(rst)
                    X, Y, Z = f.get_xyz(dots, 23)


                    x, y, z = f.xyz_transaction(float(X), float(Y), float(Z))

                    center.append([x, y, z])

                except:
                    print('cube not found')
                #cv2.imshow('frame', frame)

                # 在被中断时，返回计时器1.5秒内的中位数
                # key = cv2.waitKey(50)
                # if key != -1:
                #     J = True
                # if J == True and t >= 1500:
                #     break
                # if t > 1600:
                #     t = 0
                #     center = [] 

                # 返回处理好的数据


            #cv2.destroyAllWindows()

            center = np.array(center, dtype=np.float32)
            x, y, z = np.median(center, axis=0)

            f.serial_out(x, z, y, ind+1)


            if f.serial_in(1)[0] == 254:
                print('被停止拍照')
                break


    
            


