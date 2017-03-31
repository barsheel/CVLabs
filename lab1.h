#ifndef LAB1_H
#define LAB1_H
#include "rawimage.h"
#include <qdebug.h>
class Lab1
{
public:
    Lab1();

    static RawImage lab1Gauss(const RawImage &image, const int strength,  const int borderProcessingType);
    static RawImage lab1Sobel(const RawImage &image, const int borderProcessingType);
};

#endif // LAB1_H
