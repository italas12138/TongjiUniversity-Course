from PIL import Image, ImageDraw, ImageFont
import random

def showresult(predict_num):
    random.shuffle(predict_num)


    for i in predict_num:
        # 打开图片
        image_path = i[0]
        image = Image.open(image_path)

        # 在图片上绘制文本
        draw = ImageDraw.Draw(image)
        text = str(i[1])
        x = 0  # 标注文本的横坐标位置
        y = 0  # 标注文本的纵坐标位置
        draw.text((x, y), text, fill=255)  # 使用默认字体和红色填充文本

        # 显示图片
        image.show()
