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
#if !TTK_QT_VERSION_CHECK(6,0,0) || TTK_QT_VERSION_CHECK(6,2,0)
#  include "musicidentifysongwidget.h"
#endif
#include "musicmainrecommendwidget.h"
#include "musicsimilarquerywidget.h"
#include "musicalbumquerywidget.h"
#include "musicartistquerywidget.h"
#include "musictoplistquerywidget.h"
#include "musicplaylistquerywidget.h"
#include "musicrecommendquerywidget.h"
#include "musicartistlistquerywidget.h"
#include "musicadvancedsearchedwidget.h"
#include "musicwebmvradioquerywidget.h"
#include "musicplaylistbackupwidget.h"
#include "musicplaylistcategorywidget.h"
#include "musicpersonalradioquerywidget.h"
#include "musicsongdailywidget.h"

#ifdef Q_OS_WIN
#  include "musicplatformmanager.h"
#endif
#include "qkugou/qkugouwindow.h"

#include <QPropertyAnimation>

static constexpr int MAX_CONTAINER_SIZE = 10;
static constexpr int DEFAULT_CONTAINER_SIZE = 3;

MusicRightAreaWidget *MusicRightAreaWidget::m_instance = nullptr;

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent),
      m_lowPowerMode(false),
      m_modeIndex(MainRecommendWidget),
      m_lastModeIndex(m_modeIndex),
      m_lastWidgetIndex(0),
      m_currentWidget(nullptr),
      m_videoWidget(nullptr),
      m_interiorLrc(nullptr),
      m_desktopLrc(nullptr),
      m_wallpaperLrc(nullptr)
{
    m_instance = this;

    m_lrcAnalysis = new MusicLrcAnalysis(this);
    m_lrcAnalysis->setMaximum(MUSIC_LRC_INTERIOR_MAX_LINE);

    m_downloadStatus = new MusicDownloadStatusModule(parent);
    m_settingWidget = new MusicSettingWidget(this);
    connect(m_settingWidget, SIGNAL(parameterSettingChanged()), parent, SLOT(applyParameter()));
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_settingWidget;
    delete m_downloadStatus;
    delete m_desktopLrc;
    delete m_wallpaperLrc;

    if(m_videoWidget)
    {
        m_videoWidget->setVisible(false); //Fix bug on linux
        delete m_videoWidget;
    }
}

MusicRightAreaWidget *MusicRightAreaWidget::instance()
{
    return m_instance;
}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication *ui)
{
    m_ui = ui;
    //
    m_interiorLrc = ui->lrcForInterior;
    m_interiorLrc->setLrcAnalysisModel(m_lrcAnalysis);
    m_interiorLrc->resize(ui->functionsContainer->size());

    ui->backButton->setStyleSheet(TTK::UI::BtnBackBack);
    ui->refreshButton->setStyleSheet(TTK::UI::BtnBackFresh);

    ui->lrcDisplayAllButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcDisplayAllButton->setIconSize(QSize(15, 56));
    connect(ui->lrcDisplayAllButton, SIGNAL(clicked()), SLOT(lrcDisplayAllClicked()));
    //
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->songSearchButton, MusicRightAreaWidget::SearchWidget);
    buttonGroup->addButton(ui->windowIdentify, MusicRightAreaWidget::IndentifyWidget);
    QtButtonGroupConnect(buttonGroup, this, functionClicked, TTK_SLOT);
    //
    connect(m_interiorLrc, SIGNAL(showCurrentLrcColorSetting()), m_settingWidget, SLOT(changeInteriorLrcWidget()));
    connect(m_interiorLrc, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(currentLrcUpdated()));
    connect(m_interiorLrc, SIGNAL(backgroundChanged()), MusicTopAreaWidget::instance(), SLOT(backgroundThemeDownloadFinished()));
    connect(m_interiorLrc, SIGNAL(showCurrentLrcSetting()), MusicApplication::instance(), SLOT(showSettingWidget()));
    connect(m_interiorLrc, SIGNAL(updateCurrentTime(qint64)), MusicApplication::instance(), SLOT(updateCurrentTime(qint64)));
    connect(ui->backButton, SIGNAL(clicked()), SLOT(functionGoBack()));
    connect(ui->functionOptionWidget, SIGNAL(buttonClicked(int)), SLOT(functionClicked(int)));
    connect(ui->songSearchEdit, SIGNAL(enterFinished(QString)), SLOT(showSongSearchedFound(QString)));
}

