#include "musicdownloadwidget.h"
#include "ui_musicdownloadwidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"
#include "musicdownloadquerythread.h"
#include "musicsettingmanager.h"
#include "musicnetworkthread.h"
#include "musicmydownloadrecordobject.h"
#include "musicsongdownloadthread.h"
#include "musictextdownloadthread.h"
#include "musicdata2downloadthread.h"
#include "musicbgthemedownload.h"

#include <QFileDialog>

MusicDownloadWidget::MusicDownloadWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicDownloadWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_downloadThread = new MusicDownLoadQueryThread(this);

    connect(ui->pathChangedButton, SIGNAL(clicked()), SLOT(downloadDirSelected()));
    connect(m_downloadThread, SIGNAL(resolvedSuccess()), SLOT(queryAllFinished()));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->downloadButton, SIGNAL(clicked()), SLOT(startToDownload()));
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

    QString path = M_SETTING->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    ui->downloadPathEdit->setText(path.isEmpty() ? MUSIC_DOWNLOAD_AL : path);
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

void MusicDownloadWidget::downloadDirSelected()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory );
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        QString path;
        if(!(path = dialog.directory().absolutePath()).isEmpty())
        {
            M_SETTING->setValue(MusicSettingManager::DownloadMusicPathDirChoiced, path);
            ui->downloadPathEdit->setText(path);
        }
    }
}

void MusicDownloadWidget::startToDownload()
{
    int bitrate = -1;
    if(ui->radioButtonST->isChecked()) bitrate = 32;
    else if(ui->radioButtonHD->isChecked()) bitrate = 128;
    else if(ui->radioButtonSD->isChecked()) bitrate = 320;

    MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfo());
    if(!musicSongInfos.isEmpty())
    {
        MusicSongInfomation musicSongInfo = musicSongInfos.first();
        MusicSongAttributes musicAttrs = musicSongInfo.m_songAttrs;
        foreach(MusicSongAttribute musicAttr, musicAttrs)
        {
            if(musicAttr.m_bitrate == bitrate)
            {
                if(!M_NETWORK->isOnline())
                {
                    return;
                }
                QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
                QString downloadName = QString("%1%2.%3").arg(MUSIC_DOWNLOAD_AL)
                                                         .arg(musicSong).arg(musicAttr.m_format);
                ////////////////////////////////////////////////
                MusicDownloadRecord record;
                MusicMyDownloadRecordObject down(this);
                if(!down.readDownloadXMLConfig())
                {
                    return;
                }

                down.readDownloadConfig( record );
                record.m_names << musicSong;
                record.m_paths << QFileInfo(downloadName).absoluteFilePath();
                record.m_sizes << musicAttr.m_size;
                down.writeDownloadConfig( record );
                ////////////////////////////////////////////////

                MusicSongDownloadThread *downSong = new MusicSongDownloadThread( musicAttr.m_url,
                                                                                 downloadName, Download_Music, this);
                downSong->startToDownload();

                (new MusicTextDownLoadThread(musicSongInfo.m_lrcUrl, LRC_DOWNLOAD_AL +
                                             musicSong + LRC_FILE, Download_Lrc, this))->startToDownload();
                (new MusicData2DownloadThread(musicSongInfo.m_smallPicUrl, ART_DOWNLOAD_AL +
                                              musicSongInfo.m_singerName + SKN_FILE, Download_SmlBG, this))->startToDownload();

                ///download big picture
                new MusicBgThemeDownload(musicSongInfo.m_singerName, musicSongInfo.m_singerName, this);

                break;
            }
        }
    }
    close();
}
