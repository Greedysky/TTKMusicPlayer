#include "musicdownloadwidget.h"
#include "ui_musicdownloadwidget.h"
#include "musicbackgroundmanager.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"
#include "musicnetworkthread.h"
#include "musicmydownloadrecordconfigmanager.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicdata2downloadthread.h"
#include "musicbackgrounddownload.h"
#include "musicmessagebox.h"

#include <QFileDialog>
#include <QLabel>

MusicDownloadTableItem::MusicDownloadTableItem(QWidget *parent)
    : QWidget(parent)
{
    m_infomation = new QLabel(this);
    m_icon = new QLabel(this);
    m_text = new QLabel(this);

    m_text->setGeometry(0, 0, 60, ROW_HEIGHT);
    m_icon->setGeometry(70, 0, 30, ROW_HEIGHT);
    m_infomation->setGeometry(170, 0, 150, ROW_HEIGHT);
}

MusicDownloadTableItem::~MusicDownloadTableItem()
{
    delete m_infomation;
    delete m_icon;
    delete m_text;
}

void MusicDownloadTableItem::setIcon(const QString &name)
{
    m_icon->setPixmap(QPixmap(name).scaled(28, 18));
}

void MusicDownloadTableItem::setInfomation(const QString &info)
{
    m_infomation->setText(info);
}

void MusicDownloadTableItem::setText(const QString &text)
{
    m_text->setText(text);
}



MusicDownloadTableWidget::MusicDownloadTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(1);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 400);
    MusicUtils::setTransparent(this, 255);
}

MusicDownloadTableWidget::~MusicDownloadTableWidget()
{
    clearAllItems();
}

void MusicDownloadTableWidget::clearAllItems()
{
    while(!m_items.isEmpty())
    {
        delete m_items.takeFirst();
    }
    MusicAbstractTableWidget::clear();
    setColumnCount(1);
}

void MusicDownloadTableWidget::createItem(int bitrate, const QString &type, const QString &info, const QString &icon)
{
    int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, ROW_HEIGHT);
    QTableWidgetItem *it = new QTableWidgetItem;
    it->setData(BITRATE_ROLE, bitrate);
    setItem(index, 0,  it);

    MusicDownloadTableItem *item = new MusicDownloadTableItem(this);
    item->setIcon(icon);
    item->setInfomation(info);
    item->setText(type);
    m_items << item;

    setCellWidget(index, 0, item);
}

int MusicDownloadTableWidget::getCurrentBitrate()
{
   int bitrate = -1;
   int row = currentRow();
   if(row == -1)
   {
       return bitrate;
   }

   return item(row, 0)->data(BITRATE_ROLE).toInt();
}


MusicDownloadWidget::MusicDownloadWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicDownloadWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->pathChangedButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->settingButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle10);
#ifndef USE_MULTIPLE_QUERY
       m_downloadThread = new MusicDownLoadQuerySingleThread(this);
#else
       m_downloadThread = new MusicDownLoadQueryMultipleThread(this);
#endif
    m_queryType = MusicDownLoadQueryThreadAbstract::MusicQuery;

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
    controlEnable(true);
    QString path = M_SETTING->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    ui->downloadPathEdit->setText(path.isEmpty() ? MUSIC_DOWNLOAD_AL : path);
}

void MusicDownloadWidget::controlEnable(bool enable)
{
    ui->topTitleCloseButton->setEnabled(enable);
    ui->downloadButton->setEnabled(enable);
    ui->pathChangedButton->setEnabled(enable);
    ui->settingButton->setEnabled(enable);
}

void MusicDownloadWidget::setSongName(const QString &name, MusicDownLoadQueryThreadAbstract::QueryType type)
{
    initWidget();
    ui->downloadName->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 200));
    m_downloadThread->setQueryAllRecords(true);
    m_downloadThread->startSearchSong(m_queryType = type, name);
}

void MusicDownloadWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveWidget::show();
}

void MusicDownloadWidget::queryAllFinished()
{
    if(m_queryType == MusicDownLoadQueryThreadAbstract::MusicQuery)
    {
        queryAllFinishedMusic();
    }
    else if(m_queryType == MusicDownLoadQueryThreadAbstract::MovieQuery)
    {
        queryAllFinishedMovie();
    }
}

