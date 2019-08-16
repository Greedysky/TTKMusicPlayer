#include "musicidentifysongswidget.h"
#include "musictoolsetsuiobject.h"
#include "musicidentifysongsthread.h"
#include "musicdownloadqueryfactory.h"
#include "musicdatadownloadthread.h"
#include "musicaudiorecordercore.h"
#include "musicsongsharingwidget.h"
#include "musicdownloadwidget.h"
#include "musicnumberdefine.h"
#include "musiccoremplayer.h"
#include "musiclrcanalysis.h"
#include "musictoastlabel.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#include "musicsemaphoreloop.h"
#include "musicotherdefine.h"

#include <QMovie>
#include <QShortcut>

MusicIdentifySongsWidget::MusicIdentifySongsWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    m_mainWindow = new QStackedWidget(this);
    m_mainWindow->setObjectName("MainWindow");
    m_mainWindow->setStyleSheet(QString("#MainWindow{%1}").arg(MusicUIObject::MBackgroundStyle17));
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    m_timer = new QTimer(this);
    m_timer->setInterval(10*MT_S2MS);
    connect(m_timer, SIGNAL(timeout()), SLOT(detectedTimeOut()));

    m_lrcLabel = nullptr;
    m_mediaPlayer = nullptr;
    m_analysis = nullptr;
    m_recordCore = new MusicAudioRecorderCore(this);
    m_detectedThread = new MusicIdentifySongsThread(this);

    QShortcut *cut = new QShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_T, this);
    connect(cut, SIGNAL(activated()), SLOT(detectedButtonClicked()));

    createDetectedWidget();

    m_detectedButton->setEnabled(false);
}

MusicIdentifySongsWidget::~MusicIdentifySongsWidget()
{
    delete m_timer;
    delete m_mediaPlayer;
    delete m_analysis;
    delete m_recordCore;
    delete m_detectedThread;
    delete m_detectedButton;
    delete m_detectedLabel;
    delete m_detectedMovie;
    delete m_mainWindow;
}

void MusicIdentifySongsWidget::getKey()
{
    if(m_detectedThread->getKey())
    {
        m_detectedButton->setEnabled(true);
    }
    else
    {
        MusicToastLabel *toast = new MusicToastLabel(this);
        toast->defaultLabel(this, tr("Init Error!"));
    }
}

void MusicIdentifySongsWidget::detectedButtonClicked()
{
    if(!m_detectedButton->isEnabled())
    {
        return;
    }

    if(m_detectedButton->styleSheet().contains(MusicUIObject::MKGSongsDetectStartBtn))
    {
        m_recordCore->onRecordStart();
        if(m_recordCore->error())
        {
            return;
        }

        m_detectedMovie->start();
        m_timer->start();
        m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStopBtn);
        m_detectedLabel->setText(tr("Recogniting the music being played"));
    }
    else
    {
        m_recordCore->onRecordStop();
        m_detectedMovie->stop();
        m_timer->stop();
        m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStartBtn);
        m_detectedLabel->setText(tr("Intelligent Recognition Of The Music Being Played"));
    }
}

void MusicIdentifySongsWidget::reDetectButtonClicked()
{
    m_mainWindow->setCurrentIndex(IDENTIFY_SONGS_INDEX_0);
    if(m_mediaPlayer)
    {
        m_mediaPlayer->stop();
    }
}

