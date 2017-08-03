#ifndef MUSICTOPAREAWIDGET_H
#define MUSICTOPAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QTimer>
#include "musicglobaldefine.h"

class MusicUserWindow;
class MusicRemoteWidget;
class MusicBackgroundSkinDialog;
class MusicDownloadCounterPVThread;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app top area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicTopAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicTopAreaWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicTopAreaWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    static MusicTopAreaWidget *instance();
    /*!
     * Get class object instance.
     */
    void setupUi(Ui::MusicApplication* ui);
    /*!
     * Set up app ui.
     */

    void setParameters(const QString &skin, int alpha, int list);
    /*!
     * Set background skin and alpha and list alpha parameter.
     */
    inline QString getBackgroundPath() const { return m_backgroundImagePath; }
    /*!
     * Get background skin name.
     */
    inline int getBackgroundAlpha() const { return m_backgroundAlpha; }
    /*!
     * Get background skin alpha.
     */
    int getBackgroundListAlpha();
    /*!
     * Get list background skin alpha.
     */
    QPixmap getRendererPixmap() const;
    /*!
     * Get background skin pixmap.
     */

    bool getUserLoginState() const;
    /*!
     * Check if the user is login now.
     */

    void setTimerStop();
    /*!
     * Stop background skin change tiemr.
     */
    void showPlayStatus(bool status);
    /*!
     * Set current play state button.
     */
    void setLabelText(const QString &name) const;
    /*!
     * Set current song text.
     */
    void setVolumeValue(int value) const;
    /*!
     * Set current volume value by index.
     */

Q_SIGNALS:
    void setTransparent(int index);
    /*!
     * Set list background skin transparent emit.
     */
    void musicSearchButtonClicked();
    /*!
     * Search current music song from net.
     */

public Q_SLOTS:
    void musicSearchTopListInfoFinished(const QString &data);
    /*!
     * Search top list information finished.
     */
    void musicShowSkinChangedWindow();
    /*!
     * Show change background skin dailog.
     */
    void musicUserContextLogin();
    /*!
     * Send user to login.
     */
    void musicSetAsArtBackground();
    /*!
     * Set background as art big picture.
     */
    void musicBgTransparentChanged();
    /*!
     * Current background transparent changed.
     */
    void musicBgTransparentChanged(int index);
    /*!
     * Current background transparent changed by index.
     */
    void musicBgTransparentChanged(const QString &fileName);
    /*!
     * Current background transparent changed by string.
     */
    void musicBackgroundSkinChanged(const QString &fileName);
    /*!
     * Current background skin changed by new path.
     */
    void musicBackgroundChanged();
    /*!
     * Changed current background skin.
     */
    void musicBackgroundSliderStateChanged(bool state);
    /*!
     * Current slider state changed.
     */
    void musicBgThemeDownloadFinished();
    /*!
     * Current background skin download is finished.
     */
    void musicBgThemeChangedByResize();
    /*!
     * Resize current background skin when geometry changed.
     */
    void musicPlayListTransparent(int index);
    /*!
     * Current list background transparent changed.
     */
    ///This is a slot by MusicRemoteWidget's signal emit
    void musicCircleRemote();
    /*!
     * Changed current remote to circle mode.
     */
    void musicDiamondRemote();
    /*!
     * Changed current remote to diamond mode.
     */
    void musicSquareRemote();
    /*!
     * Changed current remote to square mode.
     */
    void musicRectangleRemote();
    /*!
     * Changed current remote to rectangle mode.
     */
    void musicSimpleStyleRemote();
    /*!
     * Changed current remote to simple style mode.
     */
    void musicComplexStyleRemote();
    /*!
     * Changed current remote to complex style mode.
     */
    void musicWallpaperRemote(bool create);
    /*!
     * Changed current remote to wallpaper mode.
     */
    void musicRipplesRemote();
    /*!
     * Changed current remote to ripples mode.
     */
    void musicDeleteRemote();
    /*!
     * Delete current remote.
     */
    void musicRemoteTypeChanged(QAction *type);
    /*!
     * Switch to diff remote type.
     */
    void musicRemoteTypeChanged(int type);
    /*!
     * Switch to diff remote type.
     */

protected:
    void createRemoteWidget();
    /*!
     * Create remote widget.
     */
    void drawWindowBackgroundRect();
    /*!
     * Draw window background rect.
     */
    void drawWindowBackgroundRectString();
    /*!
     * Draw window background rect by qimage.
     */
    void drawWindowBackgroundRectString(const QString &path);
    /*!
     * Draw window background rect by picture.
     */

    Ui::MusicApplication *m_ui;
    MusicUserWindow *m_musicUserWindow;
    MusicBackgroundSkinDialog *m_musicBackgroundWidget;
    MusicRemoteWidget *m_musicRemoteWidget;
    MusicDownloadCounterPVThread *m_counterPVThread;

    QImage m_backgroundImage;
    int m_lastRemoteBeforeWallpaper;
    QString m_backgroundImagePath;
    int m_backgroundAlpha, m_backgroundAListlpha;
    QTimer m_pictureCarouselTimer;

    static MusicTopAreaWidget *m_instance;
};

#endif // MUSICTOPAREAWIDGET_H
