#include "ossconf.h"
#/// alg import
#include "qalg/base64.h"

#define OSS_HOST_URL    "b3NzLWNuLXNoYW5naGFpLmFsaXl1bmNzLmNvbQ=="

// Resource Management Host
QString OSSConf::OSS_HOST = QString::fromStdString(Base64::base64Decode(OSS_HOST_URL));

// Access Key & Secret Key
QString OSSConf::ACCESS_KEY = QString("<Your Access Key>");
QByteArray OSSConf::SECRET_KEY = QByteArray("<Your Secret Key>");

// Version
QString OSSConf::VERSION = QString("1.0.0");

//
QString OSSConf::generateDataBucketUrl()
{
    return QString("http://%1.%2/").arg(DATA_BUCKET).arg(OSSConf::OSS_HOST);
}
