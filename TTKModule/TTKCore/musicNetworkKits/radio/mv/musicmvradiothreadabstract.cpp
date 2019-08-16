#include "musicmvradiothreadabstract.h"
#include "musicdownloadkginterface.h"

MusicMVRadioThreadAbstract::MusicMVRadioThreadAbstract(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_searchText = "1";
}

void MusicMVRadioThreadAbstract::startToSearch(QueryType type, const QString &text)
{
    Q_UNUSED(type);

    deleteAll();
    m_searchText = text.isEmpty() ? "1" : text;
    m_interrupt = true;

    QNetworkRequest request;
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(MV_CATEGORY_URL, false);
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
