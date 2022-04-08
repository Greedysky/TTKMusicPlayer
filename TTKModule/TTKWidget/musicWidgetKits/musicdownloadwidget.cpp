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
    m_information = new QLabel(this);
    m_icon = new QLabel(this);
    m_text = new QLabel(this);

    m_text->setGeometry(0, 0, 60, ITEM_ROW_HEIGHT_S);
    m_icon->setGeometry(70, 0, 30, ITEM_ROW_HEIGHT_S);
    m_information->setGeometry(170, 0, 150, ITEM_ROW_HEIGHT_S);
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
    clearAllItems();
}

void MusicDownloadTableWidget::clearAllItems()
{
    qDeleteAll(m_items);
    m_items.clear();
    MusicAbstractTableWidget::clear();
    setColumnCount(1);
}

void MusicDownloadTableWidget::createItem(const MusicObject::MusicSongProperty &prop, const QString &type, const QString &icon)
{
    const int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, ITEM_ROW_HEIGHT_S);

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

void MusicDownloadTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}



MusicDownloadWidget::MusicDownloadWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicDownloadWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->downloadPathEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    m_ui->pathChangedButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    m_ui->settingButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    m_ui->downloadButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle06);
#ifdef Q_OS_UNIX
    m_ui->pathChangedButton->setFocusPolicy(Qt::NoFocus);
    m_ui->settingButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_querySingleInfo = false;
    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);

    m_queryType = MusicAbstractQueryRequest::MusicQuery;
    m_ui->loadingLabel->setType(MusicGifLabelWidget::CicleBlue);

    connect(m_ui->pathChangedButton, SIGNAL(clicked()), SLOT(downloadDirSelected()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(startToDownload()));
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

    if(m_queryType == MusicAbstractQueryRequest::MovieQuery)
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
    m_queryType = type;
    m_singleSongInfo = info;
    m_querySingleInfo = true;

    initialize();
    m_ui->downloadName->setText(MusicUtils::Widget::elidedText(font(), QString("%1 - %2").arg(info.m_singerName, info.m_songName), Qt::ElideRight, 200));

    createAllItems(info.m_songProps);
}

void MusicDownloadWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveWidget::show();
}

void MusicDownloadWidget::downLoadFinished()
{
    if(!G_NETWORK_PTR->isOnline())
    {
        return;
    }

    m_ui->viewArea->clearAllItems();
    const MusicObject::MusicSongInformation musicSongInfo(matchMusicSongInformation());
    if(!musicSongInfo.m_songName.isEmpty() || !musicSongInfo.m_singerName.isEmpty())
    {
        createAllItems(musicSongInfo.m_songProps);
    }
    else
    {
        close();
        MusicToastLabel::popup(tr("No resource found!"));
    }
}

MusicObject::MusicSongInformation MusicDownloadWidget::matchMusicSongInformation()
{
    const MusicObject::MusicSongInformationList musicSongInfos(m_networkRequest->musicSongInfoList());
    if(!musicSongInfos.isEmpty())
    {
        const QString &fileName = m_networkRequest->queryText();
        const QString &artistName = MusicUtils::String::artistName(fileName);
        const QString &songName = MusicUtils::String::songName(fileName);

        MusicObject::MusicSongInformation musicSongInfo;
        for(const MusicObject::MusicSongInformation &var : qAsConst(musicSongInfos))
        {
            if(var.m_singerName.contains(artistName, Qt::CaseInsensitive) && var.m_songName.contains(songName, Qt::CaseInsensitive))
            {
                musicSongInfo = var;
                break;
            }
        }
        std::sort(musicSongInfo.m_songProps.begin(), musicSongInfo.m_songProps.end()); //to find out the min bitrate
        return musicSongInfo;
    }
    return MusicObject::MusicSongInformation();
}

