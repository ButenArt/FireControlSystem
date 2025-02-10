#ifndef SETTINGSFILE_H
#define SETTINGSFILE_H

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QDateTime>
#include <Windows.h>

class TSettingsFile : public QObject
{
	Q_OBJECT

public:
	TSettingsFile( );
	~TSettingsFile( );

    bool ManualMode( ) const { return _ManualMode; }

    QString IP_Server( ) const { return _IP_Server; }
    WORD PORT_Server( ) const { return _PORT_Server; }
    QString IP_Gun( ) const { return _IP_Gun; }
    WORD PORT_Gun( ) const { return _PORT_Gun; }

    bool log( ) const { return _Log; }
private:
	bool _ManualMode;              // флаг режима работы сервера - ручное управление или CAN

    QString _IP_Server = "127.0.0.1";
    WORD _PORT_Server = 40000;
    QString _IP_Gun = "127.0.0.1";
    WORD _PORT_Gun = 31414;

    bool _Log = false;

	void ReadConfigINI( QString INIfilename );
};

#endif // SETTINGSFILE_H
