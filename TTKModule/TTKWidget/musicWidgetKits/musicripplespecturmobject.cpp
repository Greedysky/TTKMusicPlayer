#include "musicripplespecturmobject.h"
#include "musicqmmputils.h"

#include "visual.h"

MusicRippleSpecturmObject::MusicRippleSpecturmObject(QObject *parent)
    : QObject(parent)
{
    m_topLayout = nullptr;
    m_topWidget = nullptr;
    m_visualWidget = nullptr;
}

MusicRippleSpecturmObject::~MusicRippleSpecturmObject()
{
    removeSpectrum();
}

void MusicRippleSpecturmObject::setVisible(bool v)
{
    if(!m_topLayout || !m_topWidget)
    {
        return;
    }

    m_visualWidget ? m_visualWidget->setVisible(v) : m_topWidget->setVisible(v);
}

void MusicRippleSpecturmObject::show()
{
    if(!m_topLayout || !m_topWidget || m_visualWidget)
    {
        return;
    }

    MusicUtils::QMMP::enableVisualPlugin("outerewave", true);

    const QList<Visual*> *vs = Visual::visuals();
    if(!vs->isEmpty() && vs->last())
    {
        m_visualWidget = vs->last();
        m_visualWidget->setMinimumHeight(65);
        m_visualWidget->setMaximumHeight(65);

        QVBoxLayout *layout = new QVBoxLayout(m_visualWidget);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        m_visualWidget->setLayout(layout);

        m_topLayout->removeWidget(m_topWidget);
        layout->addWidget(m_topWidget);
        m_topLayout->addWidget(m_visualWidget);
    }
}

void MusicRippleSpecturmObject::close()
{
    if(!m_topLayout || !m_topWidget || !m_visualWidget)
    {
        return;
    }

    m_visualWidget->layout()->removeWidget(m_topWidget);
    m_topLayout->addWidget(m_topWidget);

    removeSpectrum();
}

void MusicRippleSpecturmObject::update(bool up)
{
    up ? show() : close();

    MusicUtils::QMMP::updateRippleSpectrumConfigFile();

    if(m_visualWidget)
    {
        Visual *widget = MStatic_cast(Visual*, m_visualWidget);
        if(widget)
        {
            widget->start();
        }
    }
}

void MusicRippleSpecturmObject::init(QVBoxLayout *layout, QWidget *widget)
{
    m_topLayout = layout;
    m_topWidget = widget;
}

void MusicRippleSpecturmObject::removeSpectrum()
{
    if(m_visualWidget)
    {
        m_visualWidget = nullptr;
        MusicUtils::QMMP::enableVisualPlugin("outerewave", false);
    }
}
