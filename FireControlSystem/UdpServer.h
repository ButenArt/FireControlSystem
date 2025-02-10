#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "DataStruct.h"
#include <QObject>
#include <QUdpSocket>
#include <QDataStream>

class UDP_server: public QObject
{
    Q_OBJECT

public:
    explicit UDP_server(QObject *parent = nullptr);
    ~UDP_server();
    void setPort(int p) { port = p; }
    void setAddress(QString p) { address = p; }
private:
    DataPacket displayPacket;
    QUdpSocket *socket;
    int port = 20001;
    QHostAddress address;
    void getDataPacket(QDataStream &);
public slots:
    void readPendingDatagrams();
    void sendData(const QByteArray &data);
};

#endif // UDPSERVER_H
