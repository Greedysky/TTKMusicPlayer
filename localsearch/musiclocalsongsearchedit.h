#ifndef MUSICLOCALSONGSEARCHEDIT_H
#define MUSICLOCALSONGSEARCHEDIT_H

#include <QLineEdit>
#include "core/musiclibexportglobal.h"

class MUSIC_EXPORT MusicLocalSongSearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchEdit(QWidget *parent = 0);

signals:
public slots:

protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

};


class MusicLocalSongSearchPopWidget;

class MUSIC_EXPORT MusicLocalSongSearchInlineEdit : public MusicLocalSongSearchEdit
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
    virtual void enterEvent(QEvent *event);
    MusicLocalSongSearchPopWidget *m_popWidget;

};

#endif // MUSICLOCALSONGSEARCHEDIT_H
