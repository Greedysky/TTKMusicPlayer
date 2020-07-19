#include "musicabstractsongslisttablewidget.h"
#include "musicurlutils.h"
#include "musictoastlabel.h"
#include "musicfileinformationwidget.h"
#include "musicrightareawidget.h"
#include "musicsongsharingwidget.h"
#include "musicdownloadwidget.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"

MusicAbstractSongsListTableWidget::MusicAbstractSongsListTableWidget(QWidget *parent)
    : MusicSmoothMovingTableWidget(parent)
{
    m_playRowIndex = 0;
    m_parentToolIndex = -1;
    m_musicSongs = nullptr;
    m_hasParentToolIndex = true;
}

MusicAbstractSongsListTableWidget::~MusicAbstractSongsListTableWidget()
{

}

void MusicAbstractSongsListTableWidget::setSongsFileName(MusicSongs *songs)
{
    m_musicSongs = songs;
    updateSongsFileName(*songs);
}

void MusicAbstractSongsListTableWidget::updateSongsFileName(const MusicSongs &songs)
{
    Q_UNUSED(songs);
}

void MusicAbstractSongsListTableWidget::selectRow(int index)
{
    MusicSmoothMovingTableWidget::selectRow(index);
}

int MusicAbstractSongsListTableWidget::allRowsHeight() const
{
    int height = 0;
    for(int i=0; i<rowCount(); ++i)
    {
        height += rowHeight(i);
    }
    return height;
}

void MusicAbstractSongsListTableWidget::setParentToolIndex(int index)
{
    m_parentToolIndex = index;
}

void MusicAbstractSongsListTableWidget::musicPlayClicked()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    Q_EMIT cellDoubleClicked(currentRow(), 0);
}

void MusicAbstractSongsListTableWidget::setDeleteItemAt()
{

}

void MusicAbstractSongsListTableWidget::setDeleteItemAll()
{
    selectAll();
    setDeleteItemAt();

    bool state = false;
    Q_EMIT isCurrentIndex(state);

    if(rowCount() == 0 && state)
    {
        MusicApplication::instance()->musicPlayIndex(-1);
    }
}

void MusicAbstractSongsListTableWidget::musicOpenFileDir()
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

    if(!MusicUtils::Url::openUrl(QFileInfo(path).absoluteFilePath()))
    {
        MusicToastLabel::popup(tr("The origin one does not exist!"));
    }
}

void MusicAbstractSongsListTableWidget::musicFileInformation()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicFileInformationWidget file;
    file.setFileInformation(getCurrentSongPath());
    file.exec();
}

void MusicAbstractSongsListTableWidget::musicSongMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched(getCurrentSongName(), QString());
}

void MusicAbstractSongsListTableWidget::musicAlbumQueryWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicAlbumFound(getCurrentSongName(), QString());
}

void MusicAbstractSongsListTableWidget::musicSimilarQueryWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound(getCurrentSongName());
}

void MusicAbstractSongsListTableWidget::musicSongSharedWidget()
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

void MusicAbstractSongsListTableWidget::musicSongDownload()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(getCurrentSongName(), MusicAbstractQueryRequest::MusicQuery);
    download->show();
}

void MusicAbstractSongsListTableWidget::musicSongPlayedMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched(getSongName(m_playRowIndex), QString());
}

void MusicAbstractSongsListTableWidget::musicPlayedSimilarQueryWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound(getSongName(m_playRowIndex));
}

void MusicAbstractSongsListTableWidget::musicSongPlayedSharedWidget()
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

void MusicAbstractSongsListTableWidget::musicSongPlayedKMicroWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicLeftAreaWidget::instance()->createSoundKMicroWidget(getSongName(m_playRowIndex));
}

void MusicAbstractSongsListTableWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    QMenu *addMenu = menu->addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), this, SLOT(musicSimilarQueryWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(musicSongSharedWidget()));
}

QString MusicAbstractSongsListTableWidget::getCurrentSongPath() const
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return QString();
    }

    return getSongPath(currentRow());
}

QString MusicAbstractSongsListTableWidget::getSongPath(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicPath().trimmed() : QString();
}

QString MusicAbstractSongsListTableWidget::getCurrentSongName() const
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return QString();
    }

    return getSongName(currentRow());
}

QString MusicAbstractSongsListTableWidget::getSongName(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicName().trimmed() : QString();
}
