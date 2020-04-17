#include "qsyncconf.h"

// Resource Management Host
QString QSyncConf::HOST = QString("<Your Host Key>");

// Name & Key
QString QSyncConf::NAME = QString("<Your Name Key>");
QByteArray QSyncConf::KEY = QByteArray("<Your Key>");

// Version
QString QSyncConf::VERSION = QString("1.0.0");

//
QString QSyncConf::generateDataBucketUrl()
{
    return QString("http://%1.%2/").arg(DATA_BUCKET).arg(QSyncConf::HOST);
}
