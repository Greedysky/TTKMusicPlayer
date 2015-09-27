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

class MUSIC_WIDGET_EXPORT MusicSongsListPlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListPlayWidget(QWidget *parent = 0);
    ~MusicSongsListPlayWidget();

    void getWidget(QWidget *&one, QWidget *&two) const;
    void insertTimerLabel(const QString &t) const;
    void updateArtPicture() const;
    void setParameter(const QString &name, const QString &path);
    void setItemRename();

signals:
    void renameFinished(const QString &text);

public slots:
    void setChangItemName(const QString &name);
    void showMVButtonClicked();

protected:
    bool showArtPicture(const QString &name) const;

    QLabel *m_artPicture;
    QLabel *m_songName;
    QLabel *m_timeLabel;
    QString m_totalTime;
    QWidget *m_columnOne;
    QWidget *m_columnThree;
    QPushButton *m_loveButton;
    QPushButton *m_deleteButton;
    QPushButton *m_showMVButton;
    QPushButton *m_songShareButton;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};

#endif // MUSICSONGSLISTPLAYWIDGET_H
