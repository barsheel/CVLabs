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
    this->width = curWidth;
    this->height = curHeight;

    int octaveCount = (int)(log2(min(img.width, img.height))) - 1;

    pImages = make_unique<RawImage[]>((scalesPerOctave + 1) * octaveCount);

    double sigmaInterval = exp2(1.0 / scalesPerOctave);
    double sigmaBegin = sqrt(sigmaA * sigmaA - sigma0 * sigma0);
    double curSigma = sigmaA;
    double nextSigma;
    double deltaSigma;

    //первая октава
    pImages[0] = Lab1::lab1Gauss(img, sigmaBegin, BORDER_PROCESSING_TYPE_MIRROR);
    for (int j = 1; j < scalesPerOctave; j++)
    {
        nextSigma = curSigma * sigmaInterval;
        deltaSigma = sqrt(nextSigma * nextSigma - curSigma * curSigma);
        pImages[j] = Lab1::lab1Gauss(pImages[j - 1], (float)deltaSigma, BORDER_PROCESSING_TYPE_MIRROR);
        curSigma = nextSigma;
    }

    //следующие октавы
    for (int i = 1; i < octaveCount; i++) {

        curWidth /= 2;
        curHeight /= 2;
        curSigma /= 2.0;
        //первое изображение октавы
        RawImage& source = pImages[i * scalesPerOctave - 1];
        pImages[i * (scalesPerOctave)] = move(*(new RawImage(curWidth, curHeight)));
        for (int y = 0; y < curHeight; y++)
        {
            for (int x = 0; x < curWidth; x++)
            {
                pImages[i * (scalesPerOctave)].data[y * curWidth + x] =
                       (source.data[y * 2 * source.width + x * 2] +
                        source.data[y * 2 * source.width + x * 2 + 1] +
                        source.data[(y * 2 + 1) * source.width + x * 2] +
                        source.data[(y * 2 + 1) * source.width + x * 2 + 1]) / 4;
            }
        }

        curSigma = curSigma * sigmaInterval;

        //следующие изображения октавы
        for (int j = 1; j < scalesPerOctave; j++)
        {
            nextSigma = curSigma * sigmaInterval;
            deltaSigma = sqrt(nextSigma * nextSigma - curSigma * curSigma);
                        qInfo() << deltaSigma;
            pImages[i * scalesPerOctave + j] =
                    Lab1::lab1Gauss(pImages[i * scalesPerOctave + j - 1], (float)deltaSigma, BORDER_PROCESSING_TYPE_MIRROR);
            curSigma = nextSigma;
        }
    }

    //вывод результатов
    for(int i = 0; i < octaveCount * scalesPerOctave; i ++)
    {
        QString filename(QString::number(i));
        pImages[i].toQImage().save(
                    QCoreApplication::applicationDirPath() + "\\Pyramid\\" + filename + ".jpeg");
    }

}

float Pyramid::L(const int x, const int y, const float sigma)
{
    int layer = (int)(log2(sigma / sigmaA) / (1.0 / scalesPerOctave));

    int curOct = layer / scalesPerOctave;
    int resultX = x, resultY = y, resultWidth = width, resultHeight = height;

    for(int i = 0; i < curOct; i++)
    {
        resultX = resultX / 2;
        resultY = resultY / 2;
        resultWidth = (resultWidth) / 2 ;
        resultHeight = (resultHeight) / 2 ;
    }

    if(resultX == resultWidth) resultX--;
    if(resultY == resultHeight) resultY--;

    return pImages[layer].data[resultY * resultWidth + resultX];
}