void MusicIdentifySongsWidget::detectedTimeOut()
{
    m_recordCore->addWavHeader(MUSIC_RECORD_IN_FILE);

    MusicSemaphoreLoop loop;
    m_detectedThread->startToDownload(MUSIC_RECORD_IN_FILE);
    connect(m_detectedThread, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    detectedButtonClicked();
    if(m_detectedThread->getIdentifySongs().isEmpty())
    {
        createDetectedFailedWidget();
    }
    else
    {
        createDetectedSuccessedWidget();
    }
}

void MusicIdentifySongsWidget::musicSongPlay()
{
    if(!m_mediaPlayer)
    {
        return;
    }

    if(!m_currentSong.m_songAttrs.isEmpty())
    {
        m_mediaPlayer->setMedia(MusicCoreMPlayer::MusicCategory, m_currentSong.m_songAttrs.first().m_url);
    }
}

void MusicIdentifySongsWidget::musicSongDownload()
{
    if(!m_currentSong.m_singerName.isEmpty())
    {
        MusicDownloadWidget *download = new MusicDownloadWidget(this);
        download->setSongName(m_currentSong, MusicDownLoadQueryThreadAbstract::MusicQuery);
        download->show();
    }
}

void MusicIdentifySongsWidget::musicSongShare()
{
    if(!m_currentSong.m_singerName.isEmpty())
    {
        QVariantMap data;
        data["songName"] = m_currentSong.m_songName;

        MusicSongSharingWidget shareWidget(this);
        shareWidget.setData(MusicSongSharingWidget::Song, data);
        shareWidget.exec();
    }
}

void MusicIdentifySongsWidget::positionChanged(qint64 position)
{
    if(!m_mediaPlayer)
    {
        return;
    }

    if(m_analysis->isEmpty())
    {
        const QString &lrc = QString("<p style='font-weight:600;' align='center'>%1</p>").arg(tr("unFoundLrc"));
        m_lrcLabel->setText(lrc);
        return;
    }

    const int index = m_analysis->getCurrentIndex();
    const qint64 time = m_analysis->findTime(index);

    if(time < position*MT_S2MS && time != -1)
    {
        QString lrc;
        for(int i=0; i<m_analysis->getLineMax(); ++i)
        {
            if(i == m_analysis->getMiddle())
            {
                lrc += QString("<p style='font-weight:700;' align='center'>");
            }
            else
            {
                lrc += QString("<p align='center'>");
            }
            lrc += m_analysis->getText(i);
            lrc += QString("</p>");
        }
        m_lrcLabel->setText(lrc);
        m_analysis->setCurrentIndex(index + 1);
    }
}

void MusicIdentifySongsWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicIdentifySongsWidget::createDetectedWidget()
{
    QWidget *widget = new QWidget(m_mainWindow);
    widget->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MFontStyle05);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);

    m_detectedMovie = new QMovie(":/toolSets/lb_radar", QByteArray(), widget);
    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setFixedSize(200, 200);
    QVBoxLayout *iconLabelLayout = new QVBoxLayout(iconLabel);
    iconLabelLayout->setSpacing(0);
    iconLabelLayout->setContentsMargins(0, 0, 0, 0);
    m_detectedButton = new QPushButton(widget);
    m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStartBtn);
    m_detectedButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_detectedButton->setFixedSize(162, 162);
    iconLabelLayout->addWidget(m_detectedButton, 0, Qt::AlignCenter);
    iconLabel->setMovie(m_detectedMovie);
    iconLabel->setLayout(iconLabelLayout);
    connect(m_detectedButton, SIGNAL(clicked()), SLOT(detectedButtonClicked()));

    m_detectedLabel = new QLabel(widget);
    m_detectedLabel->setText(tr("Intelligent Recognition Of The Music Being Played"));

    QLabel *text = new QLabel(tr("ShotCut:") + " Shift + Ctrl + T", widget);
    text->setStyleSheet(MusicUIObject::MFontStyle03);

    widgetLayout->addStretch(2);
    widgetLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(m_detectedLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(text, 0, Qt::AlignCenter);
    widget->setLayout(widgetLayout);
    //
    m_mainWindow->addWidget(widget);
    m_mainWindow->setCurrentWidget(widget);
}

