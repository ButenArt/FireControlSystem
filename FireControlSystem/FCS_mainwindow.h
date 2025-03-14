#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include "CameraProcessor.h"
#include "TcpServer.h"
#include "UdpServer.h"
#include "SettingsFile.h"
#include "Logging.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFrame(const QImage &image, double angle, double pitch);
    void readyShotSlot();
    void keyPressEvent(QKeyEvent *event);
private:
    bool shot = true;
    double azimuthAngle = 0;
    double pitchAngle = 0;
    CameraProcessor *cameraProcessor;
    QLabel *videoLabel;
    TCP_server *TcpServer;
    UDP_server *UdpServer;
    TSettingsFile *ini;
    TLogging *log;
    QTimer *sendTimer;
signals:
    void readyShot(QByteArray data);
};

#endif // MAINWINDOW_H
