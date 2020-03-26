#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sserver.h"
#include <QSettings>
#include <QMessageBox>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QVariant>
#include <QIntValidator>
#include <QFile>

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
     * @brief start Запуск сервера
     */
    void start();
    /**
     * @brief stop Отключение сервера
     */
    void stop();
    /**
     * @brief iconActivated Нажатие на трей
     * @param reason Триггер
     */
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    /**
     * @brief saveSetting Сохранение настроек в ini
     */
    void saveSetting();
    /**
     * @brief savePort Сохранение выбранного порта
     * @param port Порт
     */
    void savePort(QString port);
private:
    Ui::MainWindow *ui;
    Sserver *server;
    QSystemTrayIcon* trayIcon;
    int port;
    bool check=0;
    /**
     * @brief trey Запуск трея
     */
    void trey();
    /**
     * @brief trey2 Показ контекстного меню
     * @param trayIcon Трей
     * @param menu Меню
     */
    void trey2(QSystemTrayIcon *trayIcon,QMenu * menu);
    /**
     * @brief bdFromQrc Копирование файла бд из ресурсов
     */
    void bdFromQrc();
    /**
     * @brief bikiniBottom Месседжы
     * @param text Основной текст
     * @param tytle Заголовок
     */
    void bikiniBottom(QString text,QString tytle);
};
#endif // MAINWINDOW_H
