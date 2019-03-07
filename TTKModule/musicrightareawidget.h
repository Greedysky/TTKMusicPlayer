#ifndef MUSICRIGHTAREAWIDGET_H
#define MUSICRIGHTAREAWIDGET_H

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

#include <QWidget>
#include "musicglobaldefine.h"

class MusicSettingWidget;
class MusicVideoPlayWidget;
class MusicDownloadStatusObject;
class MusicLrcContainerForDesktop;
class MusicLrcContainerForWallpaper;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app right area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicRightAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRightAreaWidget)
public:
    enum MusicFunction
    {
        KugGouSongWidget,       /*!< insert kugou song widget*/
        KugGouRadioWidget,      /*!< insert kugou radio widget*/
        kugouListWidget,        /*!< insert kugou list widget*/
        VideoWidget,            /*!< insert video widget*/
        kugouLiveWidget,        /*!< insert kugou live widget*/
        LrcWidget,              /*!< insert lrc display widget*/
        SearchWidget,           /*!< insert search display widget*/
        SearchSingleWidget,     /*!< insert search single display widget*/
        SimilarWidget,          /*!< insert similar found widget*/
        AlbumWidget,            /*!< insert album found widget*/
        ArtistWidget,           /*!< insert artist found widget*/
        ArtistCategoryWidget,   /*!< insert artist category found widget*/
        ToplistWidget,          /*!< insert toplist found widget*/
        PlaylistWidget,         /*!< insert playlist found widget*/
        AdvancedSearchWidget,   /*!< insert advanced search widget*/
        RecommendWidget,        /*!< insert recommend found widget*/
        IndentifyWidget,        /*!< insert indentify songs widget*/
        KuiSheWidget,           /*!< insert kugou kuishe widget*/
        WebDJRadioWidget,       /*!< insert web dj radio widget*/
        WebMVRadioWidget,       /*!< insert web mv radio widget*/
        CloudManagerWidget      /*!< insert cloud manager widget*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicRightAreaWidget(QWidget *parent = nullptr);

    ~MusicRightAreaWidget();

    /*!
     * Get class object instance.
     */
    static MusicRightAreaWidget *instance();
    /*!
     * Set up app ui.
     */
    void setupUi(Ui::MusicApplication* ui);
    /*!
     * Stop timer clock to draw lrc.
     */
    void stopLrcMask() const;
    /*!
     * Start timer clock to draw lrc.
     */
    void startTimerClock() const;
    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Get destop lrc visible state.
     */
    bool getDestopLrcVisible() const;
    /*!
     * Set inline lrc visible by string.
     */
    void setInlineLrcVisible(bool status) const;
    /*!
     * Get inline lrc visible state.
     */
    bool getInlineLrcVisible() const;
    /*!
     * Set setting parameter.
     */
    void setSettingParameter() const;
    /*!
     * Get setting parameter.
     */
    void getParameterSetting() const;
    /*!
     * Check the setting has open inline or desktop lrc on or not.
     */
    bool checkSettingParameterValue() const;
    /*!
     * Update current lrc by current time\total time and play state.
     */
    void updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const;
    /*!
     * Load current song lrc by name and path.
     */
    void loadCurrentSongLrc(const QString &name, const QString &path) const;
    /*!
     * Set song speed and slow by given time.
     */
    void setSongSpeedAndSlow(qint64 time) const;
    /*!
     * Check the current song already has lrc or not,
     * if not just download it.
     */
    void musicCheckHasLrcAlready() const;
    /*!
     * Show setting widget.
     */
    void showSettingWidget() const;
    /*!
     * Music artist search function.
     */
    void musicArtistSearch(const QString &id);
    /*!
     * Music album search function.
     */
    void musicAlbumSearch(const QString &id);
    /*!
     * Music movie search function.
     */
    void musicMovieSearch(const QString &id);
    /*!
     * Music movie radio search function.
     */
    void musicMovieRadioSearch(const QVariant &data);
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

Q_SIGNALS:
    /*!
     * Current background skin download.
     */
    void updateBackgroundThemeDownload();
    /*!
     * Current background transparent changed.
     */
    void updateBackgroundTheme();

public Q_SLOTS:
    /*!
     * Music function button clicked.
     */
    void musicFunctionClicked(int index);
    /*!
     * Music function button clicked by extra widget.
     */
    void musicFunctionClicked(int index, QWidget *widget);
    /*!
     * Music song comments widget.
     */
    void musicSongCommentsWidget();
    /*!
     * Music similar function that by string.
     */
    void musicSimilarFound(const QString &text);
    /*!
     * Music album function that by string.
     */
    void musicAlbumFound(const QString &text, const QString &id);
    /*!
     * Music artist category function.
     */
    void musicArtistCategoryFound();
    /*!
     * Music artist search function.
     */
    void musicArtistSearchFound();
    /*!
     * Music album search function.
     */
    void musicAlbumSearchFound();
    /*!
     * Music movie search function.
     */
    void musicMovieSearchFound();
    /*!
     * Music movie search radio function.
     */
    void musicMovieSearchRadioFound();
    /*!
     * Music artist function that by string.
     */
    void musicArtistFound(const QString &text, const QString &id);
    /*!
     * Music toplist function.
     */
    void musicToplistFound();
    /*!
     * Music playlist function.
     */
    void musicPlaylistFound(const QString &id);
    /*!
     * Music recommend function.
     */
    void musicRecommendFound();
    /*!
     * Music advanced search function.
     */
    void musicAdvancedSearch();
    /*!
     * Music song research button searched by name.
     */
    void musicSongSearchedFound(const QString &text);
    /*!
     * Music song search by given id.
     */
    void musicSingleSearchedFound(const QString &id);
    /*!
     * Music load song index widget.
     */
    void musicLoadSongIndexWidget();
    /*!
     * Delete current stacked widget.
     */
    void deleteStackedFuncWidget();
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
     * Research query by quality it changed.
     */
    void researchQueryByQuality(const QString &quality);
    /*!
     * Video button clicked by name to search.
     */
    void musicVideoButtonSearched(const QString &name, const QString &id);
    /*!
     * Set video widget popop or not.
     */
    void musicVideoSetPopup(bool popup);
    /*!
     * Set video widget actived window.
     */
    void musicVideoActiveWindow();
    /*!
     * Set video widget close and delete.
     */
    void musicVideoClosed();
    /*!
     * Set video widget fullScreen or not.
     */
    void musicVideoFullscreen(bool full);
    /*!
     * Lrc display all button clicked.
     */
    void musicLrcDisplayAllButtonClicked();
    /*!
     * Lrc desktop wallpaper button clicked.
     */
    void musicContainerForWallpaperClicked();
    /*!
     * Change to download fully widget.
     */
    void musicChangeDownloadFulllyWidget();
    /*!
     * Change to download custum widget.
     */
    void musicChangeDownloadCustumWidget();

protected:
    /*!
     * Music function parameter init.
     */
    void musicFunctionParameterInit(MusicFunction func);

    QVariant m_rawData;
    QWidget *m_stackedFuncWidget, *m_stackedAutoWidget;
    Ui::MusicApplication *m_ui;
    MusicSettingWidget *m_settingWidget;
    MusicVideoPlayWidget *m_videoPlayerWidget;
    MusicLrcContainerForDesktop *m_musicLrcForDesktop;
    MusicLrcContainerForWallpaper *m_musicLrcForWallpaper;
    MusicDownloadStatusObject *m_downloadStatusObject;

    static MusicRightAreaWidget *m_instance;
};

#endif // MUSICRIGHTAREAWIDGET_H
