#include "FCS_mainwindow.h"
#include "qdebug.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Устанавливаем размер окна 1920x1080
    resize(1920, 1080);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    videoLabel = new QLabel(this);
    videoLabel->setText("Загрузка...");
    videoLabel->setAlignment(Qt::AlignCenter);

    // Делаем QLabel адаптивным к размеру окна
    videoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoLabel->setMinimumSize(640, 480);

    layout->addWidget(videoLabel);

    QThread *cameraThread = new QThread;
    cameraProcessor = new CameraProcessor(0);
    cameraProcessor->moveToThread(cameraThread);

    connect(cameraThread, &QThread::started, this, [this]() { cameraProcessor->start(); });
    connect(cameraProcessor, &CameraProcessor::frameReady, this, &MainWindow::updateFrame, Qt::QueuedConnection);
    connect(cameraThread, &QThread::finished, cameraProcessor, &QObject::deleteLater);
    connect(this, &MainWindow::destroyed, cameraThread, &QThread::quit);
    connect(this, &MainWindow::destroyed, cameraProcessor, &QObject::deleteLater);

    cameraThread->start();

    ini = new TSettingsFile();
    log = new TLogging(ini);

    TcpServer = new TCP_server();
    TcpServer->setPort(ini->PORT_Server());

    UdpServer = new UDP_server();
    UdpServer->setAddress(ini->IP_Gun());
    UdpServer->setPort(ini->PORT_Gun());

    connect(TcpServer, &TCP_server::readyShot, this, &MainWindow::readyShotSlot);
    connect(this, &MainWindow::readyShot, UdpServer, &UDP_server::sendData);
}

MainWindow::~MainWindow()
{
    cameraProcessor->stop();
    qDebug() << "MainWindow уничтожен";
}

void MainWindow::updateFrame(const QImage &image, double angle, double pitch)
{
    // Обновляем GUI через главный поток
    QMetaObject::invokeMethod(this, [this, image, angle, pitch]() {
            if (!videoLabel) {
                qCritical() << "Ошибка: `videoLabel` не создан!";
                return;
            }
            if (image.isNull()) {
                qCritical() << "Ошибка: `QImage` оказался пуст перед установкой!";
                return;
            }
            if (image.format() == QImage::Format_Invalid) {
                qCritical() << "Ошибка: `QImage` имеет неверный формат!";
                return;
            }
            if (!ini->ManualMode()){
                azimuthAngle = angle;
                pitchAngle = pitch;
                readyShotSlot(false);
            }
            QImage fixedImage = image.convertToFormat(QImage::Format_RGB32);
            //videoLabel->setText(QString("Angle: %1°").arg(angle));
            videoLabel->setPixmap(QPixmap::fromImage(fixedImage).scaled(videoLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::FastTransformation));
            // Отображаем оба угла в заголовке окна
            setWindowTitle(QString("Azimuth: %1°, Pitch: %2°").arg(azimuthAngle).arg(pitchAngle));
        }, Qt::QueuedConnection);
}

void MainWindow::readyShotSlot(bool shot){
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream.setByteOrder(QDataStream::LittleEndian);
    data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    data_stream << static_cast<quint16>(azimuthAngle);
    data_stream << static_cast<quint16>(pitchAngle);
    data_stream << static_cast<quint16>(shot);
    emit readyShot(data);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!ini->ManualMode()){
        return;
    }
    switch(event->key())
    {
        case Qt::Key_Up:
        {
            pitchAngle += 0.1;
            if (pitchAngle > 20)
                pitchAngle = 20;
            readyShotSlot(false);
        }
        break;
        case Qt::Key_Down:
        {
            pitchAngle -= 0.1;
            if (pitchAngle < -20)
                pitchAngle = -20;
            readyShotSlot(false);
        }
        break;
        case Qt::Key_Left:
        {
            azimuthAngle -= 0.1;
            if (azimuthAngle < -25)
                azimuthAngle = -25;
            readyShotSlot(false);
        }
        break;
        case Qt::Key_Right:
        {
            azimuthAngle += 0.1;
            if (azimuthAngle > 25)
                azimuthAngle = 25;
            readyShotSlot(false);
        }
        break;
        case Qt::Key_Tab:
            readyShotSlot(true);
        break;
    }
}
