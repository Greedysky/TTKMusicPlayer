#include "musicpreviewlabel.h"
#include <QPainter>
#include <QDebug>

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
    if(type == 1)
    {
        m_font.setBold(true);
        m_font.setItalic(false);
    }
    else if(type == 2)
    {
        m_font.setBold(false);
        m_font.setItalic(true);
    }
    else if(type == 3)
    {
        m_font.setBold(true);
        m_font.setItalic(true);
    }
    else
    {
        m_font.setBold(false);
        m_font.setItalic(false);
    }
}

void MusicPreviewLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(m_font);
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");

    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");

    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(1,1,rect().width(),rect().height(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");

    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(1,1,60,rect().height(), Qt::AlignLeft | Qt::AlignVCenter, "This is QMusicPlayer");
    painter.end();
}