void MusicDownloadWidget::queryAllFinishedMusic()
{
    MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(!musicSongInfos.isEmpty())
    {
        MusicSongAttributes attrs = musicSongInfos.first().m_songAttrs;
        foreach(MusicSongAttribute attr, attrs)
        {
            if(attr.m_bitrate == 32)         ///st
            {
                ui->viewArea->createItem(32, tr("ST"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/stQuality"));
            }
            else if(attr.m_bitrate == 128)   ///sd
            {
                ui->viewArea->createItem(128, tr("SD"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/sdQuality"));
            }
            else if(attr.m_bitrate == 192)   ///hd
            {
                ui->viewArea->createItem(192, tr("HD"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/hdQuality"));
            }
            else if(attr.m_bitrate == 320)   ///sq
            {
                ui->viewArea->createItem(320, tr("SQ"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/sqQuality"));
            }
            else if(attr.m_bitrate > 320)   ///cd
            {
                ui->viewArea->createItem(321, tr("CD"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/cdQuality"));
            }
            else
            {
                break;
            }
        }
    }
    resizeWindow();
}

void MusicDownloadWidget::queryAllFinishedMovie()
{
    MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(!musicSongInfos.isEmpty())
    {
        MusicSongAttributes attrs = musicSongInfos.first().m_songAttrs;
        foreach(MusicSongAttribute attr, attrs)
        {
            if(attr.m_bitrate == 500)       ///hd
            {
                ui->viewArea->createItem(500, tr("SD"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/sdQuality"));
            }
            else if(attr.m_bitrate == 750)  ///sq
            {
                ui->viewArea->createItem(750, tr("HD"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/hdQuality"));
            }
            else if(attr.m_bitrate == 1000) ///cd
            {
                ui->viewArea->createItem(1000, tr("SQ"), QString("%1/%2KBPS/%3").arg(attr.m_size)
                                         .arg(attr.m_bitrate).arg(attr.m_format.toUpper()),
                                         QString(":/quality/sqQuality"));
            }
            else
            {
                break;
            }
        }
    }
    resizeWindow();
}

void MusicDownloadWidget::resizeWindow()
{
    int delta = ui->viewArea->rowCount();
    delta = (delta == 0) ? 0 : (delta - 1)*ROW_HEIGHT;

    setFixedHeightWidget(this, delta);
    setFixedHeightWidget(ui->backgroundMask, delta);
    setFixedHeightWidget(ui->background, delta);
    setFixedHeightWidget(ui->viewArea, delta);

    setMoveWidget(ui->label2, delta);
    setMoveWidget(ui->downloadPathEdit, delta);
    setMoveWidget(ui->pathChangedButton, delta);
    setMoveWidget(ui->settingButton, delta);
    setMoveWidget(ui->downloadButton, delta);

    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
}

void MusicDownloadWidget::setFixedHeightWidget(QWidget *w, int height)
{
    w->setFixedHeight(w->height() + height);
}

void MusicDownloadWidget::setMoveWidget(QWidget *w, int pos)
{
    QRect rect = w->geometry();
    w->move(rect.x(), rect.y() + pos);
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
    hide(); ///hide download widget
    if(m_queryType == MusicDownLoadQueryThreadAbstract::MusicQuery)
    {
        startToDownloadMusic();
    }
    else if(m_queryType == MusicDownLoadQueryThreadAbstract::MovieQuery)
    {
        startToDownloadMovie();
    }
}

void MusicDownloadWidget::dataDownloadFinished()
{
    emit dataDownloadChanged();
    close();
}

void MusicDownloadWidget::startToDownloadMusic()
{
    int bitrate = ui->viewArea->getCurrentBitrate();
    if(bitrate == -1)
    {
        MusicMessageBox message(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    if(!musicSongInfos.isEmpty())
    {
        MusicSongInfomation musicSongInfo = musicSongInfos.first();
        MusicSongAttributes musicAttrs = musicSongInfo.m_songAttrs;
        foreach(MusicSongAttribute musicAttr, musicAttrs)
        {
            if(musicAttr.m_bitrate == bitrate ||
               musicAttr.m_bitrate > 321)
            {
                if(!M_NETWORK->isOnline())
                {
                    return;
                }
                QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
                QString downloadName = QString("%1%2.%3").arg(MUSIC_DOWNLOAD_AL).arg(musicSong).arg(musicAttr.m_format);
                ////////////////////////////////////////////////
                MusicDownloadRecord record;
                MusicMyDownloadRecordConfigManager down(this);
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
                QFile file(downloadName);
                if(file.exists())
                {
                    for(int i=1; i<99; ++i)
                    {
                        if(!QFile::exists(downloadName))
                        {
                            break;
                        }
                        if(i != 1)
                        {
                            musicSong.chop(3);
                        }
                        musicSong += QString("(%1)").arg(i);
                        downloadName = QString("%1%2.%3").arg(MUSIC_DOWNLOAD_AL).arg(musicSong).arg(musicAttr.m_format);
                    }
                }
                ////////////////////////////////////////////////
                MusicDataDownloadThread *downSong = new MusicDataDownloadThread( musicAttr.m_url, downloadName,
                                                                                 MusicDownLoadThreadAbstract::Download_Music, this);
                connect(downSong, SIGNAL(musicDownLoadFinished(QString)), SLOT(dataDownloadFinished()));
                downSong->startToDownload();
                break;
            }
        }
    }
    controlEnable(false);
}

void MusicDownloadWidget::startToDownloadMovie()
{
    int bitrate = ui->viewArea->getCurrentBitrate();
    if(bitrate == -1)
    {
        MusicMessageBox message(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
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
                QString downloadName = QString("%1%2.%3").arg(MOVIE_DOWNLOAD_AL).arg(musicSong).arg(musicAttr.m_format);
                ////////////////////////////////////////////////
                QFile file(downloadName);
                if(file.exists())
                {
                    for(int i=1; i<99; ++i)
                    {
                        if(!QFile::exists(downloadName))
                        {
                            break;
                        }
                        if(i != 1)
                        {
                            musicSong.chop(3);
                        }
                        musicSong += QString("(%1)").arg(i);
                        downloadName = QString("%1%2.%3").arg(MOVIE_DOWNLOAD_AL).arg(musicSong).arg(musicAttr.m_format);
                    }
                }
                ////////////////////////////////////////////////
                MusicDataDownloadThread* download = new MusicDataDownloadThread(musicAttr.m_url, downloadName,
                                                                                MusicDownLoadThreadAbstract::Download_Video, this);
                connect(download, SIGNAL(musicDownLoadFinished(QString)), SLOT(dataDownloadFinished()));
                download->startToDownload();
                break;
            }
        }
    }
    controlEnable(false);
}
