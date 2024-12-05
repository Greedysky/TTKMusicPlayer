#ifndef MUSICRIGHTAREAWIDGET_H
#define MUSICRIGHTAREAWIDGET_H

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

#include <QWidget>
#include "musicglobaldefine.h"

class MusicSettingWidget;
class MusicVideoPlayWidget;
class MusicDownloadStatusModule;

class MusicLrcAnalysis;
class MusicLrcContainerForInterior;
class MusicLrcContainerForDesktop;
class MusicLrcContainerForWallpaper;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app right area widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRightAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRightAreaWidget)
public:
    enum FunctionModule
    {
        // kugou main submoudle
        KugGouSongWidget,       /*!< insert kugou song widget*/
        KugGouRadioWidget,      /*!< insert kugou radio widget*/
        kugouPlaylistWidget,    /*!< insert kugou playlist widget*/
        kugouMovieWidget,       /*!< insert kugou movie widget*/
        VideoWidget,            /*!< insert video widget*/
        LrcWidget,              /*!< insert lrc display widget*/
        // kugou song submoudle
        RecommendWidget,        /*!< insert recommend found widget*/
        ToplistWidget,          /*!< insert toplist found widget*/
        ArtistCategoryWidget,   /*!< insert artist category found widget*/
        PlaylistCategoryWidget, /*!< insert playlist category found widget*/
        // other moudle
        SearchWidget,           /*!< insert search display widget*/
        SearchSingleWidget,     /*!< insert search single display widget*/
        SimilarWidget,          /*!< insert similar found widget*/
        AlbumWidget,            /*!< insert album found widget*/
        ArtistWidget,           /*!< insert artist found widget*/
        PlaylistWidget,         /*!< insert playlist found widget*/
        SongDailyWidget,        /*!< insert song daily widget*/
        AdvancedSearchWidget,   /*!< insert advanced search widget*/
        IndentifyWidget,        /*!< insert indentify songs widget*/
        KuiSheWidget,           /*!< insert kugou kuishe widget*/
        WebDJRadioWidget,       /*!< insert web dj radio widget*/
        WebMVRadioWidget,       /*!< insert web mv radio widget*/
        WebPVRadioWidget,       /*!< insert web pv radio widget*/
        LocalManagerWidget,     /*!< insert local manager widget*/
        CloudManagerWidget,     /*!< insert cloud manager widget*/
        ScreenSaverWidget,      /*!< insert screen saver widget*/
        PlaylistBackupWidget,   /*!< insert playlist backup widget*/
    };

    /*!
     * Object constructor.
     */
    explicit MusicRightAreaWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicRightAreaWidget();

    /*!
     * Get class object instance.
     */
    static MusicRightAreaWidget *instance();

    /*!
     * Set up app ui.
     */
    void setupUi(Ui::MusicApplication *ui);

    /*!
     * Start timer clock to draw lrc.
     */
    void startDrawLrc() const;
    /*!
     * Stop timer clock to draw lrc.
     */
    void stopDrawLrc() const;
    /*!
     * Set current play state.
     */
    void setCurrentPlayState(bool state) const;
    /*!
     * Get destop lrc visible state.
     */
    bool destopLrcVisible() const;
    /*!
     * Set interior lrc visible by string.
     */
    void setInteriorLrcVisible(bool status) const;
    /*!
     * Get interior lrc visible state.
     */
    bool interiorLrcVisible() const;
    /*!
     * Update current lrc by current time and total time and play state.
     */
    void updateCurrentLrc(qint64 current, qint64 total, bool playState) const;
    /*!
     * Load current song lrc by name and path.
     */
    void loadCurrentSongLrc(const QString &name, const QString &path) const;
    /*!
     * Set song time speed by given time.
     */
    void setSongTimeSpeed(qint64 time) const;
    /*!
     * Check the current song meta data valid.
     */
    void checkMetaDataValid(bool mode) const;
    /*!
     * Show setting widget.
     */
    void showSettingWidget() const;
    /*!
     * Song artist search function.
     */
    void artistSearchByID(const QString &id);
    /*!
     * Song album search function.
     */
    void albumSearchByID(const QString &id);
    /*!
     * Song movie search function.
     */
    void movieSearchByID(const QString &id);
    /*!
     * Song movie radio search function.
     */
    void movieRadioSearchByID(const QVariant &data);

    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();
    /*!
     * Apply settings parameters.
     */
    void applyParameter();

