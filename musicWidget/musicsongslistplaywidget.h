#ifndef MUSICSONGSLISTPLAYWIDGET_H
#define MUSICSONGSLISTPLAYWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include "musiclibexportglobal.h"

class MusicSongsToolItemRenamedWidget;

class MUSIC_WIDGET_EXPORT MusicSongsEnterPlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsEnterPlayWidget(int index, QWidget *parent = 0);

    inline void setCurrentPlayIndex(int index) { m_currentPlayIndex = index;}
    inline int getCurrentPlayIndex() const { return m_currentPlayIndex;}

signals:
    void enterChanged(int row, int column);

protected:
    void enterEvent(QEvent *event);
    int m_currentPlayIndex;

};


class MUSIC_WIDGET_EXPORT MusicSongsListPlayWidget : public MusicSongsEnterPlayWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListPlayWidget(int index, QWidget *parent = 0);
    virtual ~MusicSongsListPlayWidget();

    void getWidget(QWidget *&one, QWidget *&two) const;
    void insertTimerLabel(const QString &t) const;
    void setParameter(const QString &name, const QString &path);
    void setItemRename();
    bool getItemRenameState() { return m_renameLine != nullptr;}

signals:
    void renameFinished(const QString &text);
    void videoButtonClicked(const QString &text);

public slots:
    void setChangItemName(const QString &name);
    void showMVButtonClicked();
    void downloadButtonClicked();
    void sharingButtonClicked();

protected:
    bool showArtPicture(const QString &name) const;

    QLabel *m_artPicture, *m_songName, *m_timeLabel;
    QString m_totalTime;
    MusicSongsEnterPlayWidget *m_columnOne, *m_columnThree;
    QPushButton *m_loveButton, *m_deleteButton,* m_showMVButton;
    QPushButton *m_downloadButton, *m_songShareButton;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};

#endif // MUSICSONGSLISTPLAYWIDGET_H
