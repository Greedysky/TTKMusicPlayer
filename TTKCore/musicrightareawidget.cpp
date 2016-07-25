#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musiclrccontainerfordesktop.h"
#include "musicvideoplaywidget.h"
#include "musicdownloadstatuslabel.h"
#include "musicsettingwidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicsimilarfoundwidget.h"
#include "musicsongsearchonlinewidget.h"
#include "musickugouuiobject.h"

#include <QPropertyAnimation>

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent), m_videoPlayer(nullptr), m_similarFoundWidget(nullptr)
{
    m_supperClass = parent;
    m_lrcDisplayAll = false;
    m_musiclrcfordesktop = new MusicLrcContainerForDesktop(parent);
    m_downloadStatusLabel = new MusicDownloadStatusLabel(parent);
    m_setting = new MusicSettingWidget(this);
    connect(m_setting, SIGNAL(parameterSettingChanged()), parent,
                       SLOT(getParameterSetting()));
    connect(m_setting, SIGNAL(soundEqualizerClicked()), parent,
                       SLOT(musicSetEqualizer()));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(MusicSongSearchOnlineTableWidget::getClassName(), getClassName());
    M_CONNECTION_PTR->poolConnect(MusicLrcContainerForInline::getClassName(), getClassName());
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_setting;
    delete m_videoPlayer;
    delete m_similarFoundWidget;
    delete m_downloadStatusLabel;
    delete m_musiclrcfordesktop;
}

QString MusicRightAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->musiclrccontainerforinline->resize(ui->surfaceStackedWidget->size());

    ui->lrcDisplayAllButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->lrcDisplayAllButton->setIconSize(QSize(15, 56));
    connect(ui->lrcDisplayAllButton, SIGNAL(clicked()), SLOT(musicLrcDisplayAllButtonClicked()));

    ui->musicSearchBackButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchBackButton->setStyleSheet(MusicKuGouUIObject::MKGBtnBack);
//    connect(ui->musicSearchBackButton, SIGNAL(clicked()), SLOT(musicSearchRefreshButtonRefreshed()));

    ui->musicSearchRefreshButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchRefreshButton->setStyleSheet(MusicKuGouUIObject::MKGBtnFresh);
    connect(ui->musicSearchRefreshButton, SIGNAL(clicked()), SLOT(musicSearchRefreshButtonRefreshed()));

    ui->musicIndexWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    connect(ui->musicIndexWidgetButton, SIGNAL(clicked()), SLOT(musicIndexWidgetButtonSearched()));

    ui->musicSearchWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    connect(ui->musicSearchWidgetButton, SIGNAL(clicked()), SLOT(musicSearchWidgetButtonSearched()));

    ui->musicLrcWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    connect(ui->musicLrcWidgetButton, SIGNAL(clicked()), SLOT(musicLrcWidgetButtonSearched()));

    ui->videoWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->videoWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    connect(ui->videoWidgetButton, SIGNAL(clicked()), SLOT(musicVideoWidgetButtonSearched()));
    ///////////////////////////////////////////////////////
    connect(m_musiclrcfordesktop, SIGNAL(theCurrentLrcUpdated()), m_supperClass,
                 SLOT(musicCurrentLrcUpdated()));
    connect(m_musiclrcfordesktop, SIGNAL(changeCurrentLrcColorSetting()), m_supperClass,
                 SLOT(musicSetting()));
    connect(m_musiclrcfordesktop, SIGNAL(changeCurrentLrcColorCustom()), m_setting,
                 SLOT(changeDesktopLrcWidget()));
    connect(m_musiclrcfordesktop, SIGNAL(desktopLrcClosed()), SIGNAL(desktopLrcClosed()));
    connect(m_musiclrcfordesktop, SIGNAL(setWindowLockedChanged(bool)), SIGNAL(lockDesktopLrc(bool)));
    ///////////////////////////////////////////////////////
    connect(ui->musiclrccontainerforinline, SIGNAL(changeCurrentLrcColorCustom()), m_setting,
                 SLOT(changeInlineLrcWidget()));
    connect(ui->musiclrccontainerforinline, SIGNAL(theCurrentLrcUpdated()), m_supperClass,
                 SLOT(musicCurrentLrcUpdated()));
    connect(ui->musiclrccontainerforinline, SIGNAL(theArtBgHasChanged()),
                 SIGNAL(updateBgThemeDownload()));
    connect(ui->musiclrccontainerforinline, SIGNAL(changeCurrentLrcColorSetting()), m_supperClass,
                 SLOT(musicSetting()));
    connect(ui->musiclrccontainerforinline, SIGNAL(updateCurrentTime(qint64)), m_supperClass,
                 SLOT(updateCurrentTime(qint64)));
    connect(ui->musicSongSearchLine, SIGNAL(enterFinished(QString)),
                 SLOT(songResearchButtonSearched(QString)));
    ///////////////////////////////////////////////////////
