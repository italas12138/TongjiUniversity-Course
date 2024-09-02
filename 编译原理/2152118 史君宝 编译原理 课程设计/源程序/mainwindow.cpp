#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ObjectCode_Generator.h"
#include "Common.h"
#include <QDebug>

#include <fstream>
#include <QProcess>

#include <QFileDialog>
#include <QLineEdit>
#include <QInputDialog>

using namespace std;

int abcde = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("C-Compiler");

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



//导入帮助文档
void MainWindow::on_Help_file_triggered()
{
    ifstream fin;
    fin.open(R"(file\help.txt)");

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


//使用文本框导入源程序
void MainWindow::on_Open_text_triggered()
{
    QString text = this->ui->Grammar_Program_Textin->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Program text is empty.");
    }
    else
    {
        ofstream fout(R"(file\Program.txt)");
        if (!fout.is_open())
        {
            QMessageBox::warning(nullptr, "警告", "无法打开程序文件");
            return;
        }

        fout << text.toStdString();
        fout.close();

        QMessageBox::information(this, "提示", "文本框中的内容已复制到 Program.txt 文件中");
        return;
    }
}


//使用文件名导入源程序
void MainWindow::on_Open_file_triggered()
{
    bool ok;
    QString fileName = QInputDialog::getText(this, "输入文件名", "文件名:", QLineEdit::Normal, "", &ok);
    if (!ok || fileName.isEmpty()) {
        QMessageBox::warning(this, "警告", "未输入文件名");
        return;
    }

    fileName = "test/" + fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       QMessageBox::warning(this, "警告", "无法打开文件");
       return;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    std::ofstream fout(R"(file\Program.txt)");
    if (!fout.is_open()) {
       QMessageBox::warning(nullptr, "警告", "无法打开程序文件");
       return;
    }

    fout << fileContent.toStdString();
    fout.close();

    QMessageBox::information(this, "提示", "文件内容已复制到 Program.txt 文件中");
}


void MainWindow::result_choice()
{
    this->ui->Text_help->setText("编译结果文件打开失败");
}


//使用文件名导入源程序
void MainWindow::on_Compiler_result_triggered()
{
    abcde = 0;
    string file_name = R"(file\Program.txt)";

    this->ui->Text_help->setPlainText(""); // 清空文本框内容

    ofstream fou(R"(file\Result.txt)"); // 打开文件以进行追加写入

    if (fou.is_open()) {
        fou << "";
        fou.close(); // 关闭文件
    } else {
        result_choice();
    }

    fou.close();


    // 先调词法工作
    wordParse wp;
    this->ui->Text_help->setText("");
    wp.work(file_name);
    wp.outputParseResult(R"(file\parseResult.txt)");

    // 报错日志
    ofstream fout(R"(file\Result.txt)", ios::app); // 打开文件以进行追加写入

    if (fout.is_open()) {
        fout << "\n词法分析完成\n";
        fout.close(); // 关闭文件
    } else {
        result_choice();
    }

    fout.close();

        _GrammarParse gp(R"(file\Grammar.txt)");
        gp._Work();



        ofstream fout1(R"(file\Result.txt)", ios::app); // 打开文件以进行追加写入

        if (fout1.is_open()) {
            fout1 << "\n语法分析完成\n";
            fout1.close(); // 关闭文件
        } else {
            result_choice();
        }

        fout1.close();

        qDebug() << "3";


        gp._SemanticAnalyse(wp.getWordParseResult());


    if(!abcde)
    {
        ofstream fout2(R"(file\Result.txt)", ios::app); // 打开文件以进行追加写入

        if (fout2.is_open()) {
            fout2 << "\n语义分析完成\n";
            fout2.close(); // 关闭文件
        } else {
            result_choice();
        }

        fout2.close();

         qDebug() << "4";

        ObjectCodeGenerator ocg(gp._code._quaCode, gp.fun_enter);

        ocg.generate_code();

        ofstream fout3(R"(file\Result.txt)", ios::app); // 打开文件以进行追加写入

        if (fout3.is_open()) {
            fout3 << "\n目标代码生成完成\n";
            fout3.close(); // 关闭文件
        } else {
            result_choice();
        }

        fout3.close();

        qDebug() << "5";
    }



    ifstream fin1;
    fin1.open(R"(file\Result.txt)");

    if (fin1.is_open()) {
        QString fileContent;
        string line;
        while (getline(fin1, line)) {
            fileContent += QString::fromStdString(line) + "\n";
        }
        fin1.close();

        // 将文件内容显示在 QTextBrowser 控件中
        this->ui->Text_help->setText(fileContent);
    }
    else {
        QMessageBox::warning(this, "Error", "未成功打开文件！");
    }

}


void MainWindow::on_Code_result_triggered()
{
    ifstream fin1;
    fin1.open(R"(file\code.asm)");

    if (fin1.is_open()) {
        QString fileContent;
        string line;
        while (getline(fin1, line)) {
            fileContent += QString::fromStdString(line) + "\n";
        }
        fin1.close();

        // 将文件内容显示在 QTextBrowser 控件中
        this->ui->Text_help->setText(fileContent);
    }
    else {
        QMessageBox::warning(this, "Error", "未成功打开文件！");
    }

}




