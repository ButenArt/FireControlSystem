#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QtGlobal>

// Заголовок пакета
struct TCPHeader
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
};

// Данные пакета
struct DataPacket
{
    bool PacketReady = 0;
    bool shot = true;        // Тумблер включения
};

#endif // DATASTRUCT_H
