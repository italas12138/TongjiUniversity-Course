from django.db import models
from django.contrib.auth.models import User
# Create your models here.

class V_blog(models.Model):
    title = models.CharField("标题", max_length = 200)
    blog_img = models.FileField("图片", upload_to="media/img")
    blog_content = models.TextField("博客内容")
    blog_time = models.DateField("创建日期")
    blog_creator = models.CharField("创建者", max_length = 100)
    def __str__(self):
        return self.title
    

class comment(models.Model):
    comment_creator = models.CharField("评论者", max_length = 100)
    comment_content = models.TextField("留言")
    comment_time = models.DateField("评论日期")

    comment_blog = models.ForeignKey(V_blog, on_delete = models.CASCADE, related_name='comen')

    def __str__(self):
        return self.comment_creator

class friend(models.Model):
    friendname = models.CharField("朋友", max_length = 100)

    username = models.ForeignKey(User, on_delete = models.CASCADE, related_name="friend_rel")


