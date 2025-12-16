#include "musicsongdlnatransferwidget.h"
#include "ui_musicsongdlnatransferwidget.h"
#include "musicplayedlistpopwidget.h"
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

    m_ui->deviceComboBox->addItem(tr("No connections"));
    m_ui->deviceComboBox->setEnabled(false);

    m_timer = new QTimer(this);
    m_dlnaFinder = new QDlnaFinder(this);
    m_dlnaFileServer = new QDlnaFileServer(this);

    startToScan();

    m_dlnaFileServer->start();
    m_timer->setInterval(TTK_DN_S2MS);

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(playSongAction()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(playPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(playNext()));
    connect(m_ui->refreshButton, SIGNAL(clicked()), SLOT(startToScan()));
    connect(m_dlnaFinder, SIGNAL(finished()), SLOT(scanFinished()));
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MusicSongDlnaTransferWidget::~MusicSongDlnaTransferWidget()
{
    TTK_REMOVE_SINGLE_WIDGET(this);

    QDlnaClient *client = getClient();
    if(client)
    {
        client->stop();
        client->remove();
        TTK_INFO_STREAM("Stop DLNA module");
    }

    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    delete m_timer;
    delete m_dlnaFinder;
    delete m_dlnaFileServer;
    delete m_ui;
}

void MusicSongDlnaTransferWidget::startToScan()
{
    m_dlnaFinder->find();
}

void MusicSongDlnaTransferWidget::scanFinished()
{
    const QStringList &clients = m_dlnaFinder->clientNames();
    if(!clients.isEmpty())
    {
        m_ui->deviceComboBox->clear();
        m_ui->deviceComboBox->setEnabled(true);
    }

    for(const QString &name : qAsConst(clients))
    {
        m_ui->deviceComboBox->addItem(name);
    }
}

void MusicSongDlnaTransferWidget::playSongAction()
{
    if(m_ui->deviceComboBox->currentText() == tr("No connections"))
    {
        MusicToastLabel::popup(tr("Can not find any clients"));
        return;
    }

    QDlnaClient *client = getClient();
    if(!client)
    {
        return;
    }

    if(m_state == TTK::PlayState::Playing)
    {
        if(!client->pause())
        {
            MusicToastLabel::popup(tr("Do DLNA operation failed"));
            return;
        }

        m_state = TTK::PlayState::Paused;
        m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    }
    else
    {
        if(m_state == TTK::PlayState::Paused)
        {
            if(!client->play())
            {
                MusicToastLabel::popup(tr("Do DLNA operation failed"));
                return;
            }

            m_state = TTK::PlayState::Playing;
            m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
            return;
        }

        m_timer->stop();
        QString path = MusicPlayedListPopWidget::instance()->currentMediaPath(m_currentPlayIndex);
        if(path.isEmpty())
        {
            // fallback set index to default one
            m_currentPlayIndex = 0;
            path = MusicPlayedListPopWidget::instance()->currentMediaPath(m_currentPlayIndex);
            if(path.isEmpty())
            {
                return;
            }
        }

        QFileInfo fin(path);
        m_dlnaFileServer->setPrefixPath(fin.path());
        if(!client->openUri(m_dlnaFileServer->localAddress(client->server()), fin.fileName()))
        {
            MusicToastLabel::popup(tr("Do DLNA operation failed"));
            return;
        }

        if(client->play())
        {
            m_timer->start();
            m_state = TTK::PlayState::Playing;
            m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));

            {
                QDlna::PositionInfo info;
                if(client->positionInfo(info))
                {
                    TTK_INFO_STREAM(info.position << " " << info.duration);
                }
            }

            {
                QDlna::MediaInfo info;
                if(client->mediaInfo(info))
                {
                    TTK_INFO_STREAM(info.duration << " " << info.nextURI << " " << info.medium);
                }
            }
        }
        else
        {
            MusicToastLabel::popup(tr("Do DLNA operation failed"));
            m_state = TTK::PlayState::Stopped;
            m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
        }
    }
}

void MusicSongDlnaTransferWidget::playPrevious()
{
    m_currentPlayIndex--;
    m_state = TTK::PlayState::Stopped;
    playSongAction();
}

void MusicSongDlnaTransferWidget::playNext()
{
    m_currentPlayIndex++;
    m_state = TTK::PlayState::Stopped;
    playSongAction();
}

void MusicSongDlnaTransferWidget::timeout()
{
    QDlnaClient *client = getClient();
    if(!client)
    {
        return;
    }

    QDlna::TransportInfo info;
    if(client->transportInfo(info))
    {
        TTK_INFO_STREAM(info.state << " " << info.status << " " << info.speed);
    }
}

QDlnaClient *MusicSongDlnaTransferWidget::getClient() const
{
    const int index = m_ui->deviceComboBox->currentIndex();
    return index < 0 ? nullptr : m_dlnaFinder->client(index);
}
