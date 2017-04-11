#include "management.h"
#include "rawimage.h"
#include "pyramid.h"
#include "qcoreapplication.h"
#include "qdir.h"
#include "qfile.h"
#define GAUSS_STRENGTH 0.9

class ManagementData : public QSharedData
{
public:

};

Management::Management() : data(new ManagementData){}

Management::Management(const Management &rhs) : data(rhs.data){}

Management &Management::operator=(const Management &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Management::~Management(){}




void Management::setImageLabel (QLabel *qLabel)
{
    imageLabel = unique_ptr<QLabel>(qLabel);
}


void Management::loadImage (QString path)
{
    Management::sourceImage = unique_ptr<QImage>(new QImage(path));
    Management::currentImage = unique_ptr<QImage>(new QImage(path));
    Management::drawImage();

}


void Management::saveImage (QString path)
{
    Management::currentImage.get()->save(path);
}


void Management::reloadImage()
{
    Management::currentImage.reset();
    Management::currentImage = unique_ptr<QImage>(new QImage(*(sourceImage.get())));
    Management::drawImage();
}


void Management::drawImage()
{
    unique_ptr<RawImage> raw((new RawImage(*Management::currentImage.get())));
    Management::currentImage = make_unique<QImage>(raw.get()->toQImage());
    Management::imageLabel->setPixmap( QPixmap::fromImage(*(Management::currentImage.get()), Qt::AutoColor));
}


void Management::normalize()
{
    unique_ptr<RawImage> raw((new RawImage(*Management::currentImage.get())));
    raw = make_unique<RawImage>(raw.get()->normalize());
    Management::currentImage = make_unique<QImage>(raw.get()->toQImage());
    drawImage();
}


void Management::gauss(int strength, int borderType)
{
    unique_ptr<RawImage> raw((new RawImage(*Management::currentImage.get())));
    raw = make_unique<RawImage>(
                Lab1:: lab1Gauss(*(raw.get()), GAUSS_STRENGTH, BORDER_PROCESSING_TYPE_MIRROR));
    Management::currentImage = make_unique<QImage>(raw->toQImage());
    drawImage();
}


void Management::sobel(int borderType)
{
    unique_ptr<RawImage> raw((new RawImage(*Management::currentImage.get())));
    raw = make_unique<RawImage>(
                Lab1:: lab1Sobel(*(raw.get()), BORDER_PROCESSING_TYPE_MIRROR));
    raw = make_unique<RawImage>(raw.get()->normalize());
    Management::currentImage = make_unique<QImage>(raw->toQImage());
    drawImage();
}

void Management::pyramid()
{
    QDir dir(QCoreApplication::applicationDirPath() + "\\Pyramid\\");
    QStringList filter;
    filter.append("*.jpg *.jpeg");
    QStringList files = dir.entryList(filter, QDir::Files);
    foreach(QString file, files){
        QFile(file).remove();
    }

    RawImage raw(*Management::currentImage.get());
    Pyramid pyramid(raw, 0.5, 1.6, 5);
    qInfo()<<"DONE";
    RawImage result(raw.width, raw.height);
    for(float k = 1.0; k < 16; k += 1){
        for (int y = 0; y < result.height; y++) {
            for (int x = 0; x < result.width; x++) {
                result.data[y * result.width + x] = pyramid.L(x, y, k);
            }
        }
        result.toQImage().save(QCoreApplication::applicationDirPath() + "\\Pyramid\\LResult" + QString::number(k) + ".jpeg");
    }
}
