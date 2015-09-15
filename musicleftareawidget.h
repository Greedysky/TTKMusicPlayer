#ifndef MUSICLEFTAREAWIDGET_H
#define MUSICLEFTAREAWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musiclibexportglobal.h"

class MusicSpectrumWidget;
class MusicLocalSongSearch;
class MusicQualityChoiceWidget;

namespace Ui {
    class MusicApplication;
}

class MUSIC_GUI_EXPORT MusicLeftAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLeftAreaWidget(QWidget *parent = 0);
    ~MusicLeftAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    QString getSearchedText() const;
    void clearSearchedText() const;

public slots:
    void musicSearch();
    void musicSpectrumWidget();
    void musicStackedSongListWidgetChanged();
    void musicStackedToolsWidgetChanged();
    void musicStackedRadioWidgetChanged();
    void musicStackedMyDownWidgetChanged();
    
protected:
    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    QWidget *m_stackedWidget;
    MusicSpectrumWidget *m_musicSpectrumWidget;
    MusicLocalSongSearch *m_musicLocalSongSearch;
    MusicQualityChoiceWidget *m_qualityChoiceWidget;

};

#endif // MUSICLEFTAREAWIDGET_H
