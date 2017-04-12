#include "pyramid.h"
#include "rawimage.h"
#include "lab1.h"
#include "qcoreapplication.h"

Pyramid::Pyramid()
{
}

Pyramid::Pyramid(const RawImage &img, float _sigmaA, float _sigma0, int _scalesPerOctave)
{
    sigmaA = _sigmaA;
    sigma0 = _sigma0;
    scalesPerOctave = _scalesPerOctave;
    int curWidth = img.width;
    int curHeight = img.height;

    octaveCount = (int)(log2(min(img.width, img.height))) - 1;

    pImages = make_unique<RawImage[]>((scalesPerOctave + 1) * octaveCount);

    double sigmaInterval = exp2(1.0 / scalesPerOctave);
    double sigmaBegin = sqrt(sigma0 * sigma0 - sigmaA * sigmaA);
    double curSigma = sigma0;
    double nextSigma;
    double deltaSigma;

    //первая октава
    pImages[0] = Lab1::lab1Gauss(img, sigmaBegin, BORDER_PROCESSING_TYPE_MIRROR);
    qInfo() << curSigma;
    for (int j = 1; j < scalesPerOctave; j++)
    {
        nextSigma = curSigma * sigmaInterval;
        deltaSigma = sqrt(nextSigma * nextSigma - curSigma * curSigma);
        pImages[j] = Lab1::lab1Gauss(pImages[j - 1], (float)deltaSigma, BORDER_PROCESSING_TYPE_MIRROR);
        curSigma = nextSigma;
        qInfo() << curSigma;
    }

    //следующие октавы
    for (int i = 1; i < octaveCount; i++) {

        curWidth /= 2;
        curHeight /= 2;
        curSigma /= 2.0;
        //первое изображение октавы
        RawImage& source = pImages[i * scalesPerOctave - 1];

        pImages[i * (scalesPerOctave)] = getHalfScaledImage(source);
        qInfo() << "-------------oct";
        curSigma = curSigma * sigmaInterval;
        qInfo() << curSigma * exp2(i);
        //следующие изображения октавы
        for (int j = 1; j < scalesPerOctave; j++)
        {
            nextSigma = curSigma * sigmaInterval;
            deltaSigma = sqrt(nextSigma * nextSigma - curSigma * curSigma);
            pImages[i * scalesPerOctave + j] =
                    Lab1::lab1Gauss(pImages[i * scalesPerOctave + j - 1], (float)deltaSigma, BORDER_PROCESSING_TYPE_MIRROR);
            curSigma = nextSigma;
            qInfo() << curSigma * exp2(i);
        }
    }
}

float Pyramid::L(const int x, const int y, const float sigma)
{
    int layer = (int)(log2(sigma / sigmaA) / (1.0 / scalesPerOctave));

    int curOct = layer / scalesPerOctave;
    int resultX = x, resultY = y, resultWidth = pImages[layer].width, resultHeight = pImages[layer].height;

    for(int i = 0; i < curOct; i++)
    {
        resultX = resultX / 2;
        resultY = resultY / 2;
    }

    return pImages[layer].data[resultY * resultWidth + resultX];
}


RawImage Pyramid::getHalfScaledImage(const RawImage &source)
{
    int newWidth = (source.width + 1) / 2;
    int newHeight = (source.height + 1) / 2;
    RawImage result(newWidth, newHeight);
    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            int count = 1;
            result.data[y * newWidth + x] = source.data[y * 2 * source.width + x * 2];
            if(x * 2 + 1 < source.width){
                result.data[y * newWidth + x] += source.data[y * 2 * source.width + x * 2 + 1];
                count++;
            }

           if(y * 2 + 1< source.height){
                result.data[y * newWidth + x] += source.data[(y * 2 + 1) * source.width + x * 2];
                count++;
           }

           if(x * 2 + 1 < source.width && y * 2 + 1< source.height){
                result.data[y * newWidth + x] += source.data[(y * 2 + 1) * source.width + x * 2 + 1];
                count++;
           }

           result.data[y * newWidth + x] /= count;
        }
    }
    return result;
}

void Pyramid::savePictures(const QString path) const{
    for(int i = 0; i < octaveCount * scalesPerOctave; i ++)
    {
        QString filename(QString::number(i));
        pImages[i].toQImage().save(path + "\\Pyramid\\" + filename + ".jpeg");
    }
}
