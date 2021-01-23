#include "musicmgartistsimilarrequest.h"

MusicMGArtistSimilarRequest::MusicMGArtistSimilarRequest(QObject *parent)
    : MusicSimilarRequest(parent)
{

}

void MusicMGArtistSimilarRequest::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicMGArtistSimilarRequest::downLoadFinished()
{

}
