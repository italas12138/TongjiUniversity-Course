from distutils.log import log
from flask import Flask,render_template,request,flash,session,redirect,url_for
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import and_, or_
from app import app
from werkzeug.utils import secure_filename
import json
import re
import os
import time

print(os.path.dirname(__file__))
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite://'
app.config['UPLOAD_FOLDER']=os.path.dirname(__file__)+'\\static\\upload\\'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True
app.secret_key = '\xc9ixnRb\xe40\xd4\xa5\x7f\x03\xd0y6\x01\x1f\x96\xeao+\x8a\x9f\xe4'

db = SQLAlchemy(app)

'''
数据库定义部分
'''
# 定义用户表
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True)
    password = db.Column(db.String(80))
    email = db.Column(db.String(120), unique=True)
    pic= db.Column(db.String(120))
    def __repr__(self):
        return '<User %r>' % self.username

# 博客表
class Blogs(db.Model):
    title = db.Column(db.String(80))
    b_id = db.Column(db.Integer, primary_key=True)
    flag = db.Column(db.String(80),)
    img=db.Column(db.String(80), unique=True)
    count = db.Column(db.Integer)
    isgive= db.Column(db.Boolean)
    content = db.Column(db.String(500), unique=True)
    
# 点赞表
class Gives(db.Model):
    g_id = db.Column(db.Integer, primary_key=True)
    b_id=db.Column(db.Integer)
    id=db.Column(db.Integer)

#日记表
class Note(db.Model):
    n_id = db.Column(db.Integer, primary_key=True)
    id=db.Column(db.Integer)
    content=db.Column(db.String(300))

