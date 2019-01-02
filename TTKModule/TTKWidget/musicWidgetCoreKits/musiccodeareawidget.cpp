#include "musiccodeareawidget.h"
#include "musictime.h"

#include <QPainter>
#include <QPaintEvent>

#define DEF_CODECOUNT       4
#define DEF_NOISYPOINTCOUNT 60
#define DEF_CONVERSEROTATE  10
#define DEF_CONVERSESCALE   15

MusicCodeAreaWidget::MusicCodeAreaWidget(QWidget *parent)
    : MusicClickedLabel(parent)
{
    MusicTime::timeSRand();
    m_slCodeRange << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" <<
       "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j" << "k" << "l" << "m" << "n" <<
       "o" << "p" << "q" << "r" << "s" << "t" << "u" << "v" << "w" << "x" << "y" << "z" <<
       "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" <<
       "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z";

    m_lCodeColor << Qt::darkRed << Qt::darkGreen << Qt::darkBlue << Qt::darkCyan << Qt::darkMagenta << Qt::darkYellow << Qt::darkGray;

    m_nNoisyPointCount = DEF_NOISYPOINTCOUNT;
    m_nConverseRotate = DEF_CONVERSEROTATE;
    m_nConverseScale = DEF_CONVERSESCALE;
    m_nCodeCount = DEF_CODECOUNT;
}

void MusicCodeAreaWidget::renderPicture()
{
    QStringList number;
    for(int i=0; i<m_nCodeCount; i++)
    {
        number << m_slCodeRange[qrand() % m_slCodeRange.count()];
    }

    m_sCode.clear();
    m_lCodePic.clear();

    QFont font;
    font.setBold(true);
    font.setPixelSize(25);

    for(int i=0; i<m_nCodeCount; i++)
    {
        m_sCode.append(number[i]);
        QPainterPath path(QPointF(0, 0));
        QRect fontBoundingRect = QFontMetrics(font).boundingRect(number[i]);
        path.addText(-QPointF(fontBoundingRect.center()), font, number[i]);
        m_lCodePic << path;
    }
    update();
}

void MusicCodeAreaWidget::setCodeCount(int nCount)
{
    m_nCodeCount = nCount;
}

void MusicCodeAreaWidget::setNoisyPointCount(int nCount)
{
    m_nNoisyPointCount = nCount;
}

void MusicCodeAreaWidget::paintEvent(QPaintEvent *event)
{
    MusicClickedLabel::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    painter.translate(0, 0);
    painter.save();
    painter.translate(event->rect().center().x() - m_lCodePic.size()/2*6, event->rect().center().y());

    for(int i=0; i<m_lCodePic.size(); i++)
    {
        drawConversion(painter);
        painter.fillPath(m_lCodePic[i], QBrush(m_lCodeColor[qrand() % m_lCodeColor.count()]));
        painter.translate(10, 0);
    }
    painter.restore();

    drawOutline(painter);
    drawNoisyPoint(painter);
}

void MusicCodeAreaWidget::drawOutline(QPainter &painter)
{
    painter.setPen(Qt::darkGreen);
    painter.setPen(Qt::DashLine);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rect());
}

void MusicCodeAreaWidget::drawNoisyPoint(QPainter &painter)
{
    painter.setPen(Qt::red);
    painter.setPen(Qt::DotLine);
    painter.setBrush(Qt::NoBrush);

    for(int i=0; i<m_nNoisyPointCount; i++)
    {
        painter.drawPoint(QPointF(qrand() % size().width(), qrand() % size().height()));
    }
}

void MusicCodeAreaWidget::drawConversion(QPainter &painter)
{
    if(qrand() % 2)
    {
        painter.rotate(qrand() % m_nConverseRotate);
    }
    else
    {
        painter.rotate(-(qrand() % m_nConverseRotate));
    }
    painter.scale((qrand() % m_nConverseScale + (100 - m_nConverseScale)) / 100.0, (qrand() % m_nConverseScale + (100 - m_nConverseScale)) / 100.0);
}
