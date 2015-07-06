#ifndef MUSICLRCCCONTAINERFORINLINELR_H
#define MUSICLRCCCONTAINERFORINLINELR_H

#include "musiclrccontainer.h"

#define MIN_LRCCONTAIN_COUNT 5
#define CURRENT_LRC_PAINT 2

class QVBoxLayout;
class MusicLRCManagerForInline;

class MUSIC_EXPORT MusicLrcContainerForInline : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForInline(QWidget *parent = 0);
    virtual ~MusicLrcContainerForInline();

    virtual void startTimerClock();
    virtual void stopLrcMask();
    virtual void setMaskLinearGradientColor(QColor = LrcColorTable::CL_Mask);
    void updateCurrentLrc(qint64 time);

    bool transLrcFileToTime(const QString& lrcFileName);
    QString text() const;
    inline QMap<qint64,QString>& getLrcContainer() {return m_lrcContainer;}
    void changeLrcSize(LrcSizeTable = Middle);
    void setSongSpeedAndSlow(qint64 time);

signals:
    void updateCurrentTime(qint64 time);

public slots:
    void changeLrcSizeSmaller(){changeLrcSize(LrcSizeTable::Smaller);}
    void changeLrcSizeSmall(){changeLrcSize(LrcSizeTable::Small);}
    void changeLrcSizeMiddle(){changeLrcSize(LrcSizeTable::Middle);}
    void changeLrcSizeBig(){changeLrcSize(LrcSizeTable::Big);}
    void changeLrcSizeBigger(){changeLrcSize(LrcSizeTable::Bigger);}

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *);
    virtual void paintEvent(QPaintEvent *event);

    QPoint m_mousePressedAt;
    QPoint m_mouseMovedAt;
    bool m_mouseLeftPressed;
    int m_currentLrcIndex;
    QVBoxLayout *m_vBoxLayout;
    QStringList m_currentShowLrcContainer;
    QMap<qint64,QString> m_lrcContainer;

};

#endif // MUSICLRCCCONTAINERFORINLINELR_H
