#include "QZXing.h"

#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/Binarizer.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/DecodeHints.h>
#include "CameraImageWrapper.h"
#include "imagehandler.h"
#include <QTime>

using namespace zxing;

QZXing::QZXing(QObject *parent) : QObject(parent)
{
    decoder = new MultiFormatReader();
    /*setDecoder(DecoderFormat_QR_CODE |
               DecoderFormat_DATA_MATRIX |
               DecoderFormat_UPC_E |
               DecoderFormat_UPC_A |
               DecoderFormat_EAN_8 |
               DecoderFormat_EAN_13 |
               DecoderFormat_CODE_128 |
               DecoderFormat_CODE_39 |
               DecoderFormat_ITF |
               DecoderFormat_Aztec);*/
    imageHandler = new ImageHandler();
}

QZXing::QZXing(QZXing::DecoderFormat decodeHints, QObject *parent) : QObject(parent)
{
    decoder = new MultiFormatReader();
    imageHandler = new ImageHandler();

    setDecoder(decodeHints);
}

void QZXing::setDecoder(const uint &hint)
{
    unsigned int newHints = 0;

    if(hint & DecoderFormat_Aztec)
        newHints |= DecodeHints::AZTEC_HINT;

    if(hint & DecoderFormat_CODABAR)
        newHints |= DecodeHints::CODABAR_HINT;

    if(hint & DecoderFormat_CODE_39)
        newHints |= DecodeHints::CODE_39_HINT;

    if(hint & DecoderFormat_CODE_93)
        newHints |= DecodeHints::CODE_93_HINT;

    if(hint & DecoderFormat_CODE_128)
        newHints |= DecodeHints::CODE_128_HINT;

    if(hint & DecoderFormat_DATA_MATRIX)
        newHints |= DecodeHints::DATA_MATRIX_HINT;

    if(hint & DecoderFormat_EAN_8)
        newHints |= DecodeHints::EAN_8_HINT;

    if(hint & DecoderFormat_EAN_13)
        newHints |= DecodeHints::EAN_13_HINT;

    if(hint & DecoderFormat_ITF)
        newHints |= DecodeHints::ITF_HINT;

    if(hint & DecoderFormat_MAXICODE)
        newHints |= DecodeHints::MAXICODE_HINT;

    if(hint & DecoderFormat_PDF_417)
        newHints |= DecodeHints::PDF_417_HINT;

    if(hint & DecoderFormat_QR_CODE)
        newHints |= DecodeHints::QR_CODE_HINT;

    if(hint & DecoderFormat_RSS_14)
        newHints |= DecodeHints::RSS_14_HINT;

    if(hint & DecoderFormat_RSS_EXPANDED)
        newHints |= DecodeHints::RSS_EXPANDED_HINT;

    if(hint & DecoderFormat_UPC_A)
        newHints |= DecodeHints::UPC_A_HINT;

    if(hint & DecoderFormat_UPC_E)
        newHints |= DecodeHints::UPC_E_HINT;

    if(hint & DecoderFormat_UPC_EAN_EXTENSION)
        newHints |= DecodeHints::UPC_EAN_EXTENSION_HINT;

    enabledDecoders = newHints;

    emit enabledFormatsChanged();
}

QString QZXing::decodeImage(QImage image, int maxWidth, int maxHeight, bool smoothTransformation)
{
    QTime t;
    t.start();
    Ref<Result> res;
    emit decodingStarted();

    if(image.isNull())
    {
        emit decodingFinished(false);
        processingTime = -1;
        return "";
    }

    try{
        CameraImageWrapper* ciw;

        if(maxWidth > 0 || maxHeight > 0)
            ciw = CameraImageWrapper::Factory(image, maxWidth, maxHeight, smoothTransformation);
        else
            ciw = new CameraImageWrapper(image);

        Ref<LuminanceSource> imageRef(ciw);
        GlobalHistogramBinarizer* binz = new GlobalHistogramBinarizer(imageRef);

        Ref<Binarizer> bz (binz);
        BinaryBitmap* bb = new BinaryBitmap(bz);

        Ref<BinaryBitmap> ref(bb);

        res = ((MultiFormatReader*)decoder)->decode(ref, DecodeHints((int)enabledDecoders));

        QString string = QString(res->getText()->getText().c_str());
        processingTime = t.elapsed();
        emit tagFound(string);
        emit decodingFinished(true);
        return string;
    }
    catch(zxing::Exception& e)
    {
       emit decodingFinished(false);
       processingTime = -1;
       return "";
    }
}

QString QZXing::decodeImageFromFile(QString imageFilePath, int maxWidth, int maxHeight, bool smoothTransformation)
{
	//used to have a check if this image exists
	//but was removed because if the image file path doesn't point to a valid image
	// then the QImage::isNull will return true and the decoding will fail eitherway.
    return decodeImage(QImage(imageFilePath), maxWidth, maxHeight, smoothTransformation);
}

QString QZXing::decodeImageQML(QObject *item)
{
    return decodeSubImageQML(item);
}

QString QZXing::decodeSubImageQML(QObject* item,
                                  const double offsetX, const double offsetY,
                                  const double width, const double height)
{
    if(item  == NULL)
    {
        emit decodingFinished(false);
        return "";
    }

    QImage img = ((ImageHandler*)imageHandler)->extractQImage(item, offsetX, offsetY, width, height);

    return decodeImage(img);
}

int QZXing::getProcessTimeOfLastDecoding()
{
    return processingTime;
}

uint QZXing::getEnabledFormats() const
{
    return enabledDecoders;
}




