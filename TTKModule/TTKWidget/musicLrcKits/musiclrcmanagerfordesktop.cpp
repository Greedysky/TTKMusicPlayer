#include "musiclrcmanagerfordesktop.h"
#include "musicsettingmanager.h"
#include "musicwidgetutils.h"

MusicLrcManagerForDesktop::MusicLrcManagerForDesktop(QWidget *parent)
    : MusicLrcManager(parent)
{
    m_font.setPointSize(30);
    m_speedLevel = 50;
}



MusicLrcManagerHorizontalDesktop::MusicLrcManagerHorizontalDesktop(QWidget *parent)
    : MusicLrcManagerForDesktop(parent)
{

}

void MusicLrcManagerHorizontalDesktop::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(m_font);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    const int fontHeight = TTK::Widget::fontTextHeight(m_font);
    const int begin = (rect().height() - fontHeight) / 2;
    m_linearGradient.setFinalStop(0, fontHeight);
    m_maskLinearGradient.setFinalStop(0, fontHeight);

    if(m_position.x() + m_intervalCount >= m_lrcPerWidth && m_lrcMaskWidth >= m_lrcPerWidth / 2)
    {
        m_intervalCount -= m_lrcMaskWidthInterval;
    }

    //Draw the underlying text, such as shadow, will make the effect more clearly,
    //and more texture
    painter.setPen(QColor(0, 0, 0, 2 * m_transparent));
    painter.drawText(m_intervalCount + 1, begin + 1, m_position.x(), m_position.y(), Qt::AlignLeft, text());

    //Then draw a gradient in the above
    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(m_intervalCount, begin, m_position.x(), m_position.y(), Qt::AlignLeft, text());

    int offsetValue = m_lrcMaskWidth;
    if(!G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toBool())
    {
        offsetValue = (m_lrcMaskWidth != 0) ? m_position.x() : m_lrcMaskWidth;
    }

    //Set lyrics mask
    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(m_intervalCount, begin, offsetValue, 60, Qt::AlignLeft, text());
}


MusicLrcManagerVerticalDesktop::MusicLrcManagerVerticalDesktop(QWidget *parent)
    : MusicLrcManagerForDesktop(parent)
{

}

void MusicLrcManagerVerticalDesktop::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(m_font);

    const int fontHeight = TTK::Widget::fontTextHeight(m_font);
    m_linearGradient.setFinalStop(0, fontHeight);
    m_maskLinearGradient.setFinalStop(0, fontHeight);

    if(m_position.x() + m_intervalCount >= m_lrcPerWidth && m_lrcMaskWidth >= m_lrcPerWidth / 2)
    {
        m_intervalCount -= m_lrcMaskWidthInterval;
    }

    painter.translate(m_position.y(), 0);
    painter.rotate(TTK_AN_90);

    //Draw the underlying text, such as shadow, will make the effect more clearly,
    //and more texture
    painter.setPen(QColor(0, 0, 0, 2 * m_transparent));
    painter.drawText(m_intervalCount + 1, 1, m_position.x(), m_position.y(), Qt::AlignLeft, text());

    //Then draw a gradient in the above
    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(m_intervalCount, 0, m_position.x(), m_position.y(), Qt::AlignLeft, text());

    int offsetValue = m_lrcMaskWidth;
    if(!G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toBool())
    {
        offsetValue = (m_lrcMaskWidth != 0) ? m_position.x() : m_lrcMaskWidth;
    }

    //Set lyrics mask
    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(m_intervalCount, 0, offsetValue, 60, Qt::AlignLeft, text());
    painter.translate(-m_position.y(), 0);
}
