#ifndef MUSICLRCMANAGERFORINLINE_H
#define MUSICLRCMANAGERFORINLINE_H

#include "musiclrcmanager.h"

class MUSIC_EXPORT MusicLRCManagerForInline : public MusicLRCManager
{
    Q_OBJECT
public:
    explicit MusicLRCManagerForInline(QWidget *parent = 0);
    ~MusicLRCManagerForInline();

    inline void setFontSize(int size)
            { m_gradientFontSize = size;}
    inline void setTransparent(int transparent)
            { m_gradientTransparent = transparent;}
    inline void setUpdateLrc(bool status)
            { m_UpdateLrc = status;}

protected:
    virtual void paintEvent(QPaintEvent *);
    int m_gradientFontSize;
    int m_gradientTransparent;
    bool m_UpdateLrc;

};

#endif // MUSICLRCMANAGERFORINLINE_H