void MusicRightAreaWidget::startDrawLrc() const
{
   m_interiorLrc->start();
   m_desktopLrc->start();
   if(m_wallpaperLrc)
   {
       m_wallpaperLrc->start();
   }
}

void MusicRightAreaWidget::stopDrawLrc() const
{
    m_interiorLrc->stop();
    m_desktopLrc->stop();
    if(m_wallpaperLrc)
    {
        m_wallpaperLrc->stop();
    }
}

void MusicRightAreaWidget::setCurrentPlayState(bool state) const
{
    m_desktopLrc->setCurrentPlayState(state);
}

bool MusicRightAreaWidget::destopLrcVisible() const
{
    return m_desktopLrc->isVisible();
}

void MusicRightAreaWidget::setInteriorLrcVisible(bool status) const
{
    m_interiorLrc->setVisible(status);
}

bool MusicRightAreaWidget::interiorLrcVisible() const
{
    return m_interiorLrc->isVisible();
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playState) const
{
    m_interiorLrc->setCurrentTime(current, total);
    QString currentLrc, laterLrc;
    qint64 intervalTime;
    if(m_lrcAnalysis->findText(current, total, currentLrc, laterLrc, intervalTime))
    {
        //If this is a new line of the lyrics, then restart lyrics display mask
        if(currentLrc != m_interiorLrc->text())
        {
            if(playState)
            {
                m_interiorLrc->updateCurrentLrc(intervalTime);
            }

            {
                m_desktopLrc->setCurrentTime(current, total);
                m_desktopLrc->updateCurrentLrc(currentLrc, laterLrc, intervalTime);
            }

            if(m_wallpaperLrc)
            {
                m_wallpaperLrc->setCurrentTime(current, total);
                m_wallpaperLrc->updateCurrentLrc(intervalTime);
            }
        }
    }
}

void MusicRightAreaWidget::loadCurrentSongLrc(const QString &name, const QString &path) const
{
    m_interiorLrc->stop();
    m_interiorLrc->setCurrentSongName(name);

    const MusicLrcAnalysis::State state = m_lrcAnalysis->loadFromFile(path);

    m_interiorLrc->updateCurrentLrc(state);
    m_desktopLrc->stop();
    m_desktopLrc->setCurrentSongName(name);

    if(state == MusicLrcAnalysis::State::Failed)
    {
        m_desktopLrc->updateCurrentLrc(tr("No lrc data file found"), {}, 0);
    }

    if(m_wallpaperLrc)
    {
        m_wallpaperLrc->stop();
        m_wallpaperLrc->setCurrentSongName(name);
        m_wallpaperLrc->render(true);

        if(state == MusicLrcAnalysis::State::Failed)
        {
            m_wallpaperLrc->updateCurrentLrc(tr("No lrc data file found"));
        }
    }
}

void MusicRightAreaWidget::findTimePosition(qint64 time) const
{
    m_interiorLrc->findTimePosition(time);
}

void MusicRightAreaWidget::checkMetaDataValid(bool mode) const
{
    m_downloadStatus->checkMetaDataValid(mode);
}

void MusicRightAreaWidget::showSettingWidget() const
{
    m_settingWidget->initialize();
    m_settingWidget->exec();
}

void MusicRightAreaWidget::applyParameter()
{
    m_desktopLrc->applyParameter();
    m_interiorLrc->applyParameter();
    if(m_wallpaperLrc)
    {
        m_wallpaperLrc->applyParameter();
    }

    bool config = G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool();
    m_desktopLrc->setVisible(config);
    m_ui->desktopLrcButton->setChecked(config);

    config = G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool();
    if(config != m_lowPowerMode)
    {
        m_lowPowerMode = config;
        if(m_modeIndex == KugouRadioWidget || m_modeIndex == KugouMovieWidget || m_modeIndex == KugouKuiSheWidget)
        {
            functionClicked(m_modeIndex);
        }
    }

    if(TTKObjectCast(MusicScreenSaverWidget*, m_currentWidget))
    {
        TTKObjectCast(MusicScreenSaverWidget*, m_currentWidget)->applyParameter();
    }
}

