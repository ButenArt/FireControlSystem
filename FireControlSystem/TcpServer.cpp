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
    TCPHeader Header;

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

                header_stream >> Header.Mark;
                header_stream >> Header.PacketType;
                header_stream >> Header.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= Header.DataSize))
            {
                QByteArray packet_data = socket->read(Header.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch (Header.PacketType)
                {
                case 1001:
                    getShotDataPacket(data_stream);
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

void TCP_server::getShotDataPacket(QDataStream &_stream)
{
    shotPacket.PacketReady = true;
    _stream >> shotPacket.shot;
}
