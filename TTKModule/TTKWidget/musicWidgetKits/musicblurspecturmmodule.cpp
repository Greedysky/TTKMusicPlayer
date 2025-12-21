#include "musicblurspecturmmodule.h"
#include "musicqmmputils.h"

#include <qmmp/visual.h>

MusicBlurSpecturmModule::MusicBlurSpecturmModule(QObject *parent)
    : QObject(parent),
      m_topAreaLayout(nullptr),
      m_topAreaWidget(nullptr),
      m_visualWidget(nullptr)
{

}

MusicBlurSpecturmModule::~MusicBlurSpecturmModule()
{
    removeSpectrum();
}

void MusicBlurSpecturmModule::setVisible(bool v)
{
    if(!m_topAreaLayout || !m_topAreaWidget)
    {
        return;
    }

    m_visualWidget ? m_visualWidget->setVisible(v) : m_topAreaWidget->setVisible(v);
}

void MusicBlurSpecturmModule::show()
{
    if(!m_topAreaLayout || !m_topAreaWidget || m_visualWidget)
    {
        return;
    }

    TTK::TTKQmmp::setVisualEnabled("outerblurwave", true, TTKStaticCast(QWidget*, parent()));

    const QList<Visual*> *vs = Visual::visuals();
    if(!vs->isEmpty() && vs->last())
    {
        m_visualWidget = vs->last();
        m_visualWidget->setFixedHeight(65);
        m_visualWidget->setMinimumWidth(CONCISE_WIDTH_MIN);

        QVBoxLayout *layout = new QVBoxLayout(m_visualWidget);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        m_visualWidget->setLayout(layout);

        m_topAreaLayout->removeWidget(m_topAreaWidget);
        layout->addWidget(m_topAreaWidget);
        m_topAreaLayout->addWidget(m_visualWidget);
    }
}

void MusicBlurSpecturmModule::close()
{
    if(!m_topAreaLayout || !m_topAreaWidget || !m_visualWidget)
    {
        return;
    }

    m_visualWidget->layout()->removeWidget(m_topAreaWidget);
    m_topAreaLayout->addWidget(m_topAreaWidget);

    removeSpectrum();
}

void MusicBlurSpecturmModule::update(bool up)
{
    up ? show() : close();

    TTK::TTKQmmp::updateBlurConfig();
    if(m_visualWidget)
    {
        Visual *widget = TTKObjectCast(Visual*, m_visualWidget);
        if(widget)
        {
            widget->start();
        }
    }
}

void MusicBlurSpecturmModule::initialize(QVBoxLayout *layout, QWidget *widget)
{
    m_topAreaLayout = layout;
    m_topAreaWidget = widget;
}

void MusicBlurSpecturmModule::removeSpectrum()
{
    if(m_visualWidget)
    {
        m_visualWidget = nullptr;
        TTK::TTKQmmp::setVisualEnabled("outerblurwave", false, TTKStaticCast(QWidget*, parent()));
    }
}
