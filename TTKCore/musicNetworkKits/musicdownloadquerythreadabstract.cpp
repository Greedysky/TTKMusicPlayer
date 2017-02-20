#include "musicdownloadquerythreadabstract.h"

MusicDownLoadQueryThreadAbstract::MusicDownLoadQueryThreadAbstract(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_queryAllRecords = false;
    m_querySimplify = false;
    m_searchQuality = tr("SD");
    m_queryServer = "Invalid";

    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

MusicDownLoadQueryThreadAbstract::~MusicDownLoadQueryThreadAbstract()
{
    deleteAll();
}

QString MusicDownLoadQueryThreadAbstract::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryThreadAbstract::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

QString MusicDownLoadQueryThreadAbstract::mapQueryServerString() const
{
    QString v = tr("Current Used Server Is %1");
    if(m_queryServer.contains("Baidu"))
        return v.arg(tr("BD"));
    else if(m_queryServer.contains("Kugou"))
        return v.arg(tr("KG"));
    else if(m_queryServer.contains("Kuwo"))
        return v.arg(tr("KW"));
    else if(m_queryServer.contains("QQ"))
        return v.arg(tr("QQ"));
    else if(m_queryServer.contains("TTpod"))
        return v.arg(tr("TT"));
    else if(m_queryServer.contains("WangYi"))
        return v.arg(tr("WY"));
    else if(m_queryServer.contains("XiaMi"))
        return v.arg(tr("XM"));
    else if(m_queryServer.contains("YinYueTai"))
        return v.arg(tr("YYT"));
    else
        return QString();
}
