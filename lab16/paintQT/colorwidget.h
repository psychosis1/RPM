#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QSlider>
#include<QDebug>
#include <QIntValidator>
namespace Ui {
class ColorWidget;
}

class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWidget(QWidget *parent = nullptr,QColor col=QColor(0,0,0));
    ~ColorWidget();
public slots:
    /**
     * @brief setLastColor Установка выбранного цвета
     */
    void setLastColor();
private slots:
    /**
     * @brief red Отображение красного цвета ползунка
     * @param v Значение цвета
     */
    void red(int v);
    /**
     * @brief green Отображение зеленого цвета ползунка
     * @param v Значение цвета
     */
    void green(int v);
    /**
     * @brief blue Отображение синего цвета ползунка
     * @param v Значение цвета
     */
    void blue(int v);
    /**
     * @brief newColor Обновление результирующего цвета
     * @param val Значение красного цвета
     */
    void newColor(QString val);
    /**
     * @brief newColor_2 Обновление результирующего цвета
     * @param val Значение зеленого цвета
     */
    void newColor_2(QString val);
    /**
     * @brief newColor_3 Обновление результирующего цвета
     * @param val Значение синего цвета
     */
    void newColor_3(QString val);
    /**
     * @brief sliderChange Изменение ползунка при ручном вводе значения
     * @param str Значение красного цвета
     */
    void sliderChange(QString str);
    /**
     * @brief sliderChange_2 Изменение ползунка при ручном вводе значения
     * @param str Значение зеленого цвета
     */
    void sliderChange_2(QString str);
    /**
     * @brief sliderChange_3 Изменение ползунка при ручном вводе значения
     * @param str Значение синего цвета
     */
    void sliderChange_3(QString str);
signals:
    /**
     * @brief setColor Сигнал для вывода цвета в главное окно
     */
    void setColor(QColor);
private:
    Ui::ColorWidget *ui;
    int xcor=0,ycor=0,zcor=0;
    /**
     * @brief setPixmap Результирующая картинка
     */
    void setPixmap();
    /**
     * @brief colors Считывание цвета с измененного ползунка
     */
    void colors();
    /**
     * @brief toMainWindow Возвращение в главное окно
     */
    void toMainWindow();
    /**
     * @brief manualSlider Сопоставление введенного значения и ползунка
     */
    void manualSlider();
    /**
     * @brief setValidators Валидаторы для цветов
     */
    void setValidators();
};

#endif // COLORWIDGET_H
