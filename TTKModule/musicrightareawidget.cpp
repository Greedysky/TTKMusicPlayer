#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musiclrccontainerfordesktop.h"
#include "musiclrccontainerforwallpaper.h"
#include "musicvideoplaywidget.h"
#include "musicdownloadstatusmodule.h"
#include "musicsettingwidget.h"
#include "musictoastlabel.h"
#include "musicalbumquerywidget.h"
#include "musicartistquerywidget.h"
#include "musicsimilarquerywidget.h"
#include "musicplaylistquerywidget.h"
#include "musictoplistquerywidget.h"
#include "musicidentifysongswidget.h"
#include "musicfunctionuiobject.h"
#include "musictinyuiobject.h"
#include "musicfunctionlistuiobject.h"
#include "musicplatformmanager.h"
#include "musictopareawidget.h"
#include "musicadvancedsearchedwidget.h"
#include "musicrecommendquerywidget.h"
#include "musicartistlistquerywidget.h"
#include "musicwebdjradiowidget.h"
#include "musicwebmvradioquerywidget.h"
#include "musiccloudmanagerwidget.h"
#include "musicscreensaverwidget.h"

#include "qkugou/qkugouwindow.h"

#include <QPropertyAnimation>

MusicRightAreaWidget *MusicRightAreaWidget::m_instance = nullptr;

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_instance = this;
    m_lowPowerMode = false;
    m_funcIndex = KugGouSongWidget;
    m_stackedFuncWidget = nullptr;
    m_stackedAutoWidget = nullptr;
    m_musicLrcForInterior = nullptr;
    m_musicLrcForDesktop = nullptr;
    m_musicLrcForWallpaper = nullptr;
    m_videoPlayerWidget = nullptr;

    m_lrcAnalysis = new MusicLrcAnalysis(this);
    m_lrcAnalysis->setLineMax(MUSIC_LRC_INTERIOR_MAX_LINE);

    m_downloadStatusObject = new MusicDownloadStatusModule(parent);
    m_settingWidget = new MusicSettingWidget(this);
    connect(m_settingWidget, SIGNAL(parameterSettingChanged()), parent, SLOT(applySettingParameter()));
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_settingWidget;
    delete m_downloadStatusObject;
    delete m_musicLrcForDesktop;
    delete m_musicLrcForWallpaper;

    if(m_videoPlayerWidget)
    {
        m_videoPlayerWidget->setVisible(false); //Fix bug on linux
        delete m_videoPlayerWidget;
    }
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
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->musicSearchButton, MusicRightAreaWidget::SearchWidget);
    buttonGroup->addButton(ui->musicWindowIdentify, MusicRightAreaWidget::IndentifyWidget);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(buttonGroup, SIGNAL(idClicked(int)), SLOT(functionClicked(int)));
#else
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(functionClicked(int)));
#endif
    connect(ui->functionOptionWidget, SIGNAL(buttonClicked(int)), SLOT(functionClicked(int)));
    //
    connect(m_musicLrcForInterior, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget, SLOT(changeInteriorLrcWidget()));
    connect(m_musicLrcForInterior, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(musicCurrentLrcUpdated()));
    connect(m_musicLrcForInterior, SIGNAL(backgroundChanged()), SIGNAL(updateBackgroundThemeDownload()));
    connect(m_musicLrcForInterior, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_musicLrcForInterior, SIGNAL(updateCurrentTime(qint64)), MusicApplication::instance(), SLOT(updateCurrentTime(qint64)));
    connect(ui->musicSongSearchEdit, SIGNAL(enterFinished(QString)), SLOT(musicSongSearchedFound(QString)));
}

void MusicRightAreaWidget::startDrawLrc() const
{
    if(checkSettingParameterValue())
    {
       m_musicLrcForInterior->startDrawLrc();
       m_musicLrcForDesktop->startDrawLrc();
       if(m_musicLrcForWallpaper)
       {
           m_musicLrcForWallpaper->startDrawLrc();
       }
    }
}

void MusicRightAreaWidget::stopDrawLrc() const
{
    if(checkSettingParameterValue())
    {
       m_musicLrcForInterior->stopDrawLrc();
       m_musicLrcForDesktop->stopDrawLrc();
       if(m_musicLrcForWallpaper)
       {
           m_musicLrcForWallpaper->stopDrawLrc();
       }
    }
}

void MusicRightAreaWidget::setCurrentPlayStatus(bool status) const
{
    m_musicLrcForDesktop->setCurrentPlayStatus(status);
}

