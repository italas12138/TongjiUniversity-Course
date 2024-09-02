"""
URL configuration for web_blog project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path

from blog.views import index, logins, register, log_out, blog_comment, blog, friend, indexs

from web_blog import settings
from django.conf.urls.static import static

urlpatterns = [
    path("admin/", admin.site.urls),
    path("", index),
    path("login/", logins),
    path("register/", register),
    path("logout/", log_out),
    path("blog/", blog),
    path("blog_comment/<int:id>", blog_comment),
    path("friend/", friend),
    path("<str:id>", indexs),
]
urlpatterns += static (settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)