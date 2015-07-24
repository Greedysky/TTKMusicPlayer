#ifndef MUSICLRCSEARCHWIDGET_H
#define MUSICLRCSEARCHWIDGET_H

#include "musicmovedialogabstract.h"

namespace Ui {
class MusicLrcSearchWidget;
}

class MUSIC_EXPORT MusicLrcSearchWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcSearchWidget(QWidget *parent = 0);
    ~MusicLrcSearchWidget();

    void setCurrentSongName(const QString&);

signals:

public slots:
    void lrcSearchButtonClicked();
    void lrcSearchDownloadClicked();
    void lrcDownloadStateChanged(const QString&);

protected:
    Ui::MusicLrcSearchWidget *ui;

};

#endif // MUSICLRCSEARCHWIDGET_H
