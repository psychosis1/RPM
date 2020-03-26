#ifndef SSERVER_H
#define SSERVER_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QtWidgets/qapplication.h>
#include <QSqlDatabase>
#include <QSqlQuery>

class Sserver : public QObject
{
    Q_OBJECT
public:
    explicit Sserver(QObject *parent = nullptr,int port=2000);
    /**
     * @brief messageFromDB Загрузка сообщений из БД
     */
    void messageFromDB();
    /**
     * @brief messageToDB Загрузка сообщений в БД
     * Открытие БД, подготовка запросов
     */
    void messageToDB();
    /**
     * @brief messageToDB2 Загрузка сообщений из БД
     * @param query Запросник
     */
    void messageToDB2(QSqlQuery query);
    /**
     * @brief connectClientToSystem Соединение клиента
     * @param client Клиент
     */
    void connectClientToSystem(QWebSocket* client);
    /**
     * @brief sendMessage Отправка сообщения
     * @param byteArray JSON сообщение
     */
    void sendMessage(QByteArray byteArray);
    /**
     * @brief createMessage Создание сообщения
     * @param nick Ник
     * @param message Сообщение
     * @return JSON сообщение
     */
    QByteArray createMessage(QString nick,QString message);
    /**
     * @brief saveMessage Сохранение сообщения
     * @param byteArray JSON сообщение
     */
    void saveMessage(QByteArray byteArray);
    QMap<QString,QWebSocket*> clients;
    QWebSocketServer* webServer;
    QJsonArray messages;
    QString dirPath=QApplication::applicationDirPath();
    ~Sserver();
signals:
private slots:
    /**
     * @brief connectClient Соединение клиента с сервером
     */
    void connectClient();
    /**
     * @brief connectClient2 Соединение клиента с сервером и отправка сообщений
     * @param connection Соединение
     * @param nick Ник
     */
    void connectClient2(QWebSocket* connection,QString nick);
    /**
     * @brief getByteArray Получение JSON сообщения
     * @param byteArray JSON сообщение
     */
    void getByteArray(QByteArray byteArray);
    /**
     * @brief disconnectedClient Отсоединение клиента
     */
    void disconnectedClient();
};

#endif // SSERVER_H
