#include "musicdownloadbatchwidget.h"
#include "ui_musicdownloadbatchwidget.h"
#include "musicdownloadrecordconfigmanager.h"
#include "musicdownloadmetadatarequest.h"

Q_DECLARE_METATYPE(TTK::MusicSongProperty)

MusicDownloadBatchTableItem::MusicDownloadBatchTableItem(QWidget *parent)
    : QWidget(parent),
      m_supperClass(parent)
{
    m_songName = new QLabel(this);
    m_singer = new QLabel(this);
    m_information = new QLabel(this);
    m_information->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_qulity = new QComboBox(this);
    TTK::Widget::generateComboBoxFormat(m_qulity, TTK::UI::ComboBoxStyle02 + TTK::UI::ItemView01);

    m_songName->setGeometry(0, 0, 190, TTK_ITEM_SIZE_S);
    m_singer->setGeometry(180, 0, 120, TTK_ITEM_SIZE_S);
    m_qulity->setGeometry(300, 0, 80, TTK_ITEM_SIZE_S);
    m_information->setGeometry(380, 0, 150, TTK_ITEM_SIZE_S);
    m_queryType = MusicAbstractQueryRequest::QueryType::Music;

    connect(m_qulity, SIGNAL(currentIndexChanged(int)), SLOT(currentQualityChanged(int)));
}

MusicDownloadBatchTableItem::~MusicDownloadBatchTableItem()
{
    delete m_songName;
    delete m_singer;
    delete m_information;
    delete m_qulity;
}

