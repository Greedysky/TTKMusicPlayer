#include "musicdownloadbatchwidget.h"
#include "ui_musicdownloadbatchwidget.h"
#include "musicuiobject.h"
#include "musicnetworkthread.h"
#include "musicsettingmanager.h"
#include "musicdownloadrecordconfigmanager.h"
#include "musicdatatagdownloadthread.h"

#include <QTimer>
#include <QComboBox>
#include <QScrollBar>
#include <QStyledItemDelegate>

#define ROW_HEIGHT  25
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

    m_songName->setGeometry(0, 0, 190, ROW_HEIGHT);
    m_singer->setGeometry(180, 0, 120, ROW_HEIGHT);
    m_qulity->setGeometry(300, 0, 80, ROW_HEIGHT);
    m_information->setGeometry(380, 0, 150, ROW_HEIGHT);

    connect(m_qulity, SIGNAL(currentIndexChanged(int)), SLOT(currentQualityChanged(int)));
}

MusicDownloadBatchTableItem::~MusicDownloadBatchTableItem()
{
    delete m_songName;
    delete m_singer;
    delete m_information;
    delete m_qulity;
}

QString MusicDownloadBatchTableItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadBatchTableItem::createItem(const MusicObject::MusicSongInformation &info)
{
    m_songName->setToolTip(info.m_songName);
    m_songName->setText(MusicUtils::Widget::elidedText(m_songName->font(), m_songName->toolTip(), Qt::ElideRight, m_songName->width() - 10));

    m_singer->setToolTip(info.m_singerName);
    m_singer->setText(MusicUtils::Widget::elidedText(m_singer->font(), m_singer->toolTip(), Qt::ElideRight, m_singer->width() - 10));

    m_smallPicUrl = info.m_smallPicUrl;

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

void MusicDownloadBatchTableItem::startToDownload()
{
    if(!M_NETWORK_PTR->isOnline() || m_qulity->currentIndex() < 0)
    {
        return;
    }

    MusicObject::MusicSongAttribute musicAttr = m_qulity->itemData(m_qulity->currentIndex()).value<MusicObject::MusicSongAttribute>();
    QString musicSong = m_singer->toolTip() + " - " + m_songName->toolTip();
    QString downloadPrefix = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    QString downloadName = QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format);
    ////////////////////////////////////////////////
    MusicDownloadRecords records;
    MusicDownloadRecordConfigManager down(MusicDownloadRecordConfigManager::Normal, this);
    if(!down.readDownloadXMLConfig())
    {
        return;
    }

    down.readDownloadConfig( records );
    MusicDownloadRecord record;
    record.m_name = musicSong;
    record.m_path = QFileInfo(downloadName).absoluteFilePath();
    record.m_size = musicAttr.m_size;
    records << record;
    down.writeDownloadConfig( records );
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
            downloadName = QString("%1%2.%3").arg(downloadPrefix).arg(musicSong).arg(musicAttr.m_format);
        }
    }
    ////////////////////////////////////////////////
    MusicDataTagDownloadThread *downSong = new MusicDataTagDownloadThread( musicAttr.m_url, downloadName,
                                                                           MusicDownLoadThreadAbstract::Download_Music, this);
    connect(downSong, SIGNAL(downLoadDataChanged(QString)), m_supperClass, SLOT(dataDownloadFinished()));
    downSong->setTags(m_smallPicUrl, m_songName->toolTip(), m_singer->toolTip());
    downSong->startToDownload();
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
        MusicObject::MusicSongAttribute attr = m_qulity->itemData(i).value<MusicObject::MusicSongAttribute>();
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

    MusicObject::MusicSongAttribute attr = m_qulity->itemData(index).value<MusicObject::MusicSongAttribute>();
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



MusicDownloadBatchTableWidget::MusicDownloadBatchTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    m_supperClass = nullptr;
    m_downloadOffset = 0;

    setColumnCount(1);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 530);

    MusicUtils::Widget::setTransparent(this, 255);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);
    setStyleSheet( styleSheet() + MusicUIObject::MTableWidgetStyle02);

}

MusicDownloadBatchTableWidget::~MusicDownloadBatchTableWidget()
{
    clearAllItems();
}

QString MusicDownloadBatchTableWidget::getClassName()
{
    return staticMetaObject.className();
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
    int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, ROW_HEIGHT);

    QTableWidgetItem *it = new QTableWidgetItem;
    setItem(index, 0,  it);

    MusicDownloadBatchTableItem *item = new MusicDownloadBatchTableItem(this);
    item->createItem(info);
    m_items << item;
    setCellWidget(index, 0, item);
}

void MusicDownloadBatchTableWidget::startToDownload()
{
    foreach(MusicDownloadBatchTableItem *item, m_items)
    {
        item->startToDownload();
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

QString MusicDownloadBatchWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadBatchWidget::setSongName(const MusicObject::MusicSongInformations &infos)
{
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
    m_ui->tableWidget->startToDownload();
    hide();
}
