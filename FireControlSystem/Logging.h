#ifndef LOGGING_H
#define LOGGING_H

#include <QFile>
#include <QObject>
#include "SettingsFile.h"

class TLogging : public QObject
{
Q_OBJECT

public:
    TLogging(TSettingsFile *ini);
    ~TLogging();
    void WriteLog(QString externalstr);

private:
    QFile *srv_logfile;
    TSettingsFile *_ini;
    bool _srv_logging;         // флаг активации журналирования событий сервера
};


#endif // LOGGING_H
