#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include "Logging.h"

TLogging::TLogging(TSettingsFile *ini)
{
    _ini = ini;
    _srv_logging = _ini->log();
    srv_logfile = new QFile();
    srv_logfile->setFileName(QCoreApplication::applicationDirPath().append("/Server.log"));
    if(_srv_logging) srv_logfile->open(QIODevice::Append);
}

TLogging::~TLogging()
{
    if(srv_logfile->isOpen()) srv_logfile->close();
    delete srv_logfile;
}

void TLogging::WriteLog(QString externalstr)
{
    if(!_srv_logging) return;
    QString logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] " + externalstr;

    if(srv_logfile->isOpen())
    {
        srv_logfile->write(logstr.toUtf8());
        srv_logfile->flush();
    }
}
