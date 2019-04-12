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
#include "musictoplistfoundwidget.h"
#include "musicsongsearchonlinewidget.h"
#include "musicidentifysongswidget.h"
#include "musicfunctionuiobject.h"
#include "musictinyuiobject.h"
#include "musicfunctionlistuiobject.h"
#include "musicregeditmanager.h"
#include "musictopareawidget.h"
#include "musicotherdefine.h"
#include "musicadvancedsearchedwidget.h"
#include "musicrecommendfoundwidget.h"
#include "musicartistlistfoundwidget.h"
#include "musicwebdjradiowidget.h"
#include "musicwebmvradiofoundwidget.h"
#include "musiccloudmanagerwidget.h"

#include "qkugou/kugouwindow.h"

#include <QPropertyAnimation>

MusicRightAreaWidget *MusicRightAreaWidget::m_instance = nullptr;

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_instance = this;
    m_stackedFuncWidget = nullptr;
    m_stackedAutoWidget = nullptr;
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

MusicRightAreaWidget *MusicRightAreaWidget::instance()
{
    return m_instance;
}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;

    ui->musiclrccontainerforinline->resize(ui->functionsContainer->size());

    ui->musicBackButton->setStyleSheet(MusicUIObject::MKGBtnBackBack);
    ui->musicRefreshButton->setStyleSheet(MusicUIObject::MKGBtnBackFresh);

    ui->lrcDisplayAllButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcDisplayAllButton->setIconSize(QSize(15, 56));
    connect(ui->lrcDisplayAllButton, SIGNAL(clicked()), SLOT(musicLrcDisplayAllButtonClicked()));
    //

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(ui->musicSearchButton, MusicRightAreaWidget::SearchWidget);
    group->addButton(ui->musicWindowIdentify, MusicRightAreaWidget::IndentifyWidget);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(musicFunctionClicked(int)));
    connect(ui->stackedWidgetFunctionOption, SIGNAL(buttonClicked(int)), SLOT(musicFunctionClicked(int)));
    //
    connect(ui->musiclrccontainerforinline, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget, SLOT(changeInlineLrcWidget()));
    connect(ui->musiclrccontainerforinline, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(musicCurrentLrcUpdated()));
    connect(ui->musiclrccontainerforinline, SIGNAL(artistBackgroundHasChanged()), SIGNAL(updateBackgroundThemeDownload()));
    connect(ui->musiclrccontainerforinline, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(ui->musiclrccontainerforinline, SIGNAL(updateCurrentTime(qint64)), MusicApplication::instance(), SLOT(updateCurrentTime(qint64)));
    connect(ui->musicSongSearchLine, SIGNAL(enterFinished(QString)), SLOT(musicSongSearchedFound(QString)));
}

void MusicRightAreaWidget::stopLrcMask() const
{
    if(checkSettingParameterValue())
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
    if(checkSettingParameterValue())
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
    return ( M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool() || M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool() );
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
    if(checkSettingParameterValue())
    {
        m_ui->musiclrccontainerforinline->stopLrcMask();
        m_ui->musiclrccontainerforinline->setCurrentSongName( name );
        const bool state = m_ui->musiclrccontainerforinline->transLyricFileToTime(path);

        m_musicLrcForDesktop->stopLrcMask();
        m_musicLrcForDesktop->setCurrentSongName( name );

        if(!state)
        {
            m_musicLrcForDesktop->updateCurrentLrc(tr("unFoundLrc"), QString(), 0);
        }

        if(m_musicLrcForWallpaper)
        {
            m_musicLrcForWallpaper->stopLrcMask();
            m_musicLrcForWallpaper->setCurrentSongName( name );
            m_musicLrcForWallpaper->start(true);

            if(!state)
            {
                m_musicLrcForWallpaper->updateCurrentLrc(tr("unFoundLrc"));
            }
        }

    }
}