//    connect(ui->musicMoreFunction, SIGNAL(clicked()), SLOT(musicSimilarFoundButtonSearched()));

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

bool MusicRightAreaWidget::getDestopLrcVisible() const
{
    return m_musiclrcfordesktop->isVisible();
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
    m_musiclrcfordesktop->setSettingParameter();
    m_ui->musiclrccontainerforinline->setSettingParameter();
}

bool MusicRightAreaWidget::checkSettingParameterValue() const
{
    return ( M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool() ||
             M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool() );
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const
{
    m_ui->musiclrccontainerforinline->setCurrentTime(current);
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
            m_musiclrcfordesktop->setCurrentTime(intervalTime);
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

        m_musiclrcfordesktop->stopLrcMask();
        m_musiclrcfordesktop->setCurrentSongName( name );

        if(!m_ui->musiclrccontainerforinline->transLyricFileToTime( path ))
        {
            m_musiclrcfordesktop->updateCurrentLrc(tr("unFoundLrc"), QString(), 0);
        }
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
    bool config = M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toBool();
    m_ui->musiclrccontainerforinline->setVisible(config);
         config = M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toBool();
    m_musiclrcfordesktop->setVisible(config);
    m_ui->musicDesktopLrc->setChecked(config);
}

void MusicRightAreaWidget::setDestopLrcVisible(bool visible) const
{
    m_ui->musicDesktopLrc->setChecked(visible);
    m_musiclrcfordesktop->setVisible(visible);
    m_musiclrcfordesktop->initCurrentLrc();
    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, visible);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_musiclrcfordesktop->setWindowLockedChanged();
}

void MusicRightAreaWidget::deleteVideoWidget()
{
    delete m_videoPlayer;
    m_videoPlayer = nullptr;
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
        m_ui->surfaceStackedWidget->setCurrentIndex(1);
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

void MusicRightAreaWidget::songResearchButtonSearched(const QString &name)
{
    m_ui->musicSongSearchLine->setText(name);
    musicSearchButtonSearched();
}

void MusicRightAreaWidget::musicIndexWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    //Show the first index of widget
    m_ui->surfaceStackedWidget->setCurrentIndex(0);
    createVideoWidget(false);
}

void MusicRightAreaWidget::musicSearchWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    //Show searched song lists
    m_ui->surfaceStackedWidget->setCurrentIndex(1);
    createVideoWidget(false);
}

void MusicRightAreaWidget::musicLrcWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    //Show lrc display widget
    m_ui->surfaceStackedWidget->setCurrentIndex(2);
    createVideoWidget(false);
    m_ui->lrcDisplayAllButton->setIcon(QIcon(":/lrc/lrcDisplayAll"));
    m_ui->lrcDisplayAllButton->setVisible(true);
    emit updateBgThemeDownload();
}

void MusicRightAreaWidget::musicVideoWidgetButtonSearched()
{
    musicButtonStyleClear();
    m_ui->videoWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
    createVideoWidget(true);
}

void MusicRightAreaWidget::musicSearchRefreshButtonRefreshed()
{
    createVideoWidget(false);
    //Refresh the search music song
    musicSearchButtonSearched();
}