Q_SIGNALS:
    /*!
     * Current background transparent changed.
     */
    void updateBackgroundTheme();
    /*!
     * Current background skin download.
     */
    void updateBackgroundThemeDownload();

public Q_SLOTS:
    /*!
     * Function go back clicked.
     */
    void functionGoBack();
    /*!
     * Function button clicked.
     */
    void functionClicked(int index, QWidget *widget = nullptr);

public Q_SLOTS:
    /*!
     * Song comments function.
     */
    void showCommentsFound();
    /*!
     * Song similar function that by string.
     */
    void showSimilarFound(const QString &text);
    /*!
     * Song album function that by string.
     */
    void showAlbumFound(const QString &text, const QString &id);
    /*!
     * Song artist search function.
     */
    void showArtistSearchFound();
    /*!
     * Song album search function.
     */
    void showAlbumSearchFound();
    /*!
     * Song movie search function.
     */
    void showMovieSearchFound();
    /*!
     * Song movie search radio function.
     */
    void showMovieSearchRadioFound();
    /*!
     * Song personal search radio function.
     */
    void showPersonalRadioFound(const QString &id);
    /*!
     * Song artist function that by string.
     */
    void showArtistFound(const QString &text, const QString &id);
    /*!
     * Song playlist function.
     */
    void showPlaylistFound(const QString &id);
    /*!
     * Song playlist category function.
     */
    void showPlaylistCategoryFound(const QString &id, const QString &value);
    /*!
     * Song research button searched by name.
     */
    void showSongSearchedFound(const QString &text);
    /*!
     * Song search by given id.
     */
    void showSingleSearchedFound(const QString &id);
    /*!
     * Video button clicked by name to search.
     */
    void showVideoSearchedFound(const QString &name, const QString &id);

public Q_SLOTS:
    /*!
     * Song load song index widget.
     */
    void showSongMainWidget();
    /*!
     * Set destop lrc visible or invisible.
     */
    void setDestopLrcVisible(bool visible) const;
    /*!
     * Lock current desktop lrc state changed.
     */
    void setWindowLockedChanged();
    /*!
     * Set current desktop lrc window type changed.
     */
    void setWindowLrcTypeChanged();
    /*!
     * Set video widget popop or not.
     */
    void videoSetPopup(bool popup);
    /*!
     * Set video widget actived window.
     */
    void videoActiveWindow();
    /*!
     * Set video widget close and delete.
     */
    void videoNeedToClose();
    /*!
     * Set video widget fullScreen or not.
     */
    void videoFullscreen(bool full);
    /*!
     * Lrc display all button clicked.
     */
    void lrcDisplayAllClicked();
    /*!
     * Lrc desktop wallpaper button clicked.
     */
    void containerForWallpaperClicked();
    /*!
     * Change to download fully widget.
     */
    void changeDownloadFulllyWidget();
    /*!
     * Change to download custum widget.
     */
    void changeDownloadCustumWidget();

private:
    /*!
     * Function initialize.
     */
    void functionInitialize();
    /*!
     * Create kugou web window.
     */
    void createkWebWindow(int type);

    Ui::MusicApplication *m_ui;
    QVariant m_rawData;
    bool m_lowPowerMode;
    FunctionModule m_funcIndex;
    QWidget *m_stackedWidget;
    QWidget *m_stackedStandWidget;
    MusicSettingWidget *m_settingWidget;
    MusicVideoPlayWidget *m_videoPlayerWidget;
    MusicLrcAnalysis *m_lrcAnalysis;
    MusicLrcContainerForInterior *m_lrcForInterior;
    MusicLrcContainerForDesktop *m_lrcForDesktop;
    MusicLrcContainerForWallpaper *m_lrcForWallpaper;
    MusicDownloadStatusModule *m_downloadStatusObject;

    static MusicRightAreaWidget *m_instance;

};

#endif // MUSICRIGHTAREAWIDGET_H
