#本python文件的作用是，导入mnist数据集，并将其分为训练集、验证集和测试集
#然后依次处理三位同学的数据

import os
import numpy as np
from PIL import Image
import tensorflow as tf
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt


def data_process():

    print('导入MNIST官方数据集......................')
    # 获取脚本文件的目录
    script_dir = os.path.dirname(os.path.realpath(__file__))
    # 将工作目录切换到脚本文件的目录
    os.chdir(script_dir)

    # 加载MNIST数据集
    mnist = tf.keras.datasets.mnist
    (train_images, train_labels), (test_images, test_labels) = mnist.load_data()

    train_images = train_images/255.0
    test_images = test_images/255.0

    train_images, val_images, train_labels, val_labels = train_test_split(train_images, train_labels, test_size=0.2, random_state=42)

    # 打印数据集的形状
    print("训练集图像形状:", train_images.shape)
    print("训练集标签形状:", train_labels.shape)
    print("验证集图像形状:", val_images.shape)
    print("验证集标签形状:", val_labels.shape)
    print("测试集图像形状:", test_images.shape)
    print("测试集标签形状:", test_labels.shape)
    print()



    print('导入自己的数据集......................')
    # 处理学生一的数据
    in_folder = "data/data_student_one/"
    student_data = [[] for _ in range(10)]

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
            student_data[digit].append(image_array)


    # 处理学生三的数据
    in_folder = "data/data_student_three/"
    number = [2, 3, 5, 7]

    # 遍历0-9的文件夹
    for digit in number:
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
            student_data[digit].append(image_array)



    student_data = [np.array(queue) for queue in student_data]
    student_label = [[] for _ in range(10)]

    #展示处理的同学的数据
    for i in range(10):
        for j in range(student_data[i].shape[0]):
            student_label[i].append(i)
        student_label[i] = np.array(student_label[i])


    for i in range(10):
        m = len(student_data[i])
        n = int(m*0.7)
        p = int(m*0.2)
        q = m-n-p
        array_data = student_data[i][:n]
        array_label = student_label[i][:n]
        train_images = np.concatenate((train_images, array_data))
        train_labels = np.concatenate((train_labels, array_label))


        array_data = student_data[i][n:n+p]
        array_label = student_label[i][n:n+p]
        val_images = np.concatenate((val_images, array_data))
        val_labels = np.concatenate((val_labels, array_label))

        array_data = student_data[i][n+p:]
        array_label = student_label[i][n+p:]
        test_images = np.concatenate((test_images, array_data))
        test_labels = np.concatenate((test_labels, array_label))


    random_state = 42
    np.random.seed(random_state)

    # 生成打乱的索引
    shuffle_index_1 = np.random.permutation(len(train_images))
    # 根据打乱的索引重新排序数据和标签
    train_images = train_images[shuffle_index_1]
    train_labels = train_labels[shuffle_index_1]


    # 生成打乱的索引
    shuffle_index_2 = np.random.permutation(len(val_images))
    # 根据打乱的索引重新排序数据和标签
    val_images = val_images[shuffle_index_2]
    val_labels = val_labels[shuffle_index_2]


    # 生成打乱的索引
    shuffle_index_3 = np.random.permutation(len(test_images))
    # 根据打乱的索引重新排序数据和标签
    test_images = test_images[shuffle_index_3]
    test_labels = test_labels[shuffle_index_3]


    # 打印数据集的形状
    print("训练集图像形状:", train_images.shape)
    print("训练集标签形状:", train_labels.shape)
    print("验证集图像形状:", val_images.shape)
    print("验证集标签形状:", val_labels.shape)
    print("测试集图像形状:", test_images.shape)
    print("测试集标签形状:", test_labels.shape)
    print()

    return train_images, train_labels, val_images, val_labels, test_images, test_labels





