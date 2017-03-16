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

using namespace std;

class ManagementData;

class Management
{
public:
    Management();
    Management(const Management &);
    Management &operator=(const Management &);
    ~Management();

    shared_ptr<QImage> Management::sourceImage;
    shared_ptr<QImage> Management::currentImage;
    shared_ptr<QLabel> Management::imageLabel;

    void Management::setImageLabel (QLabel *qLabel);
    void Management::loadImage (QString path);
    void Management::saveImage (QString path);
    void Management::reloadImage();
    void Management::drawImage();
    void Management::normalize();


private:
    QSharedDataPointer<ManagementData> data;
};

#endif // MANAGEMENT_H
