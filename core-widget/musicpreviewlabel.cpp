#include "musicpreviewlabel.h"

#include <QPainter>

MusicPreviewLabel::MusicPreviewLabel(QWidget *parent)
    : QLabel(parent)
{
    m_transparent = 255;
}

void MusicPreviewLabel::setLinearGradient(QColor &fg, QColor &bg)
{
    fg.setAlpha(m_transparent);
    bg.setAlpha(m_transparent);
    m_linearGradient.setColorAt(0.1, bg);
    m_linearGradient.setColorAt(0.5, QColor(114, 232, 255));
    m_linearGradient.setColorAt(0.9, bg);
    m_maskLinearGradient.setColorAt(0.1, fg);
    m_maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
    m_maskLinearGradient.setColorAt(0.9, fg);
}

void MusicPreviewLabel::setParameter(const QStringList &para)
{
    m_font.setFamily(para[0]);
    m_font.setPointSize(para[1].toInt() + 13);
    int type = para[2].toInt();
    m_font.setBold( (type == 1 || type == 3) ? true : false );
    m_font.setItalic( (type == 2 || type == 3) ? true : false );
}

void MusicPreviewLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(m_font);
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");

    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");

    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(1, 1, rect().width(), rect().height(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");

    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(1, 1, 60, rect().height(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");
    painter.end();
}
