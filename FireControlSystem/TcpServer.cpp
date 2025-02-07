#include "TcpServer.h"

TCP_server::TCP_server(QObject *parent):
    QObject(parent)
{
    server = new QTcpServer();
    server->setMaxPendingConnections(1);
    socket = new QTcpSocket();
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    server->listen(QHostAddress::Any, port);
}

TCP_server::~TCP_server()
{
    delete server;
}

void TCP_server::newConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readCommonTcpData()));
    qDebug() << "Connected";
}

void TCP_server::readCommonTcpData()
{
    bool processedHeader = false;
    TCPHeader displayHeader;

    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(TCPHeader); // 16 + 16 бит
        quint32 availabeBytes = socket->bytesAvailable();

        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = socket->read(header_length);
                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> displayHeader.Mark;
                header_stream >> displayHeader.PacketType;
                header_stream >> displayHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= displayHeader.DataSize))
            {
                QByteArray packet_data = socket->read(displayHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch (displayHeader.PacketType)
                {
                case 421:  //421 для танка, 420 нужно игнорировать
                    getDisplayDataPacket(data_stream);
                    break;
                default:
                    break;
                }
            }
            availabeBytes = socket->bytesAvailable();
            processedHeader = false;
        }
    }
}

void TCP_server::getDisplayDataPacket(QDataStream &_stream)
{
    displayPacket.PacketReady = true;
    _stream >> displayPacket.yaw;
    _stream >> displayPacket.SwitchGPK;
    emit readyYaw(displayPacket.yaw);
    emit readySwitch(displayPacket.SwitchGPK);
}
