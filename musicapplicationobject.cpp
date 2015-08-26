#include "musicapplicationobject.h"
#include "musicapplication.h"
#include <QPropertyAnimation>

MusicApplicationObject::MusicApplicationObject(QObject *parent)
    : QObject(parent)
{
    m_supperClass = static_cast<MusicApplication*>(parent);
    m_animation = new QPropertyAnimation(parent, "windowOpacity");
    m_animation->setDuration(1000);
    m_animation->setStartValue(0);
    m_animation->setEndValue(1);
    m_animation->start();
}

MusicApplicationObject::~MusicApplicationObject()
{
    delete m_animation;
}
