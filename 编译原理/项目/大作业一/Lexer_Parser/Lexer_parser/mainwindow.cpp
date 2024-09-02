#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "unit_type.h"
#include "Lexer.h"
#include "Parser.h"

#include <fstream>
#include <QFile>

#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QDebug>

#include <QProcess>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("LR1_Parser");

    QFont font;
    //设置字体大小为 12
    font.setPointSize(12);
    ui->Grammar_Program_Textin->setFont(font);

    ui->Text_help->setStyleSheet("font-size: 20px;");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Text_in(string choice)
{
    if(choice == "Grammar")
    {
        QFile file("D:/Qt/LR1_Parser/file/grammar.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(nullptr, "警告", "无法打开语法文件");
            return;
        }
        QTextStream out(&file);
        out << this->ui->Grammar_Program_Textin->toPlainText();
        file.close();
    }
    else if(choice == "Program")
    {
        QFile file("D:/Qt/LR1_Parser/file/source.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(nullptr, "警告", "无法打开源程序文件");
            return;
        }
        QTextStream out(&file);
        out << this->ui->Grammar_Program_Textin->toPlainText();
        file.close();
    }

    return;

}

//使用文档按钮的点击
void MainWindow::on_use_file_triggered()
{
    ifstream fin;
    fin.open(R"(D:\Qt\LR1_Parser\file\use_file.txt)");

    if (fin.is_open()) {
        QString fileContent;
        string line;
        while (getline(fin, line)) {
            fileContent += QString::fromStdString(line) + "\n";
        }
        fin.close();

        // 将文件内容显示在 QTextBrowser 控件中
        this->ui->Text_help->setText(fileContent);
    }
    else {
        QMessageBox::warning(this, "Error", "未成功打开文件！");
    }
}


//导入语法产生式按钮的点击
void MainWindow::on_Getin_grammar_triggered()
{
    QString text = this->ui->Grammar_Program_Textin->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Grammar text is empty.");
    }
    else
    {
        this->Text_in("Grammar");
    }
}


//导入源程序按钮的点击
void MainWindow::on_Getin_program_triggered()
{
    QString text = this->ui->Grammar_Program_Textin->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Program text is empty.");
    }
    else
    {
        this->Text_in("Program");
    }

}


//语法分析按钮的点击
void MainWindow::on_Parser_triggered()
{


    //清空上一次语法分析的结果
    LR1.clear_data();

    this->ui->Text_help->clear();




    int a = LR1.Get_grammar_byfile(R"(D:\Qt\LR1_Parser\file\grammar.txt)");


    if(a == 0)
    {
        this->ui->Text_help->append("\n文法产生式文件未成功打开。");
    }
    else if(a == 1)
    {

        this->ui->Text_help->append("\n文法产生式文件成功打开。");

        //首先初始化first集合
        LR1.Init_First_list();

        //初始化Action_goto集合
        int b = LR1.initAction_Goto();

        if(b == 0)
        {
            this->ui->Text_help->append("\n文法并不是LR文法，请检查一下。");
        }
        else if(b == 1)
        {
            this->ui->Text_help->append("\nAction_goto表求解成功");
        }
    }



    string str = "";

    for (const auto& kv : LR1.Action_goto) {
        int key = kv.first;
        const auto& innerMap = kv.second;

        str += "{";
        str += std::to_string(key);
        for (const auto& innerKv : innerMap) {
            Unit_type unitType = innerKv.first;
            Movement movement = innerKv.second;

            str += "{";
            str += "Unit Type: ";
            str += GetStringFromUnitType(Terminal_to_Unit_type, unitType);
            str += ", Movement: ";
            if(movement.action == Action::accept);
                str += "accept";
            if(movement.action == Action::shift_in);
                str += "shift_in";
            if(movement.action == Action::reduction);
                str += "reduction";
            str += ",";
            str += std::to_string(movement.go);
            str += "}\n";
        }
        str += "}";
    }
        QString qstr = QString::fromStdString(str);
        this->ui->Text_help->append(qstr);

    word_unit err_word_unit;

    int b = LR1.parser(R"(D:\Qt\LR1_Parser\file\source.txt)", err_word_unit);

    if(b == 1)
    {
        this->ui->Text_help->append("\n语法分析成功。");
    }
    else if(b == 0)
    {
        QString err_str;

        err_str.append("\n语法分析错误！出错词法符号为：");
        err_str.append(QString::fromStdString(err_word_unit.unit_value));
        if(err_word_unit.unit_value == " ")
            qDebug()<<"A";
        err_str.append("\n出错位置：");
        err_str.append(QString::number(LR1.lexer_program.line));
        err_str.append("行，");
        err_str.append(QString::number(LR1.lexer_program.col));
        err_str.append("列。");
        this->ui->Text_help->append(err_str);
        return;
    }
}

void MainWindow::on_Parser_DFA_triggered()
{
    ofstream DFA_out;
    DFA_out.open(R"(D:/NASM/LeeCC-master/LR1_parser/graph_DFA.dot)");

    LR1.clear_data();

    this->ui->Text_help->clear();


    int a = LR1.Get_grammar_byfile(R"(D:\Qt\LR1_Parser\file\grammar.txt)");


    if(a == 0)
    {
        this->ui->Text_help->append("\n文法产生式文件未成功打开。");
    }
    else if(a == 1)
    {

        this->ui->Text_help->append("\n文法产生式文件成功打开。");

        //首先初始化first集合
        LR1.Init_First_list();

        //初始化Action_goto集合
        int b = LR1.initAction_Goto();

        if(b == 0)
        {
            this->ui->Text_help->append("\n文法并不是LR文法，请检查一下。");
        }
        else if(b == 1)
        {
            this->ui->Text_help->append("\nAction_goto表求解成功");
        }
    }
    LR1.Parser_DFA(DFA_out);

}

void MainWindow::on_Parser_Tree_triggered()
{
    ofstream Tree_out;
    Tree_out.open(R"(D:/NASM/LeeCC-master/LR1_parser/graph_Tree.dot)");

    LR1.clear_data();

    this->ui->Text_help->clear();


    int a = LR1.Get_grammar_byfile(R"(D:\Qt\LR1_Parser\file\grammar.txt)");


    if(a == 0)
    {
        this->ui->Text_help->append("\n文法产生式文件未成功打开。");
    }
    else if(a == 1)
    {

        this->ui->Text_help->append("\n文法产生式文件成功打开。");

        //首先初始化first集合
        LR1.Init_First_list();

        //初始化Action_goto集合
        int b = LR1.initAction_Goto();

        if(b == 0)
        {
            this->ui->Text_help->append("\n文法并不是LR文法，请检查一下。");
        }
        else if(b == 1)
        {
            this->ui->Text_help->append("\nAction_goto表求解成功");
        }
    }
    LR1.Parser_PTree(Tree_out);
}




