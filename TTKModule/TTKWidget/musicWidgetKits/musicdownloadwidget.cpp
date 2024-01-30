#include "musicdownloadwidget.h"
#include "ui_musicdownloadwidget.h"
#include "musicdownloadrecordconfigmanager.h"
#include "musicdownloadmetadatarequest.h"
#include "musicdownloadqueryfactory.h"
#include "musictoastlabel.h"
#include "musicwidgetheaders.h"
#include "musicrulesanalysis.h"

MusicDownloadTableItem::MusicDownloadTableItem(QWidget *parent)
    : QWidget(parent)
{
    m_icon = new QLabel(this);
    m_text = new QLabel(this);
    m_information = new QLabel(this);

    m_text->setGeometry(0, 0, 60, TTK_ITEM_SIZE_S);
    m_icon->setGeometry(70, 0, 30, TTK_ITEM_SIZE_S);
    m_information->setGeometry(170, 0, 150, TTK_ITEM_SIZE_S);
}

MusicDownloadTableItem::~MusicDownloadTableItem()
{
    delete m_information;
    delete m_icon;
    delete m_text;
}

void MusicDownloadTableItem::setIcon(const QString &name)
{
    m_icon->setPixmap(QPixmap(name).scaled(28, 18));
}

void MusicDownloadTableItem::setInformation(const QString &info)
{
    m_information->setText(info);
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

    TTK::Widget::setTransparent(this, 255);
}

MusicDownloadTableWidget::~MusicDownloadTableWidget()
{
    removeItems();
}

void MusicDownloadTableWidget::addCellItem(const TTK::MusicSongProperty &prop, const QString &type, const QString &icon)
{
    const int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, TTK_ITEM_SIZE_S);

    QTableWidgetItem *it = new QTableWidgetItem;
    it->setData(TTK_DATA_ROLE, prop.m_bitrate);
    setItem(index, 0,  it);

    MusicDownloadTableItem *item = new MusicDownloadTableItem(this);
    item->setIcon(icon);
    item->setInformation(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
    item->setText(type);
    m_items << item;

    setCellWidget(index, 0, item);
}

int MusicDownloadTableWidget::bitrate() const
{
   const int row = currentRow();
   if(row == -1)
   {
       return TTK_BN_0;
   }

   return item(row, 0)->data(TTK_DATA_ROLE).toInt();
}

void MusicDownloadTableWidget::removeItems()
{
    qDeleteAll(m_items);
    m_items.clear();
    MusicAbstractTableWidget::removeItems();
    setColumnCount(1);
}



MusicDownloadWidget::MusicDownloadWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicDownloadWidget)
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

    m_ui->downloadButton->setStyleSheet(TTK::UI::PushButtonStyle05);
#ifdef Q_OS_UNIX
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_networkRequest = nullptr;
    m_queryType = MusicAbstractQueryRequest::QueryType::Music;

    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(startToRequest()));

    TTK::Widget::adjustWidgetPosition(this);
}

MusicDownloadWidget::~MusicDownloadWidget()
{
    delete m_ui;
    delete m_networkRequest;
}

void MusicDownloadWidget::controlEnabled(bool enabled)
{
    m_ui->topTitleCloseButton->setEnabled(enabled);
    m_ui->downloadButton->setEnabled(enabled);
}

void MusicDownloadWidget::initialize(MusicAbstractQueryRequest *request, int row)
{
    const TTK::MusicSongInformationList songInfos(request->items());
    if(row >= songInfos.count())
    {
        return;
    }

    m_songInfo = songInfos[row];
    m_queryType = request->queryType();
    m_networkRequest = request;

    m_ui->loadingLabel->run(true);
    controlEnabled(true);
    m_ui->downloadName->setText(TTK::Widget::elidedText(font(), QString("%1 - %2").arg(m_songInfo.m_artistName, m_songInfo.m_songName), Qt::ElideRight, 200));

    TTK_SIGNLE_SHOT(downLoadRequestFinished, TTK_SLOT);
}

void MusicDownloadWidget::initialize(const QString &name, MusicAbstractQueryRequest::QueryType type)
{
    m_queryType = type;

    if(!m_networkRequest)
    {
        m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
        connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadNormalFinished()));
    }

    m_ui->loadingLabel->run(true);
    controlEnabled(true);
    m_ui->downloadName->setText(TTK::Widget::elidedText(font(), name, Qt::ElideRight, 200));

    m_networkRequest->setQueryType(type);
    m_networkRequest->startToSearch(name);
}

