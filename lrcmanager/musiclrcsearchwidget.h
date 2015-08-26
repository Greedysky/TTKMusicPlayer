#ifndef MUSICLRCSEARCHWIDGET_H
#define MUSICLRCSEARCHWIDGET_H

#include "musicmovedialogabstract.h"

namespace Ui {
class MusicLrcSearchWidget;
}

class MUSIC_WIDGET_EXPORT MusicLrcSearchWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcSearchWidget(QWidget *parent = 0);
    ~MusicLrcSearchWidget();

    void setCurrentSongName(const QString &name) const;

signals:

public slots:
    void lrcSearchButtonClicked() const;
    void lrcSearchDownloadClicked();
    void lrcDownloadStateChanged(const QString &string);

protected:
    Ui::MusicLrcSearchWidget *ui;

};

#endif // MUSICLRCSEARCHWIDGET_H
