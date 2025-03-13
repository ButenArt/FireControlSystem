#include "UdpServer.h"

UDP_server::UDP_server(QObject *parent): QObject(parent)
{
    address = QHostAddress("127.0.0.1");
    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    socket->bind(QHostAddress::Any, port);
}

UDP_server::~UDP_server()
{
    delete socket;
}

void UDP_server::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QDataStream stream(datagram);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        TCPHeader displayHeader;
        stream >> displayHeader.Mark;
        stream >> displayHeader.PacketType;
        stream >> displayHeader.DataSize;

        if (datagram.size() >= sizeof(TCPHeader) + displayHeader.DataSize)
        {
            QByteArray packet_data = datagram.mid(sizeof(TCPHeader), displayHeader.DataSize);
            QDataStream data_stream(packet_data);
            data_stream.setByteOrder(QDataStream::LittleEndian);
            data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

            switch (displayHeader.PacketType)
            {
            case 421:
                getDataPacket(data_stream);
                break;
            default:
                break;
            }
        }
    }
}

void UDP_server::sendData(const QByteArray &data)
{
    qDebug() << "Отправка пакета (HEX) - 2:" << data.toHex(' ');
    socket->writeDatagram(data, address, port);
}

void UDP_server::getDataPacket(QDataStream &_stream)
{
    displayPacket.PacketReady = true;
}