void MusicDownloadWidget::initialize(const TTK::MusicSongInformation &info, MusicAbstractQueryRequest::QueryType type)
{
    m_songInfo = info;
    m_queryType = type;

    m_ui->loadingLabel->run(true);
    controlEnabled(true);
    m_ui->downloadName->setText(TTK::Widget::elidedText(font(), QString("%1 - %2").arg(info.m_artistName, info.m_songName), Qt::ElideRight, 200));

    if(m_songInfo.m_songProps.isEmpty())
    {
        close();
        MusicToastLabel::popup(tr("No resource found"));
    }
    else
    {
        std::sort(m_songInfo.m_songProps.begin(), m_songInfo.m_songProps.end()); //to find out the min bitrate
        addCellItems(m_songInfo.m_songProps);
    }
}

void MusicDownloadWidget::startToRequestMusic(const TTK::MusicSongInformation &info, int bitrate, QObject *parent)
{
    if(!G_NETWORK_PTR->isOnline() || info.m_songProps.isEmpty())
    {
        return;
    }

    TTK::MusicSongProperty prop;
    for(const TTK::MusicSongProperty &p : qAsConst(info.m_songProps))
    {
        if(p.m_bitrate == bitrate)
        {
            prop = p;
            break;
        }
    }

    if(prop.isEmpty())
    {
        return;
    }

    const QString &downloadPrefix = G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString();
    QString fileName = MusicRulesAnalysis::parse(info, G_SETTING_PTR->value(MusicSettingManager::DownloadFileNameRule).toString());
    QString downloadPath = QString("%1%2.%3").arg(downloadPrefix, fileName, prop.m_format);

    MusicDownloadRecordConfigManager manager;
    if(!manager.fromFile(TTK::toString(TTK::Record::NormalDownload)))
    {
        return;
    }

    MusicSongList records;
    manager.readBuffer(records);

    MusicSong record;
    record.setName(fileName);
    record.setPath(QFileInfo(downloadPath).absoluteFilePath());
    record.setSizeStr(prop.m_size);
    record.setAddTimeStr("-1");
    records << record;

    manager.reset();
    manager.writeBuffer(records);

    if(QFile::exists(downloadPath))
    {
        for(int i = 1; i < 99; ++i)
        {
            if(!QFile::exists(downloadPath))
            {
                break;
            }

            if(i != 1)
            {
                fileName.chop(3);
            }

            fileName += QString("(%1)").arg(i);
            downloadPath = QString("%1%2.%3").arg(downloadPrefix, fileName, prop.m_format);
        }
    }

    MusicDownloadMetaDataRequest *d = new MusicDownloadMetaDataRequest(prop.m_url, downloadPath, parent);
    connect(d, SIGNAL(downLoadDataChanged(QString)), parent, SLOT(downloadFinished()));

    MusicSongMeta meta;
    meta.setComment(info.m_coverUrl);
    meta.setTitle(info.m_songName);
    meta.setArtist(info.m_artistName);
    meta.setAlbum(info.m_albumName);
    meta.setTrackNum(info.m_trackNumber);
    meta.setYear(info.m_year);

    d->setSongMeta(meta);
    d->startToRequest();
}

void MusicDownloadWidget::startToRequestMovie(const TTK::MusicSongInformation &info, int bitrate, QObject *parent)
{
    if(!G_NETWORK_PTR->isOnline() || info.m_songProps.isEmpty())
    {
        return;
    }

    TTK::MusicSongProperty prop;
    for(const TTK::MusicSongProperty &p : qAsConst(info.m_songProps))
    {
        if(p.m_bitrate == bitrate)
        {
            prop = p;
            break;
        }
    }

    if(prop.isEmpty())
    {
        return;
    }

    const QString &downloadPrefix = G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString();
    QString fileName = MusicRulesAnalysis::parse(info, G_SETTING_PTR->value(MusicSettingManager::DownloadFileNameRule).toString());
    QString downloadPath = QString("%1%2.%3").arg(downloadPrefix, fileName, prop.m_format);

    if(QFile::exists(downloadPath))
    {
        for(int i = 1; i < 99; ++i)
        {
            if(!QFile::exists(downloadPath))
            {
                break;
            }

            if(i != 1)
            {
                fileName.chop(3);
            }

            fileName += QString("(%1)").arg(i);
            downloadPath = QString("%1%2.%3").arg(downloadPrefix, fileName, prop.m_format);
        }
    }

    MusicDownloadDataRequest *d = new MusicDownloadDataRequest(prop.m_url, downloadPath, TTK::Download::Video, parent);
    connect(d, SIGNAL(downLoadDataChanged(QString)), parent, SLOT(downloadFinished()));
    d->startToRequest();
}

