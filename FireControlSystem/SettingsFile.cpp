#include "SettingsFile.h"
#include <QDebug>

TSettingsFile::TSettingsFile()
{
    _WindowMinimize = false;
    _CanLoggingEnabled = false;
    _LanLoggingEnabled = false;
    _SoundsEnabled = false;
    _ManualMode = false;
    _Board = "MOXA";
    //_ReplayEnabled = false;

    _CanTxInterval = 20;
    _CanRxInterval = 10;

    for ( int i = 0; i < 16; ++i )
    {
        _MAB[i].Addr = 0x300 + i * 0x10;   // 300 310 320 330 ...
        _MAB[i].Port = 0;
        _MAB[i].Enable = false;
    }

    // VID
    for ( int i = 0; i < 16; ++i )
    {
        _VID[i].Addr = 0x200 + i * 0x10;   // 
        _VID[i].Port = 0;
        _VID[i].Enable = false;
    }

    // GPIN
    for ( int i = 0; i < 16; ++i )
    {
        _GPIOIN[i].Addr = 0x500 + i * 0x10;   // 
        _GPIOIN[i].Port = 0;
        _GPIOIN[i].Enable = false;
    }

    // GPOUT
    for ( int i = 0; i < 16; ++i )
    {
        _GPIOOUT[i].Addr = 0x700 + i * 0x10;   // 
        _GPIOOUT[i].Port = 0;
        _GPIOOUT[i].Enable = false;
    }

    // POT
    for ( int i = 0; i < 16; ++i )
    {
        _POT[i].Addr = 0x700 + i * 0x10;   // 
        _POT[i].Port = 0;
        _POT[i].Enable = false;
    }

    _LanTxInterval = 20;
    _LanRxInterval = 20;
    _IP_Imager = "127.0.0.1";
    _PORT_Imager = 10000;
    _IP_Instructor = "127.0.0.1";
    _PORT_Instructor = 40000;
    _IP_ScriptServer = "127.0.0.1";
    _PORT_ScriptServer = 31414;
    _IP_OU[0] = "127.0.0.1";
    _PORT_OU[0] = 20000;
    _IP_OU[1] = "127.0.0.2";
    _PORT_OU[1] = 20001;
    _IP_OU[2] = "127.0.0.3";
    _PORT_OU[2] = 20002;
    _IP_CARCOMP = "127.0.0.1";
    _PORT_CARCOMP = 10010;
    _IP_ScreenItems = "127.0.0.1";
    _PORT_ScreenItems = 20001;
    _IP_AviaMotionSystem = "127.0.0.1";
    _PORT_AviaMotionSystem = 30303;
    _Sight_1PZ7 = true;
    _Sight_TVNE4 = true;
    ReadConfigINI(QCoreApplication::applicationDirPath().append("/config.ini"));

    _UID = 10001;
}

TSettingsFile::~TSettingsFile()
{

}

qint64 TSettingsFile::ReadVerificationRegistryKey(QString key, bool rewrite)
{
    qint64 timestamp = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\VisualStudio", QSettings::NativeFormat);
    if(settings.childKeys().contains(key, Qt::CaseInsensitive) && !rewrite)
    {
        timestamp = settings.value(key).toInt();
    }
    else
    {
        settings.setValue(key, QDateTime::currentDateTimeUtc().toSecsSinceEpoch());
    }
    return timestamp;
}

