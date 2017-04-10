#ifndef PYRAMID_H
#define PYRAMID_H
#include "rawimage.h"

class Pyramid
{
public:


    unique_ptr<RawImage[]> pImages;
       float sigmaA;
       float sigma0;
       int scalesPerOctave;
       int width;
       int height;
        int octaveCount;

        Pyramid();
        Pyramid(const RawImage &img, float _sigmaA, float _sigma0, int _scalesPerOctave);
        Pyramid(Pyramid&& pyr) : pImages(move(pyr.pImages)) {}

        float L(const int x, const int y, const float sigma);

};

#endif // PYRAMID_H
