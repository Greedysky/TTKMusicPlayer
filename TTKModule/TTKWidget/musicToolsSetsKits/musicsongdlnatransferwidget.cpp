#include "musicsongdlnatransferwidget.h"
#include "ui_musicsongdlnatransferwidget.h"
#include "musicsongscontainerwidget.h"
#include "musictoastlabel.h"

#include "qdlna/qdlnafinder.h"
#include "qdlna/qdlnaclient.h"
#include "qdlna/qdlnafileserver.h"

MusicSongDlnaTransferWidget::MusicSongDlnaTransferWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongDlnaTransferWidget),
      m_state(TTK::PlayState::Stopped),
      m_currentPlayIndex(0)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->iconLabel->setPixmap(QPixmap(":/contextMenu/btn_mobile"));
    m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));
    m_ui->refreshButton->setIcon(QIcon(":/functions/btn_fresh_fore_hover"));

    m_ui->playButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->previousButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->nextButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->refreshButton->setStyleSheet(TTK::UI::BackgroundStyle01);

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

    m_ui->deviceComboBox->setStyleSheet(TTK::UI::ComboBoxStyle02);
    m_ui->timeSlider->setStyleSheet(TTK::UI::SliderStyle10);
    m_ui->timeSlider->setValue(0);

    m_ui->deviceComboBox->addItem(tr("No connections"));
    m_ui->deviceComboBox->setEnabled(false);

    m_dlnaFinder = new QDlnaFinder(this);
    m_dlnaFileServer = new QDlnaFileServer(this);

    startToScan();
    m_dlnaFileServer->start();

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(playSongClicked()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(playPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(playNext()));
    connect(m_ui->refreshButton, SIGNAL(clicked()), SLOT(startToScan()));
    connect(m_dlnaFinder, SIGNAL(finished()), SLOT(scanFinished()));
}

MusicSongDlnaTransferWidget::~MusicSongDlnaTransferWidget()
{
    TTKRemoveSingleWidget(className());

    QDlnaClient *client = getClient();
    if(client)
    {
        client->pause();
        client->stop();
        TTK_INFO_STREAM("Stop DLNA module");
    }

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
    for(const QString &name : m_dlnaFinder->clientNames())
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

    if(m_ui->deviceComboBox->currentText() == tr("No connections"))
    {
        MusicToastLabel::popup(tr("Can not find any clients"));
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

void MusicSongDlnaTransferWidget::playSongClicked()
{
    if(m_ui->deviceComboBox->currentText() == tr("No connections"))
    {
        return;
    }

    QDlnaClient *client = getClient();
    if(!client)
    {
        return;
    }

    MusicSongItemList items;
    MusicSongsContainerWidget::instance()->querySongItemList(items);

    if(items.empty())
    {
        return;
    }

    MusicSongList *songs = &items[0].m_songs;
    if(m_currentPlayIndex < 0 || m_currentPlayIndex >= songs->count())
    {
        m_currentPlayIndex = 0;
    }

    const MusicSong &song = (*songs)[m_currentPlayIndex];
    const QFileInfo fin(song.path());

    if(m_state == TTK::PlayState::Playing)
    {
        client->pause();
        m_state = TTK::PlayState::Paused;
        m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    }
    else
    {
        if(m_state == TTK::PlayState::Paused)
        {
            client->play();
            m_state = TTK::PlayState::Playing;
            m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
            return;
        }

        m_dlnaFileServer->setPrefixPath(fin.path());
        if(client->tryToPlayFile(m_dlnaFileServer->localAddress(client->server()) + fin.fileName()))
        {
            m_state = TTK::PlayState::Playing;
            m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));

            qint64 position = 0, duration = 0;
            if(client->positionInfo(position, duration))
            {
                positionChanged(position);
                durationChanged(duration);
            }
        }
        else
        {
            m_state = TTK::PlayState::Stopped;
            m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
        }
    }
}

void MusicSongDlnaTransferWidget::playPrevious()
{
    m_currentPlayIndex--;
    m_state = TTK::PlayState::Stopped;

    playSongClicked();
}

void MusicSongDlnaTransferWidget::playNext()
{
    m_currentPlayIndex++;
    m_state = TTK::PlayState::Stopped;

    playSongClicked();
}

QDlnaClient *MusicSongDlnaTransferWidget::getClient() const
{
    const int index = m_ui->deviceComboBox->currentIndex();
    if(index < 0)
    {
        return nullptr;
    }

    return m_dlnaFinder->client(index);
}
