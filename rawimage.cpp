#include "rawimage.h"
#include <QCoreApplication>
#include <string>
#include <iostream>
#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QMessageBox>
#include <qdebug.h>

QCoreApplication a();

RawImage::RawImage(){}

RawImage::RawImage(const RawImage &source)
{
    width = source.width;
    height = source.height;
    data.resize(width * height);
    copy(source.data.begin(),
         source.data.end(),
         data.begin());
}


RawImage::RawImage(const QImage &source)
{
    width = source.width();
    height = source.height();
    QByteArray byteArray((char*)source.bits(), source.byteCount());
    data.reserve(width * height);
    for(int i = 0; i < width*height; i++) {
        data.push_back(((float)((uchar)byteArray[i*4]) * 0.114 +
                        (float)((uchar)byteArray[i*4+1]) * 0.587 +
                        ((float)((uchar)byteArray[i*4+2]) * 0.299)) / 255
        );
    }
}


RawImage::RawImage(const int sizeX, const int sizeY)
{
    width = sizeX;
    height = sizeY;
    data.reserve(width * height );
    for(int i = 0; i < width*height; i++) {
        data.push_back(0);
    }
}


QImage RawImage::toQImage() const
{
    uchar* arr=(new uchar[width * height * 4]);
    for(int i = 0; i < width*height; i++)
    {
        arr[i*4] = (uchar)(data[i] * 255);
        arr[i*4+1] = (uchar)arr[i*4];
        arr[i*4+2] = (uchar)arr[i*4];
        arr[i*4+3] = (uchar)255;
    }
    return *(new QImage(arr,width,height,QImage::Format_RGB32));
}


RawImage RawImage::normalize() const
{
    RawImage retRawImage = *(new RawImage(width, height));
    float maxLuma = *max_element(data.begin(), data.end());
    float minLuma = *min_element(data.begin(), data.end());
    maxLuma -= minLuma;
    float addition = 1.0 - maxLuma;
    for(int i = 0; i < width*height; i++)
    {
        retRawImage.data[i] = ((data[i] - minLuma) + ((data[i] - minLuma) / maxLuma) * addition);
        if(retRawImage.data[i] > 1.0)
                    retRawImage.data[i] = 1.0;
        else if(retRawImage.data[i] < 0.0)
                    retRawImage.data[i] = 0.0;
    }
    return retRawImage;
}


RawImage RawImage::convolute(const Kernel &kernel, const int borderProcessingType) const
{

    RawImage result = *(new RawImage(width, height));

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            float res = 0;
            int curOffset = 0;
            for(int ky = -kernel.height; ky <= kernel.height; ky++){
                for(int kx = -kernel.width; kx <= kernel.width; kx++){
                    //qInfo() << x+kx;
                    //qInfo() << y+ky;

                    res += getPixel(x + kx, y + ky, borderProcessingType) * kernel.value[curOffset++];
                }
            }
            result.setPixel(x, y, res);
        }
    }
    return result;
}


int RawImage::getPxOffset(const int x, const int y) const
{
    return ((width * y + x));
}


float RawImage::getPixel(const int x, const int y) const
{
    return data[getPxOffset(x,y)];
}

float RawImage::getPixel(const int x, const int y, const int borderProcessingType) const
{
    int resX = x, resY = y;
    switch (borderProcessingType){
        case BORDER_PROCESSING_TYPE_NONE:
        {
            if(x < 0 || x >= width || y < 0 || y <= height) return 0;
            break;
        }
        case BORDER_PROCESSING_TYPE_MIRROR:
        {
            if(x < 0) resX = -x;
            if(x >= width) resX = 2 * width - x-1;
            if(y < 0) resY = -y;
            if(y >= height) resY = 2 * height - y-1;
            break;
        }
        case BORDER_PROCESSING_TYPE_CYLINDER:
        {
            if(x < 0) resX = width - x;
            if(x >= width) resX = x - width;
            if(y < 0) resY = height - y;
            if(y >= height) resY = height - y;
            break;
        }
    }

    //qInfo()<<resX;
    //qInfo()<<resY;
   // qInfo() << "---";
    return data[getPxOffset(resX, resY)];
}

void RawImage::setPixel(const int x, const int y, const float val){
    data[getPxOffset(x,y)] = val;
}

vector<int> RawImage::calculateKernelOffsets(const Kernel &kernel, const int width) const
{
    vector<int> offsets;
    offsets.reserve(kernel.value.size());
    for(int y = -kernel.height; y <= kernel.height; y++)
    {
        for(int x = -kernel.width; x <= kernel.width; x++)
        {
            offsets.push_back((y * width + x));
        }
    }
    return offsets;
}

RawImage::Kernel RawImage::makeKernel(const int width, const int height, const vector<float> &values)
{
    RawImage::Kernel result;
    result.width = width;
    result.height = height;
    result.value = values;
    return result;
}



