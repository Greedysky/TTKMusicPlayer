#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musiclrccontainerfordesktop.h"
#include "musicvideoplayer.h"
#include "musicdownloadstatuslabel.h"
#include "musicsettingwidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent), m_videoPlayer(NULL)
{
    m_supperClass = parent;
    m_musiclrcfordesktop = new MusicLrcContainerForDesktop(parent);
    m_downloadStatusLabel = new MusicDownloadStatusLabel(parent);
    m_setting = new MusicSettingWidget(this);
    connect(m_setting, SIGNAL(parameterSettingChanged()), parent,
                       SLOT(getParameterSetting()));

    M_Connection->setValue("MusicRightAreaWidget", this);
    M_Connection->connect("MusicSongSearchOnlineTableWidget",
                          "MusicRightAreaWidget");
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_setting;
    delete m_videoPlayer;
    delete m_downloadStatusLabel;
    delete m_musiclrcfordesktop;
}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    m_downloadStatusLabel->setMovieLabel(m_ui->showDownloadGif);

    ui->musicSearchRefreshButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchRefreshButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    ui->musicSearchRefreshButton->setIconSize(QSize(25,25));
    ui->musicSearchRefreshButton->setIcon(QIcon(QString::fromUtf8(":/image/flash")));
    connect(ui->musicSearchRefreshButton,SIGNAL(clicked()), SLOT(musicSearchRefreshButtonRefreshed()));

    ui->musicIndexWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    connect(ui->musicIndexWidgetButton,SIGNAL(clicked()), SLOT(musicIndexWidgetButtonSearched()));

    ui->musicSearchWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    connect(ui->musicSearchWidgetButton,SIGNAL(clicked()), SLOT(musicSearchWidgetButtonSearched()));

    ui->musicLrcWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    connect(ui->musicLrcWidgetButton,SIGNAL(clicked()), SLOT(musicLrcWidgetButtonSearched()));

    ui->vedioWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->vedioWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    connect(ui->vedioWidgetButton,SIGNAL(clicked()), SLOT(musicVideoWidgetButtonSearched()));
    ///////////////////////////////////////////////////////
    connect(m_musiclrcfordesktop,SIGNAL(theCurrentLrcUpdated()), m_supperClass,
                 SLOT(musicCurrentLrcUpdated()));
    connect(m_musiclrcfordesktop,SIGNAL(changeCurrentLrcColorSetting()), m_supperClass,
                 SLOT(musicSetting()));
    connect(m_musiclrcfordesktop,SIGNAL(changeCurrentLrcColorCustom()), m_setting,
                 SLOT(changeDesktopLrcWidget()));
    connect(m_musiclrcfordesktop,SIGNAL(desktopLrcClosed()), SIGNAL(desktopLrcClosed()));
    connect(m_musiclrcfordesktop,SIGNAL(setWindowLockedChanged(bool)), SIGNAL(lockDesktopLrc(bool)));
    ///////////////////////////////////////////////////////
    connect(ui->musiclrccontainerforinline,SIGNAL(changeCurrentLrcColorCustom()), m_setting,
                 SLOT(changeInlineLrcWidget()));
    connect(ui->musiclrccontainerforinline,SIGNAL(theCurrentLrcUpdated()), m_supperClass,
                 SLOT(musicCurrentLrcUpdated()));
    connect(ui->musiclrccontainerforinline, SIGNAL(theArtBgHasChanged()),
                 SIGNAL(updateBgThemeDownload()));
    connect(ui->musiclrccontainerforinline,SIGNAL(changeCurrentLrcColorSetting()), m_supperClass,
                 SLOT(musicSetting()));
    connect(ui->musiclrccontainerforinline,SIGNAL(updateCurrentTime(qint64)), m_supperClass,
                 SLOT(updateCurrentTime(qint64)));
    ///////////////////////////////////////////////////////
}

