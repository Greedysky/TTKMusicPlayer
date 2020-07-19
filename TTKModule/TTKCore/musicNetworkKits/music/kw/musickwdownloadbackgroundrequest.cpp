#include "musickwdownloadbackgroundrequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicdownloaddatarequest.h"

const QString BIG_ART_URL = "NUJnNFVlSHprVzdaMWxMdXRvbEp5a3lldU51Um9GeU5RKzRDWFNER2FHL3pSRE1uK1VNRzVhVk53Y1JBUTlMbnhjeFBvRFMySnpUSldlY21xQjBkWE5GTWVkVXFsa0lNa1RKSnE3VHEwMDFPdVRDbXhUSThhWkM4TFI4RUZqbHFzVFFnQkpOY2hUR2c2YWdzb3U2cjBKSUdMYnpnZktucEJpbDVBTDlzMGF0QVMwcEtLR2JWVnc9PQ==";

MusicKWDownloadBackgroundRequest::MusicKWDownloadBackgroundRequest(const QString &name, const QString &save, QObject *parent)
    : MusicDownloadBackgroundRequest(name, save, parent)
{

}

void MusicKWDownloadBackgroundRequest::startToDownload()
{
    MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
    ///Set search image API
    connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(BIG_ART_URL, false).arg(m_artName));
}

void MusicKWDownloadBackgroundRequest::downLoadFinished(const QByteArray &bytes)
{
    QJson::Parser parser;
    bool ok;

    if(bytes != "NO_PIC")
    {
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap dataMap = data.toMap();
            const QVariantList &datas = dataMap["array"].toList();
            foreach(const QVariant &value, datas)
            {
                dataMap = value.toMap();
                if(m_counter < 5 && !dataMap.isEmpty())
                {
                    const QString &url = dataMap.values().first().toString();
                    TTK_LOGGER_ERROR(url);
                    MusicDownloadDataRequest *download = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL).arg(m_savePath).arg(m_counter++).arg(SKN_FILE), MusicObject::DownloadBigBackground, this);
                    connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
                    download->startToDownload();
                }
            }
        }
    }
}
