#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#define BackgroundPath ":/data/background.jpg"
#define FilePath ":/data/help.html"


#include "Parser.h"
#include <QMainWindow>
#include <string>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Text_in(string choice);



public slots:
    void on_use_file_triggered();

    void on_Getin_grammar_triggered();

    void on_Getin_program_triggered();


    void on_Parser_triggered();
    void on_Parser_DFA_triggered();
    void on_Parser_Tree_triggered();

private:
    Ui::MainWindow *ui;
    Parser_program LR1;

};
#endif // MAINWINDOW_H