void MusicRightAreaWidget::resizeWidgetGeometry()
{
    m_ui->songSearchWidget->resizeGeometry();
    m_interiorLrc->resizeGeometry();

    TTKAbstractResizeInterface *currentWidget = TTKDynamicCast(TTKAbstractResizeInterface*, m_currentWidget);
    if(currentWidget)
    {
        currentWidget->resizeGeometry();
    }

    if(m_videoWidget && !m_videoWidget->isPopupMode())
    {
        m_videoWidget->resizeGeometry();
    }
}

void MusicRightAreaWidget::functionGoBack()
{
    if(m_ui->functionsContainer->count() <= DEFAULT_CONTAINER_SIZE)
    {
        return;
    }

    m_modeIndex = m_lastModeIndex;
    const int index = TTKStaticCast(int, m_modeIndex);

    if(KugouRadioWidget <= index && index <= LrcWidget)
    {
        if(index != PlaylistWidget)
        {
            m_ui->functionOptionWidget->switchToSelectedItemStyle(index);
        }
        else
        {
            disconnect(m_ui->functionOptionWidget, SIGNAL(buttonClicked(int)), this, SLOT(functionClicked(int)));
            m_ui->functionOptionWidget->switchToSelectedItemStyle(PlaylistWidget);
            connect(m_ui->functionOptionWidget, SIGNAL(buttonClicked(int)), this, SLOT(functionClicked(int)));

            functionInitialize();
            m_ui->functionOptionWidget->select(PlaylistWidget);
            m_ui->functionsContainer->setCurrentIndex(m_lastWidgetIndex);
            m_currentWidget = m_ui->functionsContainer->currentWidget();
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
        }
    }
    else
    {
        functionInitialize();
        m_ui->functionsContainer->setCurrentIndex(m_lastWidgetIndex);
        m_currentWidget = m_ui->functionsContainer->currentWidget();
        MusicTopAreaWidget::instance()->backgroundTransparentChanged();
    }

    m_ui->backButton->setEnabled(false);
}

