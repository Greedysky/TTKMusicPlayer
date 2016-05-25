#include "musicdownloadquerythreadabstract.h"

MusicDownLoadQueryThreadAbstract::MusicDownLoadQueryThreadAbstract(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("MusicDownLoadQueryThreadAbstract Support ssl: %1").arg(QSslSocket::supportsSsl()));
#endif
    m_queryAllRecords = false;
}

MusicDownLoadQueryThreadAbstract::~MusicDownLoadQueryThreadAbstract()
{
    deleteAll();    ///The release of all the objects
}
