#include "ttkthemelinelabel.h"

#include <QPainter>

TTKThemeLineLabel::TTKThemeLineLabel(QWidget *parent)
    : QLabel(parent)
{

}

void TTKThemeLineLabel::paintEvent(QPaintEvent *)
{
    const QFontMetrics ftm(font());
    const int w = QtFontWidth(ftm, text()) + 15;

    QPainter painter(this);
    painter.setFont(font());
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, text());

    painter.setPen(QPen(QColor(0, 0, 0, 25), 1));
    painter.drawLine(w, height() / 2, width(), height() / 2);
}