# 创建表格、插入数据
@app.before_first_request
def create_db():
    db.drop_all()  # 每次运行，先删除再创建
    db.create_all()

    admin = User(username='admin', password='root',pic='upload/16398470711.jpg' , email='admin@example.com')
    session['id']=None
    note=Note(id=1,content='^_^！')
    db.session.add(admin)
    db.session.add(note)
    s=[ Blogs(flag='image/1.png', img='image/scene1.jpg',  count=1, title="仰  望  星  空", content='你有多久不曾抬头仰望浩瀚的星空？你有多久不曾驻足领略蔚蓝的苍穹？三楼合围，如同一只深邃的眼睛。也许你只是从它脚下匆匆走过，它却帮你仰望春天第一只早归的燕子，帮你尽数夏夜的每一颗明星。它像一扇窗，通过一束光芒，洒下一片宁静。'),
        Blogs(flag='image/2.png', img='image/scene2.jpeg', count=1, title="衷    和    楼", content='从平原上拔地而起，矗立东南一方。它的高大使得下方经过的行人不得不抬头瞻仰，但枝头的花依然热烈地开着，树梢的嫩叶也在蓬勃地吐着新绿。它的伟岸好似承载着对等的责任，“衷和”取自和衷共济，寄寓着同心协力，共克时艰，如一座图腾，为这片土地上提供深厚的精神力量。'),
        Blogs(flag='image/2.png', img='image/scene3.jpg',  count=1, title="国    立    柱", content='前身本出自苏州明末清初的石木牌楼，其木结构化身为叹为观止的古建筑模型，而石柱被就地掩埋。近五十载后重见天日，依然有往日之风采。柱身精美，材美工巧，遂重新装顶，立于显要，上附碑铭，以示后人。如一根顶天立地的脊梁，镌刻身后的历史，记录不凡的往昔。'),
        Blogs(flag='image/2.png', img='image/scene4.jpeg', count=1, title="樱  花  大  道", content='有多少纯洁的恋情等着有圆满的结局，就有多少等待着被发现的美好的心。 Cherryblossoom代表着高雅，质朴纯洁和清秀。宛如懵懂少女的情怀，安静地在春天开放，满树的白色粉色的樱花，是对情人诉说爱情的最美语言。如果你有一段纯洁如的恋情，不如就去樱花大道吧，在樱花纷纷降落的季节，在春天开始的时候告诉他你的钟情和热忱。'),
        Blogs(flag='image/2.png', img='image/scene5.jpg',  count=1, title="图    书    馆", content='有时，我特别愿意静静地站在图书馆阅览室的门口，看那些伏案读书者专注入迷的神情，享受着遨游书海的氛围。阳光从百叶窗中一丝一丝投射进来，照在摊开的书本上，让沉浸在书海的读者平添了几分宁静与雅致。'),

        Blogs(flag='image/2.png', img='image/food1.jpg', count=1, title="同 济 大 排 档", content='屋外吹着呼呼的寒风，屋内燃着暖暖的火炉，约上三五好友，去往西苑的雅座，点上一份火锅。火焰将刺骨的寒冷驱散，食物将一天的疲惫消解。欢声笑语随着蒸腾的热气飘上天花板，飘向窗外，直到寂静的夜晚只剩促膝的长谈。'), 
        Blogs(flag='image/2.png', img='image/food2.jpg', count=1, title="北  苑  轻  食", content='当琐碎的生活成为了常态，那追求简约就成了一种时尚。清淡的饮食，很容易将人们的节奏减慢，将人们的脚步放缓，体会时间的慢慢流逝，感受岁月的涓涓细流。所谓生活，一半烟火，一半清欢。我们不可能永远在生活在浪潮里，感受时代的风浪，可以偶尔独坐屋檐下，静听点点的雨声。'),
        Blogs(flag='image/2.png', img='image/food3.jpg', count=1, title="同 济 那 碗 面", content='也许在毕业多年之后，校园的道路，草木，建筑都渐渐在记忆中淡了，化作海滩上的一朵浪花，最终随泡沫消逝。但希望你还能记得，同济那碗面的味道，还记得它在秋天送来的那些温暖。一碗清汤浸润的手工面，淋上一勺汁浓芡亮的浇头。假若带上好友一道，简直是秋天最幸福的事了。'),
        Blogs(flag='image/2.png', img='image/food4.png', count=1, title="同  济  月  饼", content='每年中秋团圆之日，同济月饼总会准时上新，五彩的颜色，丰富的口味，精美的花纹，用心的制作。无论是自己品尝，还是赠送亲朋好友，都极为亮眼。共睹明月的两颗心将交汇在一起，远隔千里的两个人将在此刻共鸣。一枚小小的月饼，从零开始到你我手中，需要经历怎样的故事？师傅们坚持创作，精心制作，一年四季，亦复如是，这些平凡的努力值得被看见。'),
        Blogs(flag='image/2.png', img='image/food5.png', count=1, title="校  内  小  吃", content='也许，是经过了连续几个小时的学习，抑或，是上完一节课匆匆赶往另一个教室的短暂课间，你想到自己辘辘的饥肠，走在树影斑驳的校道上。冻酸奶，咖啡，面包算是不二之选，全家和罗森也不甘示弱。让路过的同学先平息一下滚烫的思绪，稍微放下停滞的思维。帮助同学们在短暂的靠岸后，能够继续扬帆远航。')
        ]

    guestes = [User(username='guest1', password='guest1', email='guest1@example.com'),
               User(username='guest2', password='guest2', email='guest2@example.com'),
               User(username='guest3', password='guest3', email='guest3@example.com'),
               User(username='guest4', password='guest4', email='guest4@example.com'),  
               ]
    db.session.add_all(s)
    db.session.add_all(guestes)
    db.session.commit()

'''
辅助函数、装饰器
'''
# 登录检验（用户名、密码验证）
def valid_login(username, password):
    user = User.query.filter(and_(User.username == username, User.password == password)).first()
    if user:
        session['id'] = user.id
        return True
    else:
        return False


# 注册检验（用户名、邮箱验证）
def valid_regist(username, email):
    user = User.query.filter(or_(User.username == username, User.email == email)).first()
    if user:  # 若已经存在重复的用户名或邮箱
        return False
    else:
        return True


@app.route('/')
def home():
    return redirect('/index')
