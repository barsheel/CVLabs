#include "lab1.h"

Lab1::Lab1()
{}

static RawImage::Kernel getGaussKernel(const int sigma, const int size){
    RawImage::Kernel kernel;
    kernel.height = 0;
    kernel.width = size;
    for(int x = -kernel.width; x <= kernel.width; x++){
        kernel.value.push_back(1 / (sqrt(2.0 * 3.14) * sigma)
                               * pow(2.71,
                                     -((x * x) / (2.0 * sigma * sigma))));
        qInfo()<<kernel.value[kernel.value.size()-1];
    }
    return kernel;
}

RawImage  Lab1::lab1Gauss(const RawImage &image, const int strength,  const int borderProcessingType)
{

    RawImage result = *(new RawImage(image.width, image.height));
    RawImage::Kernel kernel = getGaussKernel(3, 9);
    result = image.convolute(kernel, borderProcessingType);
    swap(kernel.width, kernel.height);
    result = result.convolute(kernel, borderProcessingType);
    return result;
}


RawImage  Lab1::lab1Sobel(const RawImage &image, const int borderProcessingType)
{
    RawImage resultX = *(new RawImage(image.width, image.height));
    RawImage resultY = *(new RawImage(image.width, image.height));

    resultX = image.convolute(RawImage::makeKernel(1, 0, {1.0, 0.0, -1.0}), borderProcessingType);
    resultX = resultX.convolute(RawImage::makeKernel(0, 1, {1.0, 2.0, 1.0}), borderProcessingType);
    resultY = image.convolute(RawImage::makeKernel(1, 0, {1.0, 2.0, 1.0}), borderProcessingType);
    resultY = resultY.convolute(RawImage::makeKernel(0, 1, {1.0, 0.0, -1.0}), borderProcessingType);


/*

    unique_ptr<RawImage> expanded(expandBorders(1, 1, borderProcessingType));
    unique_ptr<RawImage> resultX(new RawImage(expanded.get()));
    RawImage::Kernel kernelX;
    kernelX.norm = 1;
    kernelX.width = 1;
    kernelX.height = 0;
    kernelX.value = {1.0, 0.0, -1.0};
    resultX =  expanded.get()->convolute(kernelX);

    kernelX.width = 0;
    kernelX.height = 1;
    kernelX.value = {1.0, 2.0, 1.0};
    resultX = resultX.get()->convolute(kernelX);

    unique_ptr<RawImage> resultY(new RawImage(expanded.get()));
    RawImage::Kernel kernelY;
    kernelY.norm = 1;
    kernelY.width = 1;
    kernelY.height = 0;
    kernelY.value = {1.0, 2.0, 1.0};
    resultY =  expanded.get()->convolute(kernelY);

    kernelY.width = 0;
    kernelY.height = 1;
    kernelY.value = {1.0, 0.0, -1.0};
    resultY = resultY.get()->convolute(kernelY);

    */

    RawImage result = *(new RawImage(image.width, image.height));

    int len = image.width * image.height;
    for(int i = 0; i < len; i++)
    {
        result.data[i] = sqrt(resultX.data[i] * resultX.data[i] +
                                      resultY.data[i] * resultY.data[i]);
    }

    return result;
}

