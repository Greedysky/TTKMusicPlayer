#include "ossconf.h"

// Resource Management Host
QString OSSConf::OSS_HOST = QString("oss-cn-shanghai.aliyuncs.com");

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
