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

    DataPacket displayPacket;
    QTcpServer *server;
    QTcpSocket *socket;
    int port = 20001;//8003;

    void getDisplayDataPacket(QDataStream &);

public slots:
    void newConnection();
    void readCommonTcpData();
signals:
    void readyYaw(float yaw);
    void readySwitch(bool switchgpk);
};

#endif // TCPSERVER_H
