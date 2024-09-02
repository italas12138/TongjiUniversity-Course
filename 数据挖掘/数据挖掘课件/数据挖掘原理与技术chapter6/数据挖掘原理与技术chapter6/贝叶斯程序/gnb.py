# -*- coding: utf-8 -*-

from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib as mpl
# 设置属性防止中文乱码
mpl.rcParams['font.sans-serif'] = [u'SimHei']
mpl.rcParams['axes.unicode_minus'] = False


# 加载鸢尾花数据集
iris = load_iris()
# 只使用花瓣长度和宽度作为特征（便于可视化)
x, y = iris.data[:, 2:4], iris.target
# 按照标签的比例分成训练集和测试集
x_train, x_test, y_train, y_test = train_test_split(x, y, stratify=y, test_size=0.2, random_state=42)

# 使用训练集训练一个贝叶斯分类器
gnb = GaussianNB()
gnb.fit(x_train, y_train)

# 计算预测值并计算准确率
y_train_pred = gnb.predict(x_train)
train_acc = accuracy_score(y_train, y_train_pred)
print(f'训练集准确率: {train_acc:.4f}')

y_test_pred = gnb.predict(x_test)
test_acc = accuracy_score(y_test, y_test_pred)
print(f'测试集准确率: {test_acc:.4f}')


# 将分类器绘制到图中
x1_min, x1_max = min(x[:, 0]) - 1.0, max(x[:, 0]) + 1.0
x2_min, x2_max = min(x[:, 1]) - 1.0, max(x[:, 1]) + 1.0
step_size = 0.01
x1_values, x2_values = np.meshgrid(np.arange(x1_min, x1_max, step_size),
                                   np.arange(x2_min, x2_max, step_size))

cm_light = mpl.colors.ListedColormap(['#A0FFA0', '#FFA0A0', '#A0A0FF'])
cm_dark = mpl.colors.ListedColormap(['g', 'r', 'b'])
mesh_output = gnb.predict(np.c_[x1_values.ravel(), x2_values.ravel()])
mesh_output = mesh_output.reshape(x1_values.shape)
plt.figure()
plt.pcolormesh(x1_values, x2_values, mesh_output, cmap=cm_light)

plt.scatter(x[:, 0], x[:, 1], c=y, s=80, edgecolors='black', linewidth=1, cmap=cm_dark)
plt.legend(handles=[mpatches.Patch(color='g', label='山鸢尾'),
                    mpatches.Patch(color='r', label='变色鸢尾'),
                    mpatches.Patch(color='b', label='维吉尼亚鸢尾')])
plt.xlim(x1_values.min(), x1_values.max())
plt.ylim(x2_values.min(), x2_values.max())
plt.xticks((np.arange(int(x1_min), int(x1_max)+1, 1.0)))
plt.yticks((np.arange(int(x2_min), int(x2_max)+1, 1.0)))
plt.xlabel('花瓣长度/cm')
plt.ylabel('花瓣宽度/cm')
plt.title('朴素贝叶斯鸢尾花分类器')
plt.show()
