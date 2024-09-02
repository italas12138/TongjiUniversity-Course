#include "StartWindow.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>

StartWindow::StartWindow()
{
    this->setFixedSize(1280,800);

    this->setWindowTitle("C-Compiler开始界面");

    QLabel *label1=new QLabel;
    label1->setParent(this);
    QFont font1;
    font1.setFamily("宋体");
    font1.setPointSize(30);
    QString str1=QString("编译原理课程设计——程序展示");
    label1->setFont(font1);
    label1->setText(str1);

    label1->setGeometry((this->width()-30*str1.size())/3,200,this->width(),50);

    QString str2=QString("2152118 史君宝");
    QLabel *label2=new QLabel;
    label2->setParent(this);
    QFont font2;
    font2.setFamily("宋体");
    font2.setPointSize(26);
    label2->setFont(font2);
    label2->setText(str2);
    label2->setGeometry(800,450,this->width()-800,50);


    QPushButton * startBtn=new QPushButton ("");
    startBtn->setParent(this);
    startBtn->setFixedSize(200,200);

    QString str5=QString("START");
    QFont font5;
    font5.setFamily("宋体");
    font5.setPointSize(30);
    startBtn->move(this->width()*0.5-100,this->height()-200);
    startBtn->setText(str5);
    startBtn->setFont(font5);


    connect(startBtn,&QPushButton::clicked,[=](){
        mainwindow = new MainWindow();
        mainwindow->setGeometry(this->geometry());
        this->hide();
        mainwindow->show();
    });
}

void StartWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load("img/start_background.jpg");

    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
