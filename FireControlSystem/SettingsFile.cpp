#include "SettingsFile.h"
#include <QDebug>

TSettingsFile::TSettingsFile()
{
    _ManualMode = false;

    _IP_Server = "127.0.0.1";
    _PORT_Server = 40000;
    _IP_Gun = "127.0.0.1";
    _PORT_Gun = 31414;
    _Log = false;
    ReadConfigINI(QCoreApplication::applicationDirPath().append("/cfg.ini"));
}

TSettingsFile::~TSettingsFile()
{

}

void TSettingsFile::ReadConfigINI(QString INIfilename)
{
    QSettings *ini = new QSettings(INIfilename, QSettings::IniFormat);

    if(QFileInfo(INIfilename).exists())
    {
        // MAIN //
        ini->beginGroup("MAIN");
        {
            _ManualMode     = ini->value("MANUAL_MODE", 0).toBool();

            _IP_Server = ini->value("IP_SERVER", "127.0.0.1").toString();
            _PORT_Server = ini->value("PORT_SERVER", 40000).toInt();

            _IP_Gun = ini->value("IP_GUN", "127.0.0.1").toString();
            _PORT_Gun = ini->value("PORT_GUN", 40001).toInt();

            _Log = ini->value("LOGGING", false).toBool();
        }
        ini->endGroup();
    }
    ini->destroyed();
}
