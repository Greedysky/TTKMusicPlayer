#include "musicwidgetrenderer.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundconfigmanager.h"
#include "musicextractmanager.h"

MusicWidgetRenderer::MusicWidgetRenderer()
    : m_background(nullptr)
{

}

void MusicWidgetRenderer::setBackgroundPixmap(const QSize &size)
{
    if(!m_background)
    {
        return;
    }

    MusicBackgroundImage image;
    if(MusicExtractManager::outputSkin(&image, G_BACKGROUND_PTR->backgroundUrl()))
    {
        m_background->setPixmap(image.m_pix.scaled(size));
    }
    else
    {
        m_background->setPixmap(QPixmap(G_BACKGROUND_PTR->backgroundUrl()).scaled(size));
    }
}
