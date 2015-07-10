#ifndef MUSICLOCALSONGSEARCHINLINEEDIT_H
#define MUSICLOCALSONGSEARCHINLINEEDIT_H

#include "musiclocalsongsearchedit.h"

class QListWidget;

class MusicLocalSongSearchInlineEdit : public MusicLocalSongSearchEdit
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchInlineEdit(QWidget *parent = 0);
    ~MusicLocalSongSearchInlineEdit();

signals:
public slots:

protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    QListWidget *m_historyLists;

};

#endif // MUSICLOCALSONGSEARCHINLINEEDIT_H
