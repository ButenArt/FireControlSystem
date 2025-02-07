#ifndef SETTINGSFILE_H
#define SETTINGSFILE_H

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QDateTime>
#include <Windows.h>
#include <array>

const auto N_UNITS = 16;

class TSettingsFile : public QObject
{
	Q_OBJECT

public:
	TSettingsFile( );
	~TSettingsFile( );

	bool WindowMinimize( ) const { return _WindowMinimize; }
	bool CanLoggingEnabled( ) const { return _CanLoggingEnabled; }
	bool LanLoggingEnabled( ) const { return _LanLoggingEnabled; }
	bool SoundsEnabled( ) const { return _SoundsEnabled; }
	bool ManualMode( ) const { return _ManualMode; }
	WORD CanTxInterval( ) const { return _CanTxInterval; }
	WORD CanRxInterval( ) const { return _CanRxInterval; }
	QString Board( ) const { return _Board; }
	bool ReplayEnabled( ) { return _ReplayEnabled; }
	bool DriverEnabled( ) { return _DriverEnabled; }
	bool CommanderEnabled( ) { return _CommanderEnabled; }
	bool GunnerEnabled( ) { return _GunnerEnabled; }
	bool Sight_1PZ7( ) { return _Sight_1PZ7; }
	bool Sight_TVNE4( ) { return _Sight_TVNE4; }
	float AAMGElevationAngle( ) { return _AAMGElevationAngle; }
	float AAMGDepressionAngle( ) { return _AAMGDepressionAngle; }

	// TODO: Внимание! тут дополнительно захардкожены параметры конфигурации, количество элементов! Будте внимательны!
	// Конфиг расчитан на создание до 16 юнитов каждого типа, кроме VID, у которых свои приколы с адресами
	// массивы объявлены через array и должны кидать исключения при выходе за границу, так и не проверил
	WORD MABUnitAddr( size_t unit ) const { return _MAB[unit].Addr; }
	BYTE MABUnitPort( size_t unit ) const { return _MAB[unit].Port; }
	bool MABUnitEnable( size_t unit ) const { return _MAB[unit].Enable; }

	WORD VIDUnitAddr( size_t unit ) const    { assert( unit < N_UNITS ); return _VID[unit].Addr; }
	BYTE VIDUnitPort( size_t unit ) const    { assert( unit < N_UNITS ); return _VID[unit].Port; }
	bool VIDUnitEnable( size_t unit ) const  { assert( unit < N_UNITS ); return _VID[unit].Enable; }

	WORD GPIOINUnitAddr( size_t unit ) const { assert( unit < N_UNITS ); return _GPIOIN[unit].Addr; }
	BYTE GPIOINUnitPort( size_t unit ) const { assert( unit < N_UNITS ); return _GPIOIN[unit].Port; }
	bool GPIOINUnitEnable( size_t unit ) const { assert( unit >= 0 && unit < N_UNITS ); return _GPIOIN[unit].Enable; }

	WORD GPIOOUTUnitAddr( size_t unit ) const { assert( unit < N_UNITS ); return _GPIOOUT[unit].Addr; }
	BYTE GPIOOUTUnitPort( size_t unit ) const { assert( unit < N_UNITS ); return _GPIOOUT[unit].Port; }
	bool GPIOOUTUnitEnable( size_t unit ) const { assert( unit < N_UNITS ); return _GPIOOUT[unit].Enable; }

	WORD ENUMUnitAddr( size_t unit ) const { return _ENUM[unit].Addr; }
	BYTE ENUMUnitPort( size_t unit ) const { return _ENUM[unit].Port; }
	bool ENUMUnitEnable( size_t unit ) const { return _ENUM[unit].Enable; }

	WORD POTUnitAddr( size_t unit ) const { assert( unit < N_UNITS ); return _POT[unit].Addr; }
	BYTE POTUnitPort( size_t unit ) const { assert( unit < N_UNITS ); return _POT[unit].Port; }
	bool POTUnitEnable( size_t unit ) const { assert( unit < N_UNITS ); return _POT[unit].Enable; }

	WORD ENCUnitAddr( size_t unit ) const { assert( unit < N_UNITS ); return _ENC[unit].Addr; }
	BYTE ENCUnitPort( size_t unit ) const { assert( unit < N_UNITS ); return _ENC[unit].Port; }
	bool ENCUnitEnable( size_t unit ) const { assert( unit < N_UNITS ); return _ENC[unit].Enable; }

