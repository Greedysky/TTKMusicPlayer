#include "musicdownloadwidget.h"
#include "ui_musicdownloadwidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"
#include "musicdownloadquerythread.h"

MusicDownloadWidget::MusicDownloadWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicDownloadWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_downloadThread = new MusicDownLoadQueryThread(this);
    connect(m_downloadThread, SIGNAL(resolvedSuccess()), SLOT(queryAllFinished()));
}

MusicDownloadWidget::~MusicDownloadWidget()
{
    delete ui;
    delete m_downloadThread;
}

void MusicDownloadWidget::initWidget()
{
    setMusicSTState(false);
    setMusicHDState(false);
    setMusicSDState(false);
}

void MusicDownloadWidget::setMusicSTState(bool show)
{
    ui->radioButtonST->setVisible(show);
    ui->informationST->setVisible(show);
}

void MusicDownloadWidget::setMusicHDState(bool show)
{
    ui->radioButtonHD->setVisible(show);
    ui->iconHD->setVisible(show);
    ui->informationHD->setVisible(show);
}

void MusicDownloadWidget::setMusicSDState(bool show)
{
    ui->radioButtonSD->setVisible(show);
    ui->iconSD->setVisible(show);
    ui->informationSD->setVisible(show);
}

void MusicDownloadWidget::setSongName(const QString &name)
{
    initWidget();
    ui->downloadName->setText(name);
    m_downloadThread->setQueryAllRecords(true);
    m_downloadThread->startSearchSong(MusicQuery, name);
}

int MusicDownloadWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

void MusicDownloadWidget::queryAllFinished()
{
    MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfo());
    if(!musicSongInfos.isEmpty())
    {
        MusicSongAttributes attrs = musicSongInfos.first().m_songAttrs;
        foreach(MusicSongAttribute attr, attrs)
        {
            if(attr.m_bitrate == 32)         ///st
            {
                setMusicSTState(true);
                ui->informationST->setText(QString("%1/%2KBPS/%3").arg(attr.m_size)
                                        .arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
                ui->radioButtonST->setChecked(true);
            }
            else if(attr.m_bitrate == 128)   ///hd
            {
                setMusicHDState(true);
                ui->informationHD->setText(QString("%1/%2KBPS/%3").arg(attr.m_size)
                                        .arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
                ui->radioButtonHD->setChecked(true);
            }
            else if(attr.m_bitrate == 320)   ///sd
            {
                setMusicSDState(true);
                ui->informationSD->setText(QString("%1/%2KBPS/%3").arg(attr.m_size)
                                        .arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
                ui->radioButtonSD->setChecked(true);
            }
        }
    }
}
