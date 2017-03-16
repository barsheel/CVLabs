#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#define BORDER_PROCESSING_TYPE_NONE     0
#define BORDER_PROCESSING_TYPE_MIRROR   1

#include <memory>
#include <QImage>

using namespace std;

class RawImage
{
public:
    int width;
    int height;
    vector<double> data;

    unique_ptr<QImage> toQImage();
    unique_ptr<RawImage> normalize();

    //Свертка
    unique_ptr<RawImage> convolute(double kernel[], int kernelSizeX, int kernelSizeY, int borderProcessingType );
    unique_ptr<RawImage> expand(int length, int borderProcessingType);

    //Масштабирование
    unique_ptr<RawImage> scale(int scalling);

    unique_ptr<RawImage> gauss(int strength);
    unique_ptr<RawImage> sobel(int strength);


    RawImage(QImage source);
    RawImage(int sizeX, int sizeY);
    RawImage();
};

#endif // RAWIMAGE_H
