#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPushButton>
#include <QMessageBox>
#include<QIntValidator>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief disableWidjet Дизактивирование виджетов
     * @param action Булевое значение
     */
    void disableWidjet(bool action);
    /**
     * @brief stopps Отсутсвие подключения к серверу
     */
    void stopps();
    /**
     * @brief setTextInTE Перенос
     * @param message Символ
     */
    void setTextInTE(QString message);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    /**
     * @brief clearAfterButton Блокирование кнопки отправить
     */
    void clearAfterButton();
    /**
     * @brief clearAfterButton_2 Разблокирование кнопки отправить
     */
    void clearAfterButton_2();
private slots:
    /**
     * @brief startClient Запуск клиента
     */
    void startClient();
};

#endif // MAINWINDOW_H
