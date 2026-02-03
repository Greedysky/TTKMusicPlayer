#include "musicidentifysongwidget.h"
#include "musictoolsetsuiobject.h"
#include "musicidentifysongrequest.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloaddatarequest.h"
#include "musicaudiorecordermodule.h"
#include "musicsongsharingwidget.h"
#include "musicdownloadwidget.h"
#include "musiccoremplayer.h"
#include "musiclrcanalysis.h"
#include "musictoastlabel.h"
#include "musicsong.h"

#include <QMovie>
#include <QShortcut>

MusicIdentifySongWidget::MusicIdentifySongWidget(QWidget *parent)
    : QWidget(parent),
      m_lrcLabel(nullptr),
      m_player(nullptr),
      m_analysis(nullptr)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_mainWidget = new QStackedWidget(this);
    m_mainWidget->setObjectName("MainWidget");
    m_mainWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_mainWidget->objectName(), TTK::UI::BackgroundStyle10));
    layout->addWidget(m_mainWidget);
    setLayout(layout);

    m_timer = new QTimer(this);
    m_timer->setInterval(10 * TTK_DN_S2MS);
    connect(m_timer, SIGNAL(timeout()), SLOT(detectedTimeOut()));

    m_recordCore = new MusicAudioRecorderModule(this);
    m_networkRequest = new MusicIdentifySongRequest(this);

    QShortcut *cut = new QShortcut(QtKeySequence(Qt::Key_T, Qt::SHIFT | Qt::CTRL), this);
    connect(cut, SIGNAL(activated()), SLOT(detectedButtonClicked()));

    createDetectedWidget();
    m_detectedButton->setEnabled(false);
}

MusicIdentifySongWidget::~MusicIdentifySongWidget()
{
    delete m_timer;
    delete m_player;
    delete m_analysis;
    delete m_recordCore;
    delete m_networkRequest;
    delete m_detectedButton;
    delete m_detectedLabel;
    delete m_detectedMovie;
    delete m_mainWidget;
}

void MusicIdentifySongWidget::initialize()
{
    if(m_networkRequest->initialize())
    {
        m_detectedButton->setEnabled(true);
    }
    else
    {
        MusicToastLabel::popup(tr("Config init error"));
    }
}

void MusicIdentifySongWidget::detectedButtonClicked()
{
    if(!m_detectedButton->isEnabled())
    {
        return;
    }

    reDetectButtonClicked();

    if(m_detectedButton->styleSheet() == TTK::UI::SongsDetectStartBtn)
    {
        m_recordCore->onRecordStart();
        if(m_recordCore->error())
        {
            MusicToastLabel::popup(tr("Audio init error"));
            return;
        }

        m_detectedMovie->start();
        m_timer->start();
        m_detectedButton->setStyleSheet(TTK::UI::SongsDetectStopBtn);
        m_detectedLabel->setText(tr("Recognizing playing song"));
    }
    else
    {
        m_recordCore->onRecordStop();
        m_detectedMovie->stop();
        m_timer->stop();
        m_detectedButton->setStyleSheet(TTK::UI::SongsDetectStartBtn);
        m_detectedLabel->setText(tr("Intelligent recognition of playing song"));
    }
}

void MusicIdentifySongWidget::reDetectButtonClicked()
{
    m_mainWidget->setCurrentIndex(0);
    if(m_player)
    {
        m_player->stop();
    }
}

void MusicIdentifySongWidget::detectedTimeOut()
{
    const QString &path = TTK_RECORD_DATA_FILE;
    m_recordCore->addWavHeader(qPrintable(path));

    TTKEventLoop loop;
    connect(m_networkRequest, SIGNAL(downloadDataChanged(QString)), &loop, SLOT(quit()));
    m_networkRequest->startToRequest(path);
    loop.exec();

    detectedButtonClicked();
    if(m_networkRequest->items().isEmpty())
    {
        createDetectedFailedWidget();
    }
    else
    {
        createDetectedSuccessedWidget();
    }
}

void MusicIdentifySongWidget::playSongClicked()
{
    if(!m_player)
    {
        return;
    }

    if(!m_info.m_songProps.isEmpty())
    {
        m_player->setMedia(MusicCoreMPlayer::Module::Music, m_info.m_songProps.first().m_url);
    }
}

void MusicIdentifySongWidget::showDownloadWidget()
{
    if(!m_info.m_artistName.isEmpty())
    {
        MusicDownloadWidget *widget = new MusicDownloadWidget(this);
        widget->initialize(m_info);
        widget->show();
    }
}

