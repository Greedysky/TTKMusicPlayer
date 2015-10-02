#ifndef MUSICTOPAREAWIDGET_H
#define MUSICTOPAREAWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QTimer>
#include "musiclibexportglobal.h"

class MusicUserWindow;
class MusicRemoteWidget;
class MusicBackgroundSkinDialog;

namespace Ui {
    class MusicApplication;
}

class MUSIC_GUI_EXPORT MusicTopAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicTopAreaWidget(QWidget *parent = 0);
    ~MusicTopAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    void setParameters(const QString &skin, int alpha, int alphaR);
    QString getBgSkin() const {return m_currentBgSkin;}
    int getBgSkinAlpha() const {return m_alpha;}
    int getListBgSkinAlpha();

    void setTimerStop();
    void showPlayStatus(bool status);
    void setLabelText(const QString &name) const;
    void setVolumeValue(int value) const;

signals:
    void setTransparent(int index);
    void updateArtPicture();
    void musicSearchButtonClicked();

public slots:
    void musicShowSkinChangedWindow();
    void musicUserContextLogin();
    void musicBgTransparentChanged();
    void musicBgTransparentChanged(int index);
    void musicBackgroundSkinChanged(const QString &filename);
    void musicBackgroundChanged();
    void musicBgThemeDownloadFinished();
    void musicCircleRemote();
    void musicDiamondRemote();
    void musicSquareRemote();
    void musicRectangleRemote();
    void musicDeleteRemote();
    //This is a slot by MusicRemoteWidget's signal emit
    void musicVolumeChangedFromRemote(int value);
    void musicRemoteTypeChanged(QAction *type);
    /////////////////////////////////////////////

protected:
    void drawWindowBackgroundRect();
    void drawWindowBackgroundRectString(const QString &path);
    void createRemoteWidget();

    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    MusicUserWindow *m_msuicUserWindow;
    MusicBackgroundSkinDialog *m_musicbgskin;
    MusicRemoteWidget *m_musicRemoteWidget;

    QString m_currentBgSkin;
    int m_alpha;
    int m_listAlpha;
    QTimer m_pictureCarouselTimer;
    bool m_currentPlayStatus;

};

#endif // MUSICTOPAREAWIDGET_H
