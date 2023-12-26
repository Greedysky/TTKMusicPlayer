#include "musicquerymovierequest.h"

MusicQueryMovieRequest::MusicQueryMovieRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryMovieRequest::startToQueryInfo(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToQueryInfo %2").arg(className(), value));

    m_queryValue = value;
    startToPage(0);
}
