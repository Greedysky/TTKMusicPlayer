#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicuiobject.h"
#include "musiclrccontainerfordesktop.h"
#include "musiclrccontainerforwallpaper.h"
#include "musicvideoplaywidget.h"
#include "musicdownloadstatusobject.h"
#include "musicsettingwidget.h"
#include "musictoastlabel.h"
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
#include "musicplatformmanager.h"
#include "musictopareawidget.h"
#include "musicadvancedsearchedwidget.h"
#include "musicrecommendfoundwidget.h"
#include "musicartistlistfoundwidget.h"
#include "musicwebdjradiowidget.h"
#include "musicwebmvradiofoundwidget.h"
#include "musiccloudmanagerwidget.h"
#include "musicscreensaverwidget.h"

#include "qkugou/qkugouwindow.h"

#include <QPropertyAnimation>

MusicRightAreaWidget *MusicRightAreaWidget::m_instance = nullptr;

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_instance = this;
    m_stackedFuncWidget = nullptr;
    m_stackedAutoWidget = nullptr;
    m_musicLrcForInterior = nullptr;
    m_musicLrcForDesktop = nullptr;
    m_musicLrcForWallpaper = nullptr;
    m_videoPlayerWidget = nullptr;

    m_lrcAnalysis = new MusicLrcAnalysis(this);
    m_lrcAnalysis->setLineMax(MUSIC_LRC_INTERIOR_MAX_LINE);

    m_downloadStatusObject = new MusicDownloadStatusObject(parent);
    m_settingWidget = new MusicSettingWidget(this);
    connect(m_settingWidget, SIGNAL(parameterSettingChanged()), parent, SLOT(applySettingParameter()));
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_settingWidget;
    delete m_lrcAnalysis;
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
    m_musicLrcForInterior = ui->musiclrccontainerforinterior;
    //
    m_musicLrcForInterior->setLrcAnalysisModel(m_lrcAnalysis);
    m_musicLrcForInterior->initFunctionLabel();
    m_musicLrcForInterior->resize(ui->functionsContainer->size());

    ui->musicBackButton->setStyleSheet(MusicUIObject::MQSSBtnBackBack);
    ui->musicRefreshButton->setStyleSheet(MusicUIObject::MQSSBtnBackFresh);

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
    connect(m_musicLrcForInterior, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget, SLOT(changeInteriorLrcWidget()));
    connect(m_musicLrcForInterior, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(musicCurrentLrcUpdated()));
    connect(m_musicLrcForInterior, SIGNAL(artistBackgroundHasChanged()), SIGNAL(updateBackgroundThemeDownload()));
    connect(m_musicLrcForInterior, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_musicLrcForInterior, SIGNAL(updateCurrentTime(qint64)), MusicApplication::instance(), SLOT(updateCurrentTime(qint64)));
    connect(ui->musicSongSearchEdit, SIGNAL(enterFinished(QString)), SLOT(musicSongSearchedFound(QString)));
}

void MusicRightAreaWidget::stopLrcMask() const
{
    if(checkSettingParameterValue())
    {
       m_musicLrcForInterior->stopLrcMask();
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
       m_musicLrcForInterior->startTimerClock();
       m_musicLrcForDesktop->startTimerClock();
       if(m_musicLrcForWallpaper)
       {
           m_musicLrcForWallpaper->startTimerClock();
       }
    }
}

void MusicRightAreaWidget::setCurrentPlayStatus(bool status) const
{
    m_musicLrcForDesktop->setCurrentPlayStatus(status);
}

bool MusicRightAreaWidget::getDestopLrcVisible() const
{
    return m_musicLrcForDesktop->isVisible();
}

void MusicRightAreaWidget::setInteriorLrcVisible(bool status) const
{
    m_musicLrcForInterior->setVisible(status);
}

bool MusicRightAreaWidget::getInteriorLrcVisible() const
{
    return m_musicLrcForInterior->isVisible();
}

