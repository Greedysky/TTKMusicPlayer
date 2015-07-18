#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QTimer>
#include <QSettings>
#include <QSystemTrayIcon>
#include "musicsongsearchonlinewidget.h"
#include "core/musicsonginformation.h"
#include "core/musicmovewidgetabstract.h"
#include "core/musicobject.h"
#include "core/musicversion.h"

class MusicPlayer;
class MusicPlaylist;
class MusicSettingWidget;
class MusicBackgroundSkinDialog;
class MusicLocalSongSearch;
class MusicSongsListWidget;
class MusicSongsSummarizied;
class MusicLrcContainerForDesktop;
class MusicDownloadStatusLabel;
class MusicSystemTrayMenu;
class MusicUserWindow;
class MusicRemoteWidget;
class MusicToolSetsWidget;
class MusicTimerAutoObject;
class MusicMobileDevicesWidget;
class MusicWindowExtras;
class MusicSpectrumWidget;

class MusicVideoPlayer;

class QLabel;
class QPropertyAnimation;

namespace Ui {
class MusicApplication;
}

class MUSIC_EXPORT MusicApplication : public MusicMoveWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicApplication(QWidget *parent = 0);
    virtual ~MusicApplication();

    QString getCurrentFileName() const;
    bool checkSettingParameterValue() const;
    bool checkMusicListCurrentIndex() const;
    QLabel*& getshowDownloadLabel();
    void musicLoadCurrentSongLrc();

