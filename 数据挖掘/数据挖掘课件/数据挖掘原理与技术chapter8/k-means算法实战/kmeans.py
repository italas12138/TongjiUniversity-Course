# -*- coding: utf-8 -*-

from sklearn import datasets
from sklearn.cluster import KMeans

import matplotlib.pyplot as plt
import matplotlib as mpl
# 设置属性防止中文乱码
mpl.rcParams['font.sans-serif'] = [u'SimHei']
mpl.rcParams['axes.unicode_minus'] = False


# 加载鸢尾花数据集
iris = datasets.load_iris()
# 使用花萼长度和宽度两个特征，便于可视化
X = iris.data[:, :2]

# 设置K=3开始聚类
km = KMeans(n_clusters=3, random_state=42)
km.fit(X)
centers = km.cluster_centers_   # 取得各簇的中心点
print('聚类结束！聚类中心为：\n', centers)

# 可视化
fig, axes = plt.subplots(1, 2, figsize=(16, 8))
y = iris.target
scatter = axes[0].scatter(X[:, 0], X[:, 1], c=y, cmap='gist_rainbow', edgecolor='k', s=150)
axes[0].legend(handles=scatter.legend_elements()[0], labels=['山鸢尾', '变色鸢尾', '维吉尼亚鸢尾'],
               fontsize=18)
axes[0].set_xlabel('花萼长度/cm', fontsize=18)
axes[0].set_ylabel('花萼宽度/cm', fontsize=18)
axes[0].set_title('鸢尾花数据实际分布', fontsize=18)


axes[1].scatter(X[:, 0], X[:, 1], c=km.labels_, cmap='jet', edgecolor='k', s=150)
axes[1].scatter(centers[:, 0], centers[:, 1], color='black', marker='*', edgecolor='k', s=200,
                label='中心点')
axes[1].set_xlabel('花萼长度/cm', fontsize=18)
axes[1].set_ylabel('花萼宽度/cm', fontsize=18)
axes[1].set_title('K-Means聚类结果', fontsize=18)
axes[1].legend(fontsize=18)
plt.show()
