#ifndef MUSICTOPAREAWIDGET_H
#define MUSICTOPAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QTimer>
#include <QWidget>
#include "musicglobaldefine.h"

class MusicUserWindow;
class MusicRemoteWidget;
class MusicBackgroundSkinDialog;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app top area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicTopAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTopAreaWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTopAreaWidget(QWidget *parent = nullptr);

    ~MusicTopAreaWidget();

    /*!
     * Get class object instance.
     */
    static MusicTopAreaWidget *instance();
    /*!
     * Set up app ui.
     */
    void setupUi(Ui::MusicApplication* ui);

    /*!
     * Set background skin and alpha and list alpha parameter.
     */
    void setBackgroundParams(const QString &skin, int alpha, int list);
    /*!
     * Get background skin name.
     */
    inline QString getBackgroundPath() const { return m_backgroundImagePath; }
    /*!
     * Get background skin alpha.
     */
    inline int getBackgroundAlpha() const { return m_backgroundAlpha; }
    /*!
     * Get list background skin alpha.
     */
    int getBackgroundListAlpha();
    /*!
     * Get background window skin enable.
     */
    bool getBackgroundTransparentEnable() const;
    /*!
     * Get background skin pixmap.
     */
    QPixmap getRendererPixmap() const;

    /*!
     * Check if the user is login now.
     */
    bool getUserLoginState() const;

    /*!
     * Stop background skin change tiemr.
     */
    void setTimerStop();
    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status);
    /*!
     * Set current song text.
     */
    void setLabelText(const QString &name) const;
    /*!
     * Set current volume value by index.
     */
    void setVolumeValue(int value) const;

Q_SIGNALS:
    /*!
     * Set list background skin transparent emit.
     */
    void setTransparent(int index);
    /*!
     * Search current music song from net.
     */
    void musicSearchButtonClicked();

public Q_SLOTS:
    /*!
     * Show change background skin dailog.
     */
    void musicShowSkinChangedWindow();
    /*!
     * Send user to login.
     */
    void musicUserContextLogin();
    /*!
     * Set background as artist big picture.
     */
    void musicSetAsArtistBackground();
    /*!
     * Current background transparent changed.
     */
    void musicBackgroundTransparentChanged();
    /*!
     * Current background transparent changed by index.
     */
    void musicBackgroundTransparentChanged(int index);
    /*!
     * Current background transparent changed by string.
     */
    void musicBackgroundTransparentChanged(const QString &fileName);
    /*!
     * Current background skin changed by new path.
     */
    void musicBackgroundSkinChanged(const QString &fileName);
    /*!
     * Current background skin custum changed by new path.
     */
    void musicBackgroundSkinCustumChanged(const QString &fileName);
    /*!
     * Changed current background skin.
     */
    void musicBackgroundChanged();
    /*!
     * Current slider state changed.
     */
    void musicBackgroundSliderStateChanged(bool state);
    /*!
     * Current background skin download is finished.
     */
    void musicBackgroundThemeDownloadFinished();
    /*!
     * Resize current background skin when geometry changed.
     */
    void musicBackgroundThemeChangedByResize();
    /*!
     * Current list background transparent changed.
     */
    void musicPlaylistTransparent(int index);
    /*!
     * Changed current remote to circle mode.
     */
    void musicCircleRemote();
    /*!
     * Changed current remote to diamond mode.
     */
    void musicDiamondRemote();
    /*!
     * Changed current remote to square mode.
     */
    void musicSquareRemote();
    /*!
     * Changed current remote to rectangle mode.
     */
    void musicRectangleRemote();
    /*!
     * Changed current remote to simple style mode.
     */
    void musicSimpleStyleRemote();
    /*!
     * Changed current remote to complex style mode.
     */
    void musicComplexStyleRemote();
    /*!
     * Changed current remote to wallpaper mode.
     */
    void musicWallpaperRemote(bool create);
    /*!
     * Changed current remote to ripples mode.
     */
    void musicRipplesRemote();
    /*!
     * Changed current remote to rayswave mode.
     */
    void musicRaysWaveRemote();
    /*!
     * Delete current remote.
     */
    void musicDeleteRemote();
    /*!
     * Switch to diff remote type.
     */
    void musicRemoteTypeChanged(QAction *type);
    /*!
     * Switch to diff remote type.
     */
    void musicRemoteTypeChanged(int type);
    /*!
     * Change to tool sets widget.
     */
    void musicStackedToolsWidgetChanged();

protected:
    /*!
     * Create remote widget.
     */
    void createRemoteWidget();
    /*!
     * Draw window background rect.
     */
    void drawWindowBackgroundRect();
    /*!
     * Draw window background rect.
     */
    void drawWindowBackgroundRect(const QImage &image);
    /*!
     * Draw window background rect by qimage.
     */
    void drawWindowBackgroundRectString();
    /*!
     * Draw window background rect by picture.
     */
    void drawWindowBackgroundRectString(const QString &path);

    Ui::MusicApplication *m_ui;
    MusicUserWindow *m_musicUserWindow;
    MusicBackgroundSkinDialog *m_musicBackgroundWidget;
    MusicRemoteWidget *m_musicRemoteWidget;

    QImage m_backgroundImage;
    int m_lastRemoteBeforeWallpaper;
    QString m_backgroundImagePath;
    int m_backgroundAlpha, m_backgroundAListlpha;
    QTimer m_pictureCarouselTimer;

    static MusicTopAreaWidget *m_instance;
};

#endif // MUSICTOPAREAWIDGET_H
