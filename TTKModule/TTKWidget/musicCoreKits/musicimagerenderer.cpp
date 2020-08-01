#include "musicimagerenderer.h"
#include "musicimageutils.h"

#include <QPixmap>

MusicImageRenderer::MusicImageRenderer(QObject *parent)
    : MusicAbstractThread(parent)
{

}

MusicImageRenderer::~MusicImageRenderer()
{
    stopAndQuitThread();
}

void MusicImageRenderer::setInputData(const QByteArray &data, const QSize &size)
{
    m_size = size;
    m_buffer = data;
}

void MusicImageRenderer::run()
{
    MusicAbstractThread::run();

    QPixmap pix;
    pix.loadFromData(m_buffer);
    if(!pix.isNull() || m_size.isValid())
    {
        QPixmap cv(":/image/lb_album_cover");
        cv = cv.scaled(m_size);
        pix = pix.scaled(m_size);
        MusicUtils::Image::fusionPixmap(pix, cv, QPoint(0, 0));
    }

    if(m_running)
    {
        Q_EMIT renderFinished(pix);
    }
}
