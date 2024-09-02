# 导入相关的包
import numpy as np
import os
from PIL import Image
import tensorflow as tf
#import itertools
from collections import Counter
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix

# 使用CPU训练
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"

# 使用GPU训练
# os.environ["CUDA_VISIBLE_DEVICES"] = "0"
# physical_devices = tf.config.experimental.list_physical_devices('GPU')
# assert len(physical_devices) > 0, "Not enough GPU hardware devices available"
# tf.config.experimental.set_memory_growth(physical_devices[0], True)

# 步骤一：下载手写数字数据集，进行初步的数据可视化和统计
(x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()  # 下载数据集

# 可视化训练集的前9张图片
plt.figure(figsize=(9, 9))
for i in range(9):
    plt.subplot(331 + i)
    plt.imshow(x_train[i], cmap=plt.get_cmap('gray'))
plt.savefig('sample.png', bbox_inches='tight', dpi=300)

print(f'训练集的样本数：{x_train.shape[0]}，测试集的样本数：{x_test.shape[0]}')
print(f'输入图像的大小：{x_train.shape[1]}*{x_train.shape[2]}')
label_cnt = Counter(y_train)  # 统计
print('训练集的图像类别分布：', label_cnt)
plt.figure(figsize=(5, 5))
plt.pie(x=label_cnt.values(),
        labels=label_cnt.keys(),
        autopct='%.2f%%')
plt.savefig('label_distribution.png', bbox_inches='tight', dpi=300)

# 步骤二：数据预处理
num_class = len(label_cnt)

x_train = np.expand_dims(x_train, axis=-1)  # (28, 28)扩展为(28, 28, 1)
x_test = np.expand_dims(x_test, axis=-1)

# expand_dims
x_train = x_train.astype('float32')
x_test = x_test.astype('float32')
x_train /= 255  # 规范化，将像素值缩至0-1之间
x_test /= 255

y_train = tf.keras.utils.to_categorical(y_train, num_class)  # 将标签向量转化为one-hot形式的向量
y_test = tf.keras.utils.to_categorical(y_test, num_class)

# 步骤三：模型搭建
model = tf.keras.models.Sequential()  # 序列化模型
model.add(tf.keras.layers.Conv2D(32, 3, input_shape=(28, 28, 1)))
model.add(tf.keras.layers.Activation('relu'))  # relu 非线性激活函数
model.add(tf.keras.layers.MaxPooling2D(pool_size=2))
model.add(tf.keras.layers.Dropout(0.2))  # Dropout 将部分节点的激活值置为0防止过拟合
model.add(tf.keras.layers.Conv2D(64, 3, ))
model.add(tf.keras.layers.Activation('relu'))
model.add(tf.keras.layers.MaxPooling2D(pool_size=2))
model.add(tf.keras.layers.Dropout(0.1))
model.add(tf.keras.layers.Flatten())
model.add(tf.keras.layers.Dense(64, activation='relu'))
model.add(tf.keras.layers.Dense(num_class))  # 由一个全连接层，10维输出，看作是每一类的概率值
model.add(tf.keras.layers.Activation('softmax'))  # softmax 激活函数用于分类，对概率进行归一化

model.summary()  # 查看模型架构
model.compile(loss='categorical_crossentropy',
              optimizer=tf.keras.optimizers.Adam(),
              metrics=['accuracy'])  # 定义模型训练细节，包括交叉熵损失函数，Adam优化器和准确率评价指标

# 步骤四：训练模型
h = model.fit(x_train, y_train, batch_size=128, epochs=5, validation_data=(x_test, y_test))
model.save("cnn_model.h5")

# 步骤五：评估模型
reconstructed_model = tf.keras.models.load_model("cnn_model.h5")
test_loss, test_acc = reconstructed_model.evaluate(x_test, y_test, verbose=0)
print(f'测试集损失值: {test_loss}, 测试集准确率: {test_acc}')

y_pred = reconstructed_model.predict(x_test)
y_pred = np.argmax(y_pred, axis=1)
y_true = np.argmax(y_test, axis=1)
cm = confusion_matrix(y_true, y_pred)
print(cm)

print(h.history.keys())
accuracy = h.history['accuracy']
val_accuracy = h.history['val_accuracy']
loss = h.history['loss']
val_loss = h.history['val_loss']
epochs = range(len(accuracy))

plt.figure()
plt.plot(epochs, accuracy, 'b', label='Training accuracy')
plt.plot(epochs, val_accuracy, 'bo', label='Validation accuracy')
plt.title('Training and validation accuracy')
plt.legend()
plt.savefig('accuracy.png', bbox_inches='tight', dpi=300)

plt.figure()
plt.plot(epochs, loss, 'b', label='Training loss')
plt.plot(epochs, val_loss, 'bo', label='Validation loss')
plt.title('Training and validation loss')
plt.legend()
plt.savefig('loss.png', bbox_inches='tight', dpi=300)

# 步骤七：实际图片预测
file_path = "./cnn_pic"
pic_name_list = os.listdir(file_path)
pic_list = list()
for pic in pic_name_list:
    # 以黑白方式读取图片
    img = Image.open(os.path.join(file_path, pic)).convert('L')  # 转化为黑白图片
    img = img.resize((28, 28), Image.ANTIALIAS)
    pic_list.append(img)
    #在jupyter下运行时可显示每张28x28的图片
    #plt.figure()
    #plt.imshow(np.asarray(img))

for pic in pic_list:
    img = np.reshape(pic, (28, 28, 1)) / 255.
    x = np.array([1 - img])
    # API refer: https://keras.io/models/model/
    y = model.predict(x)
    print(y[0])
    print("----------> Predict digit", np.argmax(y[0]))
    print("=================")
