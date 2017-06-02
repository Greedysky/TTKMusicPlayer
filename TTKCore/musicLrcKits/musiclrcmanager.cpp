#include "musiclrcmanager.h"

#include <QFile>
#include <QFontDatabase>

MusicLRCManager::MusicLRCManager(QWidget *parent)
    : QLabel(parent)
{
    m_intervalCount = 0.0f;

    m_linearGradient.setStart(0, 0);
    m_maskLinearGradient.setStart(0, 0);

    m_font.setFamily("Times New Roman");
    m_font.setBold(true);

    m_lrcMaskWidth = 0;
    m_lrcMaskWidthInterval = 0;
    m_speedLevel = 1;
    m_transparent = 100;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(setUpdateMask()));
}

MusicLRCManager::~MusicLRCManager()
{
    delete m_timer;
}

QString MusicLRCManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicLRCManager::startTimerClock()
{
    m_timer->start(LRC_PER_TIME);
}

void MusicLRCManager::setFontFamily(int index)
{
    if(index < 0)
    {
        return;
    }

    QStringList family = QFontDatabase().families(QFontDatabase::Any);
    if(!family.isEmpty())
    {
        if(index >= family.count())
        {
            index = 0;
        }
        m_font.setFamily(family[index]);
    }
}

void MusicLRCManager::setFontType(int type)
{
    m_font.setBold( (type == 1 || type == 3) );
    m_font.setItalic( (type == 2 || type == 3) );
}

void MusicLRCManager::setSelfGeometry(const QPoint &point)
{
    m_geometry = point;
}

void MusicLRCManager::setSelfGeometry(int x, int y)
{
    m_geometry = QPoint(x, y);
    m_lrcPerWidth = x;
}

int MusicLRCManager::x() const
{
    return m_geometry.x();
}

int MusicLRCManager::y() const
{
    return m_geometry.y();
}

void MusicLRCManager::setX(int x)
{
    m_geometry.setX(x);
}

void MusicLRCManager::setY(int y)
{
    m_geometry.setY(y);
}

void MusicLRCManager::reset()
{
    m_intervalCount = 0.0f;
    m_lrcMaskWidth = 0.0f;
    m_timer->stop();
    update();
}

void MusicLRCManager::setLrcFontSize(int size)
{
    if(size > 0)
    {
        m_font.setPointSize(size);
    }
    setText( text() );
    update();
}

void MusicLRCManager::startLrcMask(qint64 intervaltime)
{
    /*Open the mask, need to specify the current lyrics start with the interval
      between the end of,Here set every 30 msec update a mask width, because if
      too frequent updates,The CPU occupancy rate will increase, and if the time
      interval is too large, then the animation is not smooth
    */
    m_intervalCount = 0.0f;
    m_geometry.setX(QFontMetrics(m_font).width(text()));
    qreal count = intervaltime / m_speedLevel;
    m_lrcMaskWidthInterval = m_geometry.x() / count;
    m_lrcMaskWidth = 0;
    m_timer->start(LRC_PER_TIME);
}

void MusicLRCManager::stopLrcMask()
{
    m_timer->stop();
    update();
}

void MusicLRCManager::setLinearGradientColor(const QList<QColor> &colors)
{
    //The first parameter coordinates relative to the US, the area above,
    //calculated in accordance with the proportion of
    QLinearGradient linearGradient;
    for(int i=0; i<colors.count(); ++i)
    {
        QColor rgb = colors[i];
        rgb.setAlpha(m_transparent*2.55);
        linearGradient.setColorAt((i+1)*1.0/colors.count(), rgb);
    }
    m_linearGradient = linearGradient;
    update();
}

void MusicLRCManager::setMaskLinearGradientColor(const QList<QColor> &colors)
{
    QLinearGradient maskLinearGradient;
    for(int i=0; i<colors.count(); ++i)
    {
        QColor rgb = colors[i];
        rgb.setAlpha(m_transparent*2.55);
        maskLinearGradient.setColorAt((i+1)*1.0/colors.count(), rgb);
    }
    m_maskLinearGradient = maskLinearGradient;
    update();
}

void MusicLRCManager::setUpdateMask()
{
    //At a fixed period of time covered length increases.
    m_lrcMaskWidth += m_lrcMaskWidthInterval;
    update();
}

void MusicLRCManager::setText(const QString &str)
{
    m_geometry.setX(QFontMetrics(m_font).width(str));
    QLabel::setText(str);
}