bool MusicRightAreaWidget::destopLrcVisible() const
{
    return m_musicLrcForDesktop->isVisible();
}

void MusicRightAreaWidget::setInteriorLrcVisible(bool status) const
{
    m_musicLrcForInterior->setVisible(status);
}

bool MusicRightAreaWidget::interiorLrcVisible() const
{
    return m_musicLrcForInterior->isVisible();
}

bool MusicRightAreaWidget::checkSettingParameterValue() const
{
    return (G_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toBool() || G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool());
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const
{
    m_musicLrcForInterior->setCurrentTime(current, total);
    QString currentLrc, laterLrc;
    qint64 intervalTime;
    if(m_lrcAnalysis->findText(current, total, currentLrc, laterLrc, intervalTime))
    {
        //If this is a new line of the lyrics, then restart lyrics display mask
        if(currentLrc != m_musicLrcForInterior->text())
        {
            if(playStatus)
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
        m_musicLrcForInterior->stopDrawLrc();
        m_musicLrcForInterior->setCurrentSongName(name);

        MusicLrcAnalysis::State state;
        if(FILE_SUFFIX(QFileInfo(path)) == KRC_FILE_PREFIX)
        {
            TTK_LOGGER_INFO("Current in krc parser mode");
            state = m_lrcAnalysis->readFromKrcFile(path);
        }
        else
        {
            TTK_LOGGER_INFO("Current in lrc parser mode");
            state = m_lrcAnalysis->readFromLrcFile(path);
        }

        m_musicLrcForInterior->updateCurrentLrc(state);
        m_musicLrcForDesktop->stopDrawLrc();
        m_musicLrcForDesktop->setCurrentSongName(name);

        if(state == MusicLrcAnalysis::Failed)
        {
            m_musicLrcForDesktop->updateCurrentLrc(tr("No lrc data file found"), QString(), 0);
        }

        if(m_musicLrcForWallpaper)
        {
            m_musicLrcForWallpaper->stopDrawLrc();
            m_musicLrcForWallpaper->setCurrentSongName(name);
            m_musicLrcForWallpaper->start(true);

            if(state == MusicLrcAnalysis::Failed)
            {
                m_musicLrcForWallpaper->updateCurrentLrc(tr("No lrc data file found"));
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
    m_settingWidget->initialize();
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

    if(TTKObject_cast(MusicSimilarQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicSimilarQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicAlbumQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicAlbumQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicArtistQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicArtistQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicToplistQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicToplistQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicPlaylistQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicPlaylistQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicRecommendQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicRecommendQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicArtistListQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicArtistListQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicWebDJRadioWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicWebDJRadioWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicWebMVRadioQueryWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicWebMVRadioQueryWidget*, m_stackedFuncWidget)->resizeWindow();
    }
    else if(TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget)->resizeWindow();
    }

    if(TTKObject_cast(MusicCloudManagerWidget*, m_stackedAutoWidget))
    {
        TTKObject_cast(MusicCloudManagerWidget*, m_stackedAutoWidget)->resizeWindow();
    }

    if(m_videoPlayerWidget && !m_videoPlayerWidget->isPopupMode())
    {
        m_videoPlayerWidget->resizeWindow();
    }
}

void MusicRightAreaWidget::applySettingParameter()
{
    m_musicLrcForDesktop->applySettingParameter();
    m_musicLrcForInterior->applySettingParameter();
    if(m_musicLrcForWallpaper)
    {
        m_musicLrcForWallpaper->applySettingParameter();
    }

    bool config = G_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toBool();
    m_musicLrcForInterior->setVisible(config);
         config = G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool();
    m_musicLrcForDesktop->setVisible(config);
    m_ui->musicDesktopLrc->setChecked(config);

    config = G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool();
    if(config != m_lowPowerMode)
    {
        m_lowPowerMode = config;
        if(m_funcIndex == KugGouSongWidget || m_funcIndex == KugGouRadioWidget || m_funcIndex == kugouListWidget || m_funcIndex == kugouLiveWidget || m_funcIndex == KuiSheWidget)
        {
            functionClicked(m_funcIndex);
        }
    }

    if(TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget))
    {
        TTKObject_cast(MusicScreenSaverWidget*, m_stackedFuncWidget)->applySettingParameter();
    }
}

void MusicRightAreaWidget::functionClicked(int index, QWidget *widget)
{
    m_funcIndex = TTKStatic_cast(MusicFunction, index);
    functionParameterInit();

    if(widget)
    {
        m_stackedAutoWidget = widget;
        m_ui->functionsContainer->addWidget(m_stackedAutoWidget);
        m_ui->functionsContainer->setCurrentWidget(m_stackedAutoWidget);
        Q_EMIT updateBackgroundTheme();
        return;
    }

    if(0 <= index && index < LrcWidget)
    {
        m_ui->functionOptionWidget->musicButtonStyle(index);
    }

    switch(m_funcIndex)
    {
        case KugGouSongWidget: //insert kugou song widget
        {
            createkWindow(QKugouWindow::KuGouSong);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case KugGouRadioWidget: //insert kugou radio widget
        {
            createkWindow(QKugouWindow::KuGouRadio);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case kugouListWidget: //insert kugou list widget
        {
            createkWindow(QKugouWindow::KuGouList);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case VideoWidget: //insert video widget
        {
            if(!m_videoPlayerWidget)
            {
                m_videoPlayerWidget = new MusicVideoPlayWidget(this);
                connect(m_videoPlayerWidget, SIGNAL(popupButtonClicked(bool)), SLOT(musicVideoSetPopup(bool)));
                connect(m_videoPlayerWidget, SIGNAL(fullscreenButtonClicked(bool)), SLOT(musicVideoFullscreen(bool)));
            }
            m_videoPlayerWidget->popupMode(false);

            QWidget *widget = new QWidget(this);
            widget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle12);
            m_stackedFuncWidget = widget;
            m_ui->functionsContainer->addWidget(m_videoPlayerWidget);
            m_ui->functionsContainer->setCurrentWidget(m_videoPlayerWidget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case kugouLiveWidget: //insert kugou live widget
        {
            createkWindow(QKugouWindow::KugouMovie);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case LrcWidget: //insert lrc display widget
        {
            m_ui->functionsContainer->setCurrentIndex(1);
            m_ui->lrcDisplayAllButton->setStyleSheet(MusicUIObject::MQSSTinyBtnLrcCollapse);
            m_ui->lrcDisplayAllButton->setVisible(true);
            Q_EMIT updateBackgroundThemeDownload();
            break;
        }
        case SearchWidget: //insert search display widget
        {
            QString searchedString = m_ui->musicSongSearchEdit->text().trimmed();
                    searchedString = searchedString.isEmpty() ? m_ui->musicSongSearchEdit->placeholderText() : searchedString;
            //The string searched wouldn't allow to be none
            if(!searchedString.isEmpty() && searchedString != tr("Please input search words!"))
            {
                m_ui->musicSongSearchEdit->setText(searchedString);
                m_ui->songSearchWidget->startSearchQuery(searchedString, true);
            }
            else
            {
                functionClicked(MusicRightAreaWidget::KugGouSongWidget);
                MusicToastLabel::popup(tr("Please enter input search text first!"));
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
            MusicSimilarQueryWidget *widget = new MusicSimilarQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case AlbumWidget: //insert album found widget
        {
            MusicAlbumQueryWidget *widget = new MusicAlbumQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case ArtistWidget: //insert artist found widget
        {
            MusicArtistQueryWidget *widget = new MusicArtistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case ArtistCategoryWidget: //insert artist category found widget
        {
            MusicArtistListQueryWidget *widget = new MusicArtistListQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case ToplistWidget: //insert toplist found widget
        {
            MusicToplistQueryWidget *widget = new MusicToplistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case PlaylistWidget: //insert playlist found widget
        {
            MusicPlaylistQueryWidget *widget = new MusicPlaylistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case RecommendWidget: //insert recommend found widget
        {
            MusicRecommendQueryWidget *widget = new MusicRecommendQueryWidget(this);
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
            widget->queryIdentifyKey();
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case KuiSheWidget: //insert kugou kuishe widget
        {
            createkWindow(QKugouWindow::KuGouSingle);
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
            MusicWebMVRadioQueryWidget *widget = new MusicWebMVRadioQueryWidget(this);
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

void MusicRightAreaWidget::musicSongCommentsWidget()
{
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        MusicApplication::instance()->musicWindowConciseChanged();
    }

    if(m_ui->functionsContainer->currentIndex() != APP_WINDOW_INDEX_1)
    {
        functionClicked(MusicRightAreaWidget::LrcWidget);
    }

    m_musicLrcForInterior->showSongCommentsWidget();
}

void MusicRightAreaWidget::musicSimilarFound(const QString &text)
{
    functionClicked(MusicRightAreaWidget::SimilarWidget);
    TTKStatic_cast(MusicSimilarQueryWidget*, m_stackedFuncWidget)->setSongName(text);
}

void MusicRightAreaWidget::musicAlbumFound(const QString &text, const QString &id)
{
    functionClicked(MusicRightAreaWidget::AlbumWidget);
    MusicAlbumQueryWidget *w = TTKStatic_cast(MusicAlbumQueryWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicArtistCategoryFound()
{
    functionClicked(MusicRightAreaWidget::ArtistCategoryWidget);
    TTKStatic_cast(MusicArtistListQueryWidget*, m_stackedFuncWidget)->setSongName(QString());
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
    if(m_videoPlayerWidget && m_videoPlayerWidget->isPopupMode())
    {
        m_videoPlayerWidget->raise();
    }
    else
    {
        functionClicked(MusicRightAreaWidget::VideoWidget);
    }

    m_videoPlayerWidget->videoResearchButtonSearched(m_rawData);
}

void MusicRightAreaWidget::musicArtistFound(const QString &text, const QString &id)
{
    functionClicked(MusicRightAreaWidget::ArtistWidget);
    MusicArtistQueryWidget *w = TTKStatic_cast(MusicArtistQueryWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicToplistFound()
{
    functionClicked(MusicRightAreaWidget::ToplistWidget);
    TTKStatic_cast(MusicToplistQueryWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicPlaylistFound(const QString &id)
{
    functionClicked(MusicRightAreaWidget::PlaylistWidget);
    MusicPlaylistQueryWidget *w = TTKStatic_cast(MusicPlaylistQueryWidget*, m_stackedFuncWidget);
    id.isEmpty() ? w->setSongName(QString()) : w->setSongNameById(id);
}

void MusicRightAreaWidget::musicRecommendFound()
{
    functionClicked(MusicRightAreaWidget::RecommendWidget);
    TTKStatic_cast(MusicRecommendQueryWidget*, m_stackedFuncWidget)->setSongName(QString());
}

void MusicRightAreaWidget::musicAdvancedSearch()
{
    functionClicked(MusicRightAreaWidget::AdvancedSearchWidget);
}

void MusicRightAreaWidget::musicSongSearchedFound(const QString &text)
{
    m_ui->musicSongSearchEdit->setText(text.trimmed());
    functionClicked(MusicRightAreaWidget::SearchWidget);
}

void MusicRightAreaWidget::musicSingleSearchedFound(const QString &id)
{
    functionClicked(MusicRightAreaWidget::SearchSingleWidget);
    m_ui->songSearchWidget->startSearchSingleQuery(id);
}

void MusicRightAreaWidget::musicLoadSongIndexWidget()
{
    ///To prevent concise state changed while function musicWindowConciseChanged first called
    const bool pre = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, false);
    functionClicked(MusicRightAreaWidget::KugGouSongWidget);
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, pre);
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
    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, visible);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_musicLrcForDesktop->setWindowLockedChanged();
}

void MusicRightAreaWidget::setWindowLrcTypeChanged()
{
    const bool v = m_musicLrcForDesktop ? m_musicLrcForDesktop->isVerticalWindowType() : TTKStatic_cast(bool, G_SETTING_PTR->value(MusicSettingManager::DLrcWindowMode).toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometry, QPoint());

    MusicLrcContainerForDesktop *desktop = m_musicLrcForDesktop;
    if(v)
    {
        m_musicLrcForDesktop = new MusicLrcContainerHorizontalDesktop(this);
    }
    else
    {
        m_musicLrcForDesktop = new MusicLrcContainerVerticalDesktop(this);
    }
    m_musicLrcForDesktop->setLrcAnalysisModel(m_lrcAnalysis);

    if(desktop)
    {
        m_musicLrcForDesktop->makeStatusCopy(desktop);
        desktop->deleteLater();
    }

    m_musicLrcForDesktop->applySettingParameter();
    m_musicLrcForDesktop->initCurrentLrc();
    m_musicLrcForDesktop->setVisible(G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt());

    connect(m_musicLrcForDesktop, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(musicCurrentLrcUpdated()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorSetting()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_musicLrcForDesktop, SIGNAL(changeCurrentLrcColorCustom()), m_settingWidget, SLOT(changeDesktopLrcWidget()));

    G_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowMode, v);
}

void MusicRightAreaWidget::researchQueryByQuality(MusicObject::QueryQuality quality)
{
    const QString &text = m_ui->showCurrentSong->text().trimmed();
    if(text.isEmpty())
    {
        return;
    }

    m_funcIndex = MusicRightAreaWidget::SearchWidget;
    functionParameterInit();

    m_ui->songSearchWidget->researchQueryByQuality(text, quality);
    m_ui->functionsContainer->setCurrentIndex(APP_WINDOW_INDEX_0);
    Q_EMIT updateBackgroundTheme();
}

void MusicRightAreaWidget::musicVideoButtonSearched(const QString &name, const QString &id)
{
    if(m_videoPlayerWidget && m_videoPlayerWidget->isPopupMode())
    {
        m_videoPlayerWidget->raise();
    }
    else
    {
        functionClicked(MusicRightAreaWidget::VideoWidget);
    }

    id.isEmpty() ? m_videoPlayerWidget->videoResearchButtonSearched(name) : m_videoPlayerWidget->startSearchSingleQuery(id);
}

void MusicRightAreaWidget::musicVideoSetPopup(bool popup)
{
    if(!m_videoPlayerWidget)
    {
        return;
    }

    m_videoPlayerWidget->popupMode(popup);
    if(popup)
    {
        m_ui->functionsContainer->addWidget(m_stackedFuncWidget);
        m_ui->functionsContainer->setCurrentWidget(m_stackedFuncWidget);
#ifdef Q_OS_WIN
        MusicPlatformManager platform;
        platform.enabledLeftWinMode();
#endif
        QTimer::singleShot(10 * MT_MS, this, SLOT(musicVideoActiveWindow()));
    }
    else
    {
        functionClicked(MusicRightAreaWidget::VideoWidget);
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

    functionClicked(MusicRightAreaWidget::KugGouSongWidget);
    m_ui->functionOptionWidget->switchToSelectedItemStyle(MusicRightAreaWidget::KugGouSongWidget);
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

    const int height = m_musicLrcForInterior->height() - m_ui->lrcDisplayAllButton->height() - 40;
    QPropertyAnimation *lrcDisplayAllAnimation = new QPropertyAnimation(m_ui->lrcDisplayAllButton, "pos", this);
    lrcDisplayAllAnimation->setDuration(100);
    lrcDisplayAllAnimation->setStartValue(QPoint(lrcDisplayAll ? LEFT_SIDE_WIDTH_MIN - 20 : -LEFT_SIDE_WIDTH_MIN, height / 2));
    lrcDisplayAllAnimation->setEndValue(QPoint(0, height / 2));
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
        platform.enabledLeftWinMode();
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
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable, true);
}

void MusicRightAreaWidget::musicChangeDownloadCustumWidget()
{
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable, false);
    m_settingWidget->changeDownloadWidget();
    showSettingWidget();
}

void MusicRightAreaWidget::functionParameterInit()
{
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        MusicApplication::instance()->musicWindowConciseChanged();
    }

    if(m_funcIndex == LrcWidget) ///lrc option
    {
        m_ui->functionOptionWidget->musicButtonStyleClear(false);
        m_ui->stackedFunctionWidget->transparent(true);
    }
    else
    {
        m_ui->functionOptionWidget->musicButtonStyleClear(true);
        m_ui->stackedFunctionWidget->transparent(false);
    }

    deleteStackedFuncWidget();
    m_stackedAutoWidget = nullptr;

    m_ui->lrcDisplayAllButton->setVisible(false);
    if(m_musicLrcForInterior->lrcDisplayExpand() && m_funcIndex != LrcWidget)
    {
        musicLrcDisplayAllButtonClicked();
    }
}

void MusicRightAreaWidget::createkWindow(int type)
{
    QWidget *widget = nullptr;
    if(G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool())
    {
        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(MusicUIObject::MQSSBackgroundStyle12);
        label->setPixmap(QPixmap(":/image/lb_no_power_mode"));
        widget = label;
    }
    else
    {
        widget = new QKugouWindow(TTKStatic_cast(QKugouWindow::KuGouType, type), this);
        connect(m_ui->musicBackButton, SIGNAL(clicked()), widget, SLOT(goBack()));
        connect(m_ui->musicRefreshButton, SIGNAL(clicked()), widget, SLOT(refresh()));
    }

    m_ui->functionsContainer->addWidget(m_stackedFuncWidget = widget);
    m_ui->functionsContainer->setCurrentWidget(widget);
}
