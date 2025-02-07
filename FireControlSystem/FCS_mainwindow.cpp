#include "FCS_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), cameraProcessor(new CameraProcessor(0, this))
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    videoLabel = new QLabel(this);
    layout->addWidget(videoLabel);

    connect(cameraProcessor, &CameraProcessor::frameReady, this, &MainWindow::updateFrame);
    cameraProcessor->start();
}

MainWindow::~MainWindow()
{
    cameraProcessor->stop();
}

void MainWindow::updateFrame(const QImage &image, double angle)
{
    videoLabel->setPixmap(QPixmap::fromImage(image).scaled(videoLabel->size(), Qt::KeepAspectRatio));
    setWindowTitle(QString("Angle: %1°").arg(angle));
}
