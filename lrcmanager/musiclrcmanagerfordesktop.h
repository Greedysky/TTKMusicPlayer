#ifndef MUSICLRCMANAGERFORDESKTOP_H
#define MUSICLRCMANAGERFORDESKTOP_H

#include "musiclrcmanager.h"

class MUSIC_WIDGET_EXPORT MusicLRCManagerForDesktop : public MusicLRCManager
{
    Q_OBJECT
public:
    explicit MusicLRCManagerForDesktop(QWidget *parent = 0);
    void setSelfGeometry(const QPoint &point);
    void setSelfGeometry(int x, int y);
    int getGeometryX() const;
    void resetOrigin();
    void setLrcFontSize(int size);
    inline int getFirstFontSize() const { return m_font.pointSize(); }

protected:
    virtual void paintEvent(QPaintEvent *event);

};

#endif // MUSICLRCMANAGERFORDESKTOP_H
