#ifndef MUSICGIFLABELWIDGET_H
#define MUSICGIFLABELWIDGET_H

#include <QLabel>

class QTimer;

class MusicGifLabelWidget : public QLabel
{
    Q_OBJECT
public:
    enum Type{
        Gif_Cicle_Blue,
        Gif_Rice_Font_White,
        Gif_Rice_Font_Black,
        Gif_Rice_Font_Black_Big,
        Gif_Hourglass_White,
        Gif_Radio_Blue
    };
    explicit MusicGifLabelWidget(QWidget *parent = 0);
    explicit MusicGifLabelWidget(Type type, QWidget *parent = 0);
    ~MusicGifLabelWidget();

    void setType(Type type);
    Type getType() const;

    void setInterval(int value);
    int getInterval() const;

    void start();
    void stop();

public slots:
    void timeout();

protected:
    Type m_type;
    QTimer *m_timer;
    int m_index;

};

#endif // MUSICGIFLABELWIDGET_H
