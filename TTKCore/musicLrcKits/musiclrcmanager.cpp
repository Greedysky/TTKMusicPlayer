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

MusicLRCColor MusicLRCManager::mapIndexToColor(MusicLRCColor::LrcColorType index)
{
    QList<QColor> fg, bg;
    switch(index)
    {
        case MusicLRCColor::IYellow :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(250, 218, 131) << QColor(250, 218, 131) << QColor(250, 218, 131);
                break;
            }
        case MusicLRCColor::IBlue :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(93, 177, 240)  << QColor(93, 177, 240)  << QColor(93, 177, 240);
                break;
            }
        case MusicLRCColor::IGray :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(139, 135, 150) << QColor(139, 135, 150) << QColor(139, 135, 150);
                break;
            }
        case MusicLRCColor::IPink :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(225, 125, 179) << QColor(225, 125, 179) << QColor(225, 125, 179);
                break;
            }
        case MusicLRCColor::IGreen :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(157, 196, 0)   << QColor(157, 196, 0)   << QColor(157, 196, 0);
                break;
            }
        case MusicLRCColor::IRed :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(255, 42, 45)   << QColor(255, 42, 45)   << QColor(255, 42, 45);
                break;
            }
        case MusicLRCColor::IPurple :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(138, 0, 226)   << QColor(138, 0, 226)   << QColor(138, 0, 226);
                break;
            }
        case MusicLRCColor::IOrange :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(228, 157, 0)   << QColor(228, 157, 0)   << QColor(228, 157, 0);
                break;
            }
        case MusicLRCColor::IIndigo :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(23, 211, 190)  << QColor(23, 211, 190)  << QColor(23, 211, 190);
                break;
            }
        case MusicLRCColor::DWhite :
            {
                bg << QColor(255, 255, 255) << QColor(255, 255, 255) << QColor(255, 255, 255);
                fg << QColor(255, 255, 0)   << QColor(255, 255, 0)   << QColor(255, 255, 0);
                break;
            }
        case MusicLRCColor::DBlue :
            {
                bg << QColor(0, 52, 138)    << QColor(0, 128, 192)   << QColor(3, 202, 252);
                fg << QColor(130, 247, 253) << QColor(255, 255, 255) << QColor(3, 233, 252);
                break;
            }
        case MusicLRCColor::DRed :
            {
                bg << QColor(255, 172, 0)   << QColor(255, 0, 0)     << QColor(170, 0, 0);
                fg << QColor(255, 255, 164) << QColor(255, 255, 0)   << QColor(255, 100, 26);
                break;
            }
        case MusicLRCColor::DBlack :
            {
                bg << QColor(225, 225, 225) << QColor(106, 106, 106) << QColor(0, 0, 0);
                fg << QColor(255, 255, 255) << QColor(222, 222, 222) << QColor(128, 255, 255);
                break;
            }
        case MusicLRCColor::DYellow :
            {
                bg << QColor(2, 166, 174)   << QColor(128, 255, 255) << QColor(2, 166, 174);
                fg << QColor(255, 128, 0)   << QColor(255, 255, 0)   << QColor(255, 128, 0);
                break;
            }
        case MusicLRCColor::DPurple :
            {
                bg << QColor(64, 0, 128)    << QColor(255, 128, 255) << QColor(64, 0, 128);
                fg << QColor(255, 55, 146)  << QColor(255, 243, 134) << QColor(255, 55, 146);
                break;
            }
        case MusicLRCColor::DGreen :
            {
                bg << QColor(147, 255, 38)  << QColor(70, 176, 0)    << QColor(0, 85, 0);
                fg << QColor(255, 255, 255) << QColor(154, 255, 17)  << QColor(255, 255, 0);
                break;
            }
        default: break;
    };

    return MusicLRCColor(fg, bg, index);
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

void MusicLRCManager::setLinearGradientColor(const MusicLRCColor &color)
{
    QLinearGradient linearGradient;
    QList<QColor> cl = color.m_bgColor;
    for(int i=0; i<cl.count(); ++i)
    {
        QColor rgb = cl[i];
        rgb.setAlpha(m_transparent*2.55);
        linearGradient.setColorAt((i+1)*1.0/cl.count(), rgb);
    }
    m_linearGradient = linearGradient;

    QLinearGradient maskLinearGradient;
    cl = color.m_fgColor;
    for(int i=0; i<cl.count(); ++i)
    {
        QColor rgb = cl[i];
        rgb.setAlpha(m_transparent*2.55);
        maskLinearGradient.setColorAt((i+1)*1.0/cl.count(), rgb);
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
