#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musiclrccontainerfordesktop.h"
#include "musiclrccontainerforwallpaper.h"
#include "musicvideoplaywidget.h"
#include "musicdownloadstatusmodule.h"
#include "musicsettingwidget.h"
#include "musictoastlabel.h"
#include "musicfunctionuiobject.h"
#include "musictinyuiobject.h"
#include "musicfunctionlistuiobject.h"
#include "musictopareawidget.h"

#include "musicwebdjradiowidget.h"
#include "musicscreensaverwidget.h"
#include "musicidentifysongwidget.h"
#include "musicsimilarquerywidget.h"
#include "musicalbumquerywidget.h"
#include "musicartistquerywidget.h"
#include "musictoplistquerywidget.h"
#include "musicplaylistquerywidget.h"
#include "musicrecommendquerywidget.h"
#include "musicartistlistquerywidget.h"
#include "musicadvancedsearchedwidget.h"
#include "musicwebmvradioquerywidget.h"

#ifdef Q_OS_WIN
#  include "musicplatformmanager.h"
#endif
#include "qkugou/qkugouwindow.h"

#include <QPropertyAnimation>

MusicRightAreaWidget *MusicRightAreaWidget::m_instance = nullptr;

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent),
      m_lowPowerMode(false),
      m_funcIndex(KugGouSongWidget),
      m_stackedWidget(nullptr),
      m_stackedStandWidget(nullptr),
      m_videoPlayerWidget(nullptr),
      m_lrcForInterior(nullptr),
      m_lrcForDesktop(nullptr),
      m_lrcForWallpaper(nullptr)
{
    m_instance = this;

    m_lrcAnalysis = new MusicLrcAnalysis(this);
    m_lrcAnalysis->setLineMax(MUSIC_LRC_INTERIOR_MAX_LINE);

    m_downloadStatusObject = new MusicDownloadStatusModule(parent);
    m_settingWidget = new MusicSettingWidget(this);
    connect(m_settingWidget, SIGNAL(parameterSettingChanged()), parent, SLOT(applyParameter()));
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_settingWidget;
    delete m_downloadStatusObject;
    delete m_lrcForDesktop;
    delete m_lrcForWallpaper;

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

void MusicRightAreaWidget::setupUi(Ui::MusicApplication *ui)
{
    m_ui = ui;
    m_lrcForInterior = ui->musiclrccontainerforinterior;
    //
    m_lrcForInterior->setLrcAnalysisModel(m_lrcAnalysis);
    m_lrcForInterior->resize(ui->functionsContainer->size());

    ui->musicBackButton->setStyleSheet(TTK::UI::BtnBackBack);
    ui->musicRefreshButton->setStyleSheet(TTK::UI::BtnBackFresh);

    ui->lrcDisplayAllButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcDisplayAllButton->setIconSize(QSize(15, 56));
    connect(ui->lrcDisplayAllButton, SIGNAL(clicked()), SLOT(lrcDisplayAllClicked()));
    //
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->musicSearchButton, MusicRightAreaWidget::SearchWidget);
    buttonGroup->addButton(ui->musicWindowIdentify, MusicRightAreaWidget::IndentifyWidget);
    QtButtonGroupConnect(buttonGroup, this, functionClicked);
    //
    connect(ui->functionOptionWidget, SIGNAL(buttonClicked(int)), SLOT(functionClicked(int)));
    connect(m_lrcForInterior, SIGNAL(showCurrentLrcColorSetting()), m_settingWidget, SLOT(changeInteriorLrcWidget()));
    connect(m_lrcForInterior, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(currentLrcUpdated()));
    connect(m_lrcForInterior, SIGNAL(backgroundChanged()), SIGNAL(updateBackgroundThemeDownload()));
    connect(m_lrcForInterior, SIGNAL(showCurrentLrcSetting()), MusicApplication::instance(), SLOT(showSettingWidget()));
    connect(m_lrcForInterior, SIGNAL(updateCurrentTime(qint64)), MusicApplication::instance(), SLOT(updateCurrentTime(qint64)));
    connect(ui->musicSongSearchEdit, SIGNAL(enterFinished(QString)), SLOT(showSongSearchedFound(QString)));
}

void MusicRightAreaWidget::startDrawLrc() const
{
   m_lrcForInterior->startDrawLrc();
   m_lrcForDesktop->startDrawLrc();
   if(m_lrcForWallpaper)
   {
       m_lrcForWallpaper->startDrawLrc();
   }
}