void MusicIdentifySongWidget::showSongShareWidget()
{
    if(!m_info.m_artistName.isEmpty())
    {
        MusicSongSharingWidget::Data item;
        item.m_name = m_info.m_songName;

        MusicSongSharingWidget widget(this);
        widget.initialize(MusicSongSharingWidget::Module::Song, item);
        widget.exec();
    }
}

void MusicIdentifySongWidget::positionChanged(qint64 position)
{
    if(!m_player)
    {
        return;
    }

    if(m_analysis->isEmpty())
    {
        const QString &lrc = QString("<p style='font-weight:600;' align='center'>%1</p>").arg(tr("No lrc data file found"));
        m_lrcLabel->setText(lrc);
        return;
    }

    const int index = m_analysis->currentIndex();
    const qint64 time = m_analysis->findTime(index);

    if(time < position * TTK_DN_S2MS && time != -1)
    {
        QString lrc;
        for(int i = 0; i < m_analysis->maximum(); ++i)
        {
            if(i == m_analysis->middle())
            {
                lrc += QString("<p style='font-weight:700;' align='center'>");
            }
            else
            {
                lrc += QString("<p align='center'>");
            }

            lrc += m_analysis->text(i);
            lrc += QString("</p>");
        }
        m_lrcLabel->setText(lrc);
        m_analysis->setCurrentIndex(index + 1);
    }
}

void MusicIdentifySongWidget::createDetectedWidget()
{
    QWidget *widget = new QWidget(m_mainWidget);
    widget->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle05);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);

    m_detectedMovie = new QMovie(":/toolSets/lb_radar", {}, widget);
    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setFixedSize(200, 200);
    QVBoxLayout *iconLabelLayout = new QVBoxLayout(iconLabel);
    iconLabelLayout->setSpacing(0);
    iconLabelLayout->setContentsMargins(0, 0, 0, 0);

    m_detectedButton = new QPushButton(widget);
    m_detectedButton->setStyleSheet(TTK::UI::SongsDetectStartBtn);
    m_detectedButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_detectedButton->setFixedSize(162, 162);
#ifdef Q_OS_UNIX
    m_detectedButton->setFocusPolicy(Qt::NoFocus);
#endif

    iconLabelLayout->addWidget(m_detectedButton, 0, Qt::AlignCenter);
    iconLabel->setMovie(m_detectedMovie);
    iconLabel->setLayout(iconLabelLayout);
    connect(m_detectedButton, SIGNAL(clicked()), SLOT(detectedButtonClicked()));

    m_detectedLabel = new QLabel(widget);
    m_detectedLabel->setText(tr("Intelligent recognition of playing song"));

    QLabel *text = new QLabel(tr("Shortcut:") + " Shift + Ctrl + T", widget);
    text->setStyleSheet(TTK::UI::FontStyle03);

    widgetLayout->addStretch(2);
    widgetLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(m_detectedLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(text, 0, Qt::AlignCenter);
    widget->setLayout(widgetLayout);
    //
    m_mainWidget->addWidget(widget);
    m_mainWidget->setCurrentWidget(widget);
}

