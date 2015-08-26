#ifndef MUSICLRCMAKERWIDGET_H
#define MUSICLRCMAKERWIDGET_H

#include "musicmovewidgetabstract.h"

namespace Ui {
class MusicLrcMakerWidget;
}

class MUSIC_WIDGET_EXPORT MusicLrcMakerWidget : public MusicMoveWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcMakerWidget(QWidget *parent = 0);
    void setCurrentSongName(const QString &name);
    inline void setCurrentPosition(qint64 pos) { m_position = pos;}

signals:
public slots:
    void makeButtonClicked();
    void saveButtonClicked();
    void reviewButtonClicked();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    void setControlEnable(bool enable) const;
    QString translateTimeString(qint64 time);
    Ui::MusicLrcMakerWidget *ui;

    QFile m_file;
    QStringList m_plainText;
    qint64 m_position;
    int m_currentLine;

};

#endif // MUSICLRCMAKERWIDGET_H
