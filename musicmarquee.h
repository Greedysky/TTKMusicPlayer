#ifndef MUSICMARQUEE_H
#define MUSICMARQUEE_H

#include <QWidget>
#include "musiclibexportglobal.h"

class MUSIC_EXPORT MusicMarquee : public QWidget
{
    Q_OBJECT
public:
    explicit MusicMarquee(QWidget *parent = 0);

    void setText(const QString &newText);
    inline QString text() const { return m_myText; }
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void timerEvent(QTimerEvent *event);

private:
    QString m_myText;
    int m_offset;
    int m_myTimerId;

};

#endif // MUSICMARQUEE_H
