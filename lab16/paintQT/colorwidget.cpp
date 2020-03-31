#include "colorwidget.h"
#include "ui_colorwidget.h"

ColorWidget::ColorWidget(QWidget *parent,QColor col) :
    QWidget(parent),
    ui(new Ui::ColorWidget)
{
    ui->setupUi(this);
    setValidators();
    xcor=col.red();
    ycor=col.green();
    zcor=col.blue();
    colors();
    setPixmap();
    connect(ui->lineEdit,&QLineEdit::textChanged,this,&ColorWidget::newColor);
    connect(ui->lineEdit_2,&QLineEdit::textChanged,this,&ColorWidget::newColor_2);
    connect(ui->lineEdit_3,&QLineEdit::textChanged,this,&ColorWidget::newColor_3);
    manualSlider();
    toMainWindow();
}

ColorWidget::~ColorWidget()
{
    delete ui;
}

static QColor operator+(const QColor & a, const QColor & b) {
  return QColor(a.red() + b.red(), a.green() + b.green(), a.blue() + b.blue());
}
static QColor operator*(const QColor & c, const qreal r) {
  return QColor(c.red() * r, c.green() * r, c.blue() * r);
}

void ColorWidget::red(int v)
{
    QColor from(255,0,0);
    QColor to(0,0,0);
    int d = ui->horizontalSlider->maximum() - ui->horizontalSlider->minimum();
    v = v - ui->horizontalSlider->minimum();
    qreal rv = qreal(v) / d;
    QColor c = from * rv + to * (1.0 - rv);
    ui->horizontalSlider->setStyleSheet(QString("QSlider::handle:horizontal {background-color: %1;}").arg(c.name()));
    ui->lineEdit->setText(QString::number(ui->horizontalSlider->value()));
}

void ColorWidget::green(int v)
{
    QColor from(0,255,0);
    QColor to(0,0,0);
    int d = ui->horizontalSlider_2->maximum() - ui->horizontalSlider_2->minimum();
    v = v - ui->horizontalSlider_2->minimum();
    qreal rv = qreal(v) / d;
    QColor c = from * rv + to * (1.0 - rv);
    ui->horizontalSlider_2->setStyleSheet(QString("QSlider::handle:horizontal {background-color: %1;}").arg(c.name()));
    ui->lineEdit_2->setText(QString::number(ui->horizontalSlider_2->value()));
}

void ColorWidget::blue(int v)
{
    QColor from(0,0,255);
    QColor to(0,0,0);
    int d = ui->horizontalSlider_3->maximum() - ui->horizontalSlider_3->minimum();
    v = v - ui->horizontalSlider_3->minimum();
    qreal rv = qreal(v) / d;
    QColor c = from * rv + to * (1.0 - rv);
    ui->horizontalSlider_3->setStyleSheet(QString("QSlider::handle:horizontal {background-color: %1;}").arg(c.name()));
    ui->lineEdit_3->setText(QString::number(ui->horizontalSlider_3->value()));
}

void ColorWidget::newColor(QString val)
{
    xcor=val.toInt();
    setPixmap();
}

void ColorWidget::newColor_2(QString val)
{
    ycor=val.toInt();
    setPixmap();
}

void ColorWidget::newColor_3(QString val)
{
    zcor=val.toInt();
    setPixmap();
}

void ColorWidget::setPixmap(){
    QPixmap pixmap(251, 181);
    pixmap.fill(QColor(xcor,ycor,zcor));
    ui->label->setPixmap(pixmap);
}

void ColorWidget::colors(){
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &ColorWidget::red);
    red(ui->horizontalSlider->value());
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &ColorWidget::green);
    green(ui->horizontalSlider_2->value());
    connect(ui->horizontalSlider_3, &QSlider::valueChanged, this, &ColorWidget::blue);
    blue(ui->horizontalSlider_3->value());
}

void ColorWidget::toMainWindow(){
    connect(ui->pushButton,&QPushButton::clicked,this,&ColorWidget::setLastColor);
    connect(ui->pushButton,&QPushButton::clicked,this,&ColorWidget::close);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&ColorWidget::close);
}

void ColorWidget::setLastColor(){
    emit setColor(QColor(xcor,ycor,zcor));
}

void ColorWidget::sliderChange(QString str)
{
    ui->horizontalSlider->setValue(str.toInt());
}

void ColorWidget::sliderChange_2(QString str)
{
    ui->horizontalSlider_2->setValue(str.toInt());
}

void ColorWidget::sliderChange_3(QString str)
{
    ui->horizontalSlider_3->setValue(str.toInt());
}

void ColorWidget::manualSlider()
{
    sliderChange(QString::number(xcor));
    sliderChange_2(QString::number(ycor));
    sliderChange_3(QString::number(zcor));
    connect(ui->lineEdit,&QLineEdit::textChanged,this,&ColorWidget::sliderChange);
    connect(ui->lineEdit_2,&QLineEdit::textChanged,this,&ColorWidget::sliderChange_2);
    connect(ui->lineEdit_3,&QLineEdit::textChanged,this,&ColorWidget::sliderChange_3);
}

void ColorWidget::setValidators()
{
    ui->lineEdit->setValidator(new QIntValidator(0,255));
    ui->lineEdit_2->setValidator(new QIntValidator(0,255));
    ui->lineEdit_3->setValidator(new QIntValidator(0,255));
}