void MusicRightAreaWidget::stopLrcMask() const
{
    if( checkSettingParameterValue() )
    {
       m_ui->musiclrccontainerforinline->stopLrcMask();
       m_musiclrcfordesktop->stopLrcMask();
    }
}

void MusicRightAreaWidget::startTimerClock() const
{
    if( checkSettingParameterValue() )
    {
       m_ui->musiclrccontainerforinline->startTimerClock();
       m_musiclrcfordesktop->startTimerClock();
    }
}

void MusicRightAreaWidget::showPlayStatus(bool status) const
{
    m_musiclrcfordesktop->showPlayStatus(status);
}

void MusicRightAreaWidget::setDestopLrcVisible(const QString &status) const
{
    setDestopLrcVisible( status == "true" ? true : false);
}

bool MusicRightAreaWidget::getDestopLrcVisible() const
{
    return m_musiclrcfordesktop->isVisible();
}

void MusicRightAreaWidget::setInlineLrcVisible(const QString &status) const
{
    m_ui->musiclrccontainerforinline->setVisible(status == "true" ? true : false);
}

void MusicRightAreaWidget::setSettingParameter() const
{
    m_musiclrcfordesktop->setSettingParameter();
    m_ui->musiclrccontainerforinline->setSettingParameter();
}

bool MusicRightAreaWidget::checkSettingParameterValue() const
{
    return ( M_SETTING->value(MusicSettingManager::ShowInlineLrcChoiced).toBool() ||
             M_SETTING->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool() )
             ? true : false;
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const
{
    m_ui->musiclrccontainerforinline->setCurrentPosition(current);
    //Direct access to the audio file is the total time, in milliseconds
    MIntStringMap lrcContainer(m_ui->musiclrccontainerforinline->getLrcContainer());
    //The corresponding access to current time lyrics
    if(!lrcContainer.isEmpty())
    {
        //After get the current time in the lyrics of the two time points
        qint64 previous = 0;
        qint64 later = 0;
        //Keys () method returns a list of lrcContainer
        foreach (qint64 value, lrcContainer.keys())
        {
            if(current >= value)
            {
                previous = value;
            }
            else
            {
                later = value;
                break;
            }
        }
        //To the last line, set the later to song total time value
        if(later == 0)
        {
            later = total;
        }
        //The lyrics content corresponds to obtain the current time
        QString currentLrc = lrcContainer.value(previous);
        QString laterLrc = lrcContainer.value(later);
        //If this is a new line of the lyrics, then restart lyrics display mask
        if(currentLrc != m_ui->musiclrccontainerforinline->text())
        {
            qint64 intervalTime = later - previous;
            if(!playStatus)
            {
                m_ui->musiclrccontainerforinline->updateCurrentLrc(intervalTime);
            }
            m_musiclrcfordesktop->updateCurrentLrc(currentLrc, laterLrc, intervalTime);
        }
    }
}

void MusicRightAreaWidget::loadCurrentSongLrc(const QString &name, const QString &path) const
{
    if( checkSettingParameterValue() )
    {
        m_ui->musiclrccontainerforinline->stopLrcMask();
        m_ui->musiclrccontainerforinline->setCurrentSongName( name );
        m_ui->musiclrccontainerforinline->transLrcFileToTime( path.trimmed() );
        m_musiclrcfordesktop->setCurrentSongName( name );
        m_musiclrcfordesktop->initCurrentLrc();
    }
}

void MusicRightAreaWidget::setSongSpeedAndSlow(qint64 time) const
{
    m_ui->musiclrccontainerforinline->setSongSpeedAndSlow(time);
}

void MusicRightAreaWidget::musicCheckHasLrcAlready() const
{
    m_downloadStatusLabel->musicCheckHasLrcAlready();
}

void MusicRightAreaWidget::showSettingWidget() const
{
    m_setting->initControllerParameter();
    m_setting->exec();
}

void MusicRightAreaWidget::getParameterSetting() const
{
    setSettingParameter();
    bool config = M_SETTING->value(MusicSettingManager::ShowInlineLrcChoiced).toBool();
    m_ui->musiclrccontainerforinline->setVisible(config);
         config = M_SETTING->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool();
    m_musiclrcfordesktop->setVisible(config);
    m_ui->musicDesktopLrc->setChecked(config);
}

void MusicRightAreaWidget::setDestopLrcVisible(bool v) const
{
    m_ui->musicDesktopLrc->setChecked(v);
    m_musiclrcfordesktop->setVisible(v);
    M_SETTING->setValue(MusicSettingManager::ShowDesktopLrcChoiced, v);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_musiclrcfordesktop->setWindowLockedChanged();
}

void MusicRightAreaWidget::musicSearchButtonSearched()
{
    QString searchedQString = m_ui->musicSongSearchLine->text().trimmed();
    //The string searched wouldn't allow to be none
    if( !searchedQString.isEmpty() &&
         searchedQString != tr("please input search text") )
    {
        musicButtonStyleClear();
        m_ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
        m_ui->SurfaceStackedWidget->setCurrentIndex(1);
        createVideoWidget(false);
        m_ui->songSearchWidget->startSearchQuery(searchedQString);
    }
    else
    {
        MusicMessageBox message;
        message.setText(tr("please input search text"));
        message.exec();
    }
}

void MusicRightAreaWidget::musicResearchButtonSearched(const QString &name)
{
    m_ui->musicSongSearchLine->setText(name);
    musicSearchButtonSearched();
}

void MusicRightAreaWidget::musicIndexWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    //Show the first index of widget
    m_ui->SurfaceStackedWidget->setCurrentIndex(0);
    createVideoWidget(false);
}

