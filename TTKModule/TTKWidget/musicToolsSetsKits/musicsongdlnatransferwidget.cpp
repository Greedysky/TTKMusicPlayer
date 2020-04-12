#include "musicsongdlnatransferwidget.h"
#include "ui_musicsongdlnatransferwidget.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"
#include "musicsongssummariziedwidget.h"

#include "qdlna/qdlnafinder.h"
#include "qdlna/qdlnaclient.h"
#include "qdlna/qdlnafileserver.h"

MusicSongDlnaTransferWidget::MusicSongDlnaTransferWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongDlnaTransferWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_isPlaying = false;
    m_currentPlayIndex = -1;
    m_musicSongs = nullptr;

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->iconLabel->setPixmap(QPixmap(":/contextMenu/btn_mobile"));
    m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));
    m_ui->refreshButton->setIcon(QIcon(":/functions/btn_fresh_fore_hover"));

    m_ui->playButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->previousButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->nextButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->refreshButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);

#ifdef Q_OS_UNIX
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->previousButton->setFocusPolicy(Qt::NoFocus);
    m_ui->nextButton->setFocusPolicy(Qt::NoFocus);
    m_ui->refreshButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->playButton->setIconSize(QSize(31, 31));
    m_ui->previousButton->setIconSize(QSize(31, 31));
    m_ui->nextButton->setIconSize(QSize(31, 31));
    m_ui->refreshButton->setIconSize(QSize(20, 20));

    m_ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->previousButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->nextButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->refreshButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->deviceComboBox->setStyleSheet(MusicUIObject::MQSSComboBoxStyle02);
    m_ui->timeSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle10);
    m_ui->timeSlider->setValue(0);

    m_ui->deviceComboBox->addItem(tr("No Connections"));
    m_ui->deviceComboBox->setEnabled(false);

    m_dlnaFinder = new QDlnaFinder(this);
    m_dlnaFileServer = new QDlnaFileServer(this);
    startToScan();
    m_dlnaFileServer->start();

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(musicPlay()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(musicPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(musicNext()));
    connect(m_ui->refreshButton, SIGNAL(clicked()), SLOT(startToScan()));
    connect(m_dlnaFinder, SIGNAL(finished()), SLOT(scanFinished()));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicSongDlnaTransferWidget::~MusicSongDlnaTransferWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_dlnaFinder;
    delete m_ui;
}

void MusicSongDlnaTransferWidget::startToScan()
{
    m_dlnaFinder->find();
}

void MusicSongDlnaTransferWidget::scanFinished()
{
    bool init = false;
    foreach(const QString &name, m_dlnaFinder->clientNames())
    {
        if(!name.isEmpty())
        {
            if(!init)
            {
                m_ui->deviceComboBox->clear();
                m_ui->deviceComboBox->setEnabled(true);
                init = true;
            }
            m_ui->deviceComboBox->addItem(name);
        }
    }
}

void MusicSongDlnaTransferWidget::positionChanged(qint64 position)
{
    m_ui->timeSlider->setValue(position);
}

void MusicSongDlnaTransferWidget::durationChanged(qint64 duration)
{
    m_ui->timeSlider->setRange(0, duration);
}

void MusicSongDlnaTransferWidget::musicPlay()
{
    if(m_ui->deviceComboBox->currentText() == tr("No Connections"))
    {
        return;
    }

    const int index = m_ui->deviceComboBox->currentIndex();
    if(index < 0)
    {
        return;
    }

    MusicSongItems songs;
    Q_EMIT getMusicLists(songs);

    if(songs.empty())
    {
        return;
    }

    m_musicSongs = &songs[0].m_songs;
    ++m_currentPlayIndex;
    if(m_currentPlayIndex < 0 || m_currentPlayIndex >= m_musicSongs->size())
    {
        m_currentPlayIndex = 0;
    }

    const MusicSong &song = (*m_musicSongs)[m_currentPlayIndex];
    QFileInfo info(song.getMusicPath());

    QDlnaClient *client = m_dlnaFinder->client(index);
    m_dlnaFileServer->setPrefixPath(info.path());
    client->tryToPlayFile(m_dlnaFileServer->getLocalAddress(client->server()) + info.fileName());

//    m_isPlaying = !m_isPlaying;
//    m_ui->playButton->setIcon(QIcon(m_isPlaying ? ":/functions/btn_pause_hover" : ":/functions/btn_play_hover"));
}

void MusicSongDlnaTransferWidget::musicPrevious()
{
    m_currentPlayIndex--;
}

void MusicSongDlnaTransferWidget::musicNext()
{
    m_currentPlayIndex++;
}

void MusicSongDlnaTransferWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
