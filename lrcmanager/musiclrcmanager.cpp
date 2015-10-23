#include "musiclrcmanager.h"

#include <QFile>
#include <QFontDatabase>

MusicLRCManager::MusicLRCManager(QWidget *parent)
    : QLabel(parent)
{
    m_intervalCount = 0.0f;

    m_linearGradient.setStart(0, 10);//The starting point coordinates filled
    m_linearGradient.setFinalStop(0, 40);//The coordinates of the end filling

    //A linear gradient mask filling
    m_maskLinearGradient.setStart(0, 10);
    m_maskLinearGradient.setFinalStop(0, 40);

    //Set the font
    m_font.setFamily("Times New Roman");
    m_font.setBold(true);

    //Set the timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(setTimeOut()));
    m_lrcMaskWidth = 0;
    m_lrcMaskWidthInterval = 0;
    m_speedLeve = 1;

    m_transparent = 100;
}

MusicLRCManager::~MusicLRCManager()
{
    delete m_timer;
}

void MusicLRCManager::startTimerClock()
{
    m_timer->start(LRC_PER_TIME);
}

void MusicLRCManager::setLrcFontSize(LrcSizeTable size)
{
    m_font.setPointSize(size);
    setText( text() );
    update();
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
    m_font.setBold( (type == 1 || type == 3) ? true : false );
    m_font.setItalic( (type == 2 || type == 3) ? true : false );
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
    qreal count = intervaltime / m_speedLeve;
    m_lrcMaskWidthInterval = m_geometry.x() / count;
    m_lrcMaskWidth = 0;
    m_timer->start(LRC_PER_TIME);
}

void MusicLRCManager::stopLrcMask()
{
    m_timer->stop();
    update();
}

void MusicLRCManager::setLinearGradientColor(QColor color)
{
    //The first parameter coordinates relative to the US, the area above,
    //calculated in accordance with the proportion of
    color.setAlpha(m_transparent*2.55);
    m_linearGradient.setColorAt(0.1, color);
    m_linearGradient.setColorAt(0.5, QColor(114, 232, 255, m_transparent*2.55));
    m_linearGradient.setColorAt(0.9, color);
    update();
}

void MusicLRCManager::setMaskLinearGradientColor(QColor color)
{
    color.setAlpha(m_transparent*2.55);
    m_maskLinearGradient.setColorAt(0.1, color);
    m_maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16, m_transparent*2.55));
    m_maskLinearGradient.setColorAt(0.9, color);
}

void MusicLRCManager::setTimeOut()
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
