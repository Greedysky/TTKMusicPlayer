#include "musicimagerenderer.h"
#include "musicimageutils.h"

#include <QImage>

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

    QImage image;
    image.loadFromData(m_buffer);
    if(!image.isNull() || m_size.isValid())
    {
        QImage cv(":/image/lb_album_cover");
        cv = cv.scaled(m_size);
        image = image.scaled(m_size);
        MusicUtils::Image::fusionPixmap(image, cv, QPoint(0, 0));
    }

    if(m_running)
    {
        Q_EMIT renderFinished(image);
    }
}