bool MusicRightAreaWidget::checkSettingParameterValue() const
{
    return (M_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toBool() || M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool());
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const
{
    m_musicLrcForInterior->setCurrentTime(current, total);
    QString currentLrc, laterLrc;
    qint64 intervalTime;
    if(m_lrcAnalysis->findText(current, total, currentLrc, laterLrc, intervalTime))
    {   //If this is a new line of the lyrics, then restart lyrics display mask
        if(currentLrc != m_musicLrcForInterior->text())
        {
            if(!playStatus)
            {
                m_musicLrcForInterior->updateCurrentLrc(intervalTime);
            }

            {
                m_musicLrcForDesktop->setCurrentTime(current, total);
                m_musicLrcForDesktop->updateCurrentLrc(currentLrc, laterLrc, intervalTime);
            }

            if(m_musicLrcForWallpaper)
            {
                m_musicLrcForWallpaper->setCurrentTime(current, total);
                m_musicLrcForWallpaper->updateCurrentLrc(intervalTime);
            }
        }
    }
}

void MusicRightAreaWidget::loadCurrentSongLrc(const QString &name, const QString &path) const
{
    if(checkSettingParameterValue())
    {
        m_musicLrcForInterior->stopLrcMask();
        m_musicLrcForInterior->setCurrentSongName(name);

        MusicLrcAnalysis::State state;
        if(QFileInfo(path).suffix().toLower() == KRC_FILE_PREFIX)
        {
            TTK_LOGGER_INFO("Current in krc parser mode");
            state = m_lrcAnalysis->transKrcFileToTime(path);
        }
        else
        {
            TTK_LOGGER_INFO("Current in lrc parser mode");
            state = m_lrcAnalysis->transLrcFileToTime(path);
        }

        m_musicLrcForInterior->updateCurrentLrc(state);
        m_musicLrcForDesktop->stopLrcMask();
        m_musicLrcForDesktop->setCurrentSongName(name);

        if(!state)
        {
            m_musicLrcForDesktop->updateCurrentLrc(tr("unFoundLrc"), QString(), 0);
        }

        if(m_musicLrcForWallpaper)
        {
            m_musicLrcForWallpaper->stopLrcMask();
            m_musicLrcForWallpaper->setCurrentSongName(name);
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
    m_musicLrcForInterior->setSongSpeedChanged(time);
}

void MusicRightAreaWidget::checkLrcValid() const
{
    m_downloadStatusObject->checkLrcValid();
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
    m_musicLrcForInterior->resizeWindow();

    if(TTKObject_cast(MusicSimilarFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicSimilarFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicAlbumFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicAlbumFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicArtistFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicArtistFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicToplistFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicToplistFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicRecommendFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicRecommendFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicArtistListFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicArtistListFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicWebDJRadioWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicWebDJRadioWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicWebMVRadioFoundWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicWebMVRadioFoundWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget)->resizeWindow();
    }

    if(TTKObject_cast(MusicCloudManagerWidget*, m_stackedAutoWidget))
    {
        TTKObject_cast(MusicCloudManagerWidget*, m_stackedAutoWidget)->resizeWindow();
    }

    if(m_videoPlayerWidget && !m_videoPlayerWidget->isPopup())
    {
        m_videoPlayerWidget->resizeWindow();
    }
}

void MusicRightAreaWidget::applySettingParameter() const
{
    m_musicLrcForDesktop->applySettingParameter();
    m_musicLrcForInterior->applySettingParameter();
    if(m_musicLrcForWallpaper)
    {
        m_musicLrcForWallpaper->applySettingParameter();
    }

    //
    bool config = M_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toBool();
    m_musicLrcForInterior->setVisible(config);
         config = M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool();
    m_musicLrcForDesktop->setVisible(config);
    m_ui->musicDesktopLrc->setChecked(config);
    //
    if(TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget)->applySettingParameter();
    }
}

void MusicRightAreaWidget::musicFunctionClicked(int index)
{
    const MusicFunction key = TTKStatic_cast(MusicFunction, index);
    musicFunctionParameterInit(key);

    switch(key)
    {
        case KugGouSongWidget: //insert kugou song widget
            {
                m_stackedFuncWidget = new QKugouWindow(QKugouWindow::KuGouSong, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(0);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case KugGouRadioWidget: //insert kugou radio widget
            {
                m_stackedFuncWidget = new QKugouWindow(QKugouWindow::KuGouRadio, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(1);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case kugouListWidget: //insert kugou list widget
            {
                m_stackedFuncWidget = new QKugouWindow(QKugouWindow::KuGouList, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(2);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                Q_EMIT updateBackgroundTheme();
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

                QWidget *widget = new QWidget(this);
                widget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle17);
                m_stackedFuncWidget = widget;
                m_ui->functionsContainer->addWidget(m_videoPlayerWidget);
                m_ui->functionsContainer->setCurrentWidget(m_videoPlayerWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(3);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case kugouLiveWidget: //insert kugou live widget
            {
                m_stackedFuncWidget = new QKugouWindow(QKugouWindow::KugouMv, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
                m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(4);
                connect(m_ui->musicBackButton, SIGNAL(clicked()), m_stackedFuncWidget, SLOT(goBack()));
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case LrcWidget: //insert lrc display widget
            {
                m_ui->stackedWidgetFunctionOption->musicButtonStyle(5);
                m_ui->functionsContainer->setCurrentIndex(1);
                m_ui->lrcDisplayAllButton->setStyleSheet(MusicUIObject::MQSSTinyBtnLrcCollapse);
                m_ui->lrcDisplayAllButton->setVisible(true);
                Q_EMIT updateBackgroundThemeDownload();
                break;
            }
        case SearchWidget: //insert search display widget
            {
                QString searchedQString = m_ui->musicSongSearchEdit->text().trimmed();
                        searchedQString = searchedQString.isEmpty() ? m_ui->musicSongSearchEdit->placeholderText() : searchedQString;
                //The string searched wouldn't allow to be none
                if(!searchedQString.isEmpty() && searchedQString != tr("please input search text"))
                {
                    m_ui->musicSongSearchEdit->setText(searchedQString);
                    m_ui->songSearchWidget->startSearchQuery(searchedQString);
                }
                else
                {
                    musicFunctionClicked(MusicRightAreaWidget::KugGouSongWidget);

                    MusicToastLabel::popup(tr("enter input search text first"));
                    break;
                }

                m_ui->functionsContainer->setCurrentIndex(0);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case SearchSingleWidget: //insert search display widget
            {
                m_ui->functionsContainer->setCurrentIndex(0);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case SimilarWidget: //insert similar found widget
            {
                MusicSimilarFoundWidget *widget = new MusicSimilarFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case AlbumWidget: //insert album found widget
            {
                MusicAlbumFoundWidget *widget = new MusicAlbumFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case ArtistWidget: //insert artist found widget
            {
                MusicArtistFoundWidget *widget = new MusicArtistFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case ArtistCategoryWidget: //insert artist category found widget
            {
                MusicArtistListFoundWidget *widget = new MusicArtistListFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case ToplistWidget: //insert toplist found widget
            {
                MusicToplistFoundWidget *widget = new MusicToplistFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case PlaylistWidget: //insert playlist found widget
            {
                MusicPlaylistFoundWidget *widget = new MusicPlaylistFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case RecommendWidget: //insert recommend found widget
            {
                MusicRecommendFoundWidget *widget = new MusicRecommendFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case AdvancedSearchWidget: //insert advanced search widget
            {
                MusicAdvancedSearchedWidget *widget = new MusicAdvancedSearchedWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case IndentifyWidget: //insert indentify songs widget
            {
                MusicIdentifySongsWidget *widget = new MusicIdentifySongsWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                widget->getKey();
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case KuiSheWidget: //insert kugou kuishe widget
            {
                QKugouWindow *widget = new QKugouWindow(QKugouWindow::KuGouSingle, this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                widget->setUrl(QKugouUrl::getKuiSheUrl());
                connect(m_ui->musicBackButton, SIGNAL(clicked()), widget, SLOT(goBack()));
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case WebDJRadioWidget: //insert web dj radio widget
            {
                MusicWebDJRadioWidget *widget = new MusicWebDJRadioWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                widget->initialize();
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case WebMVRadioWidget: //insert web mv radio widget
            {
                MusicWebMVRadioFoundWidget *widget = new MusicWebMVRadioFoundWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                widget->setSongName(QString());
                Q_EMIT updateBackgroundTheme();
                break;
            }
        case ScreenSaverWidget: //insert screen saver widget
            {
                MusicScreenSaverWidget *widget = new MusicScreenSaverWidget(this);
                m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
                m_ui->functionsContainer->setCurrentWidget(widget);
                Q_EMIT updateBackgroundTheme();
                break;
            }
        default: break;
    }
}

void MusicRightAreaWidget::musicFunctionClicked(int index, QWidget *widget)
{
    const MusicFunction key = TTKStatic_cast(MusicFunction, index);
    musicFunctionParameterInit(key);

    m_stackedAutoWidget = widget;
    m_ui->functionsContainer->addWidget(m_stackedAutoWidget);
    m_ui->functionsContainer->setCurrentWidget(m_stackedAutoWidget);
    Q_EMIT updateBackgroundTheme();
}

void MusicRightAreaWidget::musicSongCommentsWidget()
{
    if(M_SETTING_PTR->value(MusicSettingManager::WindowConcise).toBool())
    {
        MusicApplication::instance()->musicWindowConciseChanged();
    }
    if(m_ui->functionsContainer->currentIndex() != APP_WINDOW_INDEX_1)
    {
        musicFunctionClicked(MusicRightAreaWidget::LrcWidget);
    }

    m_musicLrcForInterior->showSongCommentsWidget();
}

void MusicRightAreaWidget::musicSimilarFound(const QString &text)
{
    musicFunctionClicked(MusicRightAreaWidget::SimilarWidget);
    TTKStatic_cast(MusicSimilarFoundWidget*, m_stackedFuncWidget)->setSongName(text);
}

void MusicRightAreaWidget::musicAlbumFound(const QString &text, const QString &id)
{
    musicFunctionClicked(MusicRightAreaWidget::AlbumWidget);
    MusicAlbumFoundWidget *w = TTKStatic_cast(MusicAlbumFoundWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicArtistCategoryFound()
{
    musicFunctionClicked(MusicRightAreaWidget::ArtistCategoryWidget);
    TTKStatic_cast(MusicArtistListFoundWidget*, m_stackedFuncWidget)->setSongName(QString());
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
    MusicArtistFoundWidget *w = TTKStatic_cast(MusicArtistFoundWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicToplistFound()
{
    musicFunctionClicked(MusicRightAreaWidget::ToplistWidget);
    TTKStatic_cast(MusicToplistFoundWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicPlaylistFound(const QString &id)
{
    musicFunctionClicked(MusicRightAreaWidget::PlaylistWidget);
    MusicPlaylistFoundWidget *w = TTKStatic_cast(MusicPlaylistFoundWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(QString()) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicRecommendFound()
{
    musicFunctionClicked(MusicRightAreaWidget::RecommendWidget);
    TTKStatic_cast(MusicRecommendFoundWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicAdvancedSearch()
{
    musicFunctionClicked(MusicRightAreaWidget::AdvancedSearchWidget);
}

void MusicRightAreaWidget::musicSongSearchedFound(const QString &text)
{
    m_ui->musicSongSearchEdit->setText(text.trimmed());
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
    const bool pre = M_SETTING_PTR->value(MusicSettingManager::WindowConcise).toBool();
    M_SETTING_PTR->setValue(MusicSettingManager::WindowConcise, false);
    musicFunctionClicked(MusicRightAreaWidget::KugGouSongWidget);
    M_SETTING_PTR->setValue(MusicSettingManager::WindowConcise, pre);
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
    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, visible);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_musicLrcForDesktop->setWindowLockedChanged();
}

void MusicRightAreaWidget::setWindowLrcTypeChanged()
{
    const bool type = m_musicLrcForDesktop ? m_musicLrcForDesktop->getWindowType() : TTKStatic_cast(bool, M_SETTING_PTR->value(MusicSettingManager::DLrcWindowType).toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometry, QPoint());

    MusicLrcContainerForDesktop *deskLrc = m_musicLrcForDesktop;
    if(type)
    {
        m_musicLrcForDesktop = new MusicLrcContainerHorizontalDesktop(this);
    }
    else
    {
        m_musicLrcForDesktop = new MusicLrcContainerVerticalDesktop(this);
    }
    m_musicLrcForDesktop->setLrcAnalysisModel(m_lrcAnalysis);

    if(deskLrc)
    {
        m_musicLrcForDesktop->setCurrentSongName(deskLrc->getCurrentSongName());
        m_musicLrcForDesktop->setCurrentPlayStatus(deskLrc->getPlayStatus());
    }
    m_musicLrcForDesktop->applySettingParameter();
    m_musicLrcForDesktop->initCurrentLrc();
    m_musicLrcForDesktop->setVisible(true);

    connect(m_musicLrcForDesktop, SIGNAL(setWindowLrcTypeChanged()), SLOT(setWindowLrcTypeChanged()));
    connect(m_musicLrcForDesktop, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(musicCurrentLrcUpdated()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget, SLOT(changeDesktopLrcWidget()));

    M_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowType, type);
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
    Q_EMIT updateBackgroundTheme();
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
#ifdef Q_OS_WIN
        MusicPlatformManager platform;
        platform.setLeftWinEnabled();
#endif
        QTimer::singleShot(10 * MT_MS, this, SLOT(musicVideoActiveWindow()));
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
    const bool lrcDisplayAll = !m_musicLrcForInterior->lrcDisplayExpand();
    m_musicLrcForInterior->setLrcDisplayExpand(lrcDisplayAll);
    m_ui->centerLeftWidget->setHidden(lrcDisplayAll);

    const int height = m_musicLrcForInterior->size().height() - m_ui->lrcDisplayAllButton->height() - 40;
    QPropertyAnimation *lrcDisplayAllAnimation = new QPropertyAnimation(m_ui->lrcDisplayAllButton, "pos", this);
    lrcDisplayAllAnimation->setDuration(100);
    lrcDisplayAllAnimation->setStartValue(QPoint(lrcDisplayAll ? 300 : -320, height/2));
    lrcDisplayAllAnimation->setEndValue(QPoint(0, height/2));
    lrcDisplayAllAnimation->start();

    m_ui->lrcDisplayAllButton->setStyleSheet(lrcDisplayAll ? MusicUIObject::MQSSTinyBtnLrcExpand : MusicUIObject::MQSSTinyBtnLrcCollapse);
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
#ifdef Q_OS_WIN
        MusicPlatformManager platform;
        platform.setLeftWinEnabled();
#endif
        m_musicLrcForWallpaper = new MusicLrcContainerForWallpaper;
        m_musicLrcForWallpaper->setLrcAnalysisModel(m_lrcAnalysis);
        m_musicLrcForWallpaper->applySettingParameter();
        m_musicLrcForWallpaper->showFullScreen();
        connect(m_musicLrcForInterior, SIGNAL(linearGradientColorChanged()), m_musicLrcForWallpaper, SLOT(changeCurrentLrcColor()));

        MusicApplication::instance()->activateWindow();
        MusicApplication::instance()->showMinimized();
        MusicTopAreaWidget::instance()->musicWallpaperRemote(true);
    }
}

void MusicRightAreaWidget::musicChangeDownloadFulllyWidget()
{
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimit, true);
}

void MusicRightAreaWidget::musicChangeDownloadCustumWidget()
{
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimit, false);
    m_settingWidget->changeDownloadWidget();
    showSettingWidget();
}

void MusicRightAreaWidget::musicFunctionParameterInit(MusicFunction func)
{
    if(M_SETTING_PTR->value(MusicSettingManager::WindowConcise).toBool())
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
    if(m_musicLrcForInterior->lrcDisplayExpand() && func != LrcWidget)
    {
        musicLrcDisplayAllButtonClicked();
    }
}
