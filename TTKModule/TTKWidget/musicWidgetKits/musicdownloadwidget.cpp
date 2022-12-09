#include "musicdownloadwidget.h"
#include "ui_musicdownloadwidget.h"
#include "musicsettingmanager.h"
#include "musicdownloadrecordconfigmanager.h"
#include "musicdownloadtagdatarequest.h"
#include "musictoastlabel.h"
#include "musicdownloadqueryfactory.h"
#include "musicfileutils.h"
#include "musicwidgetheaders.h"

Q_DECLARE_METATYPE(MusicDownloadTableItemRole)

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

    MusicUtils::Widget::setTransparent(this, 255);
}

MusicDownloadTableWidget::~MusicDownloadTableWidget()
{
    removeItems();
}

void MusicDownloadTableWidget::addCellItem(const MusicObject::MusicSongProperty &prop, const QString &type, const QString &icon)
{
    const int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, TTK_ITEM_SIZE_S);

    QTableWidgetItem *it = new QTableWidgetItem;
    MusicDownloadTableItemRole role(prop.m_bitrate, prop.m_format, prop.m_size);
    it->setData(TABLE_ITEM_ROLE, QVariant::fromValue<MusicDownloadTableItemRole>(role));
    setItem(index, 0,  it);

    MusicDownloadTableItem *item = new MusicDownloadTableItem(this);
    item->setIcon(icon);
    item->setInformation(QString("%1/%2KBPS/%3").arg(prop.m_size).arg(prop.m_bitrate).arg(prop.m_format.toUpper()));
    item->setText(type);
    m_items << item;

    setCellWidget(index, 0, item);
}

MusicDownloadTableItemRole MusicDownloadTableWidget::currentItemRole() const
{
   const int row = currentRow();
   if(row == -1)
   {
       return MusicDownloadTableItemRole();
   }

   return item(row, 0)->data(TABLE_ITEM_ROLE).value<MusicDownloadTableItemRole>();
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
      m_ui(new Ui::MusicDownloadWidget),
      m_querySingleInfo(false)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->downloadPathEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->pathChangedButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    m_ui->settingButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    m_ui->downloadButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle05);
#ifdef Q_OS_UNIX
    m_ui->pathChangedButton->setFocusPolicy(Qt::NoFocus);
    m_ui->settingButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_queryType = MusicAbstractQueryRequest::QueryType::Music;
    m_ui->loadingLabel->setType(MusicGifLabelWidget::Module::CicleBlue);
    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);

    connect(m_ui->pathChangedButton, SIGNAL(clicked()), SLOT(downloadDirSelected()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(startRequest()));
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));

    MusicUtils::Widget::positionInCenter(this);
}

MusicDownloadWidget::~MusicDownloadWidget()
{
    delete m_ui;
    delete m_networkRequest;
}

void MusicDownloadWidget::initialize()
{
    m_ui->loadingLabel->run(true);
    controlEnabled(true);

    if(m_queryType == MusicAbstractQueryRequest::QueryType::Movie)
    {
        m_ui->downloadPathEdit->setText(MOVIE_DIR_FULL);
    }
    else
    {
        m_ui->downloadPathEdit->setText(G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString());
    }
}

void MusicDownloadWidget::controlEnabled(bool enable)
{
    m_ui->topTitleCloseButton->setEnabled(enable);
    m_ui->downloadButton->setEnabled(enable);
    m_ui->pathChangedButton->setEnabled(enable);
    m_ui->settingButton->setEnabled(enable);
}

void MusicDownloadWidget::setSongName(const QString &name, MusicAbstractQueryRequest::QueryType type)
{
    m_queryType = type;

    initialize();
    m_ui->downloadName->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 200));
    m_networkRequest->setQueryAllRecords(true);
    m_networkRequest->startToSearch(type, name);
}

