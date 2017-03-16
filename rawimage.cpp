#include "rawimage.h"
#include <QCoreApplication>
#include <string>
#include <iostream>
#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QPixmap>

QCoreApplication a();

RawImage::RawImage()
{

}

RawImage::RawImage(QImage source)
{
    width = source.width();
    height = source.height();
    QByteArray byteArray((char*)source.bits(), source.byteCount());
    //RawImage::data = new vector<double>(width * height * 3);
    data.reserve(width * height * 3);
    for(int i = 0; i < width*height; i++) {
        data.push_back(((double)((uchar)byteArray[i*4])) / 255);
        data.push_back(((double)((uchar)byteArray[i*4+1])) / 255);
        data.push_back(((double)((uchar)byteArray[i*4+2])) / 255);
    }
}

RawImage::RawImage(int sizeX, int sizeY)
{
    width = sizeX;
    height = sizeY;
    //RawImage::data = new vector<double>(width * height * 3);
    data.reserve(width * height * 3);
    for(int i = 0; i < data.size(); i++) {
        data.push_back(0.5);
    }
}

unique_ptr<QImage> RawImage::toQImage()
{
    uchar* arr=(new uchar[width * height * 4]);
    for(int i = 0; i < width*height; i++)
    {
        arr[i*4] = (uchar)(data[i*3] * 255);
        arr[i*4+1] = (uchar)(data[i*3+1] * 255);
        arr[i*4+2] = (uchar)(data[i*3+2] * 255);
        arr[i*4+3] = (uchar)255;
    }
    for(int i = 0; i < width*height*4; i++)
    {
        if(arr[i] > 255)
                arr[i] = 255;
        else if(arr[i] < 0)
                arr[i] = 0;
    }
    unique_ptr<QImage> retImage(new QImage(arr,width,height,QImage::Format_RGB32));
    return retImage;
}

unique_ptr<RawImage> RawImage::normalize(){
    unique_ptr<RawImage> retRawImage(new RawImage(width, height));
    double maxLuma = *max_element(data.begin(), data.end());
    double minLuma = *min_element(data.begin(), data.end());

    maxLuma -= minLuma;
    double addition = 1.0 - maxLuma;
    for(int i = 0; i < width*height*3; i++)
    {
        retRawImage.get()->data.push_back(((data[i] - minLuma) / maxLuma) * addition);
    }
    return retRawImage;
}

//Свертка
unique_ptr<RawImage> convolute(double kernel[], int kernelSizeX, int kernelSizeY, int borderProcessingType );
unique_ptr<RawImage> expand(int length, int borderProcessingType);

//Масштабирование
unique_ptr<RawImage> scale(int scalling);

unique_ptr<RawImage> gauss(int strength);
unique_ptr<RawImage> sobel(int strength);


