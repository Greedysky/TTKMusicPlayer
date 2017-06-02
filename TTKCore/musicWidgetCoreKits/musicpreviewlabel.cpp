#include "musicpreviewlabel.h"

#include <QPainter>

MusicColorPreviewLabel::MusicColorPreviewLabel(QWidget *parent)
    : MusicClickedLabel(parent)
{
    m_linearGradient.setStart(0, 0);
}

QString MusicColorPreviewLabel::getClassName()
{
    return staticMetaObject.className();
}

void MusicColorPreviewLabel::setLinearGradient(const QList<QColor> &colors)
{
    QLinearGradient linearGradient;
    for(int i=0; i<colors.count(); ++i)
    {
        linearGradient.setColorAt((i+1)*1.0/colors.count(), colors[i]);
    }
    m_linearGradient = linearGradient;
    update();
}

void MusicColorPreviewLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int h = height() - 4*2;
    m_linearGradient.setFinalStop(0, height());

    painter.setFont(font());
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, width(), height());

    painter.setPen(QPen(Qt::black, 0));
    painter.drawRect(0, 0, width() - 1, height() - 1);

    painter.setBrush(m_linearGradient);
    painter.drawRect(4, 4, h, h);

    painter.setPen(QPen(Qt::black, 0));
    painter.drawText(h + 2*4, 4, width() - h - 3*4, h, Qt::AlignCenter, text());

    painter.end();
}



MusicPreviewLabel::MusicPreviewLabel(QWidget *parent)
    : QLabel(parent)
{
    m_transparent = 255;
    m_linearGradient.setStart(0, 0);
    m_maskLinearGradient.setStart(0, 0);
}

QString MusicPreviewLabel::getClassName()
{
    return staticMetaObject.className();
}

void MusicPreviewLabel::setLinearGradient(const QList<QColor> &fg, const QList<QColor> &bg)
{
    QLinearGradient linearGradient, maskLinearGradient;
    for(int i=0; i<fg.count(); ++i)
    {
        QColor rgb = fg[i];
        rgb.setAlpha(m_transparent);
        linearGradient.setColorAt((i+1)*1.0/fg.count(), rgb);
    }
    for(int i=0; i<bg.count(); ++i)
    {
        QColor rgb = bg[i];
        rgb.setAlpha(m_transparent);
        maskLinearGradient.setColorAt((i+1)*1.0/bg.count(), rgb);
    }
    m_linearGradient = linearGradient;
    m_maskLinearGradient = maskLinearGradient;
}

void MusicPreviewLabel::setParameter(const QStringList &para)
{
    Q_ASSERT(para.count() == 3);
    m_font.setFamily(para[0]);
    m_font.setPointSize(para[1].toInt());
    int type = para[2].toInt();
    m_font.setBold( (type == 1 || type == 3) );
    m_font.setItalic( (type == 2 || type == 3) );
}

void MusicPreviewLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int h = QFontMetrics(m_font).height();
    int begin = (rect().height() - h)/2;
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
    painter.end();
}
