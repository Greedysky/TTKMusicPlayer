#ifndef MUSICSONGSTOOLITEMRENAMEDWIDGET_H
#define MUSICSONGSTOOLITEMRENAMEDWIDGET_H

#include <QLineEdit>
#include "core/musiclibexportglobal.h"

class MUSIC_EXPORT MusicSongsToolItemRenamedWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicSongsToolItemRenamedWidget(int offset,
             const QString& originText,QWidget *parent = 0);

protected:
    virtual void focusOutEvent(QFocusEvent *);
    virtual void contextMenuEvent(QContextMenuEvent *);

signals:
    void renameFinished(const QString&);

public slots:
    void renameFinished(){ emit renameFinished(text()); }

};

#endif // MUSICSONGSTOOLITEMRENAMEDWIDGET_H