	bool DPDLL_Driver_Enable( ) { return _DPDLL_Driver_Enable; }
	bool DPDLL_Gunner_Enable( ) { return _DPDLL_Gunner_Enable; }
	bool RollInverse_MotionSystemDriver( ) { return _RollInverse_MotionSystemDriver; }
	bool PitchInverse_MotionSystemDriver( )  { return _PitchInverse_MotionSystemDriver; }
	bool CourseInverse_MotionSystemDriver( ) { return _CourseInverse_MotionSystemDriver; }
	qint32 DownPosition_MotionSystemDriver( ) { return _DownPosition_MotionSystemDriver; }
	float Divider_MotionSystemDriver( )  { return _Divider_MotionSystemDriver; }
	bool RollInverse_MotionSystemGunner( ) { return _RollInverse_MotionSystemGunner; }
	bool PitchInverse_MotionSystemGunner( ) { return _PitchInverse_MotionSystemGunner; }
	bool CourseInverse_MotionSystemGunner( ) { return _CourseInverse_MotionSystemGunner; }
	qint32 DownPosition_MotionSystemGunner( ) { return _DownPosition_MotionSystemGunner; }
	float Divider_MotionSystemGunner( )  { return _Divider_MotionSystemGunner; }
	WORD LanTxInterval( ) const { return _LanTxInterval; }
	WORD LanRxInterval( ) const { return _LanRxInterval; }
	QString IP_Imager( ) const { return _IP_Imager; }
	WORD PORT_Imager( ) const { return _PORT_Imager; }
	QString IP_Instructor( ) const { return _IP_Instructor; }
	WORD PORT_Instructor( ) const { return _PORT_Instructor; }
	QString IP_ObjectivControl( ) const { return _IP_ObjectivControl; }
	WORD PORT_ObjectivControl( ) const { return _PORT_ObjectivControl; }
	QString IP_ScriptServer( ) const { return _IP_ScriptServer; }
	WORD PORT_ScriptServer( ) const { return _PORT_ScriptServer; }
	QString IP_OU( int ind = 0 ) const { return _IP_OU[ind]; }
	WORD PORT_OU( int ind = 0 ) const { return _PORT_OU[ind]; }
	QString IP_ControlsImitator( ) { return _IP_ControlsImitator; }
	WORD PORT_ControlsImitator( ) { return _PORT_ControlsImitator; }
	QString IP_MBT2000_FCC( ) { return _IP_MBT2000_FCC; }
	WORD PORT_MBT2000_FCC( ) { return _PORT_MBT2000_FCC; }
	QString IP_T59G_FCC( ) { return _IP_T59G_FCC; }
	WORD PORT_T59G_FCC( ) { return _PORT_T59G_FCC; }
	QString IP_CARCOMP( ) { return _IP_CARCOMP; }
	WORD PORT_CARCOMP( ) { return _PORT_CARCOMP; }
	QString IP_ScreenItems( ) { return _IP_ScreenItems; }
	WORD PORT_ScreenItems( ) { return _PORT_ScreenItems; }
	QString IP_MotionSystemDriver( ) { return _IP_MotionSystemDriver; }
	WORD PORT_MotionSystemDriver( ) { return _PORT_MotionSystemDriver; }
	QString IP_ResidentDriver( ) { return _IP_ResidentDriver; }
	WORD PORT_ResidentDriver( ) { return _PORT_ResidentDriver; }
	QString IP_MotionSystemGunner( ) { return _IP_MotionSystemGunner; }
	WORD PORT_MotionSystemGunner( ) { return _PORT_MotionSystemGunner; }
	QString IP_ResidentGunner( ) { return _IP_ResidentGunner; }
	WORD PORT_ResidentGunner( ) { return _PORT_ResidentGunner; }
	QString IP_OU_Broadcast() { return _IP_OU_Broadcast; }
	WORD PORT_OU_MV_UPR() { return _PORT_OU_MV_UPR; }
	WORD PORT_OU_MV_IND() { return _PORT_OU_MV_IND; }
	WORD PORT_OU_BO_UPR() { return _PORT_OU_BO_UPR; }
	WORD PORT_OU_BO_IND() { return _PORT_OU_BO_IND; }

