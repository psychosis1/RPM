#include "sserver.h"
#include <QStringList>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QSqlError>
#include <QDebug>
Sserver::Sserver(QObject *parent,int port) : QObject(parent)
{
    messageFromDB();
    webServer = new QWebSocketServer("Chat",QWebSocketServer::NonSecureMode,this);
    webServer->listen(QHostAddress::Any,port);
    if (webServer->isListening())
        qInfo()<<"Server started!";
    qInfo()<<webServer->serverUrl();
    connect(webServer,&QWebSocketServer::newConnection,this,&Sserver::connectClient);
}

Sserver::~Sserver()
{
    messageToDB();
    qInfo()<<"server deleted";
}

void Sserver::messageFromDB()
{
    QSqlDatabase database=QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QApplication::applicationDirPath()+"/last10Messages.db");
    database.open();
    QSqlQuery query(database);
    query.exec("SELECT * FROM Messages");
    while(query.next()){
        saveMessage(createMessage(query.value("nick").toString(),query.value("message").toString()));
    }
    database.close();
}

void Sserver::messageToDB()
{
    QSqlDatabase database=QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QApplication::applicationDirPath()+"/last10Messages.db");
    database.open();
    QSqlQuery query(database);
    messageToDB2(query);
    database.close();
}

void Sserver::messageToDB2(QSqlQuery query){
    query.exec("DELETE FROM Messages;");
    for(auto value:messages){
        query.prepare("INSERT INTO Messages VALUES (?,?)");
        query.bindValue(0,value.toObject()["nick"].toString());
        query.bindValue(1,value.toObject()["message"].toString());
        query.exec();
    }
}

void Sserver::connectClientToSystem(QWebSocket *client)
{
    connect(client,&QWebSocket::binaryMessageReceived,this,&Sserver::getByteArray);
    connect(client,&QWebSocket::disconnected,this,&Sserver::disconnectedClient);
}

void Sserver::sendMessage(QByteArray byteArray)
{
    QJsonArray array;
    array.append(QJsonDocument::fromJson(byteArray).object());
    QJsonDocument docc(array);
    for(auto client:clients)
        client->sendBinaryMessage(docc.toJson());
}

QByteArray Sserver::createMessage(QString nick, QString message)
{
    QJsonObject object;
    object["nick"]=nick;
    object["message"]=message;
    QJsonDocument doc(object);
    return doc.toJson();
}

void Sserver::saveMessage(QByteArray byteArray)
{
    messages.append(QJsonDocument::fromJson(byteArray).object());
    if (messages.size()>10)
        messages.removeFirst();
}

void Sserver::connectClient()
{
    QWebSocket* connection = webServer->nextPendingConnection();
    QString nick =connection->requestUrl().toString().section("login=",1,1);
    if (clients.find(nick)==clients.end())
        connectClient2(connection,nick);
    else {
        connection->close();
        qInfo()<<"user:"<<nick<<" ERROR";
    }
}

void Sserver::connectClient2(QWebSocket *connection,QString nick)
{
    clients[nick]=connection;
    connectClientToSystem(connection);
    qInfo()<<"user:"<<nick<<" was connected";
    connection->sendBinaryMessage(QJsonDocument(messages).toJson());
    sendMessage(createMessage(nick,"was connected"));
}

void Sserver::getByteArray(QByteArray byteArray)
{
    sendMessage(byteArray);
    saveMessage(byteArray);
    qInfo()<<"messages send:"<< QJsonDocument::fromJson(byteArray).object();
}



void Sserver::disconnectedClient()
{
    for(auto client:clients){
        if(!client->isValid()){
            qInfo()<<"user:"<<client->requestUrl().toString().section("login=",1,1)<<" was disconnected";
            sendMessage(createMessage(client->requestUrl().toString().section("login=",1,1),"was disconnected"));
            clients.remove(client->requestUrl().toString().section("login=",1,1));
            break;
        }
    }
    qDebug()<<"disconnected";
}

