#include "musiclrcmanagerforinterior.h"
#include "musicsettingmanager.h"
#include "musicwidgetutils.h"

MusicLrcManagerForInterior::MusicLrcManagerForInterior(QWidget *parent)
    : MusicLrcManager(parent)
{
    setAlignment(Qt::AlignCenter);
    m_geometry.setX(m_lrcPerWidth = LRC_PER_WIDTH);
    m_font.setPointSize(15);
    m_speedLevel = 40;
    m_geometry.setY(35);
    m_gradientFontSize = 0;
    m_gradientTransparent = 100;
}

void MusicLrcManagerForInterior::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QFont font(m_font);
    int ttplus = font.pointSize() - m_gradientFontSize;
    font.setPointSize(ttplus = (ttplus < 0) ? 0 : ttplus);
    painter.setFont(font);
    m_geometry.setX(MusicUtils::Widget::fontTextWidth(font, text()));

    const int fontHeight = MusicUtils::Widget::fontTextHeight(font);
    m_linearGradient.setFinalStop(0, fontHeight);
    m_maskLinearGradient.setFinalStop(0, fontHeight);

    if(m_geometry.x() + m_intervalCount >= m_lrcPerWidth && m_lrcMaskWidth >= m_lrcPerWidth / 2)
    {
        m_intervalCount -= m_lrcMaskWidthInterval;
    }
    //Draw the underlying text, such as shadow, will make the effect more clearly, and more texture
    ttplus = 2.55*m_gradientTransparent;
    painter.setPen(QColor(0, 0, 0, ttplus));

    ttplus = (m_lrcPerWidth - m_geometry.x()) / 2.0;
    painter.drawText((ttplus < 0 ? m_intervalCount : ttplus) + 1, 1,
                     m_geometry.x(), m_geometry.y(), Qt::AlignLeft | Qt::AlignVCenter, text());

    //Then draw a gradient in the above
    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(ttplus < 0 ? m_intervalCount : ttplus, 0,
                     m_geometry.x(), m_geometry.y(), Qt::AlignLeft | Qt::AlignVCenter, text());

    int offsetValue = m_lrcMaskWidth;
    if(!G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toBool())
    {
        offsetValue = (m_lrcMaskWidth != 0) ? m_geometry.x() : m_lrcMaskWidth;
    }

    //Set lyrics mask
    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(ttplus < 0 ? m_intervalCount : ttplus, 0,
                     offsetValue, m_geometry.y(), Qt::AlignLeft | Qt::AlignVCenter, text());
}
