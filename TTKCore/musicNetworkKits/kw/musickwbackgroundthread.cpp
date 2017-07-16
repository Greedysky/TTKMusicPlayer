#include "musickwbackgroundthread.h"
#include "musicdownloadsourcethread.h"
#include "musicdatadownloadthread.h"

#///QJson import
#include "qjson/parser.h"

const QString BIG_ART_URL = "NUJnNFVlSHprVzdaMWxMdXRvbEp5a3lldU51Um9GeU5RKzRDWFNER2FHL3pSRE1uK1VNRzVhVk53Y1JBUTlMbnhjeFBvRFMySnpUSldlY21xQjBkWE5GTWVkVXFsa0lNa1RKSnE3VHEwMDFPdVRDbXhUSThhWkM4TFI4RUZqbHFzVFFnQkpOY2hUR2c2YWdzb3U2cjBKSUdMYnpnZktucEJpbDVBTDlzMGF0QVMwcEtLR2JWVnc9PQ==";

MusicKWBackgroundThread::MusicKWBackgroundThread(const QString &name, const QString &save,
                                                 QObject *parent)
    : MusicDownloadBackgroundThread(name, save, parent)
{

}

QString MusicKWBackgroundThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicKWBackgroundThread::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    ///Set search image API
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadDataFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(BIG_ART_URL, false).arg(m_artName));
}

void MusicKWBackgroundThread::downLoadDataFinished(const QByteArray &bytes)
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
                    connect(down, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
                    down->startToDownload();
                }
            }
        }
    }
}
