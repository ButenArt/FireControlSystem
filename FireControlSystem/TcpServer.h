#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "DataStruct.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class TCP_server: public QObject
{
    Q_OBJECT

public:
    explicit TCP_server(QObject *parent = nullptr);
    ~TCP_server();

    void setPort(int p) { port = p; }

    DataPacket shotPacket;
    QTcpServer *server;
    QTcpSocket *socket;
    int port = 20001;//8003;

    void getShotDataPacket(QDataStream &);

public slots:
    void newConnection();
    void readCommonTcpData();
signals:
    void readyShot(bool shot);
};

#endif // TCPSERVER_H
