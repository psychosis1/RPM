#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_2->setValidator(new QIntValidator(1024,65535,this));
    trey();
    bdFromQrc();
    this->setWindowIcon(QIcon(":/image/logo2"));

    QSettings settings(QApplication::applicationDirPath() + "/sett.ini", QSettings::IniFormat);
    ui->lineEdit_2->setText(settings.value("server/port", 2000).toString());
    port=settings.value("server/port", 2000).toInt();

    connect(ui->lineEdit_2,&QLineEdit::textChanged,this,&MainWindow::savePort);

    connect(ui->radioButton,&QRadioButton::clicked,this,&MainWindow::start);
    connect(ui->radioButton_2,&QRadioButton::clicked,this,&MainWindow::stop);

    connect(ui->save,&QPushButton::clicked,this,&MainWindow::saveSetting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    if(check==0){
    QSettings settings(QApplication::applicationDirPath() + "/sett.ini", QSettings::IniFormat);
    bikiniBottom("Сервер запущен. Порт: "+settings.value("server/port", 2000).toString(),"Запуск сервера");
    server=new Sserver(nullptr,settings.value("server/port", 2000).toInt());
    check=1;
    if(!ui->radioButton->isChecked())
        ui->radioButton->click();
    }

}

void MainWindow::bikiniBottom(QString text,QString tytle)
{
    QMessageBox mb;
    mb.setText(text);
    mb.setWindowTitle(tytle);
    mb.setIcon(QMessageBox::Information);
    mb.setStandardButtons(QMessageBox::Ok);
    mb.setAttribute(Qt::WA_QuitOnClose, false);
    mb.exec();
}

void MainWindow::stop()
{
    if(check==1){
        delete server;
        bikiniBottom("Сервер выключен.","Выключение сервера");
        check=0;
        if(!ui->radioButton_2->isChecked())
            ui->radioButton_2->click();
    }
}

void MainWindow::trey(){
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon(":/image/logo"));
        QMenu * menu = new QMenu();
        QAction * startServer = new QAction("Включиться");
        QAction * exitServer = new QAction("Отключиться");
        connect(startServer, SIGNAL(triggered()), this, SLOT(start()));
        connect(exitServer, SIGNAL(triggered()), this, SLOT(stop()));
        menu->addAction(startServer);
        menu->addAction(exitServer);
        trey2(trayIcon,menu);
}

void MainWindow::trey2(QSystemTrayIcon *trayIcon,QMenu * menu)
{
    trayIcon->setContextMenu(menu);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::bdFromQrc()
{
    QFile file(QApplication::applicationDirPath()+"/last10Messages.db");
    if(!file.exists()){
        QFile::copy(":/db/last",QApplication::applicationDirPath()+"/last10Messages.db");
        QFile::setPermissions(QApplication::applicationDirPath()+"/last10Messages.db",QFile::WriteOwner | QFile::ReadOwner);
    }
    file.close();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
       case QSystemTrayIcon::Trigger:
            if (!this->isVisible())
                this->show();
            else
                this->hide();
        break;
    default:
            break;
    }
}
void MainWindow::saveSetting(){
    if(port<=1023){
        QMessageBox::critical(this,"Недопустимый порт","Выберите другой порт");
        return;
    }
    QSettings settings(QApplication::applicationDirPath() + "/sett.ini", QSettings::IniFormat);
    settings.setValue("server/port", port);
    settings.sync();
    QMessageBox::information(this,"Сохранение настроек","Сохранение настроек выполнено успешно.\nСервер будет отключен");
    if(ui->radioButton->isChecked())
        ui->radioButton_2->click();
}

void MainWindow::savePort(QString port){
    this->port=port.toInt();
}

