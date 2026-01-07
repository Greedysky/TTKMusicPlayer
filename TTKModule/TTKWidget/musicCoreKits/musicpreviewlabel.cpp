#include "musicpreviewlabel.h"
#include "musicwidgetutils.h"

MusicColorPreviewLabel::MusicColorPreviewLabel(QWidget *parent)
    : TTKClickedLabel(parent)
{
    m_linearGradient.setStart(0, 0);
}

void MusicColorPreviewLabel::setColors(const QList<QColor> &colors)
{
    QLinearGradient linearGradient;
    for(int i = 0; i < colors.count(); ++i)
    {
        linearGradient.setColorAt((i + 1) * 1.0 / colors.count(), colors[i]);
    }

    m_gradientColors = colors;
    m_linearGradient = linearGradient;
    update();
}

QList<QColor> MusicColorPreviewLabel::colors() const noexcept
{
    return m_gradientColors;
}

QColor MusicColorPreviewLabel::color() const
{
    return !m_gradientColors.empty() ? m_gradientColors.first() :  QColor();
}

void MusicColorPreviewLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    const int h = height() - 4 * 2;
    m_linearGradient.setFinalStop(0, height());

    painter.setFont(font());
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, width(), height());

    painter.setPen(QPen(Qt::black, 0));
    painter.drawRect(0, 0, width() - 1, height() - 1);

    painter.setBrush(m_linearGradient);
    painter.drawRect(4, 4, h, h);

    painter.setPen(QPen(Qt::black, 0));
    painter.drawText(h + 2 * 4, 4, width() - h - 3 * 4, h, Qt::AlignCenter, text());
}



MusicPreviewLabel::MusicPreviewLabel(QWidget *parent)
    : QLabel(parent),
      m_transparent(255)
{
    m_linearGradient.setStart(0, 0);
    m_maskLinearGradient.setStart(0, 0);
}

void MusicPreviewLabel::setLinearGradient(const MusicPreviewLabelItem &item)
{
    m_font.setFamily(item.m_family);
    m_font.setPointSize(item.m_size);
    m_font.setBold((item.m_type == 1 || item.m_type == 3));
    m_font.setItalic((item.m_type == 2 || item.m_type == 3));

    setLinearGradient(item.m_frontground, item.m_background);
}

void MusicPreviewLabel::setLinearGradient(const QList<QColor> &front, const QList<QColor> &back)
{
    QLinearGradient linearGradient, maskLinearGradient;
    for(int i = 0; i < back.count(); ++i)
    {
        QColor rgb = back[i];
        rgb.setAlpha(m_transparent);
        linearGradient.setColorAt((i + 1) * 1.0 / back.count(), rgb);
    }

    for(int i = 0; i < front.count(); ++i)
    {
        QColor rgb = front[i];
        rgb.setAlpha(m_transparent);
        maskLinearGradient.setColorAt((i + 1) * 1.0 / front.count(), rgb);
    }

    m_linearGradient = linearGradient;
    m_maskLinearGradient = maskLinearGradient;
}

void MusicPreviewLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    const int h = TTK::Widget::fontTextHeight(m_font);
    const int begin = (rect().height() - h) / 2;
    m_linearGradient.setFinalStop(0, h + begin);
    m_maskLinearGradient.setFinalStop(0, h + begin);

    painter.setFont(m_font);
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, "This is TTKMusicPlayer");

    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, "This is TTKMusicPlayer");

    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(1, 1, rect().width(), rect().height(), Qt::AlignLeft | Qt::AlignVCenter, "This is TTKMusicPlayer");

    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(1, 1, 60, rect().height(), Qt::AlignLeft | Qt::AlignVCenter, "This is TTKMusicPlayer");
}
