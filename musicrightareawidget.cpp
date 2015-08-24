#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"
#include "musiclrccontainerfordesktop.h"

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_supperClass = parent;
    m_musiclrcfordesktop = new MusicLrcContainerForDesktop(parent);
}

MusicRightAreaWidget::~MusicRightAreaWidget()
{
    delete m_musiclrcfordesktop;
}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;

    ui->musicSearchRefreshButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchRefreshButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    ui->musicSearchRefreshButton->setIconSize(QSize(25,25));
    ui->musicSearchRefreshButton->setIcon(QIcon(QString::fromUtf8(":/image/flash")));
    connect(ui->musicSearchRefreshButton,SIGNAL(clicked()), m_supperClass,
            SLOT(musicSearchRefreshButtonRefreshed()));

    ui->musicIndexWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    connect(ui->musicIndexWidgetButton,SIGNAL(clicked()), m_supperClass, SLOT(musicIndexWidgetButtonSearched()));

    ui->musicSearchWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    connect(ui->musicSearchWidgetButton,SIGNAL(clicked()),m_supperClass, SLOT(musicSearchWidgetButtonSearched()));

    ui->musicLrcWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    connect(ui->musicLrcWidgetButton,SIGNAL(clicked()), m_supperClass, SLOT(musicLrcWidgetButtonSearched()));

    ui->vedioWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->vedioWidgetButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    connect(ui->vedioWidgetButton,SIGNAL(clicked()), m_supperClass, SLOT(musicVedioWidgetButtonSearched()));
    ///////////////////////////////////////////////////////
    connect(m_musiclrcfordesktop,SIGNAL(theCurrentLrcUpdated()), m_supperClass,
                 SLOT(musicCurrentLrcUpdated()));
    connect(m_musiclrcfordesktop,SIGNAL(changeCurrentLrcColorSetting()), m_supperClass,
                 SLOT(musicSetting()));
    connect(m_musiclrcfordesktop,SIGNAL(desktopLrcClosed()), m_supperClass,
                 SLOT(desktopLrcClosed()));
    connect(m_musiclrcfordesktop,SIGNAL(changeCurrentLrcColorCustom()), m_supperClass,
                 SLOT(changeDesktopLrcWidget()));
    connect(m_musiclrcfordesktop,SIGNAL(setWindowLockedChanged(bool)), m_supperClass,
                 SLOT(lockDesktopLrc(bool)));
    ///////////////////////////////////////////////////////
    connect(ui->musiclrccontainerforinline,SIGNAL(changeCurrentLrcColorCustom()), m_supperClass,
                 SLOT(changeInlineLrcWidget()));
    connect(ui->musiclrccontainerforinline,SIGNAL(theCurrentLrcUpdated()), m_supperClass,
                 SLOT(musicCurrentLrcUpdated()));
    connect(ui->musiclrccontainerforinline, SIGNAL(theArtBgHasChanged()),
                 SIGNAL(theArtBgHasChanged()));
    connect(ui->musiclrccontainerforinline,SIGNAL(changeCurrentLrcColorSetting()), m_supperClass,
                 SLOT(musicSetting()));
    connect(ui->musiclrccontainerforinline,SIGNAL(updateCurrentTime(qint64)), m_supperClass,
                 SLOT(updateCurrentTime(qint64)));
}


void MusicRightAreaWidget::stopLrcMask()
{
    if( checkSettingParameterValue() )
    {
       m_ui->musiclrccontainerforinline->stopLrcMask();
       m_musiclrcfordesktop->stopLrcMask();
    }
}

void MusicRightAreaWidget::startTimerClock()
{
    if( checkSettingParameterValue() )
    {
       m_ui->musiclrccontainerforinline->startTimerClock();
       m_musiclrcfordesktop->startTimerClock();
    }
}

void MusicRightAreaWidget::showPlayStatus(bool status)
{
    m_musiclrcfordesktop->showPlayStatus(status);
}

void MusicRightAreaWidget::setDestopLrcVisible(const QString& status)
{
    setDestopLrcVisible( status == "true" ? true : false);
}

bool MusicRightAreaWidget::getDestopLrcVisible()
{
    return m_musiclrcfordesktop->isVisible();
}

void MusicRightAreaWidget::setInlineLrcVisible(const QString& status)
{
    m_ui->musiclrccontainerforinline->setVisible(status == "true" ? true : false);
}

void MusicRightAreaWidget::setSettingParameter()
{
    m_musiclrcfordesktop->setSettingParameter();
    m_ui->musiclrccontainerforinline->setSettingParameter();
}

bool MusicRightAreaWidget::checkSettingParameterValue()
{
    return ( M_SETTING.value(MusicSettingManager::ShowInlineLrcChoiced).toBool() ||
             M_SETTING.value(MusicSettingManager::ShowDesktopLrcChoiced).toBool() )
             ? true : false;
}

void MusicRightAreaWidget::updateCurrentLrc(qint64 current, qint64 total, bool playStatus)
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
                previous = value;
            else
            {
                later = value;
                break;
            }
        }
        //To the last line, set the later to song total time value
        if(later == 0) later = total;
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

void MusicRightAreaWidget::loadCurrentSongLrc(const QString& name, const QString &path)
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

void MusicRightAreaWidget::setSongSpeedAndSlow(qint64 time)
{
    m_ui->musiclrccontainerforinline->setSongSpeedAndSlow(time);
}

void MusicRightAreaWidget::getParameterSetting()
{
    setSettingParameter();
    if(M_SETTING.value(MusicSettingManager::ShowInlineLrcChoiced).toBool())
        m_ui->musiclrccontainerforinline->show();
    else
        m_ui->musiclrccontainerforinline->close();
    bool showDeskLrc = M_SETTING.value(MusicSettingManager::ShowDesktopLrcChoiced).toBool();
    m_musiclrcfordesktop->setVisible(showDeskLrc);
    m_ui->musicDesktopLrc->setChecked(showDeskLrc);
}

void MusicRightAreaWidget::setDestopLrcVisible(bool v)
{
    m_ui->musicDesktopLrc->setChecked(v);
    m_musiclrcfordesktop->setVisible(v);
}

void MusicRightAreaWidget::setWindowLockedChanged()
{
    m_musiclrcfordesktop->setWindowLockedChanged();
}
