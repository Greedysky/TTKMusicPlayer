#include "qnconf.h"

QNConf::QNConf()
{

}

// Upload & Resource Management Host
QString QNConf::UPLOAD_HOST = QString("http://upload.qiniu.com");
QString QNConf::RS_HOST = QString("http://rs.qbox.me");
QString QNConf::RSF_HOST = QString("http://rsf.qbox.me");
QString QNConf::IOVIP_HOST = QString("http://iovip.qbox.me");

//  Access Key & Secret Key
QString QNConf::ACCESS_KEY = QString("<Your Access Key>");
QByteArray QNConf::SECRET_KEY = QByteArray("<Your Secret Key>");

// Style Delimiter
QString QNConf::DEFAULT_STYLE_DELIMITER = QString("-");

// Version
QString QNConf::VERSION = QString("1.0.0");