void MusicRightAreaWidget::musicSearchWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    //Show searched song lists
    m_ui->SurfaceStackedWidget->setCurrentIndex(1);
    createVideoWidget(false);
}

void MusicRightAreaWidget::musicLrcWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    //Show lrc display widget
    m_ui->SurfaceStackedWidget->setCurrentIndex(2);
    createVideoWidget(false);
    emit updateBgThemeDownload();
}

void MusicRightAreaWidget::musicVideoWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->vedioWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    createVideoWidget(true);
}

void MusicRightAreaWidget::musicSearchRefreshButtonRefreshed()
{
    createVideoWidget(false);
    //Refresh the search music song
    musicSearchButtonSearched();
}

void MusicRightAreaWidget::createVideoWidget(bool create)
{
    if(create)
    {
        if(m_videoPlayer)
        {
            return;
        }
        delete m_videoPlayer;
        m_videoPlayer = new MusicVideoPlayer(false);
        m_videoPlayer->blockMoveOption(true);
        m_ui->SurfaceStackedWidget->addWidget(m_videoPlayer);
        m_ui->SurfaceStackedWidget->setCurrentIndex(3);
    }
    else if(m_videoPlayer)
    {
        m_ui->SurfaceStackedWidget->removeWidget(m_videoPlayer);
        delete m_videoPlayer;
        m_videoPlayer = NULL;
    }
    emit updateBackgroundTheme();
}

void MusicRightAreaWidget::musicButtonStyleClear()
{
    m_ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->vedioWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
}

void MusicRightAreaWidget::musicVideoButtonSearched(const QString &name)
{
    musicVideoWidgetButtonSearched();
    if(m_videoPlayer)
    {
        m_videoPlayer->musicResearchButtonSearched(name);
    }
}

void MusicRightAreaWidget::musicVideoSetPopup(bool popup)
{
    if(popup)
    {
        createVideoWidget(false);
        musicButtonStyleClear();
        m_ui->vedioWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
        m_videoPlayer = new MusicVideoPlayer(true);
        m_videoPlayer->show();
    }
    else
    {
        createVideoWidget(false);
        createVideoWidget(true);
    }
}

void MusicRightAreaWidget::musicVideoFullscreen(bool full)
{
    m_videoPlayer->resizeWindow(full);
    m_videoPlayer->blockMoveOption(full);
}
