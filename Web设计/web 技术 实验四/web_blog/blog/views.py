from django.shortcuts import render,redirect
from blog import models
from django.contrib.auth.models import User
from django.contrib.auth import authenticate,login,logout
from django.core.paginator import Paginator,EmptyPage,PageNotAnInteger
from django.db.models import Q

from web_blog import settings

from datetime import date
import os

my_one = 'italas'
my_two = 'admin'

# Create your views here.

def register(request):
    if request.method == 'POST':
        username = request.POST.get("username")
        password = request.POST.get("password")
        password2 = request.POST.get("password2")
        email = request.POST.get("email")
        if password != password2:
            tip='两次输入的密码不一样！'
            return render(request,'register.html',locals())
        elif username == '':
            tip='用户名不能为空'
            return render(request,'register.html',locals())
        elif email == '':
            tip='用户邮箱不能为空'
            return render(request,'register.html',locals())
        cuser = User.objects.create_user(username=username, password=password, email=email)
        cuser.save()
        return redirect('/login/')
    return render(request,'register.html')


def logins(request):
    if request.method == 'POST':
        username = request.POST.get("username")
        password = request.POST.get("password")
        user = authenticate(username = username, password = password)
        if user:
            login(request,user)
            return redirect('/')
        else:
            tip='用户名密码错误！'
            return render(request,'login.html',locals())
    return render(request,'login.html')

def log_out(request):
    logout(request)
    return redirect('/')


def index(request):
    people = models.friend.objects.filter(username_id=request.user.id)
    my_blogs = models.V_blog.objects.filter(Q(blog_creator=request.user.username) | Q(blog_creator=my_one) | Q(blog_creator=my_two))
    lists = Paginator(my_blogs, 8)
    pages = request.GET.get('page')
    try:
        v_blogs = lists.page(pages)
    except EmptyPage:
        v_blogs = lists.page(lists.num_pages)
    except PageNotAnInteger:
        v_blogs = lists.page(1)
    return render(request,'index.html',locals())


def indexs(request, id):
    people = models.friend.objects.filter(username_id=request.user.id)
    my_blogs = models.V_blog.objects.filter(blog_creator=id)
    lists = Paginator(my_blogs, 8)
    pages = request.GET.get('page')
    try:
        v_blogs = lists.page(pages)
    except EmptyPage:
        v_blogs = lists.page(lists.num_pages)
    except PageNotAnInteger:
        v_blogs = lists.page(1)
    return render(request,'index.html',locals())


def blog_comment(request, id):
    if request.method == 'POST':
        name = request.POST.get('username')  
        com  = request.POST.get('coment')
        # 获取网页提交的数据
        if com == "":
            tip='留言不能为空！'
        else:
            A_blog = models.V_blog.objects.get(id = id)
            A_comment = models.comment(comment_creator=name,comment_content=com,comment_blog_id=id,comment_time=date.today())
            A_comment.save()
            # 创建comm对象并装入数据保存

        A_blog = models.V_blog.objects.get(id = id)
        my_comments = models.comment.objects.filter(comment_blog_id = id)
        lists = Paginator(my_comments, 4)
        pages = request.GET.get('page')
        try:
            v_comments = lists.page(pages)
        except EmptyPage:
            v_comments = lists.page(lists.num_pages)
        except PageNotAnInteger:
            v_comments = lists.page(1)

        return render(request, 'blog_comment.html', locals())
    A_blog = models.V_blog.objects.get(id = id)
    my_comments = models.comment.objects.filter(comment_blog_id = id)
    lists = Paginator(my_comments, 4)
    pages = request.GET.get('page')
    try:
        v_comments = lists.page(pages)
    except EmptyPage:
        v_comments = lists.page(lists.num_pages)
    except PageNotAnInteger:
        v_comments = lists.page(1)
    return render(request, 'blog_comment.html', locals())

    
def blog(request):
    if request.method == 'POST':
        username = request.user.username
        blog_content = request.POST.get("coment")
        blog_time = date.today()
        blog_img = request.FILES.get("pic")
        blog_title = request.POST.get("title")

        if blog_title == '':
            tip='博客标题不能为空！'
            return render(request,'blog.html',locals())
        elif blog_img == '':
            tip='未上传图片！'
            return render(request,'blog.html',locals())
        elif blog_content == '':
            tip='博客未添加内容！'
            tip=blog_img
            return render(request,'blog.html',locals())
        

        new_blog = models.V_blog(blog_creator=username, blog_content=blog_content, title=blog_title, blog_time=blog_time, blog_img=blog_img)
        new_blog.save()

        return redirect('/blog/')
    return render(request,'blog.html')


def friend(request):
    if request.method == 'POST':
        username = request.user.username
        fri = request.POST.get("coment")
        
        users = models.User.objects.get(username = username)
        find_user = models.User.objects.filter(username = fri)
        find_frined = models.friend.objects.filter(friendname = fri, username_id=users.id)

        if fri == '':
            tip='朋友姓名不能为空！'
            return render(request,'friend.html',locals())
        elif fri == username:
            tip='不能添加你自己！'
            return render(request,'friend.html',locals())
        elif len(find_user) == 0:
            tip='这个账户暂时未注册！'
            return render(request,'friend.html',locals())
        elif len(find_frined) != 0:
            tip='这个账户已经添加过了！'
            return render(request,'friend.html',locals())
        

        new_friend = models.friend(friendname=fri, username_id=users.id)
        new_friend.save()
        return redirect('/friend/')
    return render(request,'friend.html')


