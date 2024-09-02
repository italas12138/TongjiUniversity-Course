import data
import predict
import show_result

from sklearn.model_selection import GridSearchCV

from sklearn import svm
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import cross_val_score
import numpy as np
import torch
from torch.utils.data import DataLoader
from torchvision import transforms, datasets
import os

from torch.utils.data import TensorDataset

if __name__ == '__main__':
    # 数据预处理过程
    train_images, train_labels, val_images, val_labels, test_images, test_labels = data.data_process()

    # 训练过程
    bs = 64
    lr = 0.01
    epoches = 20

    checkpoints = 'train_model'
    os.makedirs(checkpoints, exist_ok=True)

    transform = transforms.Compose([
        transforms.ToTensor()
    ])


    # 定义参数网格
    param_grid = {
        'C': [0.1, 1, 10],
        'kernel': ['linear', 'rbf', 'poly'],
        'gamma': ['scale', 'auto']
    }

    # 创建SVM模型
    model = svm.SVC()

    # 使用GridSearchCV进行参数调优和交叉验证
    grid_search = GridSearchCV(estimator=model, param_grid=param_grid, scoring='accuracy', cv=5)

    with open('log.txt', 'a') as file:
        print("开始执行GridSearchCV...")
        print("参数网格大小:", len(param_grid['C']) * len(param_grid['kernel']) * len(param_grid['gamma']))

        file.write("\n\n原数据集8分之一\n")
        file.write("\n开始执行GridSearchCV...\n")
        file.write("\n参数网格大小:")
        file.write(str(len(param_grid['C']) * len(param_grid['kernel']) * len(param_grid['gamma'])))

        train_images = train_images[:len(train_images)//8]
        train_labels = train_labels[:len(train_labels)//8]


        grid_search.fit(train_images.reshape(train_images.shape[0], -1), train_labels)

        # 输出最佳参数和最佳准确率
        print("最佳参数: ", grid_search.best_params_)
        print("最佳准确率: ", grid_search.best_score_)

        file.write("\n最佳参数: ")
        file.write(str(grid_search.best_params_))
        file.write("\n最佳准确率: ")
        file.write(str(grid_search.best_score_))

        # 获取最佳模型
        best_model = grid_search.best_estimator_

        # 保存最佳模型
        torch.save(best_model, os.path.join(checkpoints, 'best_model_8.pth'))

        # 使用最佳模型进行测试数据的预测
        test_predictions = best_model.predict(test_images.reshape(test_images.shape[0], -1))

        # 计算混淆矩阵
        cm = confusion_matrix(test_labels, test_predictions)
        print("混淆矩阵:\n", cm)

        file.write("\n混淆矩阵:\n")
        file.write(str(cm))
        

        # 使用5折交叉验证评估模型性能
        scores = cross_val_score(best_model, train_images.reshape(train_images.shape[0], -1), train_labels, cv=5)
        print("交叉验证准确率: ", np.mean(scores))

        file.write("\n交叉验证准确率: ")
        file.write(str(np.mean(scores)))

    predict_num = predict.predict(test_images, test_labels, best_model)

    show_result.showresult(predict_num)