void MusicDownloadWidget::createAllItems(const MusicObject::MusicSongPropertyList &props)
{
    MusicObject::MusicSongPropertyList propertys = props;
    std::sort(propertys.begin(), propertys.end()); //to find out the min bitrate

    for(const MusicObject::MusicSongProperty &prop : qAsConst(propertys))
    {
        if((prop.m_bitrate == MB_128 && m_queryType == MusicAbstractQueryRequest::MusicQuery) ||
           (prop.m_bitrate <= MB_250 && m_queryType == MusicAbstractQueryRequest::MovieQuery))       ///sd
        {
            m_ui->viewArea->createItem(prop, tr("SD"), QString(":/quality/lb_sd_quality"));
        }
        else if((prop.m_bitrate == MB_192 && m_queryType == MusicAbstractQueryRequest::MusicQuery) ||
                (prop.m_bitrate == MB_500 && m_queryType == MusicAbstractQueryRequest::MovieQuery))  ///hd
        {
            m_ui->viewArea->createItem(prop, tr("HQ"), QString(":/quality/lb_hd_quality"));
        }
        else if((prop.m_bitrate == MB_320 && m_queryType == MusicAbstractQueryRequest::MusicQuery) ||
                (prop.m_bitrate == MB_750 && m_queryType == MusicAbstractQueryRequest::MovieQuery))  ///sq
        {
            m_ui->viewArea->createItem(prop, tr("SQ"), QString(":/quality/lb_sq_quality"));
        }
        else if((prop.m_bitrate > MB_320 && m_queryType == MusicAbstractQueryRequest::MusicQuery) ||
                (prop.m_bitrate >= MB_1000 && m_queryType == MusicAbstractQueryRequest::MovieQuery)) ///cd
        {
            m_ui->viewArea->createItem(prop, tr("CD"), QString(":/quality/lb_cd_quality"));
        }
        else
        {
            continue;
        }
    }

    resizeWindow();
}

void MusicDownloadWidget::resizeWindow()
{
    m_ui->loadingLabel->run(false);

    int delta = m_ui->viewArea->rowCount();
        delta = ((delta == 0) ? 0 : (delta - 1) * ITEM_ROW_HEIGHT_S) - 2 * ITEM_ROW_HEIGHT_S;

    setFixedHeightWidget(this, delta);
    setFixedHeightWidget(m_ui->backgroundMask, delta);
    setFixedHeightWidget(m_ui->background, delta);
    setFixedHeightWidget(m_ui->viewArea, delta + 2 * ITEM_ROW_HEIGHT_S);

    setMoveWidget(m_ui->label2, delta);
    setMoveWidget(m_ui->downloadPathEdit, delta);
    setMoveWidget(m_ui->pathChangedButton, delta);
    setMoveWidget(m_ui->settingButton, delta);
    setMoveWidget(m_ui->downloadButton, delta);

    setBackgroundPixmap(m_ui->background, size());
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
    const QString &path = MusicUtils::File::openDirectoryDialog(nullptr);
    if(!path.isEmpty())
    {
        if(m_queryType == MusicAbstractQueryRequest::MusicQuery)
        {
            G_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicDirPath, path + TTK_SEPARATOR);
        }
        m_ui->downloadPathEdit->setText(path + TTK_SEPARATOR);
    }
}