void MusicIdentifySongsWidget::createDetectedSuccessedWidget()
{
    if(m_mainWindow->count() > 1)
    {
        delete m_lrcLabel;
        delete m_mainWindow->widget(1);
    }
    else
    {
        m_mediaPlayer = new MusicCoreMPlayer(this);
        m_analysis = new  MusicLrcAnalysis(this);
        m_analysis->setLineMax(11);
        connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    }
    const MusicSongIdentify songIdentify(m_detectedThread->getIdentifySongs().first());

    QWidget *widget = new QWidget(m_mainWindow);
    widget->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MFontStyle05);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);
    //
    QWidget *infoWidget = new QWidget(widget);
    QHBoxLayout *infoWidgetLayout = new QHBoxLayout(infoWidget);
    infoWidgetLayout->setContentsMargins(0, 0, 0, 0);
    infoWidgetLayout->setSpacing(25);

    QWidget *infoFuncWidget = new QWidget(infoWidget);
    QGridLayout *infoFuncWidgetLayout = new QGridLayout(infoFuncWidget);
    infoFuncWidgetLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *textLabel = new QLabel(widget);
    textLabel->setText(QString("%1 - %2").arg(songIdentify.m_singerName).arg(songIdentify.m_songName));
    textLabel->setAlignment(Qt::AlignCenter);
    //
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, textLabel->text().trimmed());
    loop.exec();

    if(!d->isEmpty())
    {
        foreach(const MusicObject::MusicSongInformation &info, d->getMusicSongInfos())
        {
            if(info.m_singerName.toLower().trimmed().contains(songIdentify.m_singerName.toLower().trimmed(), Qt::CaseInsensitive) &&
               info.m_songName.toLower().trimmed().contains(songIdentify.m_songName.toLower().trimmed(), Qt::CaseInsensitive) )
            {
                m_currentSong = info;
                break;
            }
        }
    }
    //
    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setMinimumSize(280, 280);
    if(!m_currentSong.m_singerName.isEmpty())
    {
        const QString &name = ART_DIR_FULL + m_currentSong.m_singerName + SKN_FILE;
        if(!QFile::exists(name))
        {
            MusicDataDownloadThread *download = new MusicDataDownloadThread(m_currentSong.m_smallPicUrl, name, MusicObject::DownloadSmallBackground, this);
            connect(download, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
            download->startToDownload();
            loop.exec();
        }
        iconLabel->setPixmap(QPixmap(name).scaled(iconLabel->size()));
    }
    else
    {
        iconLabel->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(iconLabel->size()));
    }

    QPushButton *playButton = new QPushButton(infoFuncWidget);
    QPushButton *loveButton = new QPushButton(infoFuncWidget);
    QPushButton *downButton = new QPushButton(infoFuncWidget);
    QPushButton *shareButton = new QPushButton(infoFuncWidget);

#ifdef Q_OS_UNIX
    playButton->setFocusPolicy(Qt::NoFocus);
    loveButton->setFocusPolicy(Qt::NoFocus);
    downButton->setFocusPolicy(Qt::NoFocus);
    shareButton->setFocusPolicy(Qt::NoFocus);
#endif

    playButton->setFixedSize(25, 25);
    loveButton->setFixedSize(25, 25);
    downButton->setFixedSize(25, 25);
    shareButton->setFixedSize(25, 25);

    playButton->setCursor(QCursor(Qt::PointingHandCursor));
    loveButton->setCursor(QCursor(Qt::PointingHandCursor));
    downButton->setCursor(QCursor(Qt::PointingHandCursor));
    shareButton->setCursor(QCursor(Qt::PointingHandCursor));

    playButton->setStyleSheet(MusicUIObject::MKGSongsDetectPlayBtn);
    loveButton->setStyleSheet(MusicUIObject::MKGSongsDetectUnloveBtn);
    downButton->setStyleSheet(MusicUIObject::MKGSongsDetectDownloadBtn);
    shareButton->setStyleSheet(MusicUIObject::MKGSongsDetectShareBtn);
    connect(playButton, SIGNAL(clicked()), SLOT(musicSongPlay()));
