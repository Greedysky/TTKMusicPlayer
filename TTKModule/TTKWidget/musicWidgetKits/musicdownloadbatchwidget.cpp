#include "musicdownloadbatchwidget.h"
#include "ui_musicdownloadbatchwidget.h"
#include "musicuiobject.h"
#include "musicnetworkthread.h"
#include "musicsettingmanager.h"
#include "musicdownloadrecordconfigmanager.h"
#include "musicdatatagdownloadthread.h"

#include <QTimer>
#include <QScrollBar>
#include <QStyledItemDelegate>

Q_DECLARE_METATYPE(MusicObject::MusicSongAttribute)

MusicDownloadBatchTableItem::MusicDownloadBatchTableItem(QWidget *parent)
    : QWidget(parent)
{
    m_supperClass = parent;
    m_songName = new QLabel(this);
    m_singer = new QLabel(this);
    m_information = new QLabel(this);
    m_information->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_qulity = new QComboBox(this);
    m_qulity->setItemDelegate(new QStyledItemDelegate(m_qulity));
    m_qulity->setStyleSheet(MusicUIObject::MComboBoxStyle02 + MusicUIObject::MItemView01);
    m_qulity->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_songName->setGeometry(0, 0, 190, ITEM_ROW_HEIGHT_S);
    m_singer->setGeometry(180, 0, 120, ITEM_ROW_HEIGHT_S);
    m_qulity->setGeometry(300, 0, 80, ITEM_ROW_HEIGHT_S);
    m_information->setGeometry(380, 0, 150, ITEM_ROW_HEIGHT_S);

    connect(m_qulity, SIGNAL(currentIndexChanged(int)), SLOT(currentQualityChanged(int)));
}

MusicDownloadBatchTableItem::~MusicDownloadBatchTableItem()
{
    delete m_songName;
    delete m_singer;
    delete m_information;
    delete m_qulity;
}

void MusicDownloadBatchTableItem::createItem(const MusicObject::MusicSongInformation &info)
{
    m_songName->setToolTip(info.m_songName);
    m_songName->setText(MusicUtils::Widget::elidedText(m_songName->font(), m_songName->toolTip(), Qt::ElideRight, m_songName->width() - 10));

    m_singer->setToolTip(info.m_singerName);
    m_singer->setText(MusicUtils::Widget::elidedText(m_singer->font(), m_singer->toolTip(), Qt::ElideRight, m_singer->width() - 10));

    m_songInfo = info;

    MusicObject::MusicSongAttributes attrs(info.m_songAttrs);
    qSort(attrs);

    foreach(const MusicObject::MusicSongAttribute &attr, attrs)
    {
        if(attr.m_bitrate == MB_32)         ///st
        {
            m_qulity->addItem(tr("ST"), QVariant::fromValue<MusicObject::MusicSongAttribute>(attr));
            m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
        }
        else if(attr.m_bitrate == MB_128)   ///sd
        {
            m_qulity->addItem(tr("SD"), QVariant::fromValue<MusicObject::MusicSongAttribute>(attr));
            m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
        }
        else if(attr.m_bitrate == MB_192)   ///hd
        {
            m_qulity->addItem(tr("HQ"), QVariant::fromValue<MusicObject::MusicSongAttribute>(attr));
            m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
        }
        else if(attr.m_bitrate == MB_320)   ///sq
        {
            m_qulity->addItem(tr("SQ"), QVariant::fromValue<MusicObject::MusicSongAttribute>(attr));
            m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
        }
        else if(attr.m_bitrate > MB_320)   ///cd
        {
            m_qulity->addItem(tr("CD"), QVariant::fromValue<MusicObject::MusicSongAttribute>(attr));
            m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
        }
        else
        {
            continue;
        }
    }

    m_qulity->setCurrentIndex(-1);
    m_qulity->setCurrentIndex(0);
}

void MusicDownloadBatchTableItem::startToDownload(MusicDownLoadQueryThreadAbstract::QueryType type)
{
    if(!M_NETWORK_PTR->isOnline() || m_qulity->currentIndex() < 0)
    {
        return;
    }

    if(type == MusicDownLoadQueryThreadAbstract::MusicQuery)
    {
        startToDownloadMusic();
    }
    else if(type == MusicDownLoadQueryThreadAbstract::MovieQuery)
    {
        startToDownloadMovie();
    }
}

