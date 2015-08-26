#ifndef MUSICSONGSTOOLITEMRENAMEDWIDGET_H
#define MUSICSONGSTOOLITEMRENAMEDWIDGET_H

#include <QLineEdit>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicSongsToolItemRenamedWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicSongsToolItemRenamedWidget(int offset,
             const QString &originText, QWidget *parent = 0);

protected:
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

signals:
    void renameFinished(const QString &text);

public slots:
    void renameFinished(){ emit renameFinished(text()); }

};

#endif // MUSICSONGSTOOLITEMRENAMEDWIDGET_H
