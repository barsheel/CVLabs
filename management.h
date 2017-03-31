#ifndef MANAGEMENT_H
#define MANAGEMENT_H

//#include <QDeclarativeItem>
#include <QMainWindow>
#include <QObject>
//#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>
#include <QLabel>
#include <memory>
#include "lab1.h"

using namespace std;

class ManagementData;

class Management
{
public:
    Management();
    Management(const Management &);
    Management &operator=(const Management &);
    ~Management();

    unique_ptr<QImage> Management::sourceImage;
    unique_ptr<QImage> Management::currentImage;
    unique_ptr<QLabel> Management::imageLabel;

    void Management::setImageLabel (QLabel *qLabel);
    void Management::loadImage (QString path);
    void Management::saveImage (QString path);
    void Management::reloadImage();
    void Management::drawImage();
    void Management::normalize();
    void Management::gauss(int strength, int borderType);
    void Management::sobel(int borderType);

private:
    QSharedDataPointer<ManagementData> data;
};

#endif // MANAGEMENT_H
