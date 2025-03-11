#ifndef MUSICTOPAREAWIDGET_H
#define MUSICTOPAREAWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include <QTimer>
#include <QWidget>
#include "musicglobaldefine.h"

class MusicRemoteWidget;
class MusicBackgroundSkinDialog;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app top area widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTopAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTopAreaWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicTopAreaWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTopAreaWidget();

    /*!
     * Get class object instance.
     */
    static MusicTopAreaWidget *instance();

    /*!
     * Set up app ui.
     */
    void setupUi(Ui::MusicApplication *ui);

    /*!
     * Set background skin and alpha and list alpha parameter.
     */
    void setBackgroundParameter();
    /*!
     * Get background skin name.
     */
    inline QString backgroundPath() const noexcept { return m_backgroundImagePath; }
    /*!
     * Get background skin transparent.
     */
    inline int backgroundTransparent() const noexcept { return m_backgroundTransparent; }
    /*!
     * Get list background skin transparent.
     */
    int backgroundListTransparent();
    /*!
     * Get background window skin enable.
     */
    bool backgroundTransparentEnabled() const;

    /*!
     * Get origin background image.
     */
    const QImage& originImage() const;
    /*!
     * Get render background pixmap.
     */
    const QPixmap& renderPixmap() const;

    /*!
     * Stop background skin change timer.
     */
    void setBackgroundAnimation(bool state);
    /*!
     * Set current play state.
     */
    void setCurrentPlayState(bool state) const;
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
     * Origin background image data changed.
     */
    void originImageChanged(const QImage &image);

public Q_SLOTS:
    /*!
     * Show change background skin dialog.
     */
    void showSkinManagerWidget();
    /*!
     * Set background as artist background picture.
     */
    void setArtistBackground();
    /*!
     * Current background transparent changed.
     */
    void backgroundTransparentChanged();
    /*!
     * Current background transparent changed by value.
     */
    void backgroundTransparentChanged(int value);
    /*!
     * Current background transparent changed by string.
     */
    void backgroundTransparentChanged(const QString &fileName);
    /*!
     * Current background skin changed by new path.
     */
    void backgroundSkinChanged(const QString &fileName);
    /*!
     * Changed current background skin.
     */
    void backgroundChanged();
    /*!
     * Current slider state changed.
     */
    void backgroundAnimationChanged(bool state);
    /*!
     * Current background skin download is finished.
     */
    void backgroundThemeDownloadFinished();
    /*!
     * Resize current background skin when geometry changed.
     */
    void backgroundThemeChangedByResize();
    /*!
     * Current list background transparent changed.
     */
    void playlistTransparentChanged(int index);
    /*!
     * Changed current remote to square mode.
     */
    void showSquareRemote();
    /*!
     * Changed current remote to rectangle mode.
     */
    void showRectangleRemote();
    /*!
     * Changed current remote to simple style mode.
     */
    void showSimpleStyleRemote();
    /*!
     * Changed current remote to complex style mode.
     */
    void showComplexStyleRemote();
    /*!
     * Changed current remote to ripple mode.
     */
    void showRippleRemote();
    /*!
     * Changed current remote to wallpaper mode.
     */
    void showWallpaperRemote(bool create);
    /*!
     * Delete current remote.
     */
    void deleteCurrentRemote();
    /*!
     * Switch to diff remote type.
     */
    void remoteTypeChanged(QAction *type);
    /*!
     * Switch to diff remote type.
     */
    void remoteTypeChanged(int type);
    /*!
     * Change to tool sets widget.
     */
    void showToolSetsWidget();

private:
    /*!
     * Set list background skin transparent.
     */
    void backgroundTransparent(int value);
    /*!
     * Check artist background mode.
     */
    bool isArtistBackground() const;
    /*!
     * Create remote widget.
     */
    void createRemoteWidget();
    /*!
     * Draw window background.
     */
    void drawWindowBackground();
    /*!
     * Draw window background.
     */
    void drawWindowBackground(const QImage &image);
    /*!
     * Draw window background by qimage.
     */
    void drawWindowBackgroundByImage();
    /*!
     * Draw window background by path.
     */
    void drawWindowBackgroundByPath(const QString &path);

    Ui::MusicApplication *m_ui;
    MusicBackgroundSkinDialog *m_backgroundWidget;
    MusicRemoteWidget *m_remoteWidget;
    QImage m_backgroundImage;
    int m_lastRemoteBeforeWallpaper;
    QString m_backgroundImagePath;
    int m_backgroundTransparent, m_backgroundListTransparent;
    QTimer m_pictureCarouselTimer;

    static MusicTopAreaWidget *m_instance;

};

#endif // MUSICTOPAREAWIDGET_H
