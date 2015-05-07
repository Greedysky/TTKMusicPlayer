#ifndef MUSICVIDEOPLAYLISTWIDGET_H
#define MUSICVIDEOPLAYLISTWIDGET_H

#include <QWidget>
#include "../core/musiclibexportglobal.h"

class QPushButton;
class QListWidget;

class MUSIC_EXPORT MusicVideoPlayListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicVideoPlayListWidget(QWidget *parent = 0);
    ~MusicVideoPlayListWidget();

    void addItems(const QStringList&);
    void setCurrentIndex(int index);

signals:

public slots:
protected:
    QListWidget* m_playList;
    QWidget* m_barWidget;
    QPushButton* m_closeButton;

};

#endif // MUSICVIDEOPLAYLISTWIDGET_H
