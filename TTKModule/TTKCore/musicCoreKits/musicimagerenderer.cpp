#include "musicimagerenderer.h"
#include "musicimageutils.h"

#include <QPixmap>

MusicImageRenderer::MusicImageRenderer(QObject *parent)
    : TTKAbstractThread(parent)
{

}

MusicImageRenderer::~MusicImageRenderer()
{
    stop();
}

void MusicImageRenderer::setInputData(const QByteArray &data, const QSize &size)
{
    m_size = size;
    m_buffer = data;
#if TTK_QT_VERSION_CHECK(5,0,0)
}

void MusicImageRenderer::run()
{
#endif
    QImage image;
    image.loadFromData(m_buffer);
    if(!image.isNull() && m_size.isValid())
    {
        QImage cv(":/image/lb_album_cover");
        cv = cv.scaled(m_size);
        image = image.scaled(m_size);
        TTK::Image::fusionPixmap(image, cv, QPoint(0, 0));
    }

    Q_EMIT renderFinished(QPixmap::fromImage(image));
}

#if !TTK_QT_VERSION_CHECK(5,0,0)
void MusicImageRenderer::run()
{
    TTKAbstractThread::run();
}
#endif
