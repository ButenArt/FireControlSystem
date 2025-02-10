#include "CameraProcessor.h"
#include <QDebug>
#include <QFileInfo>
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
    timer->start(20);
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
    if (!cap.isOpened()) {
        qWarning() << "Ошибка: Камера не открыта!";
        return;
    }

    cv::Mat frame;
    if (!cap.read(frame)) {
        qWarning() << "Ошибка: Не удалось захватить кадр!";
        return;
    }

    // Обработка YOLO
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::String> layerNames = net.getUnconnectedOutLayersNames();
    std::vector<cv::Mat> outs;
    outs.clear();
    net.forward(outs, layerNames);

    int personCenterX = -1;
    int personCenterY = -1;
    cv::Rect personBox;

    // Обнаружение человека и рисование прямоугольника
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
                if (classId == 0) { // Человек
                    int centerX = static_cast<int>(output.at<float>(i, 0) * frame.cols);
                    int centerY = static_cast<int>(output.at<float>(i, 1) * frame.rows);
                    int width = static_cast<int>(output.at<float>(i, 2) * frame.cols);
                    int height = static_cast<int>(output.at<float>(i, 3) * frame.rows);

                    // Вычисляем координаты прямоугольника
                    int x = centerX - width / 2;
                    int y = centerY - height / 2;
                    personBox = cv::Rect(x, y, width, height);

                    // Рисуем рамку
                    cv::rectangle(frame, personBox, cv::Scalar(0, 255, 0), 2);

                    personCenterX = centerX;
                    personCenterY = centerY;
                    break;
                }
            }
        }
    }

    if (personCenterX != -1 && personCenterY != -1) {
        angle = calculateAngle(personCenterX, frame.cols);
        pitchAngle = calculatePitchAngle(personCenterY, frame.rows);
    }

    if (frame.empty() || !frame.data) {
        qCritical() << "Ошибка: OpenCV frame пуст!";
        return;
    }

    // Конвертация в QImage
    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
    QImage image(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
    image = image.copy(); // Создаём копию, чтобы OpenCV не изменял данные

    if (image.isNull()) {
        qCritical() << "Ошибка: `QImage` оказался пуст!";
        return;
    }

    emit frameReady(image, angle, pitchAngle);
}

void CameraProcessor::loadYOLOModel()
{
    std::string modelConfig = "D:/!REPOS/FireControlSystem/FireControlSystem/modelCV/yolov4-tiny.cfg";
    std::string modelWeights = "D:/!REPOS/FireControlSystem/FireControlSystem/modelCV/yolov4-tiny.weights";
    std::string classFile = "D:/!REPOS/FireControlSystem/FireControlSystem/modelCV/coco.names";

    // Проверка существования файлов
    if (!QFileInfo::exists(QString::fromStdString(modelConfig))) {
        qCritical() << "Ошибка: не найден файл конфигурации YOLO:" << QString::fromStdString(modelConfig);
        return;
    }
    if (!QFileInfo::exists(QString::fromStdString(modelWeights))) {
        qCritical() << "Ошибка: не найден файл весов YOLO:" << QString::fromStdString(modelWeights);
        return;
    }
    if (!QFileInfo::exists(QString::fromStdString(classFile))) {
        qCritical() << "Ошибка: не найден файл классов YOLO:" << QString::fromStdString(classFile);
        return;
    }

    // Загружаем модель YOLOv4-Tiny
    try {
        net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    } catch (const cv::Exception &e) {
        qCritical() << "OpenCV ошибка загрузки модели:" << e.what();
        return;
    }

    qDebug() << "YOLOv4-Tiny загружен!";

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

double CameraProcessor::calculatePitchAngle(int centerY, int imageHeight)
{
    double verticalFOV = 45.0;  // Поле зрения камеры по вертикали (градусы)
    double pixelsPerDegree = imageHeight / verticalFOV;
    double pitchAngle = (imageHeight / 2 - centerY) / pixelsPerDegree;
    return pitchAngle;
}