	QString IP_CARCOMP( ) const { return _IP_CARCOMP; }
	WORD PORT_CARCOMP( ) const { return _PORT_CARCOMP; }
	QString IP_ScreenItems( ) const { return _IP_ScreenItems; }
	WORD PORT_ScreenItems( ) const { return _PORT_ScreenItems; }
	QString IP_MotionSystemDriver() const { return _IP_MotionSystemDriver; }
	WORD PORT_MotionSystemDriver() const { return _PORT_MotionSystemDriver; }
	QString IP_ResidentDriver() const { return _IP_ResidentDriver; }
	WORD PORT_ResidentDriver() const { return _PORT_ResidentDriver; }
	QString IP_MotionSystemGunner( ) const { return _IP_AviaMotionSystem; }
	WORD PORT_MotionSystemGunner( ) const { return _PORT_AviaMotionSystem; }
	QString IP_AU( ) { return   _IP_AU; }
	WORD  PORT_AU( ) { return _PORT_AU; }
	QString IPList( ) const { return _IPList; }
	QString pass( ) const { return _pass; }
	int port( ) const { return _port; }
	int t_ms( ) const { return _t_ms; }

	qint64 ReadVerificationRegistryKey( QString key, bool rewrite );


    quint32 Uid( ) const { return _UID; }

private:
	bool _WindowMinimize;          // сворачивать основное окно приложения
	bool _CanLoggingEnabled;       // флаг активации журналирования CAN
	bool _LanLoggingEnabled;       // флаг активации журналирования LAN
	bool _SoundsEnabled;           // флаг активации звуков
	bool _ManualMode;              // флаг режима работы сервера - ручное управление или CAN
	WORD _CanTxInterval;           // интервал в мс между отправкой CAN пакетов
	WORD _CanRxInterval;           // интервал в мс между приемом CAN пакетов
	QString _Board;                // строка с названием САН платы MOXA PCAN VIRT
	bool _ReplayEnabled;           // флаг активации сохранения сетевых пакетов для реплея
	bool _DriverEnabled;           // флаг активации компонентов механика-водителя
	bool _CommanderEnabled;        // флаг активации компонентов командира
	bool _GunnerEnabled;           // флаг активации компонентов наводчика
	bool _Sight_1PZ7;              // по комплектации установлен прицел 1ПЗ-7 или 1ПЗ-2
	bool _Sight_TVNE4;             // по комплектации установлен ПНВ ТВНЕ-4 или ТВН-5
	float _AAMGElevationAngle;     // угол подъема зенитного пулемета
	float _AAMGDepressionAngle;    // угол опускания зенитного пулемета

	struct CAN {                   // CAN agent
		WORD Addr;                 // адрес CAN блока 
		bool Enable;               // активность CAN блока
		BYTE Port;                 // порт CAN блока
	};

	std::array<CAN, N_UNITS>  _MAB;
	std::array<CAN, N_UNITS>  _VID;
	std::array<CAN, N_UNITS>  _GPIOIN;
	std::array<CAN, N_UNITS>  _GPIOOUT;
	std::array<CAN, N_UNITS>  _ENUM;
	std::array<CAN, N_UNITS>  _POT;
	std::array<CAN, N_UNITS>  _ENC;

	WORD _LanTxInterval = 20;						// интервал в мс отправки LAN пакетов
	WORD _LanRxInterval = 10;						// интервал в мс приема LAN пакетов
	QString _IP_Imager = "127.0.0.1";				// IP адрес TCP сервера визуализации
	WORD _PORT_Imager = 10000;						// порт TCP сервера визуализации
	QString _IP_Instructor = "127.0.0.1";			// IP адрес TCP сервера иструктора
	WORD _PORT_Instructor = 40000;					// порт TCP сервера иструктора
	QString _IP_ScriptServer = "127.0.0.1";			// IP адрес TCP сервера сценариев
	WORD _PORT_ScriptServer = 31414;				// порт TCP сервера сценариев

	QString _IP_OU[5] = { "127.0.0.1" };			// IP адрес TCP имитатора органов управления
	WORD _PORT_OU[5] = { 2000 };					// порт TCP имитатора органов управления

	QString _IP_AU = "127.0.0.1";		// IP адрес TCP имитатора органов управления
	WORD _PORT_AU = 20000;			// порт TCP имитатора органов управления