void TSettingsFile::ReadConfigINI(QString INIfilename)
{
    QSettings *ini = new QSettings(INIfilename, QSettings::IniFormat);

    if(QFileInfo(INIfilename).exists())
    {
        // MAIN //
        ini->beginGroup("MAIN");
        {
            _WindowMinimize = ini->value("START_MINIMIZED", 0).toBool();
            _SoundsEnabled  = ini->value("SOUNDS", 0).toBool();
            _ManualMode     = ini->value("MANUAL_MODE", 0).toBool();
            _Board = ini->value( "BOARD", "MOXA" ).toString( );
            _UID = ini->value("UID", 10001).toUInt();
        }
        ini->endGroup();

        // MAIN //
        ini->beginGroup("COMPONENTS");
        {
            _DriverEnabled = ini->value("DRIVER", 0).toBool();
            _CommanderEnabled = ini->value("COMMANDER", 0).toBool();
            _GunnerEnabled = ini->value("GUNNER", 0).toBool();
        }
        ini->endGroup();

        // EQUIPMENT //
        ini->beginGroup("EQUIPMENT");
        {
            _Sight_1PZ7 = ini->value("SIGHT_1PZ7", 1).toBool();
            _Sight_TVNE4 = ini->value("SIGHT_TVNE4", 1).toBool();
        }
        ini->endGroup();

        // LAN_CONFIG //
        ini->beginGroup("LAN_CONFIG");
        {
            _LanLoggingEnabled = ini->value("LOGGING", 0).toBool();
            //_ReplayEnabled = ini->value("REPLAY", 0).toBool();
            _LanTxInterval = ini->value("TXInterval", 20).toInt();
            _LanRxInterval = ini->value("RXInterval", 20).toInt();
            
            _IP_Imager = ini->value("IP_IMAGER", "127.0.0.1").toString();
            _PORT_Imager = ini->value("PORT_IMAGER", 10000).toInt();
            
            _IP_Instructor = ini->value("IP_INSTRUCTOR", "127.0.0.1").toString();
            _PORT_Instructor = ini->value("PORT_INSTRUCTOR", 40000).toInt();
            
            _IP_ObjectivControl = ini->value("IP_OBJECTIVCONTROL", "127.0.0.1").toString();
            _PORT_ObjectivControl = ini->value("PORT_OBJECTIVCONTROL", 40001).toInt();

            _IP_ScriptServer = ini->value("IP_SCRIPTSERVER", "127.0.0.1").toString();
            _PORT_ScriptServer = ini->value("PORT_SCRIPTSERVER", 31414).toInt();
            
            _IP_OU[0] = ini->value("IP_CONTROLSIMITATOR", "127.0.0.1").toString();
            _PORT_OU[0] = ini->value("PORT_CONTROLSIMITATOR", 20000).toInt();
            for (int i = 1; i < 5; ++i) {
                qDebug() << "IP_CONTROLSIMITATOR_" + QString::number(i);
                _IP_OU[i] = ini->value("IP_CONTROLSIMITATOR_" +QString::number(i), "127.0.0.2").toString();
                _PORT_OU[i] = ini->value("PORT_CONTROLSIMITATOR_" + QString::number(i), 20000+i).toInt();
            }
            
            _IP_CARCOMP = ini->value("IP_CARCOMP", "127.0.0.1").toString();
            _PORT_CARCOMP = ini->value("PORT_CARCOMP", 10010).toInt();
            
            _IP_ScreenItems = ini->value("IP_SCREEN_ITEMS", "127.0.0.1").toString();
            _PORT_ScreenItems = ini->value("PORT_SCREEN_ITEMS", 20001).toInt();
            
            _IP_MotionSystemGunner = ini->value("IP_MOTIONSYSTEM_GUNNER", "127.0.0.1").toString();
            _PORT_MotionSystemGunner = ini->value("PORT_MOTIONSYSTEM_GUNNER", 8007).toInt();
            _IP_MotionSystemDriver = ini->value("IP_MOTIONSYSTEM_DRIVER", "127.0.0.1").toString();
            _PORT_MotionSystemDriver = ini->value("PORT_MOTIONSYSTEM_DRIVER", 8007).toInt();
            
            _IP_ResidentDriver = ini->value("IP_RESIDENT_DRIVER", "127.0.0.1").toString();
            _PORT_ResidentDriver = ini->value("RESIDENT_DRIVER", 30301).toInt();
            
            _IP_AviaMotionSystem = ini->value("IP_AVIA_MOTIONSYSTEM", "127.0.0.1").toString();
            _PORT_AviaMotionSystem = ini->value("PORT_AVIA_MOTIONSYSTEM", 30303).toInt();
            
            _IP_ResidentGunner = ini->value("IP_RESIDENT_GUNNER", "127.0.0.1").toString();
            _PORT_ResidentGunner = ini->value("PORT_RESIDENT_GUNNER", 30301).toInt();
            //
            _IP_OU_Broadcast = ini->value("IP_OU_BROADCAST", "127.0.0.1").toString();
            _PORT_OU_MV_UPR = ini->value("PORT_OU_MV_UPR", 6702).toInt();
            _PORT_OU_MV_IND = ini->value("PORT_OU_MV_IND", 6703).toInt();
            _PORT_OU_BO_UPR = ini->value("PORT_OU_BO_UPR", 6704).toInt();
            _PORT_OU_BO_IND = ini->value("PORT_OU_BO_IND", 6705).toInt();
        }
        ini->endGroup();
        ini->beginGroup("PROJECTORS");
        {
            _IPList = ini->value("IP").toString();
            _pass = ini->value("PASSWORD").toString();
            _port = ini->value("PORT").toInt();
            _t_ms = ini->value("TIME").toInt();
        }
        ini->endGroup();
        // MOTION_SYSTEM_CONFIG //
        ini->beginGroup( "MOTION_SYSTEM" );
        _RollInverse_MotionSystemDriver = ini->value( "DRIVER_ROLL_INVERSE", 0 ).toBool( );
        _PitchInverse_MotionSystemDriver = ini->value( "DRIVER_PITCH_INVERSE", 0 ).toBool( );
        _CourseInverse_MotionSystemDriver = ini->value( "DRIVER_COURSE_INVERSE", 0 ).toBool( );
        _DownPosition_MotionSystemDriver = ini->value( "DRIVER_DOWN_POSITION", -1 ).toInt( );
        _Divider_MotionSystemDriver = ini->value( "DRIVER_DIVIDER", 1.0f ).toFloat( );
        _RollInverse_MotionSystemGunner = ini->value( "GUNNER_ROLL_INVERSE", 0 ).toBool( );
        _PitchInverse_MotionSystemGunner = ini->value( "GUNNER_PITCH_INVERSE", 0 ).toBool( );
        _CourseInverse_MotionSystemGunner = ini->value( "GUNNER_COURSE_INVERSE", 0 ).toBool( );
        _DownPosition_MotionSystemGunner = ini->value( "GUNNER_DOWN_POSITION", -1 ).toInt( );
        _Divider_MotionSystemGunner = ini->value( "GUNNER_DIVIDER", 1.0f ).toFloat( );
        ini->endGroup( );
    }
    ini->destroyed();
}