//    connect(loveButton, SIGNAL(clicked()), SLOT(musicSongShare()));
    connect(downButton, SIGNAL(clicked()), SLOT(musicSongDownload()));
    connect(shareButton, SIGNAL(clicked()), SLOT(musicSongShare()));

    infoFuncWidgetLayout->addWidget(textLabel, 0, 0, 1, 4);
    infoFuncWidgetLayout->addWidget(iconLabel, 1, 0, 1, 4);
    infoFuncWidgetLayout->addWidget(playButton, 2, 0, Qt::AlignCenter);
    infoFuncWidgetLayout->addWidget(loveButton, 2, 1, Qt::AlignCenter);
    infoFuncWidgetLayout->addWidget(downButton, 2, 2, Qt::AlignCenter);
    infoFuncWidgetLayout->addWidget(shareButton, 2, 3, Qt::AlignCenter);
    infoFuncWidget->setLayout(infoFuncWidgetLayout);
    //
    m_lrcLabel = new QLabel(widget);
    m_lrcLabel->setMinimumWidth(280);

    if(!m_currentSong.m_singerName.isEmpty())
    {
        const QString &name = MusicUtils::String::lrcPrefix() + m_currentSong.m_singerName + " - " + m_currentSong.m_songName + LRC_FILE;
        if(!QFile::exists(name))
        {
            MusicDownLoadThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getDownloadLrcThread(m_currentSong.m_lrcUrl, name, MusicObject::DownloadLrc, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
            d->startToDownload();
            loop.exec();
        }
        m_analysis->transLrcFileToTime(name);

        if(!m_currentSong.m_songAttrs.isEmpty())
        {
            m_mediaPlayer->setMedia(MusicCoreMPlayer::MusicCategory, m_currentSong.m_songAttrs.first().m_url);
        }
    }

    infoWidgetLayout->addWidget(infoFuncWidget);
    infoWidgetLayout->addWidget(m_lrcLabel);
    infoWidget->setLayout(infoWidgetLayout);
    //
    QPushButton *reDetect = new QPushButton(widget);
    reDetect->setFixedSize(56, 56);
    reDetect->setStyleSheet(MusicUIObject::MKGSongsRedetectBtn);
    reDetect->setCursor(QCursor(Qt::PointingHandCursor));
    connect(reDetect, SIGNAL(clicked()), SLOT(reDetectButtonClicked()));

    QLabel *text3Label = new QLabel(tr("Redetect"), widget);
    text3Label->setStyleSheet(MusicUIObject::MFontStyle03);

    widgetLayout->addStretch(1);
    widgetLayout->addWidget(infoWidget, 1, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(reDetect, 0, Qt::AlignCenter);
    widgetLayout->addWidget(text3Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widget->setLayout(widgetLayout);
    //
    m_mainWindow->addWidget(widget);
    m_mainWindow->setCurrentWidget(widget);
}

void MusicIdentifySongsWidget::createDetectedFailedWidget()
{
    if(m_mainWindow->count() > 1)
    {
        delete m_mainWindow->widget(1);
    }
    QWidget *widget = new QWidget(m_mainWindow);
    widget->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MFontStyle05);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);

    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setPixmap(QPixmap(":/toolSets/lb_detect_error"));
    QLabel *text1Label = new QLabel(tr("No Songs Identified"), widget);
    QLabel *text2Label = new QLabel(tr("Only The Music Being Played Can Be Recognized"), widget);
    QLabel *text3Label = new QLabel(tr("Redetect"), widget);
    text3Label->setStyleSheet(MusicUIObject::MFontStyle03);
    //
    QPushButton *reDetect = new QPushButton(widget);
    reDetect->setFixedSize(56, 56);
    reDetect->setStyleSheet(MusicUIObject::MKGSongsRedetectBtn);
    reDetect->setCursor(QCursor(Qt::PointingHandCursor));
    connect(reDetect, SIGNAL(clicked()), SLOT(reDetectButtonClicked()));
#ifdef Q_OS_UNIX
    reDetect->setFocusPolicy(Qt::NoFocus);
#endif

    widgetLayout->addStretch(2);
    widgetLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(text1Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(text2Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(2);
    widgetLayout->addWidget(reDetect, 0, Qt::AlignCenter);
    widgetLayout->addWidget(text3Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widget->setLayout(widgetLayout);
    //
    m_mainWindow->addWidget(widget);
    m_mainWindow->setCurrentWidget(widget);
}
