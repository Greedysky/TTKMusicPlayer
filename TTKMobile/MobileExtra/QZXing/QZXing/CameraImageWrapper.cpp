#include "CameraImageWrapper.h"
#include <QColor>
#include <QApplication>
#include <QDesktopWidget>

CameraImageWrapper::CameraImageWrapper() : LuminanceSource(0,0)
{
}

CameraImageWrapper::CameraImageWrapper(const QImage &sourceImage) : LuminanceSource(sourceImage.width(), sourceImage.height())
{
    image = sourceImage.copy();
}

CameraImageWrapper::CameraImageWrapper(CameraImageWrapper& otherInstance) : LuminanceSource(otherInstance.getWidth(), otherInstance.getHeight())
{
    image = otherInstance.getOriginalImage().copy();
}

CameraImageWrapper::~CameraImageWrapper()
{
}

CameraImageWrapper *CameraImageWrapper::Factory(const QImage &sourceImage, int maxWidth, int maxHeight, bool smoothTransformation)
{
    QImage image;
    if((maxWidth != 1 || maxHeight != 1) && (sourceImage.width() > maxWidth || sourceImage.height() > maxHeight))
        image = sourceImage.scaled(
                    maxWidth != -1 ? maxWidth : sourceImage.width(),
                    maxHeight != -1 ? maxHeight : sourceImage.height(),
                    Qt::KeepAspectRatio,
                    smoothTransformation ? Qt::SmoothTransformation : Qt::FastTransformation);
    else
        image = sourceImage;

    return new CameraImageWrapper(image);
}

int CameraImageWrapper::getWidth() const
{
    return image.width();
}

int CameraImageWrapper::getHeight() const
{
    return image.height();
}

unsigned char CameraImageWrapper::getPixel(int x, int y) const
{
    QRgb pixel = image.pixel(x,y);

    return qGray(pixel);//((qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3);
}

unsigned char* CameraImageWrapper::copyMatrix() const
{
    unsigned char* newMatrix = (unsigned char*)malloc(image.width()*image.height()*sizeof(unsigned char));

    int cnt = 0;
    for(int i=0; i<image.width(); i++)
    {
        for(int j=0; j<image.height(); j++)
        {
            newMatrix[cnt++] = getPixel(i,j);
        }
    }

    return newMatrix;
}

//bool CameraImageWrapper::setImage(QString fileName, int maxWidth, int maxHeight)
//{
//    bool isLoaded = image.load(fileName);

//    if(!isLoaded)
//        return false;

//    width = image.width();
//    height = image.height();

//    scale(maxWidth, maxHeight);

//    return true;
//}

//bool CameraImageWrapper::setImage(QImage newImage, int maxWidth, int maxHeight)
//{
//    if(newImage.isNull())
//        return false;

//    image = newImage.copy();

//    width = image.width();
//    height = image.height();

//    scale(maxWidth, maxHeight);

//    return true;
//}

QImage CameraImageWrapper::grayScaleImage(QImage::Format f)
{
    QImage tmp(image.width(), image.height(), f);
    for(int i=0; i<image.width(); i++)
    {
        for(int j=0; j<image.height(); j++)
        {
            int pix = (int)getPixel(i,j);
            tmp.setPixel(i,j, qRgb(pix ,pix,pix));
        }
    }

    return tmp;
}

QImage CameraImageWrapper::getOriginalImage()
{
    return image;
}

ArrayRef<char> CameraImageWrapper::getRow(int y, ArrayRef<char> row) const
{
    int width = getWidth();

    if (row->size() != width)
        row.reset(ArrayRef<char>(width));

    for (int x = 0; x < width; x++)
        row[x] = getPixel(x,y);

    return row;
}

ArrayRef<char> CameraImageWrapper::getMatrix() const
{
    int width = getWidth();
    int height =  getHeight();
    char* matrix = new char[width*height];
    char* m = matrix;

    for(int y=0; y<height; y++)
    {
        ArrayRef<char> tmpRow;
        tmpRow = getRow(y, ArrayRef<char>(width));
#if __cplusplus > 199711L
        memcpy(m, tmpRow->values().data(), width);
#else
		memcpy(m, &tmpRow->values()[0], width);
#endif
        m += width * sizeof(unsigned char);

        //delete tmpRow;
    }

    //pMatrix = matrix;
	ArrayRef<char> arr = ArrayRef<char>(matrix, width*height);

	if(matrix)
		delete matrix;


    return arr;
}

//void CameraImageWrapper::scale(int maxWidth, int maxHeight)
//{
//    image = scale_s(image, maxWidth, maxHeight, isSmoothTransformationEnabled);
//}

//QImage CameraImageWrapper::scale_s(const QImage &image, int maxWidth, int maxHeight, bool smoothTransformation)
//{
//    QImage transformedImage;

//    if((maxWidth != 1 || maxHeight != 1) && (image.width() > maxWidth || image.height() > maxHeight))
//        transformedImage = image.scaled(
//                    maxWidth != -1 ? maxWidth : image.width(),
//                    maxHeight != -1 ? maxHeight : image.height(),
//                    Qt::KeepAspectRatio,
//                    smoothTransformation ? Qt::SmoothTransformation : Qt::FastTransformation);
//    else
//        transformedImage = image;

//    return transformedImage;
//}
