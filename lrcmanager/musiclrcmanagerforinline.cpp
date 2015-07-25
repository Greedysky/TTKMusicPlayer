#include "musiclrcmanagerforinline.h"

MusicLRCManagerForInline::MusicLRCManagerForInline(QWidget *parent) :
    MusicLRCManager(parent)
{
    setAlignment(Qt::AlignCenter);
    m_font.setPointSize(15);
    m_speedLeve = 40;
    m_geometry.setX(m_lrcPerWidth = 522);
    m_geometry.setY(35);
    m_gradientFontSize = 0;
    m_gradientTransparent = 0;
}

MusicLRCManagerForInline::~MusicLRCManagerForInline()
{

}

void MusicLRCManagerForInline::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFont font(m_font);
    int ttplus = font.pointSize() - m_gradientFontSize;
    (ttplus < 0) ? ttplus = 0 : ttplus = ttplus;
    font.setPointSize(ttplus);
    painter.setFont(font);

    if(m_geometry.x() + m_intervalCount >= m_lrcPerWidth &&
       m_lrcMaskWidth >= m_lrcPerWidth / 2)
    {
        m_intervalCount -= m_lrcMaskWidthInterval;
    }
    //Draw the underlying text, such as shadow, will make the effect more clearly,
    //and more texture
    ttplus = 2*(m_transparent - m_gradientTransparent);
    (ttplus < 0) ? ttplus = 0 : ttplus = ttplus;
    painter.setPen(QColor(0, 0, 0, ttplus));

    if((ttplus = m_lrcPerWidth - m_geometry.x()) < 0)
        painter.drawText(m_intervalCount + 1, 1, m_geometry.x(), m_geometry.y(), Qt::AlignLeft, text());
    else
        painter.drawText(ttplus / 2 + 1, 1, m_geometry.x(), m_geometry.y(), Qt::AlignHCenter, text());

    //Then draw a gradient in the above
    painter.setPen(QPen(m_linearGradient, 0));
    if(ttplus < 0)
        painter.drawText(m_intervalCount, 0, m_geometry.x(), m_geometry.y(), Qt::AlignLeft, text());
    else
        painter.drawText(ttplus / 2, 0, m_geometry.x(), m_geometry.y(), Qt::AlignHCenter, text());

    //Set lyrics mask
    painter.setPen(QPen(m_maskLinearGradient, 0));
    if(ttplus < 0)
        painter.drawText(m_intervalCount, 0, m_lrcMaskWidth, 60, Qt::AlignLeft, text());
    else
        painter.drawText( m_UpdateLrc ? ttplus / 2 - 3 : ttplus / 2,
                          0, m_lrcMaskWidth, 60, Qt::AlignLeft, text());
    painter.end();
}
