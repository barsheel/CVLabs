#include "management.h"
#include "rawimage.h"
class ManagementData : public QSharedData
{
public:

};

Management::Management() : data(new ManagementData)
{

}

Management::Management(const Management &rhs) : data(rhs.data)
{

}

Management &Management::operator=(const Management &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Management::~Management()
{

}




void Management::setImageLabel (QLabel *qLabel)
{
    imageLabel = shared_ptr<QLabel>(qLabel);
}

void Management::loadImage (QString path)
{
    Management::sourceImage = shared_ptr<QImage>(new QImage(path));
    Management::currentImage = shared_ptr<QImage>(new QImage(path));
    Management::drawImage();
}

void Management::saveImage (QString path)
{
    Management::currentImage.get()->save(path);
}

void Management::reloadImage()
{
    Management::currentImage.reset();
    Management::currentImage = shared_ptr<QImage>(new QImage(*(sourceImage.get())));
    Management::drawImage();
}

void Management::drawImage()
{
    Management::imageLabel->setPixmap( QPixmap::fromImage(*(Management::currentImage.get()), Qt::AutoColor));
}

void Management::normalize()
{
    unique_ptr<RawImage> raw((new RawImage(*Management::currentImage.get()))->normalize());
    Management::currentImage = raw->toQImage();
    drawImage();
}

