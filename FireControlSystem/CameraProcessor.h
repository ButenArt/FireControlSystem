#ifndef CAMERAPROCESSOR_H
#define CAMERAPROCESSOR_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QTimer>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class CameraProcessor : public QObject
{
    Q_OBJECT

public:
    explicit CameraProcessor(int cameraIndex = 0, QObject *parent = nullptr);
    ~CameraProcessor();

    void start();
    void stop();

signals:
    void frameReady(const QImage &image, double angle, double pitch);

private slots:
    void processFrame();

private:
    int cameraIndex;
    cv::VideoCapture cap;
    QTimer *timer;
    QMutex mutex;

    cv::dnn::Net net;
    std::vector<std::string> classNames;

    double angle = 0.0;
    double pitchAngle = 0.0;

    QImage matToQImage(const cv::Mat &mat);
    double calculateAngle(int centerX, int imageWidth);
    double calculatePitchAngle(int centerY, int imageHeight);
    void loadYOLOModel();
};

#endif // CAMERAPROCESSOR_H
