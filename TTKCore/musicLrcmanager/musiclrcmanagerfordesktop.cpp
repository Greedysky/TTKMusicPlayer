#include "musiclrcmanagerfordesktop.h"

MusicLRCManagerForDesktop::MusicLRCManagerForDesktop(QWidget *parent)
    : MusicLRCManager(parent)
{
    m_font.setPointSize(30);
    m_speedLeve = 50;
}

MusicLRCManagerForDesktop::~MusicLRCManagerForDesktop()
{

}

QString MusicLRCManagerForDesktop::getClassName()
{
    return staticMetaObject.className();
}

void MusicLRCManagerForDesktop::setSelfGeometry(const QPoint &point)
{
    m_geometry = point;
}

void MusicLRCManagerForDesktop::setSelfGeometry(int x, int y)
{
    m_geometry = QPoint(x,y);
    m_lrcPerWidth = x;
}

int MusicLRCManagerForDesktop::x() const
{
    return m_geometry.x();
}

int MusicLRCManagerForDesktop::y() const
{
    return m_geometry.y();
}

void MusicLRCManagerForDesktop::resetOrigin()
{
    m_intervalCount = 0.0f;
    m_lrcMaskWidth = 0.0f;
    update();
}

void MusicLRCManagerForDesktop::setLrcFontSize(int size)
{
    m_font.setPointSize(size);
    update();
}

void MusicLRCManagerForDesktop::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(m_font);

    if(m_geometry.x() + m_intervalCount >= m_lrcPerWidth &&
       m_lrcMaskWidth >= m_lrcPerWidth / 2)
    {
        m_intervalCount -= m_lrcMaskWidthInterval;
    }
    //Draw the underlying text, such as shadow, will make the effect more clearly,
    //and more texture
    painter.setPen(QColor(0, 0, 0, 2*m_transparent));
    painter.drawText(m_intervalCount + 1, 1, m_geometry.x(), m_geometry.y(), Qt::AlignLeft, text());

    //Then draw a gradient in the above
    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(m_intervalCount, 0, m_geometry.x(), m_geometry.y(), Qt::AlignLeft, text());

    //Set lyrics mask
    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(m_intervalCount, 0, m_lrcMaskWidth, 60, Qt::AlignLeft, text());
    painter.end();
}
