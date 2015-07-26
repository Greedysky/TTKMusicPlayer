#ifndef MUSICLOCALSONGSEARCHINLINEEDIT_H
#define MUSICLOCALSONGSEARCHINLINEEDIT_H

#include "musiclocalsongsearchedit.h"

class MusicLocalSongSearchPopWidget;

class MUSIC_WIDGET_EXPORT MusicLocalSongSearchInlineEdit : public MusicLocalSongSearchEdit
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchInlineEdit(QWidget *parent = 0);
    ~MusicLocalSongSearchInlineEdit();

signals:
public slots:

protected:
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void enterEvent(QEvent *event);
    MusicLocalSongSearchPopWidget *m_popWidget;

};

#endif // MUSICLOCALSONGSEARCHINLINEEDIT_H
