#ifndef RAWIMAGE_H
#define RAWIMAGE_H
#include <memory>
#include <QImage>
using namespace std;

#define BORDER_PROCESSING_TYPE_NONE     0
#define BORDER_PROCESSING_TYPE_MIRROR   1
#define BORDER_PROCESSING_TYPE_CYLINDER  2
#define BORDER_PROCESSING_TYPE_COPY  3

class RawImage
{
public:

    int width;
    int height;
    unique_ptr<float[]> data;

    //Свертка
    struct Kernel{
        vector<float> value;
        int width;
        int height;
    };


    RawImage(const QImage &source); //СОХРАНЯТЬ ТОЛЬКО В JPEG
    RawImage(const RawImage &source);
    RawImage(const int sizeX, const int sizeY);
    RawImage();
    RawImage& operator=(RawImage&& img)
    {
        this->width = img.width;
        this->height = img.height;
        data = move(img.data);
        return *this;
    }
    RawImage(RawImage&& img) : width(img.width), height(img.height), data(move(img.data)) {}

    RawImage copyRawImage(RawImage &source);
    QImage toQImage() const;
    RawImage normalize() const;

    vector<int> calculateKernelOffsets(const Kernel &kernel, const int width) const;


    int getPxOffset(const int x, const int y) const;
    float getPixel(const int x, const int y) const;
    float getPixel(const int x, const int y, const int borderProcessingType) const;
    void setPixel(const int x, const int y, const float val);

    RawImage convolute(const Kernel &kernel, const int borderProcessingType) const;

    static Kernel makeKernel(const int width, const int height, const vector<float> &values);
};



#endif // RAWIMAGE_H