protected:
    void initWindowSurface();
    void setMenuBarButton();
    void setButtonCursor();
    void setButtonTips();
    void createSystemTrayIcon();
    void createToolPopupMenu();
    void createMenuActions();
    void createRemoteWidget();
    void createPlayModeMenu(QMenu&);
    void createPlayModeMenuIcon(QMenu&);
    void createVedioWidget(bool);
    void drawWindowRoundedRect();
    void drawWindowBackgroundRect();
    void drawWindowBackgroundRectString(const QString&);
    virtual void closeEvent(QCloseEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual bool nativeEvent(const QByteArray &, void *, long *);

    void readXMLConfigFromText();
    void writeXMLConfigToText();
    void UpdateCurrentLrc(qint64 time);
    void musicImportSongsSettingPath(const QStringList&);
    QString musicTimeTransToLabel(qint64 time);

public slots:
    void quitWindowClose();
    void positionChanged(qint64);
    void durationChanged(qint64);
    void stateChanged();
    void showCurrentSong(int);
    void musicKey();
    void musicPlayPrivious();
    void musicPlayNext();
    void musicPlayOrder();
    void musicPlayRandom();
    void musicPlayListLoop();
    void musicPlayOneLoop();
    void musicPlayItemOnce();
    void musicVolumeNULL();
    void musicVolumeChanged(int);
    void musicImportSongs();
    void musicImportSongsOnlyFile();
    void musicImportSongsOnlyDir();
    void musicPlayIndex(int,int);
    void musicPlayAnyTimeAt(int);
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void musicActionVolumeSub();
    void musicActionVolumePlus();
    void musicAboutUs();
    void musicSetting();
    void musicSearch();
    void musicCurrentPlayLocation();
    void musicShowSkinChangedWindow();
    void musicSearchButtonSearched();
    void musicIndexWidgetButtonSearched();
    void musicSearchWidgetButtonSearched();
    void musicLrcWidgetButtonSearched();
    void musicSearchRefreshButtonRefreshed();
    void musicVedioWidgetButtonSearched();
    void musicVedioWidgetButtonDoubleClicked();
    void musicAddSongToLovestListAt();
    void musicSetWindowToTop();
    void musicSetEqualizer();
    void musicAudioRecorder();
    void musicCircleRemote();
    void musicDiamondRemote();
    void musicSquareRemote();
    void musicRectangleRemote();
    void musicDeleteRemote();
    void musicStackedSongListWidgetChanged();
    void musicStackedToolsWidgetChanged();
    void musicStackedRadioWidgetChanged();
    void musicStackedMyDownWidgetChanged();
    void musicTimerWidget();
    void musicSpectrumWidget();
    void musicSetPlay3DMusic();
    void musicWindowConciseChanged();
    void musicBackgroundChanged();
    /////////////////////////////////////////////
    //This is a slot by MusicBackgroundSkin's signal emit
    void musicBgTransparentChanged(int);
    void musicBackgroundSkinChanged(const QString&);
    /////////////////////////////////////////////
    //This is a slot by MusicLocalSongSearch's signal emit
    void musicSearchIndexChanged(int,int);
    /////////////////////////////////////////////
    //This is a slot by MusicSettingWidget's signal emit
    void getParameterSetting();
    /////////////////////////////////////////////
    //This is a slot by MusicSongsSummarizied's signal emit
    void setDeleteItemAt(const MIntList &index);
    /////////////////////////////////////////////
    //This is a slot by MusicInlineLrcContainer's signal emit
    void musicCurrentLrcUpdated();
    void updateCurrentTime(qint64);
    /////////////////////////////////////////////
    //This is a slot by systemTrayMenu's signal emit
    void setShowDesktopLrc(bool);
    /////////////////////////////////////////////
    //This is a slot by MusicLrcDesktopContainer's signal emit
    void desktopLrcClosed();
    /////////////////////////////////////////////
    //This is a slot by MusicRemoteWidget's signal emit
    void musicVolumeChangedFromRemote(int);
    /////////////////////////////////////////////
    //This is a slot by MusicTimerAutoObject's signal emit
    void setPlaySongChanged(int);
    void setStopSongChanged();
    /////////////////////////////////////////////
    //This is a slot by MusicToolSetsWidget's signal emit
    void musicToolSetsParameter();
    /////////////////////////////////////////////
    //This is a slot by MusicLocalSongsManagerWidget's signal emit
    //This is a slot by MusicMyDownloadRecordWidget's signal emit
    void addSongToPlayList(const QStringList &);
    /////////////////////////////////////////////
    void musicBgThemeDownloadFinished();


private:
    Ui::MusicApplication *ui;
    bool m_playControl;
    bool m_systemCloseConfig;
    bool m_setWindowToTop;
    MusicPlayer* m_musicPlayer;
    MusicPlaylist* m_musicList;
    QSystemTrayIcon* m_systemTray;
    QMenu m_toolPopupMenu;
    QMenu m_playModeMenu;
    MusicSystemTrayMenu *m_systemTrayMenu;

    QSettings m_musicSettingParameter;
    MIntsListMap m_searchfileListCache;

    MusicSongsSummarizied* m_musicSongTree;
    MusicSettingWidget* m_setting;
    MusicBackgroundSkinDialog* m_musicbgskin;
    MusicLocalSongSearch* m_musicLocalSongSearch;
    MusicLrcContainerForDesktop* m_musiclrcfordesktop;
    MusicDownloadStatusLabel* m_downloadStatusLabel;
    MusicUserWindow* m_msuicUserWindow;
    MusicRemoteWidget* m_musicRemoteWidget;
    MusicTimerAutoObject* m_musicTimerAutoObj;
    MusicMobileDevicesWidget* m_mobileDevices;
    MusicWindowExtras* m_musicWindowExtras;
    MusicSpectrumWidget* m_musicSpectrumWidget;
    //////////////////////////////////
    MusicVideoPlayer* m_videoPlayer;
    //////////////////////////////////
    QPropertyAnimation* m_animation;
    QWidget* m_stackedWidget;
    QString m_currentBgSkin;
    int m_alpha;
    int m_listWidgetAlpha;
    int m_currentMusicSongTreeIndex;
    QTimer m_pictureCarouselTimer;
    int m_pictureCarouselIndex;

};

#endif // MAINWINDOW_H
