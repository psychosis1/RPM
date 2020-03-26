#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QtNetwork>
#include <mainwindow.h>
#include<QWebSocket>
#include <QMessageBox>

class Client : public QObject
{
    Q_OBJECT
private:
    QString message;
    QString nick;
    QWebSocket* client;
    MainWindow* window;
    QString port;

    ~Client();
public:
    /**
     * @brief connectToServer Подключение к серверу
     */
    void connectToServer();
    /**
     * @brief disconnectFromServer Отсоединение от сервера
     */
    void disconnectFromServer();
    /**
     * @brief setNick Получение ника
     * @param nick Ник
     */
    void setNick(QString nick);

    explicit Client(MainWindow* window,QObject *parent = nullptr);

signals:
    /**
     * @brief setMessageInTextEdit Установка в QTextEdit html сообщения
     */
    void setMessageInTextEdit(QString);
    /**
     * @brief setWhitespace Установка нового абзаца в QTextEdit
     */
    void setWhitespace(QString);
    /**
     * @brief hideButton Триггер для скрытия кнопки
     */
    void hideButton();
public slots:
    /**
     * @brief sendMessage Посылка сообщения на сервер
     */
    void sendMessage();
    /**
     * @brief setMessage Установка сообщения
     * @param message сообщение
     */
    void setMessage(QString message);
    /**
     * @brief deleteMe Удаление клиента
     */
    void deleteMe();
    /**
     * @brief setPort Установка порта
     * @param port порт
     */
    void setPort(QString port);
private slots:
    /**
     * @brief readSocket Чтение сокета
     */
    void readSocket(QByteArray data);
};

#endif // CLIENT_H
