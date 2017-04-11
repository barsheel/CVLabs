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
       int octaveCount;

        Pyramid();
        Pyramid(const RawImage &img, float _sigmaA, float _sigma0, int _scalesPerOctave);
        Pyramid(Pyramid&& pyr){
            pImages = move(pyr.pImages);
            sigmaA = pyr.sigmaA;
            sigma0 = pyr.sigma0;
            scalesPerOctave = pyr.scalesPerOctave;
            octaveCount = pyr.scalesPerOctave;
        }

        float L(const int x, const int y, const float sigma);

        RawImage getHalfScaledImage(const RawImage &source);
};

#endif // PYRAMID_H