	QString _IP_ObjectivControl = "127.0.0.1";		// IP адрес ObjectivControl
	WORD _PORT_ObjectivControl = 40001;				// порт TCP ObjectivControl
	QString _IP_ControlsImitator = "127.0.0.1";		// IP адрес TCP имитатора органов управления
	WORD _PORT_ControlsImitator = 20000;			// порт TCP имитатора органов управления
	QString _IP_MBT2000_FCC = "127.0.0.1";			// IP адрес TCP имитатора компьютера управления огнем танка MBT2000
	WORD _PORT_MBT2000_FCC = 10001;					// порт TCP имитатора компьютера управления огнем танка MBT2000
	QString _IP_T59G_FCC = "127.0.0.1";				// IP адрес TCP имитатора компьютера управления огнем танка T59G
	WORD _PORT_T59G_FCC = 10001;					// порт TCP имитатора компьютера управления огнем танка T59G
	QString _IP_CARCOMP = "127.0.0.1";				// IP адрес TCP имитатора бортового компьютера автомобиля
	WORD _PORT_CARCOMP = 10010;						// порт TCP имитатора бортового компьютера автомобиля
	QString _IP_ScreenItems = "127.0.0.1";			// IP адрес TCP имитатора приборов на экране
	WORD _PORT_ScreenItems = 20001;					// порт TCP имитатора приборов на экране
	QString _IP_AviaMotionSystem = "127.0.0.1";		// IP адрес TCP сервера системы подвижности стрелка-наводчика
	WORD _PORT_AviaMotionSystem = 30303;			// порт TCP сервера системы подвижности стрелка-наводчика
	QString _IP_MotionSystemDriver = "127.0.0.1";	// IP адрес TCP сервера системы подвижности механика-водителя
	WORD _PORT_MotionSystemDriver = 30303;			// порт TCP сервера системы подвижности механика-водителя
	QString _IP_ResidentDriver = "127.0.0.1";		// IP адрес резидента механика-водителя
	WORD _PORT_ResidentDriver = 30301;				// порт UDP сервера резидента механика-водителя
	QString _IP_MotionSystemGunner = "127.0.0.1";	// IP адрес TCP сервера системы подвижности стрелка-наводчика
	WORD _PORT_MotionSystemGunner = 30303;			// порт TCP сервера системы подвижности стрелка-наводчика
	QString _IP_ResidentGunner = "127.0.0.1";		// IP адрес резидента стрелка-наводчика
	WORD _PORT_ResidentGunner = 30301;				// порт UDP сервера резидента стрелка-наводчика
	bool _DPDLL_Driver_Enable = false;              // для системы подвижности механика-водителя используется библиотека DP.DLL
	bool _DPDLL_Gunner_Enable = false;              // для системы подвижности стрелка-наводчика используется библиотека DP.DLL
	bool _RollInverse_MotionSystemDriver = false;   // инвертировать направление крена системы подвижности механика-водителя
	bool _PitchInverse_MotionSystemDriver = false;  // инвертировать направление тангажа системы подвижности механика-водителя
	bool _CourseInverse_MotionSystemDriver = false; // инвертировать направление курса системы подвижности механика-водителя
	qint32 _DownPosition_MotionSystemDriver = -1;   // нижнее положение системы подвижности механика-водителя
	float _Divider_MotionSystemDriver = 1.0f;       // коэффициент делителя системы подвижности механика-водителя
	bool _RollInverse_MotionSystemGunner = false;   // инвертировать направление крена системы подвижности стрелка-наводчика
	bool _PitchInverse_MotionSystemGunner = false;  // инвертировать направление тангажа системы подвижности стрелка-наводчика
	bool _CourseInverse_MotionSystemGunner = false; // инвертировать направление курса системы подвижности стрелка-наводчика
	qint32 _DownPosition_MotionSystemGunner = -1;   // нижнее положение системы подвижности стрелка-наводчика
	float _Divider_MotionSystemGunner = 1.0f;       // коэффициент делителя системы подвижности стрелка-наводчика

	QString _IPList;    // лист IP адресов проекторов
	QString _pass;      // пароль проекторов (если он есть)
	int _port = 4352;   // порт (4352 для PJLINK)
	int _t_ms = 0;      // частота опроса проекторов вкл/выкл

	QString _IP_OU_Broadcast;      // UDP OU broadcast IP
	WORD _PORT_OU_MV_UPR;          // UDP порт передачи данных UPR для приложения OU механика-водителя
	WORD _PORT_OU_MV_IND;          // UDP порт передачи данных IND для приложения OU механика-водителя
	WORD _PORT_OU_BO_UPR;          // UDP порт передачи данных UPR для приложения OU боевого отделения
	WORD _PORT_OU_BO_IND;          // UDP порт передачи данных IND для приложения OU боевого отделения

	void ReadConfigINI( QString INIfilename );

    quint32 _UID = 10001;
};

#endif // SETTINGSFILE_H
