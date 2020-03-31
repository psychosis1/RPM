#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::openFile);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::saveFile);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::saveAsFile);
    scene = new PaintScene();
    ui->graphicsView->setScene(scene);
    QCursor cursorBrush = QCursor(QPixmap(":/cursor/brush.png"),0,32);
    ui->graphicsView->setCursor(cursorBrush);
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
    scene->size=ui->spinBox->value();
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(setSizeBrush(int)));
    listWidget();
    ui->commandLinkButton->setIcon(QIcon(":/icon/rainbow.png"));
    ui->lineEdit->setDisabled(1);
    connect(ui->commandLinkButton,&QCommandLinkButton::clicked,this,&MainWindow::startWidget);
    colorr(QColor(0,0,0));
    context();
    key();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startWidget()
{
    colors=new ColorWidget(nullptr,scene->color);
    colors->setAttribute(Qt :: WA_DeleteOnClose);
    colors->show();
    colors->focusWidget();
    connect(colors,&ColorWidget::setColor,this,&MainWindow::colorr);
}

void MainWindow::openFile()
{
    filename = QFileDialog::getOpenFileName(this, "Открыть изображение", "", "*.bmp *.jpg *.png");
    scene->addPixmap(QPixmap(filename));
}

void MainWindow::saveFile()
{
    if(filename.isEmpty()){
       saveAsFile();
       return;
    }
    fileWork();
    QMessageBox::information(this,"Сохранение","Файл сохранен",QMessageBox::Ok);
}

void MainWindow::saveAsFile()
{
   filename = QFileDialog::getSaveFileName(this, "Сохранить изображение как", "", "*.jpg *.png *.bmp");
   fileWork();
}

void MainWindow::fileWork()
{
    QImage image(scene->width(), scene->height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::white).rgb());
    QPainter painter(&image);
    ui->graphicsView->render(&painter);
    image.save(filename);
}

void MainWindow::listWidget()
{
    list={"SolidLine","DashLine","DotLine"};
    ui->listWidget->addItems(list);
    connect(ui->listWidget,&QListWidget::clicked,this,&MainWindow::setStyleBrush);
}

void MainWindow::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 5, ui->graphicsView->height() - 5);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QWidget::resizeEvent(event);
}

void MainWindow::setSizeBrush(int value)
{
    scene->size=value;
}

void MainWindow::setStyleBrush(){
    QMap <QString,int>map;
    int i=0;
    for (auto iter = list.begin(); iter != list.end(); iter++)
       {
           i++;
           map.insert(*iter,i);
       }
    switchStyle(map.find(ui->listWidget->currentItem()->text()).value());
}

void MainWindow::switchStyle(int value)
{
    switch (value) {
    case 1:
        scene->style=Qt::SolidLine;
        break;
    case 2:
        scene->style=Qt::DashLine;
        break;
    case 3:
        scene->style=Qt::DotLine;
        break;
    default:
        return;
    }
}

void MainWindow::colorr(QColor col)
{
    QPixmap pixmap(32, 32);
    pixmap.fill(col);
    ui->label->setPixmap(pixmap);
    scene->color=col;
}

void MainWindow::context(){
    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    QMenu * menu = new QMenu(this);
    QList <QAction*> listAction;
    listAction={new QAction("Сохранить"),new QAction("Сохранить как"),new QAction("Очистить")};
    connect(listAction.first(), SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(listAction.at(1), SIGNAL(triggered()), this, SLOT(saveAsFile()));
    connect(listAction.last(), SIGNAL(triggered()), scene, SLOT(clear()));
    menu->addActions(listAction);
    menu->popup(ui->graphicsView->viewport()->mapToGlobal(pos));
}

void MainWindow::key()
{
    keyCtrX = new QShortcut(this);
    keyCtrS = new QShortcut(this);
    keyCtrX->setKey(Qt::CTRL + Qt::Key_X);
    keyCtrS->setKey(Qt::CTRL + Qt::Key_S);
    connect(keyCtrX, &QShortcut::activated, scene, &PaintScene::clear);
    connect(keyCtrS, &QShortcut::activated, this,&MainWindow::saveFile);
}
