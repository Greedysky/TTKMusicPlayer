#include "musicsongslistabstracttablewidget.h"
#include "musicurlutils.h"
#include "musicmessagebox.h"
#include "musicfileinformationwidget.h"
#include "musicrightareawidget.h"
#include "musicsongsharingwidget.h"
#include "musicdownloadwidget.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"

MusicSongsListAbstractTableWidget::MusicSongsListAbstractTableWidget(QWidget *parent)
    : MusicSmoothMovingTableWidget(parent)
{
    m_playRowIndex = 0;
    m_parentToolIndex = -1;
    m_musicSongs = nullptr;
    m_hasParentToolIndex = true;
}

MusicSongsListAbstractTableWidget::~MusicSongsListAbstractTableWidget()
{

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
    MusicSmoothMovingTableWidget::selectRow(index);
}

int MusicSongsListAbstractTableWidget::allRowsHeight() const
{
    int height = 0;
    for(int i=0; i<rowCount(); ++i)
    {
        height += rowHeight(i);
    }
    return height;
}

void MusicSongsListAbstractTableWidget::setParentToolIndex(int index)
{
    m_parentToolIndex = index;
}

void MusicSongsListAbstractTableWidget::musicPlayClicked()
{
    if(rowCount() == 0 || currentRow() < 0)
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
    emit isCurrentIndex(state);

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

    const QString &path = !m_musicSongs->isEmpty() ? m_musicSongs->at(currentRow()).getMusicPath() : QString();
    if(QFileInfo(path).baseName() == MusicUtils::Algorithm::mdII(getCurrentSongName(), ALG_DOWNLOAD_KEY, true))
    {
        //cache song should not allow open url
        return;
    }

    if(!MusicUtils::Url::openUrl(QFileInfo(path).absoluteFilePath(), true))
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

    MusicRightAreaWidget::instance()->musicVideoButtonSearched(getCurrentSongName(), QString());
}

void MusicSongsListAbstractTableWidget::musicAlbumFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicAlbumFound( getCurrentSongName(), QString() );
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

    QVariantMap data;
    data["songName"] = getCurrentSongName();

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setData(MusicSongSharingWidget::Song, data);
    shareWidget.exec();
}

void MusicSongsListAbstractTableWidget::musicSongDownload()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(getCurrentSongName(), MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongsListAbstractTableWidget::musicSongPlayedMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched(getSongName(m_playRowIndex), QString());
}

void MusicSongsListAbstractTableWidget::musicPlayedSimilarFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound( getSongName(m_playRowIndex) );
}

void MusicSongsListAbstractTableWidget::musicSongPlayedSharedWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QVariantMap data;
    data["songName"] = getSongName(m_playRowIndex);

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setData(MusicSongSharingWidget::Song, data);
    shareWidget.exec();
}

void MusicSongsListAbstractTableWidget::musicSongPlayedKMicroWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicLeftAreaWidget::instance()->createSoundKMicroWidget( getSongName(m_playRowIndex) );
}

void MusicSongsListAbstractTableWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QMenu *addMenu = menu->addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), this, SLOT(musicSimilarFoundWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(musicSongSharedWidget()));
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
