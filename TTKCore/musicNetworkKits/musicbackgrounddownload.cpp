#include "musicbackgrounddownload.h"
#include "musicdownloadqueryttthread.h"
#include "musicsourcedownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicbackgroundmanager.h"
#ifndef MUSIC_MOBILE
#include "musictopareawidget.h"
#endif
#///QJson import
#include "qjson/parser.h"

MusicBackgroundDownload::MusicBackgroundDownload(const QString &name, const QString &save,
                                           QObject *parent)
    : QObject(parent), m_artName(name), m_savePath(save), m_index(0), m_counter(0)
{

}

QString MusicBackgroundDownload::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundDownload::startToDownload()
{
    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    ///Set search image API
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicCryptographicHash::decryptData(BIG_ART_URL, URL_KEY).arg(m_artName));
}

void MusicBackgroundDownload::downLoadFinished(const QByteArray &bytes)
{
    QJson::Parser parser;
    bool ok;

    if(bytes != "NO_PIC")
    {
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap dataMap = data.toMap();
            QVariantList datas = dataMap["array"].toList();
            foreach(const QVariant &value, datas)
            {
                dataMap = value.toMap();
                if(m_counter < 5 && !dataMap.isEmpty())
                {
                    QString url = dataMap.values().first().toString();
                    M_LOGGER_ERROR(url);
                    MusicDataDownloadThread *down = new MusicDataDownloadThread(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL)
                                            .arg(m_savePath).arg(m_counter++).arg(SKN_FILE),
                                            MusicDownLoadThreadAbstract::Download_BigBG, this);
                    connect(down, SIGNAL(downLoadDataChanged(QString)), SLOT(bgDownLoadFinished()));
                    down->startToDownload();
                }
            }
        }
    }
}

void MusicBackgroundDownload::bgDownLoadFinished()
{
    if( ++m_index >= m_counter)
    {
        M_BACKGROUND_PTR->setArtName( m_artName );
#ifndef MUSIC_MOBILE
        MusicTopAreaWidget::instance()->musicBgThemeDownloadFinished();
#else
        QString path = QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL).arg(m_savePath).arg(0).arg(SKN_FILE);
        M_BACKGROUND_PTR->setMBackground(path);
        emit M_BACKGROUND_PTR->userSelectIndexChanged();
#endif
        deleteLater();
    }
}
