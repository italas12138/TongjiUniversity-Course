import torch
import os
from PIL import Image
import numpy as np

def predict(test_images, test_labels, model):

    # 处理学生二的数据
    in_folder = "data/data_student_two/"

    predict_data = [[] for _ in range(10)]

    # 遍历0-9的文件夹
    for digit in range(10):
        digit_folder = os.path.join(in_folder, str(digit))

        # 遍历文件夹中的文件
        for filename in os.listdir(digit_folder):
            # 构建完整的文件路径
            file_path = os.path.join(digit_folder, filename)

            # 打开图像
            image = Image.open(file_path)
            image = image.resize((28, 28))
            image = image.convert('L')
            image_array = np.array(image)
            image_array = image_array / 255.0

            # 将图像和标签添加到列表中
            predict_data[digit].append(image_array)

    predict_data = [np.array(queue) for queue in predict_data]
    predict_label = [[] for _ in range(10)]

        #展示处理的同学的数据
    for i in range(10):
        for j in range(predict_data[i].shape[0]):
            predict_label[i].append(i)
        predict_label[i] = np.array(predict_label[i])
    
    test_image = []
    test_label = []

    for i in range(10):
        for j in predict_data[i]:
            test_image.append(j)
    
    for i in range(10):
        for j in predict_label[i]:
            test_label.append(j)

    path = 'data\\data_student_two'
    result_number = []
    for i in range(10):
        result_number.append(('{}\\{}\\image_{}.jpg'.format(path, i, i), i))

    return result_number