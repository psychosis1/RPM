#include "client.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonValue>
#include<QUrl>
#include <QtWebSockets>

void Client::sendMessage()
{
    QJsonObject json;
    json.insert("nick", nick);
    json.insert("message", message);
    QJsonDocument doc(json);
    client->sendBinaryMessage(doc.toJson());
}

Client::~Client()
{
    qDebug()<<"deleted";
}

void Client::deleteMe()
{
    window->disableWidjet(false);
    client->close();
    deleteLater();
}

void Client::setPort(QString port)
{
    this->port=port;
}

void Client::connectToServer()
{
    client = new QWebSocket();
    QString urlString = "ws://127.0.0.1:"+port+"/"+QString("login=")+nick;
    qDebug()<<urlString;
    QUrl url(urlString);
    client->open(url);
    emit hideButton();
    connect(client, &QWebSocket::binaryMessageReceived,this,&Client::readSocket);
    connect(client,QOverload < QAbstractSocket :: SocketError > :: of ( & QWebSocket :: error ),window,&MainWindow::stopps);
}

void Client::disconnectFromServer()
{

}

void Client::setNick(QString nick)
{
    this->nick=nick;
}


Client::Client(MainWindow* window,QObject *parent) : window(window),QObject(parent)
{
}

void Client::setMessage(QString message)
{
    this->message=message;
}

void Client::readSocket(QByteArray data)
{
    QJsonDocument jsonDoc =QJsonDocument::fromJson(data);
    QJsonArray array = jsonDoc.array();
    for (auto ob:array){
        emit setMessageInTextEdit("<div><font color=\"black\">&lt;</font></div>");
        emit setMessageInTextEdit("<div><font color=\"blue\">"+ob.toObject()["nick"].toString()+"</font></div>");
        emit setMessageInTextEdit("<div><font color=\"black\">&gt;&nbsp;"+ob.toObject()["message"].toString()+"</font></div>");
        emit setWhitespace("");
    }
}
