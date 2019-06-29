#include "musicripplespecturmobject.h"
#include "musicqmmputils.h"
#include "musicobject.h"

#include "visual.h"

MusicRippleSpecturmObject::MusicRippleSpecturmObject(QObject *parent)
    : QObject(parent)
{
    m_topAreaLayout = nullptr;
    m_topAreaWidget = nullptr;
    m_visualWidget = nullptr;
}

MusicRippleSpecturmObject::~MusicRippleSpecturmObject()
{
    removeSpectrum();
}

void MusicRippleSpecturmObject::setVisible(bool v)
{
    if(!m_topAreaLayout || !m_topAreaWidget)
    {
        return;
    }

    m_visualWidget ? m_visualWidget->setVisible(v) : m_topAreaWidget->setVisible(v);
}

void MusicRippleSpecturmObject::show()
{
    if(!m_topAreaLayout || !m_topAreaWidget || m_visualWidget)
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

void MusicRippleSpecturmObject::close()
{
    if(!m_topAreaLayout || !m_topAreaWidget || !m_visualWidget)
    {
        return;
    }

    m_visualWidget->layout()->removeWidget(m_topAreaWidget);
    m_topAreaLayout->addWidget(m_topAreaWidget);

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
    m_topAreaLayout = layout;
    m_topAreaWidget = widget;
}

void MusicRippleSpecturmObject::removeSpectrum()
{
    if(m_visualWidget)
    {
        m_visualWidget = nullptr;
        MusicUtils::QMMP::enableVisualPlugin("outerewave", false);
    }
}
