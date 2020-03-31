#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include "paintscene.h"
#include "colorwidget.h"
#include <QTimer>
#include <QShortcut>
#include <QMessageBox>

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
    /**
     * @brief startWidget Окно с выбором цвета
     */
    void startWidget();
    /**
     * @brief colorr Установка в пробник цвета
     * @param col Цвет
     */
    void colorr(QColor col);
private slots:
    /**
     * @brief openFile Открытие файла
     */
    void openFile();
    /**
     * @brief saveFile Сохранение файла
     */
    void saveFile();
    /**
     * @brief saveAsFile Сохранение как файла
     */
    void saveAsFile();
    /**
     * @brief slotTimer Рендер полотна рисования
     */
    void slotTimer();
    /**
     * @brief setSizeBrush Выбор размера кисточки
     * @param value Рарзмер кисти
     */
    void setSizeBrush(int value);
    /**
     * @brief setStyleBrush Выбор стиля кисти
     */
    void setStyleBrush();
    /**
     * @brief slotCustomMenuRequested Контекстное меню
     * @param pos Указатель
     */
    void slotCustomMenuRequested(QPoint pos);
private:
    Ui::MainWindow *ui;
    ColorWidget *colors;
    QString filename;
    PaintScene *scene;
    QTimer *timer;
    QList <QString> list;
    QShortcut *keyCtrX,*keyCtrS;
    /**
     * @brief resizeEvent Обновление полотна при изменении размера
     * @param event Событие
     */
    void resizeEvent(QResizeEvent * event);
    /**
     * @brief fileWork Сохранение картинки в файл
     */
    void fileWork();
    /**
     * @brief listWidget Стили кисти
     */
    void listWidget();
    /**
     * @brief context Привязка контекстного меню к полотну
     */
    void context();
    /**
     * @brief key Горячие клавиши
     */
    void key();
    /**
     * @brief switchStyle Выбор стиля
     * @param value Выбранный стиль
     */
    void switchStyle(int value);
};
#endif // MAINWINDOW_H
