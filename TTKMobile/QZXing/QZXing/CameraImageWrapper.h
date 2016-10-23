#ifndef CAMERAIMAGE_H
#define CAMERAIMAGE_H

#include <QImage>
#include <QString>
#include "zxing/zxing/LuminanceSource.h"

using namespace zxing;

class CameraImageWrapper : public LuminanceSource
{
public:
    CameraImageWrapper();
    CameraImageWrapper(const QImage& sourceImage);
    CameraImageWrapper(CameraImageWrapper& otherInstance);
    ~CameraImageWrapper();

    static CameraImageWrapper* Factory(const QImage& image, int maxWidth=-1, int maxHeight=-1, bool smoothTransformation=false);
    
    int getWidth() const;
    int getHeight() const;
    
    unsigned char getPixel(int x, int y) const;
    unsigned char* copyMatrix() const;
    
    QImage grayScaleImage(QImage::Format f);
    QImage getOriginalImage();

    // Callers take ownership of the returned memory and must call delete [] on it themselves.
    ArrayRef<char> getRow(int y, ArrayRef<char> row) const;
    ArrayRef<char> getMatrix() const;
  
private:
    QImage image;
#ifndef Q_OS_MAC
    unsigned char* pRow;
    unsigned char* pMatrix;
#endif
};

#endif //CAMERAIMAGE_H
