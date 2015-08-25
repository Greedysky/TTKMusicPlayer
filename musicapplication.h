#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QTimer>
#include "musicmovewidgetabstract.h"
#include "musicobject.h"

class QPropertyAnimation;

class MusicPlayer;
class MusicPlaylist;
class MusicLocalSongSearch;
class MusicSongsListWidget;
class MusicSongsSummarizied;
class MusicTimerAutoObject;
class MusicMobileDevicesWidget;
class MusicWindowExtras;

class MusicBottomAreaWidget;
class MusicTopAreaWidget;
class MusicRightAreaWidget;
class MusicLeftAreaWidget;

namespace Ui {
class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicApplication : public MusicMoveWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicApplication(QWidget *parent = 0);
    virtual ~MusicApplication();

    QString getCurrentFileName() const;
    bool checkMusicListCurrentIndex() const;
    void musicLoadCurrentSongLrc();

protected:
    void initWindowSurface();
    void createPlayModeMenu(QMenu&);
    void createPlayModeMenuIcon(QMenu&);
    void drawWindowRoundedRect();
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
    void musicActionVolumeSub();
    void musicActionVolumePlus();
    void musicAboutUs();
    void musicSetting();
    void musicSearch();
    void musicCurrentPlayLocation();
    void musicAddSongToLovestListAt();
    void musicSetWindowToTop();
    void musicSetEqualizer();
    void musicAudioRecorder();
    void musicTimerWidget();
    void musicSetPlay3DMusic();
    void musicWindowConciseChanged();
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
    //This is a slot by MusicTimerAutoObject's signal emit
    void setPlaySongChanged(int);
    void setStopSongChanged();
    /////////////////////////////////////////////
    //This is a slot by MusicToolSetsWidget's signal emit
    void musicToolSetsParameter();
    /////////////////////////////////////////////
    //This is a slot by MusicLeftAreaWidget's signal emit
    void addSongToPlayList(const QStringList &);
    void setSpectrum(HWND,int,int);
    void getCurrentPlayList(QStringList&);
    /////////////////////////////////////////////
    //This is a slot by MusicRightAreaWidget's signal emit
//    void changeDesktopLrcWidget();
//    void changeInlineLrcWidget();

private:
    Ui::MusicApplication *ui;
    bool m_playControl;
    bool m_setWindowToTop;
    MusicPlayer* m_musicPlayer;
    MusicPlaylist* m_musicList;
    MIntsListMap m_searchfileListCache;

    QMenu m_playModeMenu;
    MusicSongsSummarizied* m_musicSongTree;
    MusicLocalSongSearch* m_musicLocalSongSearch;

    MusicTimerAutoObject* m_musicTimerAutoObj;
    MusicMobileDevicesWidget* m_mobileDevices;
    MusicWindowExtras* m_musicWindowExtras;
    //////////////////////////////////
    QPropertyAnimation* m_animation;
    int m_currentMusicSongTreeIndex;

    MusicBottomAreaWidget* m_bottomAreaWidget;
    MusicTopAreaWidget* m_topAreaWidget;
    MusicRightAreaWidget* m_rightAreaWidget;
    MusicLeftAreaWidget* m_leftAreaWidget;

};

#endif // MAINWINDOW_H