void MusicDownloadWidget::startToDownload()
{
    if(m_ui->viewArea->currentItemRole().isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    hide(); ///hide download widget
    if(m_queryType == MusicAbstractQueryRequest::MusicQuery)
    {
        m_querySingleInfo ? startToDownloadMusic(m_singleSongInfo) : startToDownloadMusic();
    }
    else if(m_queryType == MusicAbstractQueryRequest::MovieQuery)
    {
        m_querySingleInfo ? startToDownloadMovie(m_singleSongInfo) : startToDownloadMovie();
    }
    controlEnabled(false);
}

void MusicDownloadWidget::dataDownloadFinished()
{
    Q_EMIT dataDownloadChanged();
    close();
}

void MusicDownloadWidget::startToDownloadMusic()
{
    const MusicObject::MusicSongInformation musicSongInfo(matchMusicSongInformation());
    if(!musicSongInfo.m_songName.isEmpty() || !musicSongInfo.m_singerName.isEmpty())
    {
        startToDownloadMusic(musicSongInfo);
    }
}

void MusicDownloadWidget::startToDownloadMusic(const MusicObject::MusicSongInformation &musicSongInfo)
{
    const MusicDownloadTableItemRole &role = m_ui->viewArea->currentItemRole();
    const MusicObject::MusicSongPropertyList &props = musicSongInfo.m_songProps;
    for(const MusicObject::MusicSongProperty &prop : qAsConst(props))
    {
        if(role.isEqual(MusicDownloadTableItemRole(prop.m_bitrate, prop.m_format, prop.m_size)))
        {
            if(!G_NETWORK_PTR->isOnline())
            {
                return;
            }

            QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
            const QString &downloadPrefix = m_ui->downloadPathEdit->text().isEmpty() ? MUSIC_DIR_FULL : m_ui->downloadPathEdit->text();
            QString downloadName = QString("%1%2.%3").arg(downloadPrefix, musicSong, prop.m_format);

            MusicDownloadRecordConfigManager down(MusicObject::RecordNormalDownload, this);
            if(!down.fromFile())
            {
                return;
            }

            MusicSongList records;
            down.readBuffer(records);

            MusicSong record;
            record.setMusicName(musicSong);
            record.setMusicPath(QFileInfo(downloadName).absoluteFilePath());
            record.setMusicSizeStr(prop.m_size);
            record.setMusicAddTimeStr("-1");
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

            MusicDownloadTagDataRequest *downSong = new MusicDownloadTagDataRequest(prop.m_url, downloadName, MusicObject::DownloadMusic, this);
            downSong->setRecordType(MusicObject::RecordNormalDownload);
            connect(downSong, SIGNAL(downLoadDataChanged(QString)), SLOT(dataDownloadFinished()));

            MusicSongMeta meta;
            meta.setComment(musicSongInfo.m_coverUrl);
            meta.setTitle(musicSongInfo.m_songName);
            meta.setArtist(musicSongInfo.m_singerName);
            meta.setAlbum(musicSongInfo.m_albumName);
            meta.setTrackNum(musicSongInfo.m_trackNumber);
            meta.setYear(musicSongInfo.m_year);

            downSong->setSongMeta(meta);
            downSong->startToDownload();
            break;
        }
    }
}

void MusicDownloadWidget::startToDownloadMovie()
{
    const MusicObject::MusicSongInformation musicSongInfo(matchMusicSongInformation());
    if(!musicSongInfo.m_songName.isEmpty() || !musicSongInfo.m_singerName.isEmpty())
    {
        startToDownloadMovie(musicSongInfo);
    }
}

void MusicDownloadWidget::startToDownloadMovie(const MusicObject::MusicSongInformation &musicSongInfo)
{
    const MusicDownloadTableItemRole &role = m_ui->viewArea->currentItemRole();
    const MusicObject::MusicSongPropertyList &props = musicSongInfo.m_songProps;
    for(const MusicObject::MusicSongProperty &prop : qAsConst(props))
    {
        if(role.isEqual(MusicDownloadTableItemRole(prop.m_bitrate, prop.m_format, prop.m_size)))
        {
            if(!G_NETWORK_PTR->isOnline())
            {
                return;
            }

            QString musicSong = musicSongInfo.m_singerName + " - " + musicSongInfo.m_songName;
            const QString &downloadPrefix = m_ui->downloadPathEdit->text().isEmpty() ? MOVIE_DIR_FULL : m_ui->downloadPathEdit->text();
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
            MusicDownloadDataRequest *download = new MusicDownloadDataRequest(prop.m_url, downloadName, MusicObject::DownloadVideo, this);
            connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(dataDownloadFinished()));
            download->startToDownload();
        }
    }
}