void MusicRightAreaWidget::setSongSpeedAndSlow(qint64 time) const
{
    m_ui->musiclrccontainerforinline->setSongSpeedChanged(time);
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

void MusicRightAreaWidget::musicArtistSearch(const QString &id)
{
    m_rawData = id;
    QTimer::singleShot(MT_MS, this, SLOT(musicArtistSearchFound()));
}

void MusicRightAreaWidget::musicAlbumSearch(const QString &id)
{
    m_rawData = id;
    QTimer::singleShot(MT_MS, this, SLOT(musicAlbumSearchFound()));
}

void MusicRightAreaWidget::musicMovieSearch(const QString &id)
{
    m_rawData = id;
    QTimer::singleShot(MT_MS, this, SLOT(musicMovieSearchFound()));
}

void MusicRightAreaWidget::musicMovieRadioSearch(const QVariant &data)
{
    m_rawData = data;
    QTimer::singleShot(MT_MS, this, SLOT(musicMovieSearchRadioFound()));
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
    else if(MObject_cast(MusicToplistFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicToplistFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicRecommendFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicRecommendFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicArtistListFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicArtistListFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicWebDJRadioWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicWebDJRadioWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(MObject_cast(MusicWebMVRadioFoundWidget*, m_stackedFuncWidget))
    {
        MObject_cast(MusicWebMVRadioFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }

    if(MObject_cast(MusicCloudManagerWidget*, m_stackedAutoWidget))
    {
        MObject_cast(MusicCloudManagerWidget*, m_stackedAutoWidget)->resizeWindow();
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
    const MusicFunction key = MStatic_cast(MusicFunction, index);
    musicFunctionParameterInit(key);

    switch(key)
    {
        case KugGouSongWidget: //insert kugou song widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KuGouSong, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(0);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case KugGouRadioWidget: //insert kugou radio widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KuGouRadio, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(1);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case kugouListWidget: //insert kugou list widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KuGouList, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(2);
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
                m_ui->functionsContainer->addWidget(m_videoPlayerWidget);
                m_ui->functionsContainer->setCurrentWidget(m_videoPlayerWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(3);
                emit updateBackgroundTheme();
                break;
            }
        case kugouLiveWidget: //insert kugou live widget
            {
                m_stackedFuncWidget = new KugouWindow(KugouWindow::KugouMv, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(4);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case LrcWidget: //insert lrc display widget
            {
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(5);
                m_ui->functionsContainer->setCurrentIndex(1);
                m_ui->lrcDisplayAllButton->setStyleSheet(MusicUIObject::MKGTinyBtnLrcCollapse);
                m_ui->lrcDisplayAllButton->setVisible(true);
                emit updateBackgroundThemeDownload();
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

                m_ui->functionsContainer->setCurrentIndex(0);
                emit updateBackgroundTheme();
                break;
            }
        case SearchSingleWidget: //insert search display widget
            {
                m_ui->functionsContainer->setCurrentIndex(0);
                emit updateBackgroundTheme();
                break;
            }
        case SimilarWidget: //insert similar found widget
            {
                MusicSimilarFoundWidget *similarFoundWidget = new MusicSimilarFoundWidget(this);
                m_ui->functionsContainer->addWidget(similarFoundWidget);
                m_ui->functionsContainer->setCurrentWidget(similarFoundWidget);
                m_stackedFuncWidget = similarFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case AlbumWidget: //insert album found widget
            {
                MusicAlbumFoundWidget *albumFoundWidget = new MusicAlbumFoundWidget(this);
                m_ui->functionsContainer->addWidget(albumFoundWidget);
                m_ui->functionsContainer->setCurrentWidget(albumFoundWidget);
                m_stackedFuncWidget = albumFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case ArtistWidget: //insert artist found widget
            {
                MusicArtistFoundWidget *artistFoundWidget = new MusicArtistFoundWidget(this);
                m_ui->functionsContainer->addWidget(artistFoundWidget);
                m_ui->functionsContainer->setCurrentWidget(artistFoundWidget);
                m_stackedFuncWidget = artistFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case ArtistCategoryWidget: //insert artist category found widget
            {
                MusicArtistListFoundWidget *artistlistFoundWidget = new MusicArtistListFoundWidget(this);
                m_ui->functionsContainer->addWidget(artistlistFoundWidget);
                m_ui->functionsContainer->setCurrentWidget(artistlistFoundWidget);
                m_stackedFuncWidget = artistlistFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case ToplistWidget: //insert toplist found widget
            {
                MusicToplistFoundWidget *toplistFoundWidget = new MusicToplistFoundWidget(this);
                m_ui->functionsContainer->addWidget(toplistFoundWidget);
                m_ui->functionsContainer->setCurrentWidget(toplistFoundWidget);
                m_stackedFuncWidget = toplistFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case PlaylistWidget: //insert playlist found widget
            {
                MusicPlaylistFoundWidget *playlistFoundWidget = new MusicPlaylistFoundWidget(this);
                m_ui->functionsContainer->addWidget(playlistFoundWidget);
                m_ui->functionsContainer->setCurrentWidget(playlistFoundWidget);
                m_stackedFuncWidget = playlistFoundWidget;
                emit updateBackgroundTheme();
                break;
            }
        case RecommendWidget: //insert recommend found widget
            {
                MusicRecommendFoundWidget *recommendWidget = new MusicRecommendFoundWidget(this);
                m_ui->functionsContainer->addWidget(recommendWidget);
                m_ui->functionsContainer->setCurrentWidget(recommendWidget);
                m_stackedFuncWidget = recommendWidget;
                emit updateBackgroundTheme();
                break;
            }
        case AdvancedSearchWidget: //insert advanced search widget
            {
                MusicAdvancedSearchedWidget *advancedWidget = new MusicAdvancedSearchedWidget(this);
                m_ui->functionsContainer->addWidget(advancedWidget);
                m_ui->functionsContainer->setCurrentWidget(advancedWidget);
                m_stackedFuncWidget = advancedWidget;
                emit updateBackgroundTheme();
                break;
            }
        case IndentifyWidget: //insert indentify songs widget
            {
                MusicIdentifySongsWidget *songsIdentifyWidget = new MusicIdentifySongsWidget(this);
                m_ui->functionsContainer->addWidget(songsIdentifyWidget);
                m_ui->functionsContainer->setCurrentWidget(songsIdentifyWidget);
                songsIdentifyWidget->getKey();
                m_stackedFuncWidget = songsIdentifyWidget;
                emit updateBackgroundTheme();
                break;
            }
        case KuiSheWidget: //insert kugou kuishe widget
            {
                KugouWindow *kugouWindow = new KugouWindow(KugouWindow::KuGouSingle, this);
                kugouWindow->setUrl(KugouUrl::getKuiSheUrl());
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = kugouWindow);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                emit updateBackgroundTheme();
                break;
            }
        case WebDJRadioWidget: //insert web dj radio widget
            {
                MusicWebDJRadioWidget *djRadio = new MusicWebDJRadioWidget(this);
                djRadio->init();
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = djRadio);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                emit updateBackgroundTheme();
                break;
            }
        case WebMVRadioWidget: //insert web mv radio widget
            {
                MusicWebMVRadioFoundWidget *mvRadio = new MusicWebMVRadioFoundWidget(this);
                mvRadio->setSongName(QString());
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = mvRadio);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                emit updateBackgroundTheme();
                break;
            }
        default: break;
    }
}

void MusicRightAreaWidget::musicFunctionClicked(int index, QWidget *widget)
{
    const MusicFunction key = MStatic_cast(MusicFunction, index);
    musicFunctionParameterInit(key);

    m_stackedAutoWidget = widget;
    m_ui->functionsContainer->addWidget(m_stackedAutoWidget);
    m_ui->functionsContainer->setCurrentWidget(m_stackedAutoWidget);
    emit updateBackgroundTheme();
}

void MusicRightAreaWidget::musicSongCommentsWidget()
{
    if(M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool())
    {
        MusicApplication::instance()->musicWindowConciseChanged();
    }
    if(m_ui->functionsContainer->currentIndex() != APP_WINDOW_INDEX_1)
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

void MusicRightAreaWidget::musicAlbumFound(const QString &text, const QString &id)
{
    musicFunctionClicked(MusicRightAreaWidget::AlbumWidget);
    MusicAlbumFoundWidget *w = MStatic_cast(MusicAlbumFoundWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicArtistCategoryFound()
{
    musicFunctionClicked(MusicRightAreaWidget::ArtistCategoryWidget);
    MStatic_cast(MusicArtistListFoundWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicArtistSearchFound()
{
    musicArtistFound(QString(), m_rawData.toString());
}

void MusicRightAreaWidget::musicAlbumSearchFound()
{
    musicAlbumFound(QString(), m_rawData.toString());
}

void MusicRightAreaWidget::musicMovieSearchFound()
{
    musicVideoButtonSearched(QString(), m_rawData.toString());
}

void MusicRightAreaWidget::musicMovieSearchRadioFound()
{
    if(m_videoPlayerWidget && m_videoPlayerWidget->isPopup())
    {
        m_videoPlayerWidget->raise();
    }
    else
    {
        musicFunctionClicked(MusicRightAreaWidget::VideoWidget);
    }

    m_videoPlayerWidget->videoResearchButtonSearched(m_rawData);
}

void MusicRightAreaWidget::musicArtistFound(const QString &text, const QString &id)
{
    musicFunctionClicked(MusicRightAreaWidget::ArtistWidget);
    MusicArtistFoundWidget *w = MStatic_cast(MusicArtistFoundWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicToplistFound()
{
    musicFunctionClicked(MusicRightAreaWidget::ToplistWidget);
    MStatic_cast(MusicToplistFoundWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicPlaylistFound(const QString &id)
{
    musicFunctionClicked(MusicRightAreaWidget::PlaylistWidget);
    MusicPlaylistFoundWidget *w = MStatic_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(QString()) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicRecommendFound()
{
    musicFunctionClicked(MusicRightAreaWidget::RecommendWidget);
    MStatic_cast(MusicRecommendFoundWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicAdvancedSearch()
{
    musicFunctionClicked(MusicRightAreaWidget::AdvancedSearchWidget);
}

void MusicRightAreaWidget::musicSongSearchedFound(const QString &text)
{
    m_ui->musicSongSearchLine->setText(text.trimmed());
    musicFunctionClicked(MusicRightAreaWidget::SearchWidget);
}

void MusicRightAreaWidget::musicSingleSearchedFound(const QString &id)
{
    musicFunctionClicked(MusicRightAreaWidget::SearchSingleWidget);
    m_ui->songSearchWidget->startSearchSingleQuery(id);
}

void MusicRightAreaWidget::musicLoadSongIndexWidget()
{
    ///To prevent concise state changed while function musicWindowConciseChanged first called
    const bool pre = M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool();
    M_SETTING_PTR->setValue(MusicSettingManager::WindowConciseChoiced, false);
    musicFunctionClicked(MusicRightAreaWidget::KugGouSongWidget);
    M_SETTING_PTR->setValue(MusicSettingManager::WindowConciseChoiced, pre);
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
    const bool type = m_musicLrcForDesktop ? m_musicLrcForDesktop->getWindowType() : MStatic_cast(bool, M_SETTING_PTR->value(MusicSettingManager::DLrcWindowTypeChoiced).toInt());
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
    connect(m_musicLrcForDesktop, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(musicCurrentLrcUpdated()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget, SLOT(changeDesktopLrcWidget()));

    M_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowTypeChoiced, type);
    deskLrc->deleteLater();
}

void MusicRightAreaWidget::researchQueryByQuality(const QString &quality)
{
    const QString &text = m_ui->showCurrentSong->text().trimmed();
    if(text.isEmpty())
    {
        return;
    }

    musicFunctionParameterInit(MusicRightAreaWidget::SearchWidget);
    m_ui->songSearchWidget->researchQueryByQuality(text, quality);
    m_ui->functionsContainer->setCurrentIndex(APP_WINDOW_INDEX_0);
    emit updateBackgroundTheme();
}

void MusicRightAreaWidget::musicVideoButtonSearched(const QString &name, const QString &id)
{
    if(m_videoPlayerWidget && m_videoPlayerWidget->isPopup())
    {
        m_videoPlayerWidget->raise();
    }
    else
    {
        musicFunctionClicked(MusicRightAreaWidget::VideoWidget);
    }

    id.isEmpty() ? m_videoPlayerWidget->videoResearchButtonSearched(name) : m_videoPlayerWidget->startSearchSingleQuery(id);
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
        m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
        m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);

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
    const bool lrcDisplayAll = !m_ui->musiclrccontainerforinline->lrcDisplayExpand();
    m_ui->musiclrccontainerforinline->setLrcDisplayExpand(lrcDisplayAll);
    m_ui->centerLeftWidget->setHidden(lrcDisplayAll);

    const int height = m_ui->musiclrccontainerforinline->size().height() - m_ui->lrcDisplayAllButton->height() - 40;
    QPropertyAnimation *lrcDisplayAllAnimation = new QPropertyAnimation(m_ui->lrcDisplayAllButton, "pos", this);
    lrcDisplayAllAnimation->setDuration(100);
    lrcDisplayAllAnimation->setStartValue(QPoint(lrcDisplayAll ? 300 : -320, height/2));
    lrcDisplayAllAnimation->setEndValue(QPoint(0, height/2));
    lrcDisplayAllAnimation->start();

    m_ui->lrcDisplayAllButton->setStyleSheet(lrcDisplayAll ? MusicUIObject::MKGTinyBtnLrcExpand : MusicUIObject::MKGTinyBtnLrcCollapse);
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
        connect(m_ui->musiclrccontainerforinline, SIGNAL(linearGradientColorChanged()), m_musicLrcForWallpaper, SLOT(changeCurrentLrcColor()));

        MusicApplication::instance()->activateWindow();
        MusicApplication::instance()->showMinimized();
        MusicTopAreaWidget::instance()->musicWallpaperRemote(true);
    }
}

void MusicRightAreaWidget::musicChangeDownloadFulllyWidget()
{
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitChoiced, true);
}

void MusicRightAreaWidget::musicChangeDownloadCustumWidget()
{
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitChoiced, false);
    m_settingWidget->changeDownloadWidget();
    showSettingWidget();
}

void MusicRightAreaWidget::musicFunctionParameterInit(MusicFunction func)
{
    if(M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toBool())
    {
        MusicApplication::instance()->musicWindowConciseChanged();
    }

    if(func == LrcWidget) ///lrc option
    {
        m_ui->stackedWidgetFunctionOption->musicButtonStyleClear(false);
        m_ui->stackedFunctionWidget->transparent(true);
    }
    else
    {
        m_ui->stackedWidgetFunctionOption->musicButtonStyleClear(true);
        m_ui->stackedFunctionWidget->transparent(false);
    }

    deleteStackedFuncWidget();
    m_stackedAutoWidget = nullptr;

    m_ui->songSearchWidget->auditionStop();
    m_ui->lrcDisplayAllButton->setVisible(false);
    if(m_ui->musiclrccontainerforinline->lrcDisplayExpand() && func != LrcWidget)
    {
        musicLrcDisplayAllButtonClicked();
    }
}
