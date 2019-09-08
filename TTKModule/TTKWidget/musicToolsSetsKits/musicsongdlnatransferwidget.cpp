#include "musicsongdlnatransferwidget.h"
#include "ui_musicsongdlnatransferwidget.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"
#include "musicsongssummariziedwidget.h"

#include "qdlna/dlnafinder.h"

MusicSongDlnaTransferWidget::MusicSongDlnaTransferWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongDlnaTransferWidget)
{
    m_ui->setupUi(this);

    m_isPlaying = false;

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->iconLabel->setPixmap(QPixmap(":/contextMenu/btn_mobile"));
    m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));

    m_ui->playButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_ui->previousButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_ui->nextButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);

#ifdef Q_OS_UNIX
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->previousButton->setFocusPolicy(Qt::NoFocus);
    m_ui->nextButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->playButton->setIconSize(QSize(31, 31));
    m_ui->previousButton->setIconSize(QSize(31, 31));
    m_ui->nextButton->setIconSize(QSize(31, 31));

    m_ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->previousButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->nextButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->deviceComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle02);
    m_ui->timeSlider->setStyleSheet(MusicUIObject::MSliderStyle10);
    m_ui->timeSlider->setValue(0);

    m_ui->deviceComboBox->addItem(tr("No Connections"));
    m_ui->deviceComboBox->setEnabled(false);

    m_dlnaFinder = new DlnaFinder(this);
    startToScan();

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(musicPlay()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(musicPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(musicNext()));
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
    m_ui->deviceComboBox->removeItem(0);
    m_ui->deviceComboBox->setEnabled(true);

    foreach(const QString &name, m_dlnaFinder->clientNames())
    {
        if(!name.isEmpty())
        {
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
    MusicSongItems songs;
    emit getMusicLists(songs);

//    m_isPlaying = !m_isPlaying;
//    m_ui->playButton->setIcon(QIcon(m_isPlaying ? ":/functions/btn_pause_hover" : ":/functions/btn_play_hover"));
}

void MusicSongDlnaTransferWidget::musicPrevious()
{

}

void MusicSongDlnaTransferWidget::musicNext()
{

}

void MusicSongDlnaTransferWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
