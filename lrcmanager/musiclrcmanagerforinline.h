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
            { gradientFontSize = size;}
    inline void setTransparent(int transparent)
            { gradientTransparent = transparent;}

protected:
    virtual void paintEvent(QPaintEvent *);
    int gradientFontSize;
    int gradientTransparent;

};

#endif // MUSICLRCMANAGERFORINLINE_H
