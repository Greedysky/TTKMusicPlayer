#include "musicsongslistabstracttablewidget.h"
#include "musiccoreutils.h"
#include "musicmessagebox.h"
#include "musicfileinformationwidget.h"
#include "musicrightareawidget.h"
#include "musicsongsharingwidget.h"
#include "musicdownloadwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicapplication.h"

MusicSongsListAbstractTableWidget::MusicSongsListAbstractTableWidget(QWidget *parent)
    : MusicSlowMovingTableWidget(parent)
{
    m_playRowIndex = 0;
    m_parentToolIndex = -1;
    m_hasParentToolIndex = true;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MusicSongsListAbstractTableWidget::~MusicSongsListAbstractTableWidget()
{

}

QString MusicSongsListAbstractTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsListAbstractTableWidget::setSongsFileName(MusicSongs *songs)
{
    m_musicSongs = songs;
    updateSongsFileName(*songs);
}

void MusicSongsListAbstractTableWidget::updateSongsFileName(const MusicSongs &songs)
{
    Q_UNUSED(songs);
}

void MusicSongsListAbstractTableWidget::selectRow(int index)
{
    MusicSlowMovingTableWidget::selectRow(index);
}

void MusicSongsListAbstractTableWidget::setParentToolIndex(int index)
{
    m_parentToolIndex = index;
}

void MusicSongsListAbstractTableWidget::musicPlayClicked()
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return;
    }
    emit cellDoubleClicked(currentRow(), 0);
}


void MusicSongsListAbstractTableWidget::setDeleteItemAt()
{

}

void MusicSongsListAbstractTableWidget::setDeleteItemAll()
{
    selectAll();
    setDeleteItemAt();

    bool state = false;
    emit isCurrentIndexs(state);
    if(rowCount() == 0 && state)
    {
        MusicApplication::instance()->musicPlayIndex(-1);
    }
}

void MusicSongsListAbstractTableWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QString path = !m_musicSongs->isEmpty() ? m_musicSongs->at(currentRow()).getMusicPath() : QString();
    if(QFileInfo(path).baseName() == MusicCryptographicHash::encryptData(getCurrentSongName(), DOWNLOAD_KEY))
    {
        //cache song should not allow open url
        return;
    }

    if(!MusicUtils::Core::openUrl(QFileInfo(path).absoluteFilePath(), true))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
    }
}

void MusicSongsListAbstractTableWidget::musicFileInformation()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicFileInformationWidget file;
    file.setFileInformation( getCurrentSongPath() );
    file.exec();
}

void MusicSongsListAbstractTableWidget::musicSongMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched( getCurrentSongName() );
}

void MusicSongsListAbstractTableWidget::musicAlbumFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicAlbumFound( getCurrentSongName() );
}

void MusicSongsListAbstractTableWidget::musicSimilarFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound( getCurrentSongName() );
}

void MusicSongsListAbstractTableWidget::musicSongSharedWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicSongSharingWidget shareWidget;
    shareWidget.setSongName( getCurrentSongName() );
    shareWidget.exec();
}

void MusicSongsListAbstractTableWidget::musicSongTransferWidget()
{
    if(m_musicSongs->isEmpty())
    {
        return;
    }

    int index = m_hasParentToolIndex ? m_parentToolIndex : m_musicSongs->first().getMusicToolIndex();
    MusicConnectTransferWidget transferWidget;
    transferWidget.redirectToCurrentSong(index, currentRow());
    transferWidget.exec();
}

void MusicSongsListAbstractTableWidget::musicSongDownload()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget;
    download->setSongName(getCurrentSongName(), MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongsListAbstractTableWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QMenu *addMenu = menu->addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu->addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"), this, SLOT(musicSongTransferWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_ring"), tr("ringToMobile"), this, SLOT(musicSongTransferWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), this, SLOT(musicSimilarFoundWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(musicSongSharedWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"));
}

QString MusicSongsListAbstractTableWidget::getCurrentSongPath() const
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return QString();
    }

    return getSongPath( currentRow() );
}

QString MusicSongsListAbstractTableWidget::getSongPath(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicPath().trimmed() : QString();
}

QString MusicSongsListAbstractTableWidget::getCurrentSongName() const
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return QString();
    }

    return getSongName( currentRow() );
}

QString MusicSongsListAbstractTableWidget::getSongName(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicName().trimmed() : QString();
}
