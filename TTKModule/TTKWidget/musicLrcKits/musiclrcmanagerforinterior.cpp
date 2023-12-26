#include "musiclrcmanagerforinterior.h"
#include "musicsettingmanager.h"
#include "musicwidgetutils.h"

MusicLrcManagerForInterior::MusicLrcManagerForInterior(QWidget *parent)
    : MusicLrcManager(parent),
      m_gradientFontSize(0),
      m_gradientTransparent(100)
{
    setAlignment(Qt::AlignCenter);

    m_font.setPointSize(15);
    m_speedLevel = 40;
    m_position.setX(m_lrcPerWidth = LRC_PER_WIDTH);
    m_position.setY(35);
}

void MusicLrcManagerForInterior::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QFont font(m_font);
    int value = font.pointSize() - m_gradientFontSize;
    font.setPointSize(value < 0 ? 0 : value);
    painter.setFont(font);
    m_position.setX(TTK::Widget::fontTextWidth(font, text()));

    const int fontHeight = TTK::Widget::fontTextHeight(font);
    m_linearGradient.setFinalStop(0, fontHeight);
    m_maskLinearGradient.setFinalStop(0, fontHeight);

    if(m_position.x() + m_intervalCount >= m_lrcPerWidth && m_lrcMaskWidth >= m_lrcPerWidth / 2)
    {
        m_intervalCount -= m_lrcMaskWidthInterval;
    }
    //Draw the underlying text, such as shadow, will make the effect more clearly, and more texture
    value = 2.55 * m_gradientTransparent;
    painter.setPen(QColor(0, 0, 0, value));

    value = (m_lrcPerWidth - m_position.x()) / 2.0;
    painter.drawText((value < 0 ? m_intervalCount : value) + 1, 1, m_position.x(), m_position.y(), Qt::AlignLeft | Qt::AlignVCenter, text());

    //Then draw a gradient in the above
    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(value < 0 ? m_intervalCount : value, 0, m_position.x(), m_position.y(), Qt::AlignLeft | Qt::AlignVCenter, text());

    int offsetValue = m_lrcMaskWidth;
    if(!G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toBool())
    {
        offsetValue = (m_lrcMaskWidth != 0) ? m_position.x() : m_lrcMaskWidth;
    }

    //Set lyrics mask
    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(value < 0 ? m_intervalCount : value, 0, offsetValue, m_position.y(), Qt::AlignLeft | Qt::AlignVCenter, text());
}
