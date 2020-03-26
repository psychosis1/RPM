#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <client.h>

void MainWindow::disableWidjet(bool action)
{
    ui->nick->setDisabled(action);
    ui->connect->setDisabled(action);
    ui->lineEdit->setDisabled(action);
    ui->disconnect->setDisabled(!action);
    ui->send->setDisabled(!action);
    ui->text->setDisabled(!action);
}

void MainWindow::setTextInTE(QString message)
{
    ui->textEdit->append(message);

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator(1023,65535,this));
    connect(ui->connect,&QPushButton::clicked,this,&MainWindow::startClient);
    disableWidjet(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearAfterButton()
{
    ui->send->setDisabled(1);
}

void MainWindow::clearAfterButton_2()
{
    ui->send->setDisabled(0);
}

void MainWindow::startClient()
{
    Client* client = new Client(this);
    disableWidjet(true);
    ui->textEdit->setDisabled(1);
    client->setNick(ui->nick->text());
    client->setPort(ui->lineEdit->text());
    client->connectToServer();
    connect(ui->disconnect,&QPushButton::clicked,ui->textEdit,&QTextEdit::clear);
    connect(ui->connect,&QPushButton::clicked,this,&MainWindow::clearAfterButton);

    connect(ui->disconnect,&QPushButton::clicked,client,&Client::deleteMe);
    connect(ui->text,&QLineEdit::textEdited,client,&Client::setMessage);
    connect(ui->send,&QPushButton::clicked,client,&Client::sendMessage);

    connect(client,&Client::setMessageInTextEdit,ui->textEdit,&QTextEdit::insertHtml);
    connect(client,&Client::setWhitespace,ui->textEdit,&QTextEdit::append);
    connect(ui->send,&QPushButton::clicked,ui->text,&QLineEdit::clear);

    connect(ui->send,&QPushButton::clicked,this,&MainWindow::clearAfterButton);
    connect(ui->text,&QLineEdit::textEdited,this,&MainWindow::clearAfterButton_2);
}

void MainWindow::stopps(){
    disableWidjet(false);
    ui->textEdit->clear();
    QMessageBox::warning(this,"Сервер недоступен","Повторите попытку позже");
}


