#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicuiobject.h"
#include "musiclrccontainerfordesktop.h"
#include "musiclrccontainerforwallpaper.h"
#include "musicvideoplaywidget.h"
#include "musicdownloadstatusobject.h"
#include "musicsettingwidget.h"
#include "musicmessagebox.h"
#include "musicalbumfoundwidget.h"
#include "musicartistfoundwidget.h"
#include "musicsimilarfoundwidget.h"
#include "musicplaylistfoundwidget.h"
#include "musicsongsearchonlinewidget.h"
#include "musicidentifysongswidget.h"
#include "musicfunctionuiobject.h"
#include "musictinyuiobject.h"
#include "musicfunctionlistuiobject.h"
#include "musicregeditmanager.h"
#include "musictopareawidget.h"
#include "kugouwindow.h"

#include <QPropertyAnimation>

MusicRightAreaWidget *MusicRightAreaWidget::m_instance = nullptr;

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_instance = this;
    m_stackedFuncWidget = nullptr;
    m_musicLrcForDesktop = nullptr;
    m_musicLrcForWallpaper = nullptr;
    m_videoPlayerWidget = nullptr;

    m_downloadStatusObject = new MusicDownloadStatusObject(parent);
    m_settingWidget = new MusicSettingWidget(this);
    connect(m_settingWidget, SIGNAL(parameterSettingChanged()), parent, SLOT(getParameterSetting()));
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_settingWidget;
    delete m_downloadStatusObject;
    delete m_musicLrcForDesktop;
    delete m_musicLrcForWallpaper;
    delete m_videoPlayerWidget;
}

QString MusicRightAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

MusicRightAreaWidget *MusicRightAreaWidget::instance()
{
    return m_instance;
}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->musiclrccontainerforinline->resize(ui->surfaceStackedWidget->size());

    ui->lrcDisplayAllButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcDisplayAllButton->setIconSize(QSize(15, 56));
    connect(ui->lrcDisplayAllButton, SIGNAL(clicked()), SLOT(musicLrcDisplayAllButtonClicked()));
    ///////////////////////////////////////////////////////

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(ui->musicSongWidgetButton, 0);
    group->addButton(ui->musicRadioWidgetButton, 1);
    group->addButton(ui->musicListWidgetButton, 2);
    group->addButton(ui->musicVideoWidgetButton, 3);
    group->addButton(ui->musicLiveWidgetButton, 4);
    group->addButton(ui->musicLrcWidgetButton, 5);
    group->addButton(ui->musicSearchButton, 6);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(musicFunctionClicked(int)));

    ///////////////////////////////////////////////////////
    connect(ui->musiclrccontainerforinline, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget,
                 SLOT(changeInlineLrcWidget()));
    connect(ui->musiclrccontainerforinline, SIGNAL(theCurrentLrcUpdated()), MusicApplication::instance(),
                 SLOT(musicCurrentLrcUpdated()));
    connect(ui->musiclrccontainerforinline, SIGNAL(theArtBgHasChanged()),
                 SIGNAL(updateBgThemeDownload()));
    connect(ui->musiclrccontainerforinline, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(),
                 SLOT(musicSetting()));
    connect(ui->musiclrccontainerforinline, SIGNAL(updateCurrentTime(qint64)), MusicApplication::instance(),
                 SLOT(updateCurrentTime(qint64)));
    connect(ui->musicSongSearchLine, SIGNAL(enterFinished(QString)),
                 SLOT(songResearchButtonSearched(QString)));
    ///////////////////////////////////////////////////////
    QTimer::singleShot(MT_MS, this, SLOT(musicLoadSongIndexWidget()));
}

void MusicRightAreaWidget::stopLrcMask() const
{
    if( checkSettingParameterValue() )
    {
       m_ui->musiclrccontainerforinline->stopLrcMask();
       m_musicLrcForDesktop->stopLrcMask();
       if(m_musicLrcForWallpaper)
       {
           m_musicLrcForWallpaper->stopLrcMask();
       }
    }
}

void MusicRightAreaWidget::startTimerClock() const
{
    if( checkSettingParameterValue() )
    {
       m_ui->musiclrccontainerforinline->startTimerClock();
       m_musicLrcForDesktop->startTimerClock();
       if(m_musicLrcForWallpaper)
       {
           m_musicLrcForWallpaper->startTimerClock();
       }
    }
}

