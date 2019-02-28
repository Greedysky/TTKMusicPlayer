#include "musiclrcmanager.h"

#include <QFile>
#include <QFontDatabase>

MusicLrcColor::MusicLrcColor()
{

}

MusicLrcColor::MusicLrcColor(const QList<QColor> &front, const QList<QColor> &back, MusicLrcColor::LrcColorType index)
{
    m_frontColor = front;
    m_backColor = back;
    m_index = index;
}

MusicLrcColor MusicLrcColor::mapIndexToColor(MusicLrcColor::LrcColorType index)
{
    QList<QColor> front, back;
    switch(index)
    {
        case MusicLrcColor::IYellow :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(250, 218, 131) << QColor(250, 218, 131) << QColor(250, 218, 131);
                break;
            }
        case MusicLrcColor::IBlue :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(93, 177, 240)  << QColor(93, 177, 240)  << QColor(93, 177, 240);
                break;
            }
        case MusicLrcColor::IGray :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(139, 135, 150) << QColor(139, 135, 150) << QColor(139, 135, 150);
                break;
            }
        case MusicLrcColor::IPink :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(225, 125, 179) << QColor(225, 125, 179) << QColor(225, 125, 179);
                break;
            }
        case MusicLrcColor::IGreen :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(157, 196, 0)   << QColor(157, 196, 0)   << QColor(157, 196, 0);
                break;
            }
        case MusicLrcColor::IRed :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(255, 42, 45)   << QColor(255, 42, 45)   << QColor(255, 42, 45);
                break;
            }
        case MusicLrcColor::IPurple :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(138, 0, 226)   << QColor(138, 0, 226)   << QColor(138, 0, 226);
                break;
            }
        case MusicLrcColor::IOrange :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(228, 157, 0)   << QColor(228, 157, 0)   << QColor(228, 157, 0);
                break;
            }
        case MusicLrcColor::IIndigo :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(23, 211, 190)  << QColor(23, 211, 190)  << QColor(23, 211, 190);
                break;
            }
        case MusicLrcColor::DWhite :
            {
                back  << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                front << QColor(255, 255, 0)   << QColor(255, 255, 0)   << QColor(255, 255, 0);
                break;
            }
        case MusicLrcColor::DBlue :
            {
                back  << QColor(0, 52, 138)    << QColor(0, 128, 192)   << QColor(3, 202, 252);
                front << QColor(130, 247, 253) << QColor(255, 255, 255) << QColor(3, 233, 252);
                break;
            }
        case MusicLrcColor::DRed :
            {
                back  << QColor(255, 172, 0)   << QColor(255, 0, 0)     << QColor(170, 0, 0);
                front << QColor(255, 255, 164) << QColor(255, 255, 0)   << QColor(255, 100, 26);
                break;
            }
        case MusicLrcColor::DBlack :
            {
                back  << QColor(225, 225, 225) << QColor(106, 106, 106) << QColor(0, 0, 0);
                front << QColor(255, 255, 255) << QColor(222, 222, 222) << QColor(128, 255, 255);
                break;
            }
        case MusicLrcColor::DYellow :
            {
                back  << QColor(2, 166, 174)   << QColor(128, 255, 255) << QColor(2, 166, 174);
                front << QColor(255, 128, 0)   << QColor(255, 255, 0)   << QColor(255, 128, 0);
                break;
            }
        case MusicLrcColor::DPurple :
            {
                back  << QColor(64, 0, 128)    << QColor(255, 128, 255) << QColor(64, 0, 128);
                front << QColor(255, 55, 146)  << QColor(255, 243, 134) << QColor(255, 55, 146);
                break;
            }
        case MusicLrcColor::DGreen :
            {
                back  << QColor(147, 255, 38)  << QColor(70, 176, 0)    << QColor(0, 85, 0);
                front << QColor(255, 255, 255) << QColor(154, 255, 17)  << QColor(255, 255, 0);
                break;
            }
        default: break;
    };

    return MusicLrcColor(front, back, index);
}



MusicLrcManager::MusicLrcManager(QWidget *parent)
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

MusicLrcManager::~MusicLrcManager()
{
    delete m_timer;
}

void MusicLrcManager::startTimerClock()
{
    m_timer->start(LRC_PER_TIME);
}

void MusicLrcManager::setFontFamily(int index)
{
    if(index < 0)
    {
        return;
    }

    const QStringList &family = QFontDatabase().families(QFontDatabase::Any);
    if(!family.isEmpty())
    {
        if(index >= family.count())
        {
            index = 0;
        }
        m_font.setFamily(family[index]);
    }
}

void MusicLrcManager::setFontType(int type)
{
    m_font.setBold( (type == 1 || type == 3) );
    m_font.setItalic( (type == 2 || type == 3) );
}

void MusicLrcManager::setSelfGeometry(const QPoint &point)
{
    m_geometry = point;
}

void MusicLrcManager::setSelfGeometry(int x, int y)
{
    m_geometry = QPoint(x, y);
    m_lrcPerWidth = x;
}

int MusicLrcManager::x() const
{
    return m_geometry.x();
}

int MusicLrcManager::y() const
{
    return m_geometry.y();
}

void MusicLrcManager::setX(int x)
{
    m_geometry.setX(x);
}

void MusicLrcManager::setY(int y)
{
    m_geometry.setY(y);
}

void MusicLrcManager::reset()
{
    m_intervalCount = 0.0f;
    m_lrcMaskWidth = 0.0f;
    m_timer->stop();
    update();
}

void MusicLrcManager::setLrcFontSize(int size)
{
    if(size > 0)
    {
        m_font.setPointSize(size);
    }
    setText( text() );
    update();
}

void MusicLrcManager::startLrcMask(qint64 intervaltime)
{
    m_intervalCount = 0.0f;
    m_geometry.setX(QFontMetrics(m_font).width(text()));

    const qreal count = intervaltime / m_speedLevel;
    m_lrcMaskWidthInterval = (count != 0) ? m_geometry.x() / count : 0;
    m_lrcMaskWidth = 0;
    m_timer->start(LRC_PER_TIME);
}

void MusicLrcManager::stopLrcMask()
{
    m_timer->stop();
    update();
}

void MusicLrcManager::setLinearGradientColor(const MusicLrcColor &color)
{
    QLinearGradient linearGradient;
    QList<QColor> cl = color.m_backColor;
    for(int i=0; i<cl.count(); ++i)
    {
        QColor rgb = cl[i];
        rgb.setAlpha(m_transparent*2.55);
        linearGradient.setColorAt((i+1)*1.0/cl.count(), rgb);
    }
    m_linearGradient = linearGradient;

    QLinearGradient maskLinearGradient;
    cl = color.m_frontColor;
    for(int i=0; i<cl.count(); ++i)
    {
        QColor rgb = cl[i];
        rgb.setAlpha(m_transparent*2.55);
        maskLinearGradient.setColorAt((i+1)*1.0/cl.count(), rgb);
    }
    m_maskLinearGradient = maskLinearGradient;

    update();
}

void MusicLrcManager::setUpdateMask()
{
    //At a fixed period of time covered length increases.
    m_lrcMaskWidth += m_lrcMaskWidthInterval;
    update();
}

void MusicLrcManager::setText(const QString &str)
{
    m_geometry.setX(QFontMetrics(m_font).width(str));
    QLabel::setText(str);
}
