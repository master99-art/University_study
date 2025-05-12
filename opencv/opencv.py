import cv2
import numpy as np
from pyzbar.pyzbar import decode
from PIL import Image
import math
import os
import pytesseract
AREA = 2000
AREA_ROI = 1000
WH_MIN = 1.1
WH_MAX = 4
epsilon = 5
long = 3

def Get_image():
    # 读取图片
    cap = cv2.VideoCapture(1)

    ret,image = cap.read()

    cv2.imwrite("D:\develop\py_study\opencv_test\datasets\cache_video_img\image.bmp", image)
    cap.release()
    return 1

def Get_ROI():

    # 这里是提取ROI区域
    img = cv2.imread(r'D:\develop\py_study\opencv_test\datasets\cache_video_img\image.bmp', cv2.IMREAD_REDUCED_COLOR_2)

    # copy 照片用来裁剪ROI区域
    img_ROI = img.copy()
    kernel1 = np.ones((3, 3), np.uint8)

    # 提取图像尺寸？？？？？
    img_size = img.shape[:2]
    img_size = list(img_size)
    img_size[0], img_size[1] = img_size[1], img_size[0]


    # 膨胀
    img_pen = cv2.dilate(img, kernel1)

    # 变灰度
    img_gray = cv2.cvtColor(img_pen, cv2.COLOR_BGR2GRAY)
    img_gray = cv2.dilate(img_gray, kernel1)

    # 二值化
    _, img_er = cv2.threshold(img_gray, 200, 255, cv2.THRESH_BINARY)

    # find轮廓
    contours, a = cv2.findContours(img_er, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 判断轮廓面积
    contour = []
    for con in contours:
        area = cv2.contourArea(con)
        if area >= AREA:
            contour.append(con)

    # 寻找合适的识别版轮廓
    # 长宽比
    # 顺便把ROI给提取出来
    x = 0
    y = 0
    w = 0
    h = 0


    for con in contour:
        approxCurve = cv2.approxPolyDP(con, epsilon, True)
        # 确认长度角点为四
        if len(approxCurve) == 4:
            x, y, w, h = cv2.boundingRect(approxCurve)
            if WH_MIN <= w / h <= WH_MAX:
                # print(w,h)
                img = cv2.drawContours(img, [approxCurve], -1, (0, 0, 255), -1)
                x, y, w, h = cv2.boundingRect(approxCurve)



    # 把ROI提取出来了
    ROI = img_ROI[y:y + h, x:x + w]
    res_ROI = ROI.copy()

    cv2.imshow("ROI", res_ROI)
    cv2.imwrite('D:\develop\py_study\opencv_test\datasets\cache_video_img\ROI.bmp',ROI)
    return 1

def Get_zbar():
    kernel1 = np.ones((3, 3), np.uint8)
    # 寻找二维码，并返回字典
    res_dic = {1: '0', 2: '0', 3: '0', 4: '0'}
    # 预处理ROI
    ROI = cv2.imread('D:\develop\py_study\opencv_test\datasets\cache_video_img\ROI.bmp')
    ROI_gray = cv2.cvtColor(ROI, cv2.COLOR_BGR2GRAY)
    ROI_dilate = cv2.dilate(ROI_gray, kernel1)
    # 阈值化处理 反阈值化，只接受第二个返回参数
    _, ROI_er = cv2.threshold(ROI_dilate, 240, 255, cv2.THRESH_BINARY_INV)

    contours, a = cv2.findContours(ROI_er, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contour = []

    # 限制面积寻找正方形
    for i in contours:
        # 计算轮廓面积
        area = cv2.contourArea(i)
        # 判断如果面积大于200，那么符合条件
        if area > AREA_ROI:
            contour.append(i)

    # 找到合适的拟合图像

    curves = []
    for con in contour:
        # 多边形拟合 闭合 距离小于等于58
        # 返回的是点集，因为是多边形，那么有几个点就有几个顶点
        # temp_len = cv2.arcLength(con, True)
        # epsilon = temp_len * 0.1

        approxCurve = cv2.approxPolyDP(con, epsilon, True)
        # result = cv2.drawContours(img, [approxCurve], -1, (0,0,255), -1)

        # 拟合完之后就把这个添加进我们创建好的列表中
        curves.append(approxCurve)

    # 按照边数为4、对边横纵坐标差之差小于指定误差long来筛选正方形

    ge = 0
    for i in curves:
        # 顶点有四个的符合条件
        if len(i) == 4:

            if -long < abs((i[0][0][0] - i[1][0][0])) - abs((i[2][0][0] - i[3][0][0])) < long and -long < abs(
                    (i[0][0][1] - i[1][0][1])) - abs((i[2][0][1] - i[3][0][1])) < long:
                result = cv2.drawContours(ROI, [i], -1, (0, 0, 255), -1)
                ge = ge + 1

    # 再次在画了线的ROI中寻找正方形
    # 其目的是为了精确的只找到四个正方体
    ROI_gray = cv2.cvtColor(ROI, cv2.COLOR_BGR2GRAY)
    _, ROI_er = cv2.threshold(ROI_gray, 150, 255, cv2.THRESH_BINARY_INV)

    # 寻找到了轮廓
    contours, a = cv2.findContours(ROI_er, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    ge = 0
    curve = []
    for con in contours:
        # 顶点有四个的符合条件
        area = cv2.contourArea(con)
        # 判断如果面积大于200，那么符合条件
        if area > AREA_ROI:
            approxCurve = cv2.approxPolyDP(con, 5, True)
            if len(approxCurve) == 4:

                if -long < abs((approxCurve[0][0][0] - approxCurve[1][0][0])) - abs(
                        (approxCurve[2][0][0] - approxCurve[3][0][0])) < long and -long < abs(
                    (approxCurve[0][0][1] - approxCurve[1][0][1])) - abs(
                    (approxCurve[2][0][1] - approxCurve[3][0][1])) < long:
                    result = cv2.drawContours(ROI, [approxCurve], -1, (0, 0, 255), -1)
                    ge = ge + 1
                    print(approxCurve)
                    curve.append(approxCurve)


    print(f"符合条件的有{ge}张图片")

    """
    curve：现在是获取到了四个正方体
    一个一个匹配
    """
    ge = 0
    centers = []

    for square in curve:
        x, y, w, h = cv2.boundingRect(square)
        center_x = x + w / 2
        center_y = y + h / 2
        ge += 1
        centers.append((center_x, center_y))
    print(f"aaa{ge}")
    # 确定相对位置
    # 按照 y 坐标排序，得到上下关系
    centers.sort(key=lambda point: point[1])
    if len(centers) == 4:
        # 分成两组，每组两个正方形
        top_two = centers[:2]
        bottom_two = centers[2:]

        # 按照 x 坐标排序，得到左右关系
        top_two.sort(key=lambda point: point[0])
        bottom_two.sort(key=lambda point: point[0])

        # 输出相对位置
        top_left, top_right = top_two
        bottom_left, bottom_right = bottom_two

        print(f"左上: {top_left}, 右上: {top_right}")
        print(f"左下: {bottom_left}, 右下: {bottom_right}")


        decoded_obj = decode(ROI)

        reference_points = [top_left, top_right, bottom_left, bottom_right]

        for obj in decoded_obj:
            qr_data = obj.data.decode('utf-8')
            qr_rect = obj.rect
            x, y, width, height = qr_rect.left, qr_rect.top, qr_rect.width, qr_rect.height

            # 计算每个参考点与目标点的距离
            distances = []
            for point in reference_points:
                distance = math.sqrt((point[0] - x) ** 2 + (point[1] - y) ** 2)
                distances.append(distance)
            min_distance = min(distances)
            nearest_point_index = distances.index(min_distance)
            nearest_point = reference_points[nearest_point_index]
            print(f"最近的参考点是: {nearest_point}")
            res_dic[nearest_point + 1] = qr_data
    if res_dic[1]==0 and res_dic[2]==0 and res_dic[3]==0 and res_dic[4]==0:
        return 0
    else:
        return res_dic

def Get_text():
    # 寻找汉字返回这段话
    ROI_path = 'D:\develop\py_study\opencv_test\datasets\cache_video_img'
    ROI_dir = os.path.join(ROI_path, 'ROI.bmp')

    ROI = Image.open(ROI_dir)
    text = pytesseract.image_to_string(ROI, lang='chi_sim').strip()
    print('aaa')
    print(text)
    print('bbb')
    if "空闲中" in text:
        print('0')
    elif "无空闲" in text:
        print('1')
    else:
        print('2')
    if text is None:
        return 0
    else:
        return text
