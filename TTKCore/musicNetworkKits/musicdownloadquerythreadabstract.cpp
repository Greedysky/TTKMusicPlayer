#include "musicdownloadquerythreadabstract.h"

MusicDownLoadQueryThreadAbstract::MusicDownLoadQueryThreadAbstract(QObject *parent)
    : MusicDownLoadPagingThread(parent)
{
    m_queryAllRecords = false;
    m_querySimplify = false;
    m_queryExtraMovie = true;
    m_searchQuality = tr("SD");
    m_queryServer = "Invalid";
}

MusicDownLoadQueryThreadAbstract::~MusicDownLoadQueryThreadAbstract()
{
    deleteAll();
}

QString MusicDownLoadQueryThreadAbstract::getClassName()
{
    return staticMetaObject.className();
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
