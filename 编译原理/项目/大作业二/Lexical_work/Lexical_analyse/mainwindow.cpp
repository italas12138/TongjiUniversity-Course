#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <fstream>
#include <QProcess>

#include <QFileDialog>
#include <QLineEdit>
#include <QInputDialog>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Lexical_analyse");

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


void MainWindow::Word_choice(string choice)
{
    ifstream fin;
    fin.open("file/Program.txt", ios::in);// 打开源程序文件

    WordAnalyze word(fin);
    word.wordsdivide();

    word.Word_output(choice);
}

void MainWindow::Grammar_choice(string choice)
{
    ifstream fin;
    fin.open("file/Program.txt", ios::in);// 打开源程序文件

    WordAnalyze word(fin);
    word.wordsdivide();

    GrammerAnalyze grammer(word);
    grammer.grammeranalyze();

    grammer.output(choice);
}



void MainWindow::on_Grammar_fourline_triggered()
{
    Grammar_choice("fourline");
    QMessageBox::information(this, "提示", "四元式已加载完成");

    ifstream fin;
    fin.open(R"(file\Grammar_fourline.txt)");

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

    return;
}



void MainWindow::on_Word_error_triggered()
{
    Word_choice("Error");
    QMessageBox::information(this, "提示", "错误报告已加载完成");

    ifstream fin;
    fin.open(R"(file\Word_error.txt)");

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

    return;

}

void MainWindow::on_Word_ID_triggered()
{
    Word_choice("ID");
    QMessageBox::information(this, "提示", "标识符表已加载完成");

    ifstream fin;
    fin.open(R"(file\Word_ID.txt)");

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

    return;
}

void MainWindow::on_Word_Sign_triggered()
{
    Word_choice("Sign");
    QMessageBox::information(this, "提示", "运算符表已加载完成");

    ifstream fin;
    fin.open(R"(file\Word_Sign.txt)");

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

    return;
}

void MainWindow::on_Word_all_triggered()
{
    Word_choice("ALL");
    QMessageBox::information(this, "提示", "全符号表已加载完成");

    ifstream fin;
    fin.open(R"(file\Word_all.txt)");

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

    return;
}