void MusicDownloadWidget::setSongName(const MusicObject::MusicSongInformation &info, MusicAbstractQueryRequest::QueryType type)
{
    m_songInfo = info;
    m_queryType = type;
    m_querySingleInfo = true;

    initialize();
    m_ui->downloadName->setText(MusicUtils::Widget::elidedText(font(), QString("%1 - %2").arg(info.m_singerName, info.m_songName), Qt::ElideRight, 200));

    addCellItems(info.m_songProps);
}

void MusicDownloadWidget::downLoadFinished()
{
    if(!G_NETWORK_PTR->isOnline())
    {
        return;
    }

    m_ui->viewArea->removeItems();
    const MusicObject::MusicSongInformation info(matchMusicSongInformation());
    if(!info.m_songName.isEmpty() || !info.m_singerName.isEmpty())
    {
        addCellItems(info.m_songProps);
    }
    else
    {
        close();
        MusicToastLabel::popup(tr("No resource found!"));
    }
}

MusicObject::MusicSongInformation MusicDownloadWidget::matchMusicSongInformation()
{
    const MusicObject::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(!songInfos.isEmpty())
    {
        const QString &fileName = m_networkRequest->queryValue();
        const QString &artistName = MusicUtils::String::artistName(fileName);
        const QString &songName = MusicUtils::String::songName(fileName);

        MusicObject::MusicSongInformation info;
        for(const MusicObject::MusicSongInformation &var : qAsConst(songInfos))
        {
            if(var.m_singerName.contains(artistName, Qt::CaseInsensitive) && var.m_songName.contains(songName, Qt::CaseInsensitive))
            {
                info = var;
                break;
            }
        }
        std::sort(info.m_songProps.begin(), info.m_songProps.end()); //to find out the min bitrate
        return info;
    }
    return MusicObject::MusicSongInformation();
}