void MusicDownloadBatchTableItem::setCurrentQuality(int bitrate)
{
    if(bitrate < 0)
    {
        return;
    }

    int index = -1;
    for(int i=0; i<m_qulity->count(); ++i)
    {
        const MusicObject::MusicSongAttribute &attr = m_qulity->itemData(i).value<MusicObject::MusicSongAttribute>();
        if(attr.m_bitrate == bitrate || (bitrate > MB_320 && attr.m_bitrate > MB_320))
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        m_qulity->setCurrentIndex(index);
    }
}

void MusicDownloadBatchTableItem::currentQualityChanged(int index)
{
    if(index < 0)
    {
        return;
    }

    const MusicObject::MusicSongAttribute &attr = m_qulity->itemData(index).value<MusicObject::MusicSongAttribute>();
    if(attr.m_bitrate == MB_32)         ///st
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
    }
    else if(attr.m_bitrate == MB_128)   ///sd
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
    }
    else if(attr.m_bitrate == MB_192)   ///hd
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
    }
    else if(attr.m_bitrate == MB_320)   ///sq
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
    }
    else if(attr.m_bitrate > MB_320)   ///cd
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(attr.m_size).arg(attr.m_bitrate).arg(attr.m_format.toUpper()));
    }
}

void MusicDownloadBatchTableItem::startToDownloadMusic()
{
    const MusicObject::MusicSongAttribute &musicAttr = m_qulity->itemData(m_qulity->currentIndex()).value<MusicObject::MusicSongAttribute>();
    QString musicSong = m_singer->toolTip() + " - " + m_songName->toolTip();
    const QString &downloadPrefix = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    QString downloadName = QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format);
    //
    MusicSongs records;
    MusicDownloadRecordConfigManager down(MusicObject::RecordNormalDownload, this);
    if(!down.readConfig())
    {
        return;
    }

    down.readDownloadData( records );
    MusicSong record;
    record.setMusicName(musicSong);
    record.setMusicPath(QFileInfo(downloadName).absoluteFilePath());
    record.setMusicSizeStr(musicAttr.m_size);
    record.setMusicAddTimeStr("-1");
    records << record;
    down.writeDownloadData( records );
    //
    if(QFile::exists(downloadName))
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
            downloadName = QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format);
        }
    }
    //
    MusicDataTagDownloadThread *downSong = new MusicDataTagDownloadThread(musicAttr.m_url, downloadName, MusicObject::DownloadMusic, this);
    downSong->setRecordType(MusicObject::RecordNormalDownload);
    connect(downSong, SIGNAL(downLoadDataChanged(QString)), m_supperClass, SLOT(dataDownloadFinished()));

    MusicSongTag tag;
    tag.setComment(m_songInfo.m_smallPicUrl);
    tag.setTitle(m_songInfo.m_songName);
    tag.setArtist(m_songInfo.m_singerName);
    tag.setAlbum(m_songInfo.m_albumName);
    tag.setTrackNum(m_songInfo.m_trackNumber);
    tag.setYear(m_songInfo.m_year);
    downSong->setSongTag(tag);
    downSong->startToDownload();
}

void MusicDownloadBatchTableItem::startToDownloadMovie()
{
    const MusicObject::MusicSongAttribute &musicAttr = m_qulity->itemData(m_qulity->currentIndex()).value<MusicObject::MusicSongAttribute>();
    const QString &downloadPrefix = MOVIE_DIR_FULL;
    QString musicSong = m_singer->toolTip() + " - " + m_songName->toolTip();
    //
    const QStringList &urls = musicAttr.m_multiPart ? musicAttr.m_url.split(TTK_STR_SPLITER) : QStringList(musicAttr.m_url);
    for(int ul=0; ul<urls.count(); ++ul)
    {
        QString downloadName = (urls.count() == 1) ? QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format)
                                                   : QString("%1%2.part%3.%4").arg(downloadPrefix).arg(musicSong).arg(ul+1).arg(musicAttr.m_format);
        if(QFile::exists(downloadName))
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
                downloadName = (urls.count() == 1) ? QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format)
                                                   : QString("%1%2.part%3.%4").arg(downloadPrefix).arg(musicSong).arg(ul+1).arg(musicAttr.m_format);
            }
        }
        //
        MusicDataDownloadThread *download = new MusicDataDownloadThread(urls[ul], downloadName, MusicObject::DownloadVideo, this);
        download->startToDownload();
    }
}



