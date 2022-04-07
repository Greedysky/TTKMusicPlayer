#include "musicripplespecturmmodule.h"
#include "musicqmmputils.h"

#include <qmmp/visual.h>

MusicRippleSpecturmModule::MusicRippleSpecturmModule(QObject *parent)
    : QObject(parent)
{
    m_topAreaLayout = nullptr;
    m_topAreaWidget = nullptr;
    m_visualWidget = nullptr;
}

MusicRippleSpecturmModule::~MusicRippleSpecturmModule()
{
    removeSpectrum();
}

void MusicRippleSpecturmModule::setVisible(bool v)
{
    if(!m_topAreaLayout || !m_topAreaWidget)
    {
        return;
    }

    m_visualWidget ? m_visualWidget->setVisible(v) : m_topAreaWidget->setVisible(v);
}

void MusicRippleSpecturmModule::show()
{
    if(!m_topAreaLayout || !m_topAreaWidget || m_visualWidget)
    {
        return;
    }

    MusicUtils::QMMP::enabledVisualPlugin("outerblurwave", true);

    const QList<Visual*> *vs = Visual::visuals();
    if(!vs->isEmpty() && vs->back())
    {
        m_visualWidget = vs->back();
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

void MusicRippleSpecturmModule::close()
{
    if(!m_topAreaLayout || !m_topAreaWidget || !m_visualWidget)
    {
        return;
    }

    m_visualWidget->layout()->removeWidget(m_topAreaWidget);
    m_topAreaLayout->addWidget(m_topAreaWidget);

    removeSpectrum();
}

void MusicRippleSpecturmModule::update(bool up)
{
    up ? show() : close();

    MusicUtils::QMMP::updateRippleSpectrumConfigFile();
    if(m_visualWidget)
    {
        Visual *widget = TTKStatic_cast(Visual*, m_visualWidget);
        if(widget)
        {
            widget->start();
        }
    }
}

void MusicRippleSpecturmModule::initialize(QVBoxLayout *layout, QWidget *widget)
{
    m_topAreaLayout = layout;
    m_topAreaWidget = widget;
}

void MusicRippleSpecturmModule::removeSpectrum()
{
    if(m_visualWidget)
    {
        m_visualWidget = nullptr;
        MusicUtils::QMMP::enabledVisualPlugin("outerblurwave", false);
    }
}