# 首页
@app.route('/index',methods=['GET','POST'])
def index():
    if request.method=='GET':
        note=[]
        user={
            'pic':'',
            'username':'',
            'islogin':False
        }
        if session['id']:
            user=User.query.filter(User.id == session['id']).first()
            note=Note.query.filter(Note.id == session['id']).all()
            flag= True if len(note)>=1 else False
            user.islogin=True

            print('通过')
            return render_template('index.html', user=user,note=note ,flag=flag)
        else :
            return render_template('index.html', user=user,note=note,flag=False)


# 登录页
@app.route('/login',methods=['GET','POST'])
def login():
    if request.method == 'POST':
        name = request.form.get('username')
        psw = request.form.get('password')
        if valid_login(name, psw):
            print(session['id'])
            print('通过验证')
            return redirect(url_for('index'))
        else:
            flash("错误的用户名或密码！")
            return redirect('/login')
    else:
        return render_template('login.html')

    


# 注销
@app.route('/logout')
def logout():
    session['id']= None
    return redirect(url_for('index'))


# 注册
@app.route('/register',methods=['GET','POST'])
def register_form():
    # error = None
    if request.method == 'POST':
        if valid_regist(request.form['username'], request.form['email']):

            print(request.form['username'])

            print(request.form['password'])
            user = User(username=request.form['username'], password=request.form['password'],
                        email=request.form['email'],pic='upload/LOGO.png')
            db.session.add(user)
            db.session.commit()
            print('成功注册！')
            flash("成功注册！")
            return redirect('/login')
        else:
            flash("该用户名或邮箱已被注册！")

    return render_template('register.html')


# 博客
@app.route('/blogs',methods=['GET','POST'])
def blogs():
    blogs=Blogs.query.all()
    return render_template('gallary.html',blogs=blogs)
#博客详情
@app.route('/detail<id>',methods=['GET','POST'])
def detail(id):
    id=int(id)
    detail=Blogs.query.filter(or_(Blogs.b_id == id)).first()
    detail.count=Gives.query.filter(Gives.b_id == id).count()
    detail.isgive=queryisgive(session['id'], id)
    return render_template('detail.html',detail=detail)
#点赞
@app.route('/isgive',methods=['GET','POST'])
def isgive():
    b_id=request.args.get('b_id')
    count=int(request.args.get('count'))
    b_id=int(b_id)
    id=session['id']
    isgive=queryisgive(id,b_id)
    isgive=not isgive

    if isgive:
         db.session.add(Gives(b_id=b_id, id=id))
    else:
         stu =Gives.query.filter(Gives.b_id == b_id).filter(Gives.id == id).delete()  
         

    
    count=   count+1 if isgive else count-1
    Blogs.query.filter(Blogs.b_id == b_id).update({'count':count })
    Blogs.query.filter(Blogs.b_id == b_id).update({'isgive': isgive})
   
    db.session.commit()
    detail=Blogs.query.filter(or_(Blogs.b_id == b_id)).first()
    return render_template('detail.html',detail=detail)

#查询当前用户是否点赞
def queryisgive(id,b_id):
    give = Gives.query.filter(and_(Gives.id == id, Gives.b_id == b_id)).first()
    if give:
       
        return True
    else:
        return False
#上传图片
@app.route('/upload', methods=['POST'])
def upload():
    if request.method == 'POST':
      f = request.files['file']
      f.save(os.path.join(app.config['UPLOAD_FOLDER'],secure_filename(f.filename)))
      print(f.filename)
      User.query.filter(User.id ==session['id']).update({'pic':'upload/'+f.filename })
      db.session.commit()
      return redirect('/index')
#写日记
@app.route('/note', methods=['GET','POST'])
def note(): 
    if session['id']:
        if request.method == 'GET':
            return  render_template('note.html')
        else:
            content=request.form.get('content')
            if len(content) ==0 :
                return redirect('/note') 
            else:
                id=session['id']
                db.session.add(Note(id=id, content=content))
                print(content)
                db.session.commit()
                return redirect('/index') 
    else:
        flash("未登录")
        return redirect('/login') 




