from django.contrib import admin

# Register your models here.

from blog.models import V_blog, comment, friend
# Register your models here.  

class V_log(admin.ModelAdmin):
    list_display = ["title", "blog_creator", "blog_time"]

class commen(admin.ModelAdmin):
    list_display = ["comment_creator", "comment_time"]

class frien(admin.ModelAdmin):
    list_display = ["friendname"]



admin.site.register(V_blog, V_log)
admin.site.register(comment,commen)
admin.site.register(friend, frien)