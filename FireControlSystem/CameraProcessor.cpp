#include "CameraProcessor.h"
#include <QDebug>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <fstream>

CameraProcessor::CameraProcessor(int cameraIndex, QObject *parent)
    : QObject(parent), cameraIndex(cameraIndex), timer(new QTimer())
{
    loadYOLOModel();
    timer->start(20);
    connect(timer, &QTimer::timeout, this, &CameraProcessor::processFrame);
}

CameraProcessor::~CameraProcessor()
{
    stop();
}

void CameraProcessor::start()
{
    QMutexLocker locker(&mutex);
    if (!cap.isOpened()) {
        cap.open(cameraIndex);
        if (!cap.isOpened()) {
            qWarning() << "Ошибка открытия камеры!";
            return;
        }
    }
    timer->start(30);
}

void CameraProcessor::stop()
{
    QMutexLocker locker(&mutex);
    if (cap.isOpened()) {
        cap.release();
    }
    timer->stop();
}

void CameraProcessor::processFrame()
{
    QMutexLocker locker(&mutex);
    cv::Mat frame;
    if (!cap.read(frame)) {
        qWarning() << "Ошибка захвата кадра!";
        return;
    }

    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());

    double angle = 0.0;
    int personCenterX = -1;

    float confidenceThreshold = 0.5;
    for (const auto &output : outs) {
        for (int i = 0; i < output.rows; i++) {
            float confidence = output.at<float>(i, 4);

            if (confidence > confidenceThreshold) {
                int classId = -1;
                float maxConfidence = 0;

                for (int j = 5; j < output.cols; j++) {
                    float score = output.at<float>(i, j);
                    if (score > maxConfidence) {
                        maxConfidence = score;
                        classId = j - 5;
                    }
                }

                if (classId == 0) { // "person" в COCO dataset
                    int centerX = static_cast<int>(output.at<float>(i, 0) * frame.cols);
                    int centerY = static_cast<int>(output.at<float>(i, 1) * frame.rows);
                    int width = static_cast<int>(output.at<float>(i, 2) * frame.cols);
                    int height = static_cast<int>(output.at<float>(i, 3) * frame.rows);

                    cv::Rect box(centerX - width / 2, centerY - height / 2, width, height);
                    cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 3);

                    personCenterX = centerX;
                    break; // Берем первого найденного человека
                }
            }
        }
    }

    if (personCenterX != -1) {
        angle = calculateAngle(personCenterX, frame.cols);
    }

    QImage qimg = matToQImage(frame);
    emit frameReady(qimg, angle);
}

void CameraProcessor::loadYOLOModel()
{
    std::string modelConfig = "yolov4-tiny.cfg";
    std::string modelWeights = "yolov4-tiny.weights";
    std::string classFile = "coco.names";

    // Загружаем модель YOLOv4-Tiny
    net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);

    // Включаем CUDA
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);

    // Загружаем классы COCO
    std::ifstream ifs(classFile);
    std::string line;
    while (std::getline(ifs, line)) {
        classNames.push_back(line);
    }

    qDebug() << "YOLOv4-Tiny загружен с CUDA!";
}

QImage CameraProcessor::matToQImage(const cv::Mat &mat)
{
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
}

double CameraProcessor::calculateAngle(int centerX, int imageWidth)
{
    double fov = 60.0;
    double pixelsPerDegree = imageWidth / fov;
    double angle = (centerX - imageWidth / 2) / pixelsPerDegree;
    return angle;
}