void MusicDownloadBatchTableItem::addCellItem(const TTK::MusicSongInformation &info, MusicAbstractQueryRequest::QueryType type)
{
    m_songName->setToolTip(info.m_songName);
    m_songName->setText(TTK::Widget::elidedText(m_songName->font(), m_songName->toolTip(), Qt::ElideRight, m_songName->width() - 10));

    m_singer->setToolTip(info.m_singerName);
    m_singer->setText(TTK::Widget::elidedText(m_singer->font(), m_singer->toolTip(), Qt::ElideRight, m_singer->width() - 10));

    m_queryType = type;
    m_songInfo = info;

    TTK::MusicSongPropertyList props(info.m_songProps);
    std::sort(props.begin(), props.end()); //to find out the min bitrate

    for(const TTK::MusicSongProperty &prop : qAsConst(props))
    {
        if((prop.m_bitrate == TTK_BN_128 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
           (prop.m_bitrate <= TTK_BN_250 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))       ///sd
        {
            m_qulity->addItem(tr("SD"), QVariant::fromValue<TTK::MusicSongProperty>(prop));
            m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
        }
        else if((prop.m_bitrate == TTK_BN_192 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate == TTK_BN_500 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///hd
        {
            m_qulity->addItem(tr("HQ"), QVariant::fromValue<TTK::MusicSongProperty>(prop));
            m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
        }
        else if((prop.m_bitrate == TTK_BN_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate == TTK_BN_750 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///sq
        {
            m_qulity->addItem(tr("SQ"), QVariant::fromValue<TTK::MusicSongProperty>(prop));
            m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
        }
        else if((prop.m_bitrate > TTK_BN_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate >= TTK_BN_1000 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie)) ///cd
        {
            m_qulity->addItem(tr("CD"), QVariant::fromValue<TTK::MusicSongProperty>(prop));
            m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
        }
        else
        {
            continue;
        }
    }

    m_qulity->setCurrentIndex(TTK_NORMAL_LEVEL);
    m_qulity->setCurrentIndex(0);
}

void MusicDownloadBatchTableItem::startRequest()
{
    if(!G_NETWORK_PTR->isOnline() || m_qulity->currentIndex() < 0)
    {
        return;
    }

    if(m_queryType == MusicAbstractQueryRequest::QueryType::Music)
    {
        startRequestMusic();
    }
    else if(m_queryType == MusicAbstractQueryRequest::QueryType::Movie)
    {
        startRequestMovie();
    }
}

void MusicDownloadBatchTableItem::setCurrentQuality(int index)
{
    int bitrate = -1;
    if(m_queryType == MusicAbstractQueryRequest::QueryType::Music)
    {
        switch(index)
        {
            case 1: bitrate = TTK_BN_128; break;
            case 2: bitrate = TTK_BN_192; break;
            case 3: bitrate = TTK_BN_320; break;
            case 4: bitrate = TTK_BN_1000; break;
            default: break;
        }
    }
    else if(m_queryType == MusicAbstractQueryRequest::QueryType::Movie)
    {
        switch(index)
        {
            case 1: bitrate = TTK_BN_250; break;
            case 2: bitrate = TTK_BN_500; break;
            case 3: bitrate = TTK_BN_750; break;
            case 4: bitrate = TTK_BN_1000; break;
            default: break;
        }
    }

    index = -1;
    for(int i = 0; i < m_qulity->count(); ++i)
    {
        const TTK::MusicSongProperty &prop = m_qulity->itemData(i).value<TTK::MusicSongProperty>();
        if(prop.m_bitrate == bitrate)
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

    const TTK::MusicSongProperty &prop = m_qulity->itemData(index).value<TTK::MusicSongProperty>();
    if((prop.m_bitrate == TTK_BN_128 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
       (prop.m_bitrate <= TTK_BN_250 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))       ///sd
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
    }
    else if((prop.m_bitrate == TTK_BN_192 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
            (prop.m_bitrate == TTK_BN_500 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///hd
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
    }
    else if((prop.m_bitrate == TTK_BN_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
            (prop.m_bitrate == TTK_BN_750 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///sq
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
    }
    else if((prop.m_bitrate > TTK_BN_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
            (prop.m_bitrate >= TTK_BN_1000 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie)) ///cd
    {
        m_information->setText(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
    }
}

void MusicDownloadBatchTableItem::startRequestMusic()
{
    const TTK::MusicSongProperty &prop = m_qulity->itemData(m_qulity->currentIndex()).value<TTK::MusicSongProperty>();
    QString musicSong = m_singer->toolTip() + " - " + m_songName->toolTip();
    const QString &downloadPrefix = G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString();
    QString downloadName = QString("%1%2.%3").arg(downloadPrefix, musicSong, prop.m_format);

    MusicDownloadRecordConfigManager manager(this);
    if(!manager.fromFile(TTK::toString(TTK::Record::NormalDownload)))
    {
        return;
    }

    MusicSongList records;
    manager.readBuffer(records);

    MusicSong record;
    record.setName(musicSong);
    record.setPath(QFileInfo(downloadName).absoluteFilePath());
    record.setSizeStr(prop.m_size);
    record.setAddTimeStr("-1");
    records << record;

    manager.reset();
    manager.writeBuffer(records);

    if(QFile::exists(downloadName))
    {
        for(int i = 1; i < 99; ++i)
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
            downloadName = QString("%1%2.%3").arg(downloadPrefix, musicSong, prop.m_format);
        }
    }

    MusicDownloadMetaDataRequest *d = new MusicDownloadMetaDataRequest(prop.m_url, downloadName, TTK::Download::Music, this);
    d->setRecordType(TTK::Record::NormalDownload);
    connect(d, SIGNAL(downLoadDataChanged(QString)), m_supperClass, SLOT(dataDownloadFinished()));

    MusicSongMeta meta;
    meta.setComment(m_songInfo.m_coverUrl);
    meta.setTitle(m_songInfo.m_songName);
    meta.setArtist(m_songInfo.m_singerName);
    meta.setAlbum(m_songInfo.m_albumName);
    meta.setTrackNum(m_songInfo.m_trackNumber);
    meta.setYear(m_songInfo.m_year);

    d->setSongMeta(meta);
    d->startRequest();
}

void MusicDownloadBatchTableItem::startRequestMovie()
{
    const TTK::MusicSongProperty &prop = m_qulity->itemData(m_qulity->currentIndex()).value<TTK::MusicSongProperty>();
    const QString &downloadPrefix = MOVIE_DIR_FULL;
    QString musicSong = m_singer->toolTip() + " - " + m_songName->toolTip();
    //
    QString downloadName = QString("%1%2.%3").arg(downloadPrefix, musicSong, prop.m_format);
    if(QFile::exists(downloadName))
    {
        for(int i = 1; i < 99; ++i)
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
            downloadName = QString("%1%2.%3").arg(downloadPrefix, musicSong, prop.m_format);
        }
    }
    //
    MusicDownloadDataRequest *download = new MusicDownloadDataRequest(prop.m_url, downloadName, TTK::Download::Video, this);
    download->startRequest();
}



MusicDownloadBatchTableWidget::MusicDownloadBatchTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent),
      m_supperClass(nullptr),
      m_downloadOffset(0)
{
    setColumnCount(1);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 530);

    setStyleSheet(styleSheet() + TTK::UI::TableWidgetStyle02);
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);

    TTK::Widget::setTransparent(this, 255);
#if defined Q_OS_UNIX && !TTK_QT_VERSION_CHECK(5,7,0) //Fix linux selection-background-color stylesheet bug
    TTK::Widget::setTransparent(this, QColor(220, 220, 220));
#endif
}

MusicDownloadBatchTableWidget::~MusicDownloadBatchTableWidget()
{
    removeItems();
}

void MusicDownloadBatchTableWidget::setParentClass(QWidget *parent)
{
    m_supperClass = parent;
}

void MusicDownloadBatchTableWidget::addCellItem(const TTK::MusicSongInformation &info, MusicAbstractQueryRequest::QueryType type)
{
    const int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, TTK_ITEM_SIZE_S);

    QTableWidgetItem *it = new QTableWidgetItem;
    setItem(index, 0,  it);

    MusicDownloadBatchTableItem *item = new MusicDownloadBatchTableItem(this);
    item->addCellItem(info, type);
    m_items << item;
    setCellWidget(index, 0, item);
}

void MusicDownloadBatchTableWidget::startRequest()
{
    for(MusicDownloadBatchTableItem *item : qAsConst(m_items))
    {
        item->startRequest();
    }
}

void MusicDownloadBatchTableWidget::removeItems()
{
    m_downloadOffset = 0;
    qDeleteAll(m_items);
    m_items.clear();

    MusicAbstractTableWidget::removeItems();
    setColumnCount(1);
}

void MusicDownloadBatchTableWidget::dataDownloadFinished()
{
    ++m_downloadOffset;
    if(m_downloadOffset >= rowCount())
    {
        QTimer::singleShot(TTK_DN_S2MS, m_supperClass, SLOT(close()));
    }
}

void MusicDownloadBatchTableWidget::currentQualityChanged(int index)
{
    for(MusicDownloadBatchTableItem *item : qAsConst(m_items))
    {
        ///Remove first null item object
        item->setCurrentQuality(index);
    }
}



MusicDownloadBatchWidget::MusicDownloadBatchWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicDownloadBatchWidget)
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

    TTK::Widget::generateComboBoxFormat(m_ui->qualityBox, TTK::UI::ComboBoxStyle02 + TTK::UI::ItemView01);
    m_ui->qualityBox->addItems({tr("Null"), tr("SD"), tr("HQ"), tr("SQ"), tr("CD")});
    connect(m_ui->qualityBox, SIGNAL(currentIndexChanged(int)), m_ui->tableWidget, SLOT(currentQualityChanged(int)));

    m_ui->qualityBox->setCurrentIndex(0);

    m_ui->tableWidget->setParentClass(this);
    m_ui->downloadButton->setStyleSheet(TTK::UI::PushButtonStyle05);
#ifdef Q_OS_UNIX
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(startRequest()));
}

MusicDownloadBatchWidget::~MusicDownloadBatchWidget()
{
    G_SINGLE_MANAGER_PTR->removeObject(className());
    delete m_ui;
}

void MusicDownloadBatchWidget::setSongName(const TTK::MusicSongInformationList &songInfos, MusicAbstractQueryRequest::QueryType type)
{
    for(const TTK::MusicSongInformation &info : qAsConst(songInfos))
    {
        m_ui->tableWidget->addCellItem(info, type);
    }
    m_ui->songCountLabel->setText(tr("All Songs Count %1").arg(songInfos.count()));
}

void MusicDownloadBatchWidget::startRequest()
{
    m_ui->tableWidget->startRequest();
    hide();
}
