#ifndef LOGGING_H
#define LOGGING_H

#include <QFile>
#include <QObject>
#include "DataStruct.h"
#include "SettingsFile.h"

class TLogging : public QObject
{
Q_OBJECT

public:
    TLogging(TSettingsFile *ini);
    ~TLogging();
    void WriteBallisticsLog(QString externalstr);
    void WriteMOXALog(int error, QString externalstr);
    void WriteCANLog(int event, QString externalstr);

private:
    QFile *lan_logfile;
    QFile *can_logfile;
    QFile *can_data_logfile;
    QFile *srv_logfile;
    QFile *ballistics_logfile;
    TSettingsFile *_ini;
    bool _lan_logging;         // флаг активации журналирования ошибок LAN
    bool _can_logging;         // флаг активации журналирования ошибок CAN
    bool _can_data_logging;    // флаг активации журналирования CAN TX/RX пакетов
    bool _srv_logging;         // флаг активации журналирования событий сервера
    bool _ballistics_logging;  // флаг активации журналирования событий баллистики
};


#endif // LOGGING_H
