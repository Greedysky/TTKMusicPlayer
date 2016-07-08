#ifndef QNIOHELPER_H
#define QNIOHELPER_H

#include "qnconf.h"
#include <QHttpMultiPart>

class QNMac;
class QNPutExtra;

class MUSIC_EXTRAS_EXPORT QNIOHelper
{

public:
    static QHttpMultiPart* createPutDataMultiPart(const QString &uploadToken, const QByteArray &data,
                                                  const QString &key = 0, const QString &fileName = 0,
                                                  const QNPutExtra *putExtra = 0);

    static QNetworkRequest listRequest(const QString &bucket, const QNMac *mac = 0);

};

#endif // QNIO_H
