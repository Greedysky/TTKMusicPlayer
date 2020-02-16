#include "qossconf.h"

// Resource Management Host
QString QOSSConf::OSS_HOST = QString("<Your Host Key>");

// Access Key & Secret Key
QString QOSSConf::ACCESS_KEY = QString("<Your Access Key>");
QByteArray QOSSConf::SECRET_KEY = QByteArray("<Your Secret Key>");

// Version
QString QOSSConf::VERSION = QString("1.0.0");

//
QString QOSSConf::generateDataBucketUrl()
{
    return QString("http://%1.%2/").arg(DATA_BUCKET).arg(QOSSConf::OSS_HOST);
}
