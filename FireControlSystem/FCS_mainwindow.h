#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
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
    void readyShotSlot(bool shot);
    void keyPressEvent(QKeyEvent *event);
private:
    double azimuthAngle;
    double pitchAngle;
    CameraProcessor *cameraProcessor;
    QLabel *videoLabel;
    TCP_server *TcpServer;
    UDP_server *UdpServer;
    TSettingsFile *ini;
    TLogging *log;
signals:
    void readyShot(QByteArray data);
};

#endif // MAINWINDOW_H