void MusicDownloadWidget::downLoadNormalFinished()
{
    if(!G_NETWORK_PTR->isOnline())
    {
        return;
    }

    m_ui->viewArea->removeItems();

    const TTK::MusicSongInformationList songInfos(m_networkRequest->items());
    if(songInfos.isEmpty())
    {
        return;
    }

    const QString &fileName = m_networkRequest->queryValue();
    const QString &songName = TTK::generateSongTitle(fileName);
    const QString &artistName = TTK::generateSongArtist(fileName);

    for(const TTK::MusicSongInformation &var : qAsConst(songInfos))
    {
        if(var.m_artistName.contains(artistName, Qt::CaseInsensitive) && var.m_songName.contains(songName, Qt::CaseInsensitive))
        {
            m_songInfo = var;
            break;
        }
    }

    m_networkRequest->startToQueryResult(&m_songInfo, TTK_BN_0);

    if(m_songInfo.m_songProps.isEmpty())
    {
        close();
        MusicToastLabel::popup(tr("No resource found"));
    }
    else
    {
        std::sort(m_songInfo.m_songProps.begin(), m_songInfo.m_songProps.end()); //to find out the min bitrate
        addCellItems(m_songInfo.m_songProps);
    }
}

void MusicDownloadWidget::downLoadRequestFinished()
{
    m_networkRequest->startToQueryResult(&m_songInfo, TTK_BN_0);
    m_networkRequest = nullptr;

    if(m_songInfo.m_songProps.isEmpty())
    {
        close();
        MusicToastLabel::popup(tr("No resource found"));
    }
    else
    {
        std::sort(m_songInfo.m_songProps.begin(), m_songInfo.m_songProps.end()); //to find out the min bitrate
        addCellItems(m_songInfo.m_songProps);
    }
}

void MusicDownloadWidget::addCellItems(const TTK::MusicSongPropertyList &props)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(props))
    {
        if((prop.m_bitrate == TTK_BN_128 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
           (prop.m_bitrate <= TTK_BN_250 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))       ///sd
        {
            m_ui->viewArea->addCellItem(prop, QObject::tr("SD"), QString(":/quality/lb_sd_quality"));
        }
        else if((prop.m_bitrate == TTK_BN_192 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate == TTK_BN_500 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///hd
        {
            m_ui->viewArea->addCellItem(prop, QObject::tr("HQ"), QString(":/quality/lb_hd_quality"));
        }
        else if((prop.m_bitrate == TTK_BN_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate == TTK_BN_750 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///sq
        {
            m_ui->viewArea->addCellItem(prop, QObject::tr("SQ"), QString(":/quality/lb_sq_quality"));
        }
        else if((prop.m_bitrate > TTK_BN_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate >= TTK_BN_1000 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie)) ///cd
        {
            m_ui->viewArea->addCellItem(prop, QObject::tr("CD"), QString(":/quality/lb_cd_quality"));
        }
        else
        {
            continue;
        }
    }

    m_ui->loadingLabel->run(false);

    int delta = m_ui->viewArea->rowCount();
        delta = ((delta == 0) ? 0 : (delta - 1) * TTK_ITEM_SIZE_S) - 2 * TTK_ITEM_SIZE_S;

    setFixedHeightWidget(this, delta);
    setFixedHeightWidget(m_ui->backgroundMask, delta);
    setFixedHeightWidget(m_ui->background, delta);
    setFixedHeightWidget(m_ui->viewArea, delta + 2 * TTK_ITEM_SIZE_S);

    setMoveWidget(m_ui->label2, delta);
    setMoveWidget(m_ui->fileNameLabel, delta);
    setMoveWidget(m_ui->downloadButton, delta);

    setBackgroundPixmap(size());
    m_ui->fileNameLabel->setText(MusicRulesAnalysis::parse(m_songInfo, G_SETTING_PTR->value(MusicSettingManager::DownloadFileNameRule).toString()));
}

void MusicDownloadWidget::setFixedHeightWidget(QWidget *w, int height)
{
    w->setFixedHeight(w->height() + height);
}

void MusicDownloadWidget::setMoveWidget(QWidget *w, int pos)
{
    const QRect &rect = w->geometry();
    w->move(rect.x(), rect.y() + pos);
}

void MusicDownloadWidget::startToRequest()
{
    const int bitrate = m_ui->viewArea->bitrate();
    if(bitrate == TTK_BN_0)
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    hide(); ///hide download widget

    if(m_queryType == MusicAbstractQueryRequest::QueryType::Music)
    {
        MusicDownloadWidget::startToRequestMusic(m_songInfo, bitrate, this);
    }
    else if(m_queryType == MusicAbstractQueryRequest::QueryType::Movie)
    {
        MusicDownloadWidget::startToRequestMovie(m_songInfo, bitrate, this);
    }
    controlEnabled(false);
}

void MusicDownloadWidget::downloadFinished()
{
    Q_EMIT dataDownloadChanged();
    close();
}