void MusicRightAreaWidget::musicSimilarFoundButtonSearched()
{
    musicButtonStyleClear();

    createVideoWidget(false);
    m_similarFoundWidget = new MusicSimilarFoundWidget(this);
    m_ui->surfaceStackedWidget->addWidget(m_similarFoundWidget);
    m_ui->surfaceStackedWidget->setCurrentWidget(m_similarFoundWidget);
    m_similarFoundWidget->setSongName(m_ui->showCurrentSong->text().trimmed());
}

void MusicRightAreaWidget::createVideoWidget(bool create)
{
    if(create)
    {
        if(m_videoPlayer)
        {
            return;
        }
        deleteVideoWidget();
        m_videoPlayer = new MusicVideoPlayWidget(false);
        m_videoPlayer->setObjectToClose(this);
        m_videoPlayer->blockMoveOption(true);
        connect(m_videoPlayer, SIGNAL(freshButtonClicked(bool)), SLOT(musicVideoSetPopup(bool)));
        m_ui->surfaceStackedWidget->addWidget(m_videoPlayer);
        m_ui->surfaceStackedWidget->setCurrentWidget(m_videoPlayer);
    }
    else if(m_videoPlayer)
    {
        m_ui->surfaceStackedWidget->removeWidget(m_videoPlayer);
        deleteVideoWidget();
    }
    ////////////////////////////////////////////////////////////
    if(m_similarFoundWidget)
    {
        m_ui->surfaceStackedWidget->removeWidget(m_similarFoundWidget);
        delete m_similarFoundWidget;
        m_similarFoundWidget = nullptr;
    }
    ////////////////////////////////////////////////////////////
    m_ui->lrcDisplayAllButton->setVisible(false);
    if(m_lrcDisplayAll)
    {
        musicLrcDisplayAllButtonClicked();
    }
    ////////////////////////////////////////////////////////////
    emit updateBackgroundTheme();
}

void MusicRightAreaWidget::musicButtonStyleClear()
{
    m_ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
    m_ui->videoWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle07);
}

void MusicRightAreaWidget::musicVideoButtonSearched(const QString &name)
{
    musicVideoWidgetButtonSearched();
    if(m_videoPlayer)
    {
        m_videoPlayer->videoResearchButtonSearched(name);
    }
}

void MusicRightAreaWidget::musicVideoSetPopup(bool popup)
{
    if(popup)
    {
        createVideoWidget(false);
        musicButtonStyleClear();
        m_ui->videoWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle16);
        m_videoPlayer = new MusicVideoPlayWidget(true);
        m_videoPlayer->setObjectToClose(this);
        m_videoPlayer->show();
        connect(m_videoPlayer, SIGNAL(freshButtonClicked(bool)), SLOT(musicVideoSetPopup(bool)));
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

void MusicRightAreaWidget::musicLrcDisplayAllButtonClicked()
{
    m_lrcDisplayAll = !m_lrcDisplayAll;
    m_ui->songsContainer->setHidden(m_lrcDisplayAll);

    QPropertyAnimation *lrcDisplayAllAnimation = new QPropertyAnimation(m_ui->lrcDisplayAllButton, "pos", this);
    lrcDisplayAllAnimation->setDuration(100);
    lrcDisplayAllAnimation->setStartValue(QPoint(m_lrcDisplayAll ? 380 : 61, 300));
    lrcDisplayAllAnimation->setEndValue(QPoint(m_lrcDisplayAll ? 61 : 380, 300));
    lrcDisplayAllAnimation->start();

    m_ui->surfaceStackedWidget->setGeometry(m_lrcDisplayAll ? 60 : 380, 115, m_lrcDisplayAll ? 950 : 633, 455);
    m_ui->musiclrccontainerforinline->resizeWidth(m_lrcDisplayAll ? 320 : 0, 0);
    m_ui->lrcDisplayAllButton->setIcon(QIcon(m_lrcDisplayAll ? ":/lrc/lrcDisplayNor" : ":/lrc/lrcDisplayAll"));
    m_ui->musicWindowConcise->setEnabled(!m_lrcDisplayAll);
}
