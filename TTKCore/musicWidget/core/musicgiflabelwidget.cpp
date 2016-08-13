#include "musicgiflabelwidget.h"

#include <QTimer>

#define GIF_CICLE_BLUE          58
#define GIF_RICE_FONT_WHITE     26
#define GIF_RICE_FONT_BLACK_BIG 42
#define GIF_RICE_FONT_BLACK     16
#define GIF_HOURGLASS_WHITE     38
#define GIF_RADIO_BLUE          14

MusicGifLabelWidget::MusicGifLabelWidget(QWidget *parent)
    : QLabel(parent)
{
    m_index = 0;
    setFixedSize(GIF_CICLE_BLUE, GIF_CICLE_BLUE);

    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MusicGifLabelWidget::MusicGifLabelWidget(Type type, QWidget *parent)
    : MusicGifLabelWidget(parent)
{
    setType(type);
}

MusicGifLabelWidget::~MusicGifLabelWidget()
{
    delete m_timer;
}

void MusicGifLabelWidget::setType(Type type)
{
    switch(m_type = type)
    {
        case Gif_Cicle_Blue: setFixedSize(GIF_CICLE_BLUE, GIF_CICLE_BLUE); break;
        case Gif_Rice_Font_White: setFixedSize(GIF_RICE_FONT_WHITE, GIF_RICE_FONT_WHITE); break;
        case Gif_Rice_Font_Black_Big: setFixedSize(GIF_RICE_FONT_BLACK_BIG, GIF_RICE_FONT_BLACK_BIG); break;
        case Gif_Rice_Font_Black: setFixedSize(GIF_RICE_FONT_BLACK, GIF_RICE_FONT_BLACK); break;
        case Gif_Hourglass_White: setFixedSize(GIF_HOURGLASS_WHITE, GIF_HOURGLASS_WHITE); break;
        case Gif_Radio_Blue: setFixedSize(GIF_RADIO_BLUE, GIF_RADIO_BLUE); break;
        default: break;
    }
}

MusicGifLabelWidget::Type MusicGifLabelWidget::getType() const
{
    return m_type;
}

void MusicGifLabelWidget::setInterval(int value)
{
    m_timer->setInterval(value);
}

int MusicGifLabelWidget::getInterval() const
{
    return m_timer->interval();
}

void MusicGifLabelWidget::start()
{
    m_timer->start();
}

void MusicGifLabelWidget::stop()
{
    m_timer->stop();
}

void MusicGifLabelWidget::timeout()
{
    ++m_index;
    switch(m_type)
    {
        case Gif_Cicle_Blue:
            setStyleSheet(QString("background-image: url(':/gif/lb_cicle_blue'); \
                                   margin-left:-%1px;").arg(GIF_CICLE_BLUE*m_index));
            if(m_index == 13)
            {
                m_index = 0;
            }
            break;
        case Gif_Rice_Font_White:
            setStyleSheet(QString("background-image: url(':/gif/lb_rice_font_white'); \
                                   margin-left:-%1px;").arg(GIF_RICE_FONT_WHITE*m_index));
            if(m_index == 9)
            {
                m_index = 0;
            }
            break;
        case Gif_Rice_Font_Black_Big:
            setStyleSheet(QString("background-image: url(':/gif/lb_rice_font_black_big'); \
                                   margin-left:-%1px;").arg(GIF_RICE_FONT_BLACK_BIG*m_index));
            if(m_index == 9)
            {
                m_index = 0;
            }
            break;
        case Gif_Rice_Font_Black:
            setStyleSheet(QString("background-image: url(':/gif/lb_rice_font_black'); \
                                   margin-left:-%1px;").arg(GIF_RICE_FONT_BLACK*m_index));
            if(m_index == 22)
            {
                m_index = 0;
            }
            break;
        case Gif_Hourglass_White:
            setStyleSheet(QString("background-image: url(':/gif/lb_hourglass_white'); \
                                   margin-left:-%1px;").arg(GIF_HOURGLASS_WHITE*m_index));
            if(m_index == 17)
            {
                m_index = 0;
            }
            break;
        case Gif_Radio_Blue:
            setStyleSheet(QString("background-image: url(':/gif/lb_radio_blue'); \
                                   margin-left:-%1px;").arg(GIF_RADIO_BLUE*m_index));
            if(m_index == 11)
            {
                m_index = 0;
            }
            break;
        default: break;
    }
}