MusicDownloadBatchTableWidget::MusicDownloadBatchTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    m_supperClass = nullptr;
    m_downloadOffset = 0;

    setColumnCount(1);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 530);

    MusicUtils::Widget::setTransparent(this, 255);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setStyleSheet(styleSheet() + MusicUIObject::MTableWidgetStyle02);

}

MusicDownloadBatchTableWidget::~MusicDownloadBatchTableWidget()
{
    clearAllItems();
}

void MusicDownloadBatchTableWidget::setParentObject(QWidget *parent)
{
    m_supperClass = parent;
}

void MusicDownloadBatchTableWidget::clearAllItems()
{
    m_downloadOffset = 0;
    qDeleteAll(m_items);
    m_items.clear();
    MusicAbstractTableWidget::clear();
    setColumnCount(1);
}

void MusicDownloadBatchTableWidget::createItem(const MusicObject::MusicSongInformation &info)
{
    const int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, ITEM_ROW_HEIGHT_S);

    QTableWidgetItem *it = new QTableWidgetItem;
    setItem(index, 0,  it);

    MusicDownloadBatchTableItem *item = new MusicDownloadBatchTableItem(this);
    item->createItem(info);
    m_items << item;
    setCellWidget(index, 0, item);
}

void MusicDownloadBatchTableWidget::startToDownload(MusicDownLoadQueryThreadAbstract::QueryType type)
{
    foreach(MusicDownloadBatchTableItem *item, m_items)
    {
        item->startToDownload(type);
    }
}

void MusicDownloadBatchTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicDownloadBatchTableWidget::dataDownloadFinished()
{
    ++m_downloadOffset;
    if(m_downloadOffset >= rowCount())
    {
        QTimer::singleShot(MT_S2MS, m_supperClass, SLOT(close()));
    }
}

void MusicDownloadBatchTableWidget::currentQualityChanged(int index)
{
    int bitRate = -1;
    switch(index)
    {
        case 1: bitRate = MB_32; break;
        case 2: bitRate = MB_128; break;
        case 3: bitRate = MB_192; break;
        case 4: bitRate = MB_320; break;
        case 5: bitRate = MB_1000; break;
        default: break;
    }

    foreach(MusicDownloadBatchTableItem *item, m_items)
    {
        ///Remove first null item object
        item->setCurrentQuality(bitRate);
    }
}



MusicDownloadBatchWidget::MusicDownloadBatchWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicDownloadBatchWidget)
{
    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->qualityBox->setItemDelegate(new QStyledItemDelegate(m_ui->qualityBox));
    m_ui->qualityBox->setStyleSheet(MusicUIObject::MComboBoxStyle02 + MusicUIObject::MItemView01);
    m_ui->qualityBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->qualityBox->addItems(QStringList() << tr("Null") << tr("ST") << tr("SD") << tr("HQ") << tr("SQ") << tr("CD"));
    connect(m_ui->qualityBox, SIGNAL(currentIndexChanged(int)), m_ui->tableWidget, SLOT(currentQualityChanged(int)));

    m_ui->qualityBox->setCurrentIndex(0);

    m_queryType = MusicDownLoadQueryThreadAbstract::MusicQuery;

    m_ui->tableWidget->setParentObject(this);
    m_ui->downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
#ifdef Q_OS_UNIX
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(startToDownload()));

}

MusicDownloadBatchWidget::~MusicDownloadBatchWidget()
{
    delete m_ui;
}

void MusicDownloadBatchWidget::setSongName(const MusicObject::MusicSongInformations &infos, MusicDownLoadQueryThreadAbstract::QueryType type)
{
    m_queryType = type;

    foreach(const MusicObject::MusicSongInformation &info, infos)
    {
        m_ui->tableWidget->createItem(info);
    }
    m_ui->songCountLabel->setText(tr("All Songs Count %1").arg(infos.count()));
}

void MusicDownloadBatchWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveWidget::show();
}

void MusicDownloadBatchWidget::startToDownload()
{
    m_ui->tableWidget->startToDownload(m_queryType);
    hide();
}
