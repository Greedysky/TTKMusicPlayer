#ifndef MUSICSONGSLISTPLAYWIDGET_H
#define MUSICSONGSLISTPLAYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include "core/musiclibexportglobal.h"

class MusicSongsToolItemRenamedWidget;

class MUSIC_EXPORT MusicSongsListPlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListPlayWidget(QWidget *parent = 0);
    ~MusicSongsListPlayWidget();

    void getWidget(QWidget *&one, QWidget *&two);
    void insertTimerLabel(const QString& t);
    void setParameter(const QString& name,const QString& path);
    void setItemRename();

signals:
    void renameFinished(const QString &);

public slots:
    void setChangItemName(const QString &);

protected:
    bool showArtPicture(const QString& name);

    QLabel *m_artPicture;
    QLabel *m_songName;
    QLabel *m_timeLabel;
    QString m_totalTime;
    QWidget *m_columnOne;
    QWidget *m_columnThree;
    QPushButton *m_loveButton;
    QPushButton *m_deleteButton;
    QToolButton *m_addButton;
    QMenu m_listMenu;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};

#endif // MUSICSONGSLISTPLAYWIDGET_H
