#include "lab1.h"

Lab1::Lab1()
{}

RawImage::Kernel Lab1::getGaussKernel(const float sigma, const int size)
{
    RawImage::Kernel kernel;
    kernel.height = 0;
    kernel.width = size;
    for(int x = -kernel.width; x <= kernel.width; x++){
        kernel.value.push_back(1 / (sqrt(2.0 * 3.14) * sigma) * pow(2.71, -((x * x) / (2.0 * sigma * sigma))));
    }
    return kernel;
}

RawImage  Lab1::lab1Gauss(const RawImage &image, const float strength,  const int borderProcessingType)
{
    RawImage::Kernel kernel = getGaussKernel(strength, (int)(strength * 3));
    RawImage result = *(new RawImage(image.convolute(kernel, borderProcessingType)));
    swap(kernel.width, kernel.height);
    result = result.convolute(kernel, borderProcessingType);
    return result;
}


RawImage  Lab1::lab1Sobel(const RawImage &image, const int borderProcessingType)
{
    RawImage resultX(image.width, image.height);
    RawImage resultY(image.width, image.height);

    resultX = image.convolute(RawImage::makeKernel(1, 0, {1.0, 0.0, -1.0}), borderProcessingType);
    resultX = resultX.convolute(RawImage::makeKernel(0, 1, {1.0, 2.0, 1.0}), borderProcessingType);
    resultY = image.convolute(RawImage::makeKernel(1, 0, {1.0, 2.0, 1.0}), borderProcessingType);
    resultY = resultY.convolute(RawImage::makeKernel(0, 1, {1.0, 0.0, -1.0}), borderProcessingType);

    RawImage result(image.width, image.height);

    int len = image.width * image.height;
    for(int i = 0; i < len; i++)
    {
        result.data.get()[i] = sqrt(resultX.data.get()[i] * resultX.data.get()[i] +
                                    resultY.data.get()[i] * resultY.data.get()[i]);
    }
    return result;
}

