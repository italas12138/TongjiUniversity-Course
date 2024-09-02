#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
class StartWindow : public QMainWindow
{
    Q_OBJECT

public:

     StartWindow();

     void paintEvent(QPaintEvent *);

     MainWindow * mainwindow = NULL;


signals:

};

#endif // STARTWINDOW_H