void MusicRightAreaWidget::stopDrawLrc() const
{
    m_lrcForInterior->stopDrawLrc();
    m_lrcForDesktop->stopDrawLrc();
    if(m_lrcForWallpaper)
    {
        m_lrcForWallpaper->stopDrawLrc();
    }
}

void MusicRightAreaWidget::setCurrentPlayStatus(bool status) const
{
    m_lrcForDesktop->setCurrentPlayStatus(status);
}

bool MusicRightAreaWidget::destopLrcVisible() const
{
    return m_lrcForDesktop->isVisible();
}

void MusicRightAreaWidget::setInteriorLrcVisible(bool status) const
{
    m_lrcForInterior->setVisible(status);
}

bool MusicRightAreaWidget::interiorLrcVisible() const
{
    return m_lrcForInterior->isVisible();
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const
{
    m_lrcForInterior->setCurrentTime(current, total);
    QString currentLrc, laterLrc;
    qint64 intervalTime;
    if(m_lrcAnalysis->findText(current, total, currentLrc, laterLrc, intervalTime))
    {
        //If this is a new line of the lyrics, then restart lyrics display mask
        if(currentLrc != m_lrcForInterior->text())
        {
            if(playStatus)
            {
                m_lrcForInterior->updateCurrentLrc(intervalTime);
            }

            {
                m_lrcForDesktop->setCurrentTime(current, total);
                m_lrcForDesktop->updateCurrentLrc(currentLrc, laterLrc, intervalTime);
            }

            if(m_lrcForWallpaper)
            {
                m_lrcForWallpaper->setCurrentTime(current, total);
                m_lrcForWallpaper->updateCurrentLrc(intervalTime);
            }
        }
    }
}

void MusicRightAreaWidget::loadCurrentSongLrc(const QString &name, const QString &path) const
{
    m_lrcForInterior->stopDrawLrc();
    m_lrcForInterior->setCurrentSongName(name);

    MusicLrcAnalysis::State state;
    if(TTK_FILE_SUFFIX(QFileInfo(path)) == KRC_FILE_SUFFIX)
    {
        TTK_INFO_STREAM("Current in krc parser mode");
        state = m_lrcAnalysis->loadFromKrcFile(path);
    }
    else
    {
        TTK_INFO_STREAM("Current in lrc parser mode");
        state = m_lrcAnalysis->loadFromLrcFile(path);
    }

    m_lrcForInterior->updateCurrentLrc(state);
    m_lrcForDesktop->stopDrawLrc();
    m_lrcForDesktop->setCurrentSongName(name);

    if(state == MusicLrcAnalysis::State::Failed)
    {
        m_lrcForDesktop->updateCurrentLrc(tr("No lrc data file found"), {}, 0);
    }

    if(m_lrcForWallpaper)
    {
        m_lrcForWallpaper->stopDrawLrc();
        m_lrcForWallpaper->setCurrentSongName(name);
        m_lrcForWallpaper->start(true);

        if(state == MusicLrcAnalysis::State::Failed)
        {
            m_lrcForWallpaper->updateCurrentLrc(tr("No lrc data file found"));
        }
    }
}

void MusicRightAreaWidget::setSongTimeSpeed(qint64 time) const
{
    m_lrcForInterior->setSongTimeSpeed(time);
}

void MusicRightAreaWidget::checkMetaDataValid(bool mode) const
{
    m_downloadStatusObject->checkMetaDataValid(mode);
}

void MusicRightAreaWidget::showSettingWidget() const
{
    m_settingWidget->initialize();
    m_settingWidget->exec();
}

void MusicRightAreaWidget::artistSearchBy(const QString &id)
{
    m_rawData = id;
    TTK_SIGNLE_SHOT(showArtistSearchFound);
}

void MusicRightAreaWidget::albumSearchBy(const QString &id)
{
    m_rawData = id;
    TTK_SIGNLE_SHOT(showAlbumSearchFound);
}

void MusicRightAreaWidget::movieSearchBy(const QString &id)
{
    m_rawData = id;
    TTK_SIGNLE_SHOT(showMovieSearchFound);
}

void MusicRightAreaWidget::movieRadioSearchBy(const QVariant &data)
{
    m_rawData = data;
    TTK_SIGNLE_SHOT(showMovieSearchRadioFound);
}

void MusicRightAreaWidget::resizeWindow()
{
    m_ui->songSearchWidget->resizeWindow();
    m_lrcForInterior->resizeWindow();

    TTKAbstractResizeInterface *stackedWidget = TTKDynamicCast(TTKAbstractResizeInterface*, m_stackedWidget);
    if(stackedWidget)
    {
        stackedWidget->resizeWidget();
    }

    TTKAbstractResizeInterface *stackedStandWidget = TTKDynamicCast(TTKAbstractResizeInterface*, m_stackedStandWidget);
    if(stackedStandWidget)
    {
        stackedStandWidget->resizeWidget();
    }

    if(m_videoPlayerWidget && !m_videoPlayerWidget->isPopupMode())
    {
        m_videoPlayerWidget->resizeWindow();
    }
}

void MusicRightAreaWidget::applyParameter()
{
    m_lrcForDesktop->applyParameter();
    m_lrcForInterior->applyParameter();
    if(m_lrcForWallpaper)
    {
        m_lrcForWallpaper->applyParameter();
    }

    bool config = G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool();
    m_lrcForDesktop->setVisible(config);
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

    if(TTKObjectCast(MusicScreenSaverWidget*, m_stackedWidget))
    {
        TTKObjectCast(MusicScreenSaverWidget*, m_stackedWidget)->applyParameter();
    }
}

void MusicRightAreaWidget::functionClicked(int index, QWidget *widget)
{
    m_funcIndex = TTKStaticCast(FunctionModule, index);
    functionInitialize();

    if(widget)
    {
        m_stackedStandWidget = widget;
        m_ui->functionsContainer->addWidget(m_stackedStandWidget);
        m_ui->functionsContainer->setCurrentWidget(m_stackedStandWidget);
        Q_EMIT updateBackgroundTheme();
        return;
    }

    if(0 <= index && index < LrcWidget)
    {
        m_ui->functionOptionWidget->buttonStyleSwitch(index);
    }

    switch(m_funcIndex)
    {
        case KugGouSongWidget: //insert kugou song widget
        {
            createkWebWindow(QKugouWindow::KuGouSong);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case KugGouRadioWidget: //insert kugou radio widget
        {
            createkWebWindow(QKugouWindow::KuGouRadio);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case kugouListWidget: //insert kugou list widget
        {
            createkWebWindow(QKugouWindow::KuGouList);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case VideoWidget: //insert video widget
        {
            if(!m_videoPlayerWidget)
            {
                m_videoPlayerWidget = new MusicVideoPlayWidget(this);
                connect(m_videoPlayerWidget, SIGNAL(popupButtonClicked(bool)), SLOT(videoSetPopup(bool)));
                connect(m_videoPlayerWidget, SIGNAL(fullscreenButtonClicked(bool)), SLOT(videoFullscreen(bool)));
            }
            m_videoPlayerWidget->popupMode(false);

            QWidget *widget = new QWidget(this);
            widget->setStyleSheet(TTK::UI::BackgroundStyle10);
            m_stackedWidget = widget;
            m_ui->functionsContainer->addWidget(m_videoPlayerWidget);
            m_ui->functionsContainer->setCurrentWidget(m_videoPlayerWidget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case kugouLiveWidget: //insert kugou live widget
        {
            createkWebWindow(QKugouWindow::KugouMovie);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case LrcWidget: //insert lrc display widget
        {
            m_ui->functionsContainer->setCurrentIndex(MUSIC_LRC_PAGE);
            m_ui->lrcDisplayAllButton->setStyleSheet(TTK::UI::TinyBtnLrcCollapse);
            m_ui->lrcDisplayAllButton->setVisible(true);
            Q_EMIT updateBackgroundThemeDownload();
            break;
        }
        case SearchWidget: //insert search display widget
        {
            QString searchedString = m_ui->musicSongSearchEdit->text().trimmed();
                    searchedString = searchedString.isEmpty() ? m_ui->musicSongSearchEdit->placeholderText() : searchedString;
            //The string searched wouldn't allow to be none
            if(!searchedString.isEmpty() && searchedString != tr("Please input search words"))
            {
                m_ui->musicSongSearchEdit->setText(searchedString);
                m_ui->songSearchWidget->startSearchQuery(searchedString);
            }
            else
            {
                functionClicked(MusicRightAreaWidget::KugGouSongWidget);
                MusicToastLabel::popup(tr("Please enter input search text first"));
                break;
            }

            m_ui->functionsContainer->setCurrentIndex(MUSIC_SEARCH_PAGE);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case SearchSingleWidget: //insert search display widget
        {
            m_ui->functionsContainer->setCurrentIndex(MUSIC_SEARCH_PAGE);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case SimilarWidget: //insert similar found widget
        {
            MusicSimilarQueryWidget *widget = new MusicSimilarQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case AlbumWidget: //insert album found widget
        {
            MusicAlbumQueryWidget *widget = new MusicAlbumQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case ArtistWidget: //insert artist found widget
        {
            MusicArtistQueryWidget *widget = new MusicArtistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case ArtistCategoryWidget: //insert artist category found widget
        {
            MusicArtistListQueryWidget *widget = new MusicArtistListQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case ToplistWidget: //insert toplist found widget
        {
            MusicToplistQueryWidget *widget = new MusicToplistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case PlaylistWidget: //insert playlist found widget
        {
            MusicPlaylistQueryWidget *widget = new MusicPlaylistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case RecommendWidget: //insert recommend found widget
        {
            MusicRecommendQueryWidget *widget = new MusicRecommendQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case AdvancedSearchWidget: //insert advanced search widget
        {
            MusicAdvancedSearchedWidget *widget = new MusicAdvancedSearchedWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case IndentifyWidget: //insert indentify songs widget
        {
            MusicIdentifySongWidget *widget = new MusicIdentifySongWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->queryIdentifyKey();
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case KuiSheWidget: //insert kugou kuishe widget
        {
            createkWebWindow(QKugouWindow::KuGouSingle);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case WebDJRadioWidget: //insert web dj radio widget
        {
            MusicWebDJRadioWidget *widget = new MusicWebDJRadioWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->initialize();
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case WebMVRadioWidget: //insert web mv radio widget
        {
            MusicWebMVRadioQueryWidget *widget = new MusicWebMVRadioQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->setSongName({});
            Q_EMIT updateBackgroundTheme();
            break;
        }
        case ScreenSaverWidget: //insert screen saver widget
        {
            MusicScreenSaverWidget *widget = new MusicScreenSaverWidget(this);
            m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            Q_EMIT updateBackgroundTheme();
            break;
        }
        default: break;
    }
}

void MusicRightAreaWidget::showSongCommentsWidget()
{
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        MusicApplication::instance()->windowConciseChanged();
    }

    if(m_ui->functionsContainer->currentIndex() != MUSIC_LRC_PAGE)
    {
        functionClicked(MusicRightAreaWidget::LrcWidget);
    }

    m_lrcForInterior->showSongCommentsWidget();
}

void MusicRightAreaWidget::showSimilarFound(const QString &text)
{
    functionClicked(MusicRightAreaWidget::SimilarWidget);
    TTKObjectCast(MusicSimilarQueryWidget*, m_stackedWidget)->setSongName(text);
}

void MusicRightAreaWidget::showAlbumFound(const QString &text, const QString &id)
{
    functionClicked(MusicRightAreaWidget::AlbumWidget);
    MusicAlbumQueryWidget *w = TTKObjectCast(MusicAlbumQueryWidget*, m_stackedWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameByID(id);
}

void MusicRightAreaWidget::showArtistCategoryFound()
{
    functionClicked(MusicRightAreaWidget::ArtistCategoryWidget);
    TTKObjectCast(MusicArtistListQueryWidget*, m_stackedWidget)->setSongName({});
}

void MusicRightAreaWidget::showArtistSearchFound()
{
    showArtistFound({}, m_rawData.toString());
}

void MusicRightAreaWidget::showAlbumSearchFound()
{
    showAlbumFound({}, m_rawData.toString());
}

void MusicRightAreaWidget::showMovieSearchFound()
{
    showVideoSearchedFound({}, m_rawData.toString());
}

void MusicRightAreaWidget::showMovieSearchRadioFound()
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

void MusicRightAreaWidget::showArtistFound(const QString &text, const QString &id)
{
    functionClicked(MusicRightAreaWidget::ArtistWidget);
    MusicArtistQueryWidget *w = TTKObjectCast(MusicArtistQueryWidget*, m_stackedWidget);
    id.isEmpty() ? w->setSongName(text) : w->setSongNameByID(id);
}

void MusicRightAreaWidget::showToplistFound()
{
    functionClicked(MusicRightAreaWidget::ToplistWidget);
    TTKObjectCast(MusicToplistQueryWidget*, m_stackedWidget)->setSongName({});
}

void MusicRightAreaWidget::showPlaylistFound(const QString &id)
{
    functionClicked(MusicRightAreaWidget::PlaylistWidget);
    MusicPlaylistQueryWidget *w = TTKObjectCast(MusicPlaylistQueryWidget*, m_stackedWidget);
    id.isEmpty() ? w->setSongName({}) : w->setSongNameByID(id);
}

void MusicRightAreaWidget::showRecommendFound()
{
    functionClicked(MusicRightAreaWidget::RecommendWidget);
    TTKObjectCast(MusicRecommendQueryWidget*, m_stackedWidget)->setSongName({});
}

void MusicRightAreaWidget::showAdvancedSearchFound()
{
    functionClicked(MusicRightAreaWidget::AdvancedSearchWidget);
}

void MusicRightAreaWidget::showSongSearchedFound(const QString &text)
{
    m_ui->musicSongSearchEdit->setText(text.trimmed());
    functionClicked(MusicRightAreaWidget::SearchWidget);
}

void MusicRightAreaWidget::showSingleSearchedFound(const QString &id)
{
    functionClicked(MusicRightAreaWidget::SearchSingleWidget);
    m_ui->songSearchWidget->startSearchSingleQuery(id);
}

void MusicRightAreaWidget::showVideoSearchedFound(const QString &name, const QString &id)
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

void MusicRightAreaWidget::showSongMainWidget()
{
    ///To prevent concise state changed while function windowConciseChanged first called
    const bool pre = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, false);
    functionClicked(MusicRightAreaWidget::KugGouSongWidget);
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, pre);
}

void MusicRightAreaWidget::deleteStackedFuncWidget()
{
    delete m_stackedWidget;
    m_stackedWidget = nullptr;
}

void MusicRightAreaWidget::setDestopLrcVisible(bool visible) const
{
    m_ui->musicDesktopLrc->setChecked(visible);
    m_lrcForDesktop->setVisible(visible);
    m_lrcForDesktop->initCurrentLrc();
    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, visible);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_lrcForDesktop->setWindowLockedChanged();
}

void MusicRightAreaWidget::setWindowLrcTypeChanged()
{
    const bool type = m_lrcForDesktop ? m_lrcForDesktop->isVerticalWindowType() : TTKStaticCast(bool, G_SETTING_PTR->value(MusicSettingManager::DLrcWindowMode).toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometry, QPoint());

    MusicLrcContainerForDesktop *desktop = m_lrcForDesktop;
    if(type)
    {
        m_lrcForDesktop = new MusicLrcContainerHorizontalDesktop;
    }
    else
    {
        m_lrcForDesktop = new MusicLrcContainerVerticalDesktop;
    }
    m_lrcForDesktop->setLrcAnalysisModel(m_lrcAnalysis);

    if(desktop)
    {
        m_lrcForDesktop->statusCopyFrom(desktop);
        desktop->deleteLater();
    }

    m_lrcForDesktop->applyParameter();
    m_lrcForDesktop->initCurrentLrc();
    m_lrcForDesktop->setVisible(G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt());

    connect(m_lrcForDesktop, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(currentLrcUpdated()));
    connect(m_lrcForDesktop, SIGNAL(showCurrentLrcSetting()), MusicApplication::instance(), SLOT(showSettingWidget()));
    connect(m_lrcForDesktop, SIGNAL(showCurrentLrcColorSetting()), m_settingWidget, SLOT(changeDesktopLrcWidget()));

    G_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowMode, type);
}

void MusicRightAreaWidget::videoSetPopup(bool popup)
{
    if(!m_videoPlayerWidget)
    {
        return;
    }

    m_videoPlayerWidget->popupMode(popup);
    if(popup)
    {
        m_ui->functionsContainer->addWidget(m_stackedWidget);
        m_ui->functionsContainer->setCurrentWidget(m_stackedWidget);
#ifdef Q_OS_WIN
        MusicPlatformManager manager;
        manager.windowsEnableLeftMode();
#endif
        TTK_SIGNLE_SHOT(videoActiveWindow);
    }
    else
    {
        functionClicked(MusicRightAreaWidget::VideoWidget);
    }
}

void MusicRightAreaWidget::videoActiveWindow()
{
    if(m_videoPlayerWidget)
    {
        MusicApplication::instance()->activateWindow();
        m_videoPlayerWidget->activateWindow();
    }
}

void MusicRightAreaWidget::videoClosed()
{
    delete m_videoPlayerWidget;
    m_videoPlayerWidget = nullptr;

    functionClicked(MusicRightAreaWidget::KugGouSongWidget);
    m_ui->functionOptionWidget->switchToSelectedItemStyle(MusicRightAreaWidget::KugGouSongWidget);
}

void MusicRightAreaWidget::videoFullscreen(bool full)
{
    if(!m_videoPlayerWidget)
    {
        return;
    }

    m_videoPlayerWidget->resizeGeometry(full);
    m_videoPlayerWidget->blockMoveOption(full);
}

void MusicRightAreaWidget::lrcDisplayAllClicked()
{
    const bool lrcDisplayAll = !m_lrcForInterior->lrcDisplayExpand();
    m_lrcForInterior->setLrcDisplayExpand(lrcDisplayAll);
    m_ui->centerLeftWidget->setVisible(!lrcDisplayAll);

    const int height = m_lrcForInterior->height() - m_ui->lrcDisplayAllButton->height() - 40;
    QPropertyAnimation *lrcDisplayAllAnimation = new QPropertyAnimation(m_ui->lrcDisplayAllButton, "pos", this);
    lrcDisplayAllAnimation->setDuration(100);
    lrcDisplayAllAnimation->setStartValue(QPoint(lrcDisplayAll ? LEFT_SIDE_WIDTH_MIN - 20 : -LEFT_SIDE_WIDTH_MIN, height / 2));
    lrcDisplayAllAnimation->setEndValue(QPoint(0, height / 2));
    lrcDisplayAllAnimation->start();

    m_ui->lrcDisplayAllButton->setStyleSheet(lrcDisplayAll ? TTK::UI::TinyBtnLrcExpand : TTK::UI::TinyBtnLrcCollapse);
    m_ui->musicWindowConcise->setEnabled(!lrcDisplayAll);
}

void MusicRightAreaWidget::containerForWallpaperClicked()
{
    if(m_lrcForWallpaper)
    {
        MusicTopAreaWidget::instance()->showWallpaperRemote(false);
        delete m_lrcForWallpaper;
        m_lrcForWallpaper = nullptr;
    }
    else
    {
#ifdef Q_OS_WIN
        MusicPlatformManager manager;
        manager.windowsEnableLeftMode();
#endif
        m_lrcForWallpaper = new MusicLrcContainerForWallpaper;
        m_lrcForWallpaper->setLrcAnalysisModel(m_lrcAnalysis);
        m_lrcForWallpaper->applyParameter();
        m_lrcForWallpaper->showFullScreen();
        connect(m_lrcForInterior, SIGNAL(linearGradientColorChanged()), m_lrcForWallpaper, SLOT(changeCurrentLrcColor()));

        MusicApplication::instance()->activateWindow();
        MusicApplication::instance()->showMinimized();
        MusicTopAreaWidget::instance()->showWallpaperRemote(true);
    }
}

void MusicRightAreaWidget::changeDownloadFulllyWidget()
{
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable, true);
}

void MusicRightAreaWidget::changeDownloadCustumWidget()
{
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable, false);
    m_settingWidget->changeDownloadWidget();
    showSettingWidget();
}

void MusicRightAreaWidget::functionInitialize()
{
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        MusicApplication::instance()->windowConciseChanged();
    }

    if(m_funcIndex == LrcWidget) ///lrc option
    {
        m_ui->functionOptionWidget->buttonStyleClear(false);
        m_ui->stackedFunctionWidget->transparent(true);
    }
    else
    {
        m_ui->functionOptionWidget->buttonStyleClear(true);
        m_ui->stackedFunctionWidget->transparent(false);
    }

//    deleteStackedFuncWidget();
    m_stackedStandWidget = nullptr;

    m_ui->lrcDisplayAllButton->setVisible(false);
    if(m_lrcForInterior->lrcDisplayExpand() && m_funcIndex != LrcWidget)
    {
        lrcDisplayAllClicked();
    }
}

void MusicRightAreaWidget::createkWebWindow(int type)
{
    QWidget *widget = nullptr;
    if(G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool())
    {
        QLabel *label = new QLabel(this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(TTK::UI::BackgroundStyle10);
        label->setPixmap(QPixmap(":/image/lb_no_power_mode"));
        widget = label;
    }
    else
    {
        widget = new QKugouWindow(TTKStaticCast(QKugouWindow::Module, type), this);
        connect(m_ui->musicBackButton, SIGNAL(clicked()), widget, SLOT(goBack()));
        connect(m_ui->musicRefreshButton, SIGNAL(clicked()), widget, SLOT(refresh()));
    }

    m_ui->functionsContainer->addWidget(m_stackedWidget = widget);
    m_ui->functionsContainer->setCurrentWidget(widget);
}