void MusicRightAreaWidget::showPlayStatus(bool status) const
{
    m_musicLrcForDesktop->showPlayStatus(status);
}

bool MusicRightAreaWidget::getDestopLrcVisible() const
{
    return m_musicLrcForDesktop->isVisible();
}

void MusicRightAreaWidget::setInlineLrcVisible(bool status) const
{
    m_ui->musiclrccontainerforinline->setVisible(status);
}

bool MusicRightAreaWidget::getInlineLrcVisible() const
{
    return m_ui->musiclrccontainerforinline->isVisible();
}

void MusicRightAreaWidget::setSettingParameter() const
{
    m_musicLrcForDesktop->setSettingParameter();
    m_ui->musiclrccontainerforinline->setSettingParameter();
    if(m_musicLrcForWallpaper)
    {
        m_musicLrcForWallpaper->setSettingParameter();
    }
}

bool MusicRightAreaWidget::checkSettingParameterValue() const
{
    return ( M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool() ||
             M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool() );
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const
{
    m_ui->musiclrccontainerforinline->setCurrentTime(current, total);
    //Direct access to the audio file is the total time, in milliseconds
    QString currentLrc, laterLrc;
    qint64 intervalTime;
    if(m_ui->musiclrccontainerforinline->findText(total, currentLrc, laterLrc, intervalTime))
    {   //If this is a new line of the lyrics, then restart lyrics display mask
        if(currentLrc != m_ui->musiclrccontainerforinline->text())
        {
            if(!playStatus)
            {
                m_ui->musiclrccontainerforinline->updateCurrentLrc(intervalTime);
            }
            m_musicLrcForDesktop->setCurrentTime(intervalTime, total);
            m_musicLrcForDesktop->updateCurrentLrc(currentLrc, laterLrc, intervalTime);
            if(m_musicLrcForWallpaper)
            {
                m_musicLrcForWallpaper->updateCurrentLrc(intervalTime);
            }
        }
    }
}

void MusicRightAreaWidget::loadCurrentSongLrc(const QString &name, const QString &path) const
{
    if( checkSettingParameterValue() )
    {
        m_ui->musiclrccontainerforinline->stopLrcMask();
        m_ui->musiclrccontainerforinline->setCurrentSongName( name );
        m_musicLrcForDesktop->stopLrcMask();
        m_musicLrcForDesktop->setCurrentSongName( name );
        if(m_musicLrcForWallpaper)
        {
            m_musicLrcForWallpaper->stopLrcMask();
            m_musicLrcForWallpaper->setCurrentSongName( name );
        }

        if(!m_ui->musiclrccontainerforinline->transLyricFileToTime( path ))
        {
            m_musicLrcForDesktop->updateCurrentLrc(tr("unFoundLrc"), QString(), 0);
        }
    }
}

void MusicRightAreaWidget::setSongSpeedAndSlow(qint64 time) const
{
    m_ui->musiclrccontainerforinline->setSongSpeedAndSlow(time);
}

void MusicRightAreaWidget::musicCheckHasLrcAlready() const
{
    m_downloadStatusObject->musicCheckHasLrcAlready();
}

void MusicRightAreaWidget::showSettingWidget() const
{
    m_settingWidget->initControllerParameter();
    m_settingWidget->exec();
}

void MusicRightAreaWidget::resizeWindow()
{
    m_ui->songSearchWidget->resizeWindow();
    m_ui->musiclrccontainerforinline->resizeWindow();

    if(MObject_cast(MusicSimilarFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicSimilarFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicAlbumFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicAlbumFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicArtistFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicArtistFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }

    if(m_videoPlayerWidget && !m_videoPlayerWidget->isPopup())
    {
        m_videoPlayerWidget->resizeWindow();
    }
}

void MusicRightAreaWidget::getParameterSetting() const
{
    setSettingParameter();
    bool config = M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool();
    m_ui->musiclrccontainerforinline->setVisible(config);
         config = M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool();
    m_musicLrcForDesktop->setVisible(config);
    m_ui->musicDesktopLrc->setChecked(config);
}

void MusicRightAreaWidget::musicFunctionClicked(int index)
{
    if(M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool())
    {
        MusicApplication::instance()->musicWindowConciseChanged();
    }

    MusicFunction key = MStatic_cast(MusicFunction, index);
    if(key == LrcWidget) ///lrc option
    {
        musicButtonStyleClear(false);
        m_ui->stackedFunctionWidget->setStyleSheet(QString("#stackedFunctionWidget{%1}").arg(MusicUIObject::MBackgroundStyle01));
        m_ui->musicBackButton->setStyleSheet(MusicUIObject::MKGBtnBackBack);
        m_ui->musicRefreshButton->setStyleSheet(MusicUIObject::MKGBtnBackFresh);
        m_ui->line->hide();
    }
    else
    {
        musicButtonStyleClear(true);
        m_ui->stackedFunctionWidget->setStyleSheet(QString("#stackedFunctionWidget{%1}").arg(MusicUIObject::MBackgroundStyle17));
        m_ui->musicBackButton->setStyleSheet(MusicUIObject::MKGBtnForeBack);
        m_ui->musicRefreshButton->setStyleSheet(MusicUIObject::MKGBtnForeFresh);
        m_ui->line->show();
    }

    deleteStackedFuncWidget();
    m_ui->songSearchWidget->auditionStop();
    m_ui->lrcDisplayAllButton->setVisible(false);
    if(m_ui->musiclrccontainerforinline->lrcDisplayExpand())
    {
        musicLrcDisplayAllButtonClicked();
    }

    switch(key)
    {
        case KugGouSongWidget: //insert kugou song widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KuGouSong, this);
                m_ui->surfaceStackedWidget->addWidget(m_stackedFuncWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(m_stackedFuncWidget);
                m_ui->musicSongWidgetButton->setStyleSheet(MusicUIObject::MKGFuncSongForeClicked);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case KugGouRadioWidget: //insert kugou radio widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KuGouRadio, this);
                m_ui->surfaceStackedWidget->addWidget(m_stackedFuncWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(m_stackedFuncWidget);
                m_ui->musicRadioWidgetButton->setStyleSheet(MusicUIObject::MKGFuncRadioForeClicked);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case kugouListWidget: //insert kugou list widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KuGouList, this);
                m_ui->surfaceStackedWidget->addWidget(m_stackedFuncWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(m_stackedFuncWidget);
                m_ui->musicListWidgetButton->setStyleSheet(MusicUIObject::MKGFuncListForeClicked);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case VideoWidget: //insert video widget
            {
                if(!m_videoPlayerWidget)
                {
                    m_videoPlayerWidget = new MusicVideoPlayWidget(this);
                    connect(m_videoPlayerWidget, SIGNAL(freshButtonClicked(bool)), SLOT(musicVideoSetPopup(bool)));
                    connect(m_videoPlayerWidget, SIGNAL(fullscreenButtonClicked(bool)), SLOT(musicVideoFullscreen(bool)));
                }
                m_videoPlayerWidget->popup(false);

                QWidget *background = new QWidget(this);
                background->setStyleSheet(MusicUIObject::MBackgroundStyle17);
                m_stackedFuncWidget = background;
                m_ui->surfaceStackedWidget->addWidget(m_videoPlayerWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(m_videoPlayerWidget);
                m_ui->musicVideoWidgetButton->setStyleSheet(MusicUIObject::MKGFuncMVForeClicked);
                emit updateBackgroundTheme();
                break;
            }
        case kugouLiveWidget: //insert kugou live widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KugouMv, this);
                m_ui->surfaceStackedWidget->addWidget(m_stackedFuncWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(m_stackedFuncWidget);
                m_ui->musicLiveWidgetButton->setStyleSheet(MusicUIObject::MKGFuncLiveForeClicked);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case LrcWidget: //insert lrc display widget
            {
                m_ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MKGFuncLrcForeClicked);
                m_ui->surfaceStackedWidget->setCurrentIndex(1);
                m_ui->lrcDisplayAllButton->setStyleSheet(MusicUIObject::MKGTinyBtnLrcCollapse);
                m_ui->lrcDisplayAllButton->setVisible(true);
                emit updateBgThemeDownload();
                break;
            }
        case SearchWidget: //insert search display widget
            {
                QString searchedQString = m_ui->musicSongSearchLine->text().trimmed();
                searchedQString = searchedQString.isEmpty() ? m_ui->musicSongSearchLine->placeholderText() : searchedQString;
                //The string searched wouldn't allow to be none
                if(!searchedQString.isEmpty() && searchedQString != tr("please input search text"))
                {
                    m_ui->musicSongSearchLine->setText(searchedQString);
                    m_ui->songSearchWidget->startSearchQuery(searchedQString);
                }
                else
                {
                    musicFunctionClicked(MusicRightAreaWidget::KugGouSongWidget);

                    MusicMessageBox message;
                    message.setText(tr("enter input search text first"));
                    message.exec();
                    break;
                }

                m_ui->surfaceStackedWidget->setCurrentIndex(0);
                emit updateBackgroundTheme();
                break;
            }
        case SimilarWidget: //insert similar found widget
            {
                MusicSimilarFoundWidget *similarFoundWidget = new MusicSimilarFoundWidget(this);
                m_ui->surfaceStackedWidget->addWidget(similarFoundWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(similarFoundWidget);
                m_stackedFuncWidget = similarFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case AlbumWidget: //insert album found widget
            {
                MusicAlbumFoundWidget *albumFoundWidget = new MusicAlbumFoundWidget(this);
                m_ui->surfaceStackedWidget->addWidget(albumFoundWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(albumFoundWidget);
                m_stackedFuncWidget = albumFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case ArtistWidget: //insert artist found widget
            {
                MusicArtistFoundWidget *artistFoundWidget = new MusicArtistFoundWidget(this);
                m_ui->surfaceStackedWidget->addWidget(artistFoundWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(artistFoundWidget);
                m_stackedFuncWidget = artistFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case PlaylistWidget: //insert playlist found widget
            {
                MusicPlaylistFoundWidget *playlistFoundWidget = new MusicPlaylistFoundWidget(this);
                m_ui->surfaceStackedWidget->addWidget(playlistFoundWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(playlistFoundWidget);
                m_stackedFuncWidget = playlistFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case IndentifyWidget: //insert indentify songs widget
            {
                MusicIdentifySongsWidget *songsIdentifyWidget = new MusicIdentifySongsWidget(this);
                m_ui->surfaceStackedWidget->addWidget(songsIdentifyWidget);
                m_ui->surfaceStackedWidget->setCurrentWidget(songsIdentifyWidget);
                songsIdentifyWidget->getKey();
                m_stackedFuncWidget = songsIdentifyWidget;
                emit updateBackgroundTheme();
                break;
            }
        case KuiSheWidget: //insert kugou kuishe widget
            {
                KugouWindow *kugouWindow = new KugouWindow(KugouWindow::KuGouSingle, this);
                kugouWindow->setUrl(KugouUrl::getKuiSheUrl());
                m_ui->surfaceStackedWidget->addWidget(m_stackedFuncWidget = kugouWindow);
                m_ui->surfaceStackedWidget->setCurrentWidget(m_stackedFuncWidget);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        default: break;
    }
}

void MusicRightAreaWidget::musicSongCommentsWidget()
{
    if(M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool())
    {
        MusicApplication::instance()->musicWindowConciseChanged();
    }
    if(m_ui->surfaceStackedWidget->currentIndex() != 1)
    {
        musicFunctionClicked(MusicRightAreaWidget::LrcWidget);
    }

    m_ui->musiclrccontainerforinline->showSongCommentsWidget();
}

void MusicRightAreaWidget::musicSimilarFound(const QString &text)
{
    musicFunctionClicked(MusicRightAreaWidget::SimilarWidget);
    MStatic_cast(MusicSimilarFoundWidget*, m_stackedFuncWidget)->setSongName(text);
}

void MusicRightAreaWidget::musicAlbumFound(const QString &text)
{
    musicFunctionClicked(MusicRightAreaWidget::AlbumWidget);
    MStatic_cast(MusicAlbumFoundWidget*, m_stackedFuncWidget)->setSongName(text);
}

void MusicRightAreaWidget::musicArtistFound(const QString &text)
{
    musicFunctionClicked(MusicRightAreaWidget::ArtistWidget);
    MStatic_cast(MusicArtistFoundWidget*, m_stackedFuncWidget)->setSongName(text);
}

void MusicRightAreaWidget::musicPlaylistFound()
{
    musicFunctionClicked(MusicRightAreaWidget::PlaylistWidget);
    MStatic_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicLoadSongIndexWidget()
{
    musicFunctionClicked(MusicRightAreaWidget::KugGouSongWidget);
}

void MusicRightAreaWidget::deleteStackedFuncWidget()
{
    delete m_stackedFuncWidget;
    m_stackedFuncWidget = nullptr;
}

void MusicRightAreaWidget::setDestopLrcVisible(bool visible) const
{
    m_ui->musicDesktopLrc->setChecked(visible);
    m_musicLrcForDesktop->setVisible(visible);
    m_musicLrcForDesktop->initCurrentLrc();
    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, visible);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_musicLrcForDesktop->setWindowLockedChanged();
}

void MusicRightAreaWidget::setWindowLrcTypeChanged()
{
    bool type = m_musicLrcForDesktop ? m_musicLrcForDesktop->getWindowType() :
                MStatic_cast(bool, M_SETTING_PTR->value(MusicSettingManager::DLrcWindowTypeChoiced).toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometryChoiced, QPoint());

    MusicLrcContainerForDesktop *deskLrc = m_musicLrcForDesktop;
    if(type)
    {
        m_musicLrcForDesktop = new MusicLrcContainerHorizontalDesktop(this);
    }
    else
    {
        m_musicLrcForDesktop = new MusicLrcContainerVerticalDesktop(this);
    }

    if(deskLrc)
    {
        m_musicLrcForDesktop->setCurrentSongName( deskLrc->getCurrentSongName() );
        m_musicLrcForDesktop->showPlayStatus( deskLrc->getPlayStatus() );
    }
    m_musicLrcForDesktop->setSettingParameter();
    m_musicLrcForDesktop->initCurrentLrc();
    m_musicLrcForDesktop->setVisible(true);

    connect(m_musicLrcForDesktop, SIGNAL(setWindowLrcTypeChanged()), SLOT(setWindowLrcTypeChanged()));
    connect(m_musicLrcForDesktop, SIGNAL(theCurrentLrcUpdated()), MusicApplication::instance(),
                                  SLOT(musicCurrentLrcUpdated()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(),
                                  SLOT(musicSetting()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget,
                                  SLOT(changeDesktopLrcWidget()));

    M_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowTypeChoiced, type);
    deskLrc->deleteLater();
}

void MusicRightAreaWidget::songResearchButtonSearched(const QString &name)
{
    m_ui->musicSongSearchLine->setText(name.trimmed());
    musicFunctionClicked(MusicRightAreaWidget::SearchWidget);
}

void MusicRightAreaWidget::researchQueryByQuality(const QString &quality)
{
    QString text = m_ui->showCurrentSong->text().trimmed();
    if(text.isEmpty())
    {
        return;
    }

    m_ui->songSearchWidget->researchQueryByQuality(text, quality);
    m_ui->surfaceStackedWidget->setCurrentIndex(0);
    emit updateBackgroundTheme();
}

void MusicRightAreaWidget::musicButtonStyleClear(bool fore)
{
    m_ui->musicSongWidgetButton->setStyleSheet(fore ? MusicUIObject::MKGFuncSongFore :
                                                      MusicUIObject::MKGFuncSongBack);
    m_ui->musicRadioWidgetButton->setStyleSheet(fore ?MusicUIObject::MKGFuncRadioFore :
                                                      MusicUIObject::MKGFuncRadioBack);
    m_ui->musicListWidgetButton->setStyleSheet(fore ? MusicUIObject::MKGFuncListFore :
                                                      MusicUIObject::MKGFuncListBack);
    m_ui->musicVideoWidgetButton->setStyleSheet(fore ? MusicUIObject::MKGFuncMVFore :
                                                       MusicUIObject::MKGFuncMVBack);
    m_ui->musicLiveWidgetButton->setStyleSheet(fore ? MusicUIObject::MKGFuncLiveFore :
                                                      MusicUIObject::MKGFuncLiveBack);
    m_ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MKGFuncLrcFore);
}

void MusicRightAreaWidget::musicVideoButtonSearched(const QString &name)
{
    if(m_videoPlayerWidget && m_videoPlayerWidget->isPopup())
    {
        m_videoPlayerWidget->raise();
    }
    else
    {
        musicFunctionClicked(MusicRightAreaWidget::VideoWidget);
    }
    m_videoPlayerWidget->videoResearchButtonSearched(name);
}

void MusicRightAreaWidget::musicVideoSetPopup(bool popup)
{
    if(!m_videoPlayerWidget)
    {
        return;
    }

    m_videoPlayerWidget->popup(popup);
    if(popup)
    {
        m_ui->surfaceStackedWidget->addWidget(m_stackedFuncWidget);
        m_ui->surfaceStackedWidget->setCurrentWidget(m_stackedFuncWidget);

        MusicRegeditManager().setLeftWinEnable();
        QTimer::singleShot(10*MT_MS, this, SLOT(musicVideoActiveWindow()));
    }
    else
    {
        musicFunctionClicked(MusicRightAreaWidget::VideoWidget);
    }
}

void MusicRightAreaWidget::musicVideoActiveWindow()
{
    if(m_videoPlayerWidget)
    {
        MusicApplication::instance()->activateWindow();
        m_videoPlayerWidget->activateWindow();
    }
}

void MusicRightAreaWidget::musicVideoClosed()
{
    delete m_videoPlayerWidget;
    m_videoPlayerWidget = nullptr;
    musicFunctionClicked(MusicRightAreaWidget::LrcWidget);
}

void MusicRightAreaWidget::musicVideoFullscreen(bool full)
{
    if(!m_videoPlayerWidget)
    {
        return;
    }

    m_videoPlayerWidget->resizeWindow(full);
    m_videoPlayerWidget->blockMoveOption(full);
}

void MusicRightAreaWidget::musicLrcDisplayAllButtonClicked()
{
    bool lrcDisplayAll = !m_ui->musiclrccontainerforinline->lrcDisplayExpand();
    m_ui->musiclrccontainerforinline->setLrcDisplayExpand(lrcDisplayAll);
    m_ui->songsContainer->setHidden(lrcDisplayAll);

    int height = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height() - WINDOW_HEIGHT_MIN;

    QPropertyAnimation *lrcDisplayAllAnimation = new QPropertyAnimation(m_ui->lrcDisplayAllButton, "pos", this);
    lrcDisplayAllAnimation->setDuration(100);
    lrcDisplayAllAnimation->setStartValue(QPoint(lrcDisplayAll ? 350 : -370, 220 + height/2));
    lrcDisplayAllAnimation->setEndValue(QPoint(0, 220 + height/2));
    lrcDisplayAllAnimation->start();

    m_ui->lrcDisplayAllButton->setStyleSheet(lrcDisplayAll ? MusicUIObject::MKGTinyBtnLrcExpand :
                                                             MusicUIObject::MKGTinyBtnLrcCollapse);
    m_ui->musicWindowConcise->setEnabled(!lrcDisplayAll);
}

void MusicRightAreaWidget::musicContainerForWallpaperClicked()
{
    if(m_musicLrcForWallpaper)
    {
        MusicTopAreaWidget::instance()->musicWallpaperRemote(false);
        delete m_musicLrcForWallpaper;
        m_musicLrcForWallpaper = nullptr;
    }
    else
    {
        MusicRegeditManager().setLeftWinEnable();

        m_musicLrcForWallpaper = new MusicLrcContainerForWallpaper;
        m_musicLrcForWallpaper->setLrcAnalysisModel(m_ui->musiclrccontainerforinline->getLrcAnalysisModel());
        m_musicLrcForWallpaper->setSettingParameter();
        m_musicLrcForWallpaper->showFullScreen();
        connect(m_ui->musiclrccontainerforinline, SIGNAL(linearGradientColorChanged()), m_musicLrcForWallpaper,
                                                  SLOT(changeCurrentLrcColor()));

        MusicApplication::instance()->activateWindow();
        MusicApplication::instance()->showMinimized();
        MusicTopAreaWidget::instance()->musicWallpaperRemote(true);
    }
}
