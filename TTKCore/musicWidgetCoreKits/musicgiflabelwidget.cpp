#include "musicgiflabelwidget.h"

#include <QTimer>

#define GIF_BALLON_WHITE        35
#define GIF_CICLE_BLUE          58
#define GIF_RICE_FONT_WHITE     26
#define GIF_RICE_FONT_BLACK_BIG 42
#define GIF_RICE_FONT_BLACK     16
#define GIF_HOURGLASS_WHITE     38
#define GIF_RADIO_BLUE          14
#define GIF_CHECK_BLUE          93
#define GIF_RECORD_RED          30

MusicGifLabelWidget::MusicGifLabelWidget(QWidget *parent)
    : QLabel(parent)
{
    m_index = -1;
    m_isRunning = false;

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
        case Gif_Ballon_White: setFixedSize(GIF_BALLON_WHITE, GIF_BALLON_WHITE); break;
        case Gif_Cicle_Blue: setFixedSize(GIF_CICLE_BLUE, GIF_CICLE_BLUE); break;
        case Gif_Rice_Font_White: setFixedSize(GIF_RICE_FONT_WHITE, GIF_RICE_FONT_WHITE); break;
        case Gif_Rice_Font_Black_Big: setFixedSize(GIF_RICE_FONT_BLACK_BIG, GIF_RICE_FONT_BLACK_BIG); break;
        case Gif_Rice_Font_Black: setFixedSize(GIF_RICE_FONT_BLACK, GIF_RICE_FONT_BLACK); break;
        case Gif_Hourglass_White: setFixedSize(GIF_HOURGLASS_WHITE, GIF_HOURGLASS_WHITE); break;
        case Gif_Radio_Blue: setFixedSize(GIF_RADIO_BLUE, GIF_RADIO_BLUE); break;
        case Gif_Check_Blue: setFixedSize(GIF_CHECK_BLUE, GIF_CHECK_BLUE); break;
        case Gif_Record_red: setFixedSize(GIF_RECORD_RED, GIF_RECORD_RED); break;
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
    m_isRunning = true;
}

void MusicGifLabelWidget::stop()
{
    m_timer->stop();
    m_isRunning = false;
}

void MusicGifLabelWidget::timeout()
{
    ++m_index;
    switch(m_type)
    {
        case Gif_Ballon_White:
            {
                if(m_index == 40)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_ballon_white'); \
                                       margin-left:-%1px;").arg(GIF_BALLON_WHITE*m_index));
                break;
            }
        case Gif_Cicle_Blue:
            {
                if(m_index == 12)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_cicle_blue'); \
                                       margin-left:-%1px;").arg(GIF_CICLE_BLUE*m_index));
                break;
            }
        case Gif_Rice_Font_White:
            {
                if(m_index == 8)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_rice_font_white'); \
                                       margin-left:-%1px;").arg(GIF_RICE_FONT_WHITE*m_index));

                break;
            }
        case Gif_Rice_Font_Black_Big:
            {
                if(m_index == 8)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_rice_font_black_big'); \
                                       margin-left:-%1px;").arg(GIF_RICE_FONT_BLACK_BIG*m_index));
                break;
            }
        case Gif_Rice_Font_Black:
            {
                if(m_index == 12)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_rice_font_black'); \
                                       margin-left:-%1px;").arg(GIF_RICE_FONT_BLACK*m_index));
                break;
            }
        case Gif_Hourglass_White:
            {
                if(m_index == 16)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_hourglass_white'); \
                                       margin-left:-%1px;").arg(GIF_HOURGLASS_WHITE*m_index));
                break;
            }
        case Gif_Radio_Blue:
            {
                if(m_index == 10)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_radio_blue'); \
                                       margin-left:-%1px;").arg(GIF_RADIO_BLUE*m_index));
                break;
            }
        case Gif_Check_Blue:
            {
                if(m_index == 22)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_check_blue'); \
                                       margin-left:-%1px;").arg(GIF_CHECK_BLUE*m_index));
                break;
            }
        case Gif_Record_red:
            {
                if(m_index == 5)
                {
                    m_index = 0;
                }
                setStyleSheet(QString("background-image: url(':/gif/lb_record_red'); \
                                       margin-left:-%1px;").arg(GIF_RECORD_RED*m_index));
                break;
            }
        default: break;
    }
}