void MusicDownloadWidget::addCellItems(const MusicObject::MusicSongPropertyList &props)
{
    MusicObject::MusicSongPropertyList propertys = props;
    std::sort(propertys.begin(), propertys.end()); //to find out the min bitrate

    for(const MusicObject::MusicSongProperty &prop : qAsConst(propertys))
    {
        if((prop.m_bitrate == MB_128 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
           (prop.m_bitrate <= MB_250 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))       ///sd
        {
            m_ui->viewArea->addCellItem(prop, tr("SD"), QString(":/quality/lb_sd_quality"));
        }
        else if((prop.m_bitrate == MB_192 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate == MB_500 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///hd
        {
            m_ui->viewArea->addCellItem(prop, tr("HQ"), QString(":/quality/lb_hd_quality"));
        }
        else if((prop.m_bitrate == MB_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate == MB_750 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie))  ///sq
        {
            m_ui->viewArea->addCellItem(prop, tr("SQ"), QString(":/quality/lb_sq_quality"));
        }
        else if((prop.m_bitrate > MB_320 && m_queryType == MusicAbstractQueryRequest::QueryType::Music) ||
                (prop.m_bitrate >= MB_1000 && m_queryType == MusicAbstractQueryRequest::QueryType::Movie)) ///cd
        {
            m_ui->viewArea->addCellItem(prop, tr("CD"), QString(":/quality/lb_cd_quality"));
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
    setMoveWidget(m_ui->downloadPathEdit, delta);
    setMoveWidget(m_ui->pathChangedButton, delta);
    setMoveWidget(m_ui->settingButton, delta);
    setMoveWidget(m_ui->downloadButton, delta);

    setBackgroundPixmap(size());
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
    const QString &path = MusicUtils::File::getExistingDirectory(nullptr);
    if(!path.isEmpty())
    {
        if(m_queryType == MusicAbstractQueryRequest::QueryType::Music)
        {
            G_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicDirPath, path);
        }
        m_ui->downloadPathEdit->setText(path);
    }
}

void MusicDownloadWidget::startRequest()
{
    if(m_ui->viewArea->currentItemRole().isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    hide(); ///hide download widget
    if(m_queryType == MusicAbstractQueryRequest::QueryType::Music)
    {
        m_querySingleInfo ? startRequestMusic(m_songInfo) : startRequestMusic();
    }
    else if(m_queryType == MusicAbstractQueryRequest::QueryType::Movie)
    {
        m_querySingleInfo ? startRequestMovie(m_songInfo) : startRequestMovie();
    }
    controlEnabled(false);
}

void MusicDownloadWidget::dataDownloadFinished()
{
    Q_EMIT dataDownloadChanged();
    close();
}

void MusicDownloadWidget::startRequestMusic()
{
    const MusicObject::MusicSongInformation info(matchMusicSongInformation());
    if(!info.m_songName.isEmpty() || !info.m_singerName.isEmpty())
    {
        startRequestMusic(info);
    }
}

void MusicDownloadWidget::startRequestMusic(const MusicObject::MusicSongInformation &info)
{
    const MusicDownloadTableItemRole &role = m_ui->viewArea->currentItemRole();
    const MusicObject::MusicSongPropertyList &props = info.m_songProps;
    for(const MusicObject::MusicSongProperty &prop : qAsConst(props))
    {
        if(role.isEqual(MusicDownloadTableItemRole(prop.m_bitrate, prop.m_format, prop.m_size)))
        {
            if(!G_NETWORK_PTR->isOnline())
            {
                return;
            }

            QString musicSong = info.m_singerName + " - " + info.m_songName;
            const QString &downloadPrefix = m_ui->downloadPathEdit->text().isEmpty() ? MUSIC_DIR_FULL : m_ui->downloadPathEdit->text();
            QString downloadName = QString("%1%2.%3").arg(downloadPrefix, musicSong, prop.m_format);

            MusicDownloadRecordConfigManager down(MusicObject::Record::NormalDownload, this);
            if(!down.fromFile())
            {
                return;
            }

            MusicSongList records;
            down.readBuffer(records);

            MusicSong record;
            record.setName(musicSong);
            record.setPath(QFileInfo(downloadName).absoluteFilePath());
            record.setSizeStr(prop.m_size);
            record.setAddTimeStr("-1");
            records << record;
            down.writeBuffer(records);

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

            MusicDownloadTagDataRequest *d = new MusicDownloadTagDataRequest(prop.m_url, downloadName, MusicObject::Download::Music, this);
            d->setRecordType(MusicObject::Record::NormalDownload);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(dataDownloadFinished()));

            MusicSongMeta meta;
            meta.setComment(info.m_coverUrl);
            meta.setTitle(info.m_songName);
            meta.setArtist(info.m_singerName);
            meta.setAlbum(info.m_albumName);
            meta.setTrackNum(info.m_trackNumber);
            meta.setYear(info.m_year);

            d->setSongMeta(meta);
            d->startRequest();
            break;
        }
    }
}

void MusicDownloadWidget::startRequestMovie()
{
    const MusicObject::MusicSongInformation info(matchMusicSongInformation());
    if(!info.m_songName.isEmpty() || !info.m_singerName.isEmpty())
    {
        startRequestMovie(info);
    }
}

void MusicDownloadWidget::startRequestMovie(const MusicObject::MusicSongInformation &info)
{
    const MusicDownloadTableItemRole &role = m_ui->viewArea->currentItemRole();
    const MusicObject::MusicSongPropertyList &props = info.m_songProps;
    for(const MusicObject::MusicSongProperty &prop : qAsConst(props))
    {
        if(role.isEqual(MusicDownloadTableItemRole(prop.m_bitrate, prop.m_format, prop.m_size)))
        {
            if(!G_NETWORK_PTR->isOnline())
            {
                return;
            }

            QString musicSong = info.m_singerName + " - " + info.m_songName;
            const QString &downloadPrefix = m_ui->downloadPathEdit->text().isEmpty() ? MOVIE_DIR_FULL : m_ui->downloadPathEdit->text();

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

            MusicDownloadDataRequest *d = new MusicDownloadDataRequest(prop.m_url, downloadName, MusicObject::Download::Video, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(dataDownloadFinished()));
            d->startRequest();
        }
    }
}
