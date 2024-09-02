#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>

#include "Word_analyse.h"
#include "Grammar_analyse.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void on_Help_file_triggered();
    void on_Open_text_triggered();
    void on_Open_file_triggered();

    void on_Word_error_triggered();
    void on_Word_ID_triggered();
    void on_Word_Sign_triggered();
    void on_Word_all_triggered();

    void Word_choice(string choice);


    void Grammar_choice(string choice);

    void on_Grammar_fourline_triggered();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
