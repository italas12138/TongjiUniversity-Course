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

    void result_choice();
    void on_Compiler_result_triggered();
    void on_Code_result_triggered();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