void MusicIdentifySongWidget::createDetectedSuccessedWidget()
{
    if(m_mainWidget->count() > 1)
    {
        delete m_lrcLabel;
        delete m_mainWidget->widget(1);
    }
    else
    {
        m_player = new MusicCoreMPlayer(this);
        m_analysis = new MusicLrcAnalysis(this);
        m_analysis->setMaximum(11);
        connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    }
    const MusicSongIdentifyData &songIdentify = m_networkRequest->items().first();

    QWidget *widget = new QWidget(m_mainWidget);
    widget->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle05);
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
    textLabel->setText(QString("%1 - %2").arg(songIdentify.m_artistName, songIdentify.m_songName));
    textLabel->setAlignment(Qt::AlignCenter);
    //
    TTKEventLoop loop;
    MusicAbstractQueryRequest *req = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
    connect(req, SIGNAL(downloadDataChanged(QString)), &loop, SLOT(quit()));
    req->startToSearch(textLabel->text().trimmed());
    loop.exec();

    if(!req->isEmpty())
    {
        for(const TTK::MusicSongInformation &info : req->items())
        {
            if(info.m_artistName.trimmed().contains(songIdentify.m_artistName.trimmed(), Qt::CaseInsensitive) &&
               info.m_songName.trimmed().contains(songIdentify.m_songName.trimmed(), Qt::CaseInsensitive))
            {
                m_info = info;
                break;
            }
        }
    }
    //
    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setMinimumSize(280, 280);
    if(!m_info.m_artistName.isEmpty())
    {
        const QString &name = ART_DIR_FULL + m_info.m_artistName + SKN_FILE;
        if(!QFile::exists(name))
        {
            MusicDownloadDataRequest *req = new MusicDownloadDataRequest(m_info.m_coverUrl, name, TTK::Download::Cover, this);
            connect(req, SIGNAL(downloadDataChanged(QString)), &loop, SLOT(quit()));
            req->startToRequest();
            loop.exec();
        }
        iconLabel->setPixmap(QPixmap(name).scaled(iconLabel->size()));
    }
    else
    {
        iconLabel->setPixmap(QPixmap(":/image/lb_default_art").scaled(iconLabel->size()));
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

    playButton->setStyleSheet(TTK::UI::SongsDetectPlayBtn);
    loveButton->setStyleSheet(TTK::UI::SongsDetectUnloveBtn);
    downButton->setStyleSheet(TTK::UI::SongsDetectDownloadBtn);
    shareButton->setStyleSheet(TTK::UI::SongsDetectShareBtn);

    connect(playButton, SIGNAL(clicked()), SLOT(playSongClicked()));
    connect(downButton, SIGNAL(clicked()), SLOT(showDownloadWidget()));
    connect(shareButton, SIGNAL(clicked()), SLOT(showSongShareWidget()));

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

    if(!m_info.m_artistName.isEmpty())
    {
        const QString &path = TTK::String::lrcDirPrefix() + TTK::generateSongName(m_info.m_songName, m_info.m_artistName) + LRC_FILE;
        if(!QFile::exists(path))
        {
            MusicAbstractDownloadRequest *req = G_DOWNLOAD_QUERY_PTR->makeLrcRequest(m_info.m_lrcUrl, path, this);
            connect(req, SIGNAL(downloadDataChanged(QString)), &loop, SLOT(quit()));
            req->startToRequest();
            loop.exec();
        }

        m_analysis->loadFromFile(path);

        req->startToQueryResult(&m_info, TTK_BN_128);
        if(!m_info.m_songProps.isEmpty())
        {
            m_player->setMedia(MusicCoreMPlayer::Module::Music, m_info.m_songProps.first().m_url);
        }
    }

    infoWidgetLayout->addWidget(infoFuncWidget);
    infoWidgetLayout->addWidget(m_lrcLabel);
    infoWidget->setLayout(infoWidgetLayout);
    //
    QPushButton *reDetect = new QPushButton(widget);
    reDetect->setFixedSize(56, 56);
    reDetect->setStyleSheet(TTK::UI::SongsRedetectBtn);
    reDetect->setCursor(QCursor(Qt::PointingHandCursor));
    connect(reDetect, SIGNAL(clicked()), SLOT(reDetectButtonClicked()));

    QLabel *text3Label = new QLabel(tr("Redetect"), widget);
    text3Label->setStyleSheet(TTK::UI::FontStyle03);

    widgetLayout->addStretch(1);
    widgetLayout->addWidget(infoWidget, 1, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(reDetect, 0, Qt::AlignCenter);
    widgetLayout->addWidget(text3Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widget->setLayout(widgetLayout);
    //
    m_mainWidget->addWidget(widget);
    m_mainWidget->setCurrentWidget(widget);
}

void MusicIdentifySongWidget::createDetectedFailedWidget()
{
    if(m_mainWidget->count() > 1)
    {
        delete m_mainWidget->widget(1);
    }

    QWidget *widget = new QWidget(m_mainWidget);
    widget->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::FontStyle05);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);

    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setPixmap(QPixmap(":/toolSets/lb_detect_error"));
    QLabel *text1Label = new QLabel(tr("There are no recognized songs"), widget);
    QLabel *text2Label = new QLabel(tr("Only playing songs can be recognized"), widget);
    QLabel *text3Label = new QLabel(tr("Redetect"), widget);
    text3Label->setStyleSheet(TTK::UI::FontStyle03);
    //
    QPushButton *reDetect = new QPushButton(widget);
    reDetect->setFixedSize(56, 56);
    reDetect->setStyleSheet(TTK::UI::SongsRedetectBtn);
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
    m_mainWidget->addWidget(widget);
    m_mainWidget->setCurrentWidget(widget);
}
