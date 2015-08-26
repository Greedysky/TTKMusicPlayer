#ifndef MUSICLOCALSONGSEARCHEDIT_H
#define MUSICLOCALSONGSEARCHEDIT_H

#include <QLineEdit>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicLocalSongSearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchEdit(QWidget *parent = 0);

protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

};

#endif // MUSICLOCALSONGSEARCHEDIT_H