void MusicRightAreaWidget::functionClicked(int index, QWidget *widget)
{
    m_lastModeIndex = m_modeIndex;
    m_lastWidgetIndex = m_ui->functionsContainer->currentIndex();
    m_modeIndex = TTKStaticCast(FunctionMode, index);

    functionCleanup();
    functionInitialize();

    if(widget)
    {
        m_currentWidget = widget;
        m_ui->functionsContainer->addWidget(m_currentWidget);
        m_ui->functionsContainer->setCurrentWidget(m_currentWidget);
        MusicTopAreaWidget::instance()->backgroundTransparentChanged();
        return;
    }

    if(MainRecommendWidget <= index && index < LrcWidget)
    {
        m_ui->functionOptionWidget->select(index);
    }

    switch(m_modeIndex)
    {
        case MainRecommendWidget: //insert main recommend widget
        {
            MusicMainRecommendWidget *widget = new MusicMainRecommendWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case KugouRadioWidget: //insert kugou radio widget
        {
            createkWebWindow(QKugouWindow::Radio);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case PlaylistWidget: //insert playlist widget
        {
            MusicPlaylistQueryWidget *widget = new MusicPlaylistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->setCurrentValue({});
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case KugouMovieWidget: //insert kugou movie widget
        {
            createkWebWindow(QKugouWindow::Movie);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case VideoWidget: //insert video widget
        {
            if(!m_videoWidget)
            {
                m_videoWidget = new MusicVideoPlayWidget(this);
                connect(m_videoWidget, SIGNAL(popupButtonClicked(bool)), SLOT(videoSetPopup(bool)));
                connect(m_videoWidget, SIGNAL(fullscreenButtonClicked(bool)), SLOT(videoFullscreen(bool)));
            }

            m_videoWidget->popupMode(false);
            m_ui->functionsContainer->addWidget(m_videoWidget);
            m_ui->functionsContainer->setCurrentWidget(m_videoWidget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case LrcWidget: //insert lrc display widget
        {
            m_ui->functionsContainer->setCurrentIndex(MUSIC_LRC_PAGE);
            m_ui->lrcDisplayAllButton->setStyleSheet(TTK::UI::TinyBtnLrcCollapse);
            m_ui->lrcDisplayAllButton->setVisible(true);
            MusicTopAreaWidget::instance()->backgroundThemeDownloadFinished();
            break;
        }
        case SongRecommendWidget: //insert song recommend found widget
        {
            MusicRecommendQueryWidget *widget = new MusicRecommendQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->setCurrentValue({});
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case ToplistWidget: //insert toplist found widget
        {
            MusicToplistQueryWidget *widget = new MusicToplistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->setCurrentValue({});
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case ArtistCategoryWidget: //insert artist category found widget
        {
            MusicArtistListQueryWidget *widget = new MusicArtistListQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->setCurrentValue({});
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case PlaylistCategoryWidget: //insert playlist category widget
        {
            MusicPlaylistCategoryWidget *widget = new MusicPlaylistCategoryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case SearchWidget: //insert search display widget
        {
            QString searchedString = m_ui->songSearchEdit->text().trimmed();
                    searchedString = searchedString.isEmpty() ? m_ui->songSearchEdit->placeholderText() : searchedString;
            //The string searched wouldn't allow to be none
            if(!searchedString.isEmpty() && searchedString != tr("Please input search words"))
            {
                m_ui->songSearchEdit->setText(searchedString);
                m_ui->songSearchWidget->startToSearchByValue(searchedString);
            }
            else
            {
                functionClicked(MusicRightAreaWidget::MainRecommendWidget);
                MusicToastLabel::popup(tr("Please enter input search text first"));
                break;
            }

            m_ui->functionsContainer->setCurrentIndex(MUSIC_SEARCH_PAGE);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case SearchSingleWidget: //insert search display widget
        {
            m_ui->functionsContainer->setCurrentIndex(MUSIC_SEARCH_PAGE);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case SimilarWidget: //insert similar found widget
        {
            MusicSimilarQueryWidget *widget = new MusicSimilarQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case AlbumWidget: //insert album found widget
        {
            MusicAlbumQueryWidget *widget = new MusicAlbumQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case ArtistWidget: //insert artist found widget
        {
            MusicArtistQueryWidget *widget = new MusicArtistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case PlaylistInfoWidget: //insert playlist found widget
        {
            MusicPlaylistQueryWidget *widget = new MusicPlaylistQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case SongDailyWidget: //insert song daily widget
        {
            MusicSongDailyWidget *widget = new MusicSongDailyWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case AdvancedSearchWidget: //insert advanced search widget
        {
            MusicAdvancedSearchedWidget *widget = new MusicAdvancedSearchedWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case IndentifyWidget: //insert indentify songs widget
        {
#if !TTK_QT_VERSION_CHECK(6,0,0) || TTK_QT_VERSION_CHECK(6,2,0)
            MusicIdentifySongWidget *widget = new MusicIdentifySongWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->initialize();
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
#else
            MusicToastLabel::popup(tr("Unsupported identify song module"));
#endif
            break;
        }
        case KugouKuiSheWidget: //insert kugou kuishe widget
        {
            createkWebWindow(QKugouWindow::Single);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case WebDJRadioWidget: //insert web dj radio widget
        {
            MusicWebDJRadioWidget *widget = new MusicWebDJRadioWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->initialize();
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case WebMVRadioWidget: //insert web mv radio widget
        {
            MusicWebMVRadioQueryWidget *widget = new MusicWebMVRadioQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            widget->setCurrentValue({});
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case WebPVRadioWidget: //insert web pv radio widget
        {
            MusicPersonalRadioQueryWidget *widget = new MusicPersonalRadioQueryWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case ScreenSaverWidget: //insert screen saver widget
        {
            MusicScreenSaverWidget *widget = new MusicScreenSaverWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        case PlaylistBackupWidget: //insert playlist backup widget
        {
            MusicPlaylistBackupWidget *widget = new MusicPlaylistBackupWidget(this);
            m_ui->functionsContainer->addWidget(m_currentWidget = widget);
            m_ui->functionsContainer->setCurrentWidget(widget);
            MusicTopAreaWidget::instance()->backgroundTransparentChanged();
            break;
        }
        default: break;
    }

    m_ui->backButton->setEnabled(m_ui->functionsContainer->count() > DEFAULT_CONTAINER_SIZE);
}

void MusicRightAreaWidget::showCommentsFound()
{
    if(G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool())
    {
        MusicApplication::instance()->windowConciseChanged();
    }

    if(m_ui->functionsContainer->currentIndex() != MUSIC_LRC_PAGE)
    {
        functionClicked(MusicRightAreaWidget::LrcWidget);
    }

    m_interiorLrc->showSongCommentsWidget();
}

void MusicRightAreaWidget::showSimilarFound(const QString &text)
{
    functionClicked(MusicRightAreaWidget::SimilarWidget);
    TTKObjectCast(MusicSimilarQueryWidget*, m_currentWidget)->setCurrentValue(text);
}

void MusicRightAreaWidget::showAlbumFound(const QString &text, const QString &id)
{
    functionClicked(MusicRightAreaWidget::AlbumWidget);
    MusicAlbumQueryWidget *w = TTKObjectCast(MusicAlbumQueryWidget*, m_currentWidget);
    id.isEmpty() ? w->setCurrentValue(text) : w->setCurrentKey(id);
}

void MusicRightAreaWidget::showArtistFound(const QString &text, const QString &id)
{
    functionClicked(MusicRightAreaWidget::ArtistWidget);
    MusicArtistQueryWidget *w = TTKObjectCast(MusicArtistQueryWidget*, m_currentWidget);
    id.isEmpty() ? w->setCurrentValue(text) : w->setCurrentKey(id);
}

void MusicRightAreaWidget::showPlaylistFound(const QString &id)
{
    functionClicked(MusicRightAreaWidget::PlaylistInfoWidget);
    MusicPlaylistQueryWidget *w = TTKObjectCast(MusicPlaylistQueryWidget*, m_currentWidget);
    w->setCurrentKey(id);
}

void MusicRightAreaWidget::showPlaylistCategoryFound(const QString &id, const QString &value)
{
    functionClicked(MusicRightAreaWidget::PlaylistInfoWidget);
    MusicPlaylistQueryWidget *w = TTKObjectCast(MusicPlaylistQueryWidget*, m_currentWidget);
    w->setCurrentCategory({id, value});
}

void MusicRightAreaWidget::showMovieRadioFound(const QVariant &data)
{
    if(m_videoWidget && m_videoWidget->isPopupMode())
    {
        m_videoWidget->raise();
    }
    else
    {
        functionClicked(MusicRightAreaWidget::VideoWidget);
    }

    m_videoWidget->videoResearchButtonSearched(data);
}

void MusicRightAreaWidget::showMovieSearchedFound(const QString &name, const QString &id)
{
    if(m_videoWidget && m_videoWidget->isPopupMode())
    {
        m_videoWidget->raise();
    }
    else
    {
        functionClicked(MusicRightAreaWidget::VideoWidget);
    }

    id.isEmpty() ? m_videoWidget->videoResearchButtonSearched(name) : m_videoWidget->startToSearchByID(id);
}

void MusicRightAreaWidget::showPersonalRadioFound(const QString &id)
{
    functionClicked(MusicRightAreaWidget::WebPVRadioWidget);
    MusicPersonalRadioQueryWidget *w = TTKObjectCast(MusicPersonalRadioQueryWidget*, m_currentWidget);
    w->setCurrentValue(id);
}

void MusicRightAreaWidget::showSongSearchedFound(const QString &text)
{
    m_ui->songSearchEdit->setText(text.trimmed());
    functionClicked(MusicRightAreaWidget::SearchWidget);
}

void MusicRightAreaWidget::showSingleSearchedFound(const QString &id)
{
    functionClicked(MusicRightAreaWidget::SearchSingleWidget);
    m_ui->songSearchWidget->startToSearchByID(id);
}

void MusicRightAreaWidget::showSongMainWidget()
{
    ///To prevent concise state changed while function windowConciseChanged first called
    const bool pre = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, false);
#ifdef Q_OS_UNIX
    m_ui->functionsContainer->setCurrentIndex(MUSIC_LRC_PAGE);
#endif
    functionClicked(MusicRightAreaWidget::MainRecommendWidget);
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, pre);
}

void MusicRightAreaWidget::setDestopLrcVisible(bool visible) const
{
    m_ui->desktopLrcButton->setChecked(visible);
    m_desktopLrc->setVisible(visible);
    m_desktopLrc->initCurrentLrc();
    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, visible);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_desktopLrc->setWindowLockedChanged();
}

void MusicRightAreaWidget::setWindowLrcTypeChanged()
{
    const bool type = m_desktopLrc ? m_desktopLrc->isVerticalWindowType() : TTKStaticCast(bool, G_SETTING_PTR->value(MusicSettingManager::DLrcWindowMode).toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometry, QPoint());

    MusicLrcContainerForDesktop *desktop = m_desktopLrc;
    if(type)
    {
        m_desktopLrc = new MusicLrcContainerHorizontalDesktop;
    }
    else
    {
        m_desktopLrc = new MusicLrcContainerVerticalDesktop;
    }
    m_desktopLrc->setLrcAnalysisModel(m_lrcAnalysis);

    if(desktop)
    {
        m_desktopLrc->statusCopyFrom(desktop);
        desktop->deleteLater();
    }

    m_desktopLrc->applyParameter();
    m_desktopLrc->initCurrentLrc();
    m_desktopLrc->setVisible(G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt());

    connect(m_desktopLrc, SIGNAL(currentLrcUpdated()), MusicApplication::instance(), SLOT(currentLrcUpdated()));
    connect(m_desktopLrc, SIGNAL(showCurrentLrcSetting()), MusicApplication::instance(), SLOT(showSettingWidget()));
    connect(m_desktopLrc, SIGNAL(showCurrentLrcColorSetting()), m_settingWidget, SLOT(changeDesktopLrcWidget()));

    G_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowMode, type);
}

void MusicRightAreaWidget::videoSetPopup(bool popup)
{
    if(!m_videoWidget)
    {
        return;
    }

    m_videoWidget->popupMode(popup);
    if(popup)
    {
        m_ui->functionsContainer->addWidget(m_currentWidget);
        m_ui->functionsContainer->setCurrentWidget(m_currentWidget);
#ifdef Q_OS_WIN
        MusicPlatformManager manager;
        manager.windowsEnableLeftMode();
#endif
        TTK_SIGNLE_SHOT(videoActiveWindow, TTK_SLOT);
    }
    else
    {
        functionClicked(MusicRightAreaWidget::VideoWidget);
    }
}

void MusicRightAreaWidget::videoActiveWindow()
{
    if(m_videoWidget)
    {
        MusicApplication::instance()->activateWindow();
        m_videoWidget->activateWindow();
    }
}

void MusicRightAreaWidget::videoNeedToClose()
{
    functionClicked(MusicRightAreaWidget::LrcWidget);
    m_ui->functionOptionWidget->switchToSelectedItemStyle(MusicRightAreaWidget::LrcWidget);

    delete m_videoWidget;
    m_videoWidget = nullptr;
}

void MusicRightAreaWidget::videoFullscreen(bool full)
{
    if(m_videoWidget)
    {
        m_videoWidget->resizeGeometry(full);
        m_videoWidget->setBlockOption(full);
    }
}

void MusicRightAreaWidget::lrcDisplayAllClicked()
{
    const bool lrcDisplayAll = !m_interiorLrc->lrcDisplayExpand();
    m_interiorLrc->setLrcDisplayExpand(lrcDisplayAll);
    m_ui->centerLeftWidget->setVisible(!lrcDisplayAll);

    const int height = m_interiorLrc->height() - m_ui->lrcDisplayAllButton->height() - 40;
    QPropertyAnimation *lrcDisplayAllAnimation = new QPropertyAnimation(m_ui->lrcDisplayAllButton, "pos", this);
    lrcDisplayAllAnimation->setDuration(100);
    lrcDisplayAllAnimation->setStartValue(QPoint(lrcDisplayAll ? LEFT_SIDE_WIDTH_MIN - 20 : -LEFT_SIDE_WIDTH_MIN, height / 2));
    lrcDisplayAllAnimation->setEndValue(QPoint(0, height / 2));
    lrcDisplayAllAnimation->start();

    m_ui->lrcDisplayAllButton->setStyleSheet(lrcDisplayAll ? TTK::UI::TinyBtnLrcExpand : TTK::UI::TinyBtnLrcCollapse);
    m_ui->windowConcise->setEnabled(!lrcDisplayAll);
}

void MusicRightAreaWidget::showContainerForWallpaper()
{
    if(m_wallpaperLrc)
    {
        MusicTopAreaWidget::instance()->showWallpaperRemote(false);
        delete m_wallpaperLrc;
        m_wallpaperLrc = nullptr;
    }
    else
    {
#ifdef Q_OS_WIN
        MusicPlatformManager manager;
        manager.windowsEnableLeftMode();
#endif
        m_wallpaperLrc = new MusicLrcContainerForWallpaper;
        m_wallpaperLrc->setLrcAnalysisModel(m_lrcAnalysis);
        m_wallpaperLrc->applyParameter();
        m_wallpaperLrc->showFullScreen();
        connect(m_interiorLrc, SIGNAL(linearGradientColorChanged()), m_wallpaperLrc, SLOT(changeCurrentLrcColor()));

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

    if(m_modeIndex == LrcWidget) ///lrc option
    {
        m_ui->functionOptionWidget->reset(false);
        m_ui->stackedFunctionWidget->transparent(true);
    }
    else
    {
        m_ui->functionOptionWidget->reset(true);
        m_ui->stackedFunctionWidget->transparent(false);
    }

    m_ui->lrcDisplayAllButton->setVisible(false);
    if(m_interiorLrc->lrcDisplayExpand() && m_modeIndex != LrcWidget)
    {
        lrcDisplayAllClicked();
    }
}

void MusicRightAreaWidget::functionCleanup()
{
    if(m_ui->functionsContainer->count() <= MAX_CONTAINER_SIZE) // max item size
    {
        return;
    }

    /* 1.MusicSongSearchOnlineWidget
       2.MusicLrcContainerForInterior
       3.QKugouWindow
       top 3 items reserved, remove from item index 3.
    */
    for(int index = DEFAULT_CONTAINER_SIZE; index < MAX_CONTAINER_SIZE; ++index)
    {
        QWidget *w = m_ui->functionsContainer->widget(index);
        if(w->objectName() == "MusicCloudManagerWidget")
        {
            // permanent widget
            continue;
        }

        if(index < m_lastWidgetIndex)
        {
            --m_lastWidgetIndex;
        }
        else if(index == m_lastWidgetIndex)
        {
            continue;
        }

        m_ui->functionsContainer->removeWidget(w);
        delete w;
        break;
    }
}

void MusicRightAreaWidget::createkWebWindow(int type)
{
    QKugouWindow::Module module = TTKStaticCast(QKugouWindow::Module, type);

    if(G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool())
    {
        module = QKugouWindow::None;
    }

    QWidget *widget = new QKugouWindow(module, this);
    connect(m_ui->refreshButton, SIGNAL(clicked()), widget, SLOT(refresh()));

    m_ui->functionsContainer->addWidget(m_currentWidget = widget);
    m_ui->functionsContainer->setCurrentWidget(widget);
}
