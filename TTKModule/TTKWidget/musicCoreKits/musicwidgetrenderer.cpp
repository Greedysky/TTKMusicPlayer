#include "musicwidgetrenderer.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundconfigmanager.h"
#include "musicextractwrapper.h"

MusicWidgetRenderer::MusicWidgetRenderer()
    : m_background(nullptr)
{

}

void MusicWidgetRenderer::setBackgroundPixmap(QLabel *label, const QSize &size)
{
    m_background = label;
    setBackgroundPixmap(size);
}

void MusicWidgetRenderer::setBackgroundPixmap(const QSize &size)
{
    MusicBackgroundImage image;
    if(MusicExtractWrapper::outputSkin(&image, G_BACKGROUND_PTR->backgroundUrl()))
    {
        m_background->setPixmap(image.m_pix.scaled(size));
    }
    else
    {
        m_background->setPixmap(QPixmap(G_BACKGROUND_PTR->backgroundUrl()).scaled(size));
    }
}
