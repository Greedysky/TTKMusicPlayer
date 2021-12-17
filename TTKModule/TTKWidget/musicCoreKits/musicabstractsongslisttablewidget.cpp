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
    m_playRowIndex = -1;
    m_toolIndex = -1;
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

int MusicAbstractSongsListTableWidget::totalHeight() const
{
    int height = 0;
    for(int i=0; i<rowCount(); ++i)
    {
        height += rowHeight(i);
    }
    return height;
}

void MusicAbstractSongsListTableWidget::setToolIndex(int index)
{
    m_toolIndex = index;
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
        MusicApplication::instance()->musicPlayIndex(DEFAULT_NORMAL_LEVEL);
    }
}

void MusicAbstractSongsListTableWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    const QString &path = !m_musicSongs->isEmpty() ? m_musicSongs->at(currentRow()).musicPath() : QString();
    if(QFileInfo(path).baseName() == MusicUtils::Algorithm::mdII(currentSongName(), ALG_ARC_KEY, true))
    {
        //cache song should not allow open url
        return;
    }

    if(!MusicUtils::Url::openUrl(QFileInfo(path).absoluteFilePath()))
    {
        MusicToastLabel::popup(tr("The file has been moved or does not exist!"));
    }
}

void MusicAbstractSongsListTableWidget::musicFileInformation()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicFileInformationWidget file;
    file.setFileInformation(currentSongPath());
    file.exec();
}

void MusicAbstractSongsListTableWidget::musicSongMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched(currentSongName(), QString());
}

void MusicAbstractSongsListTableWidget::musicAlbumQueryWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicAlbumFound(currentSongName(), QString());
}

void MusicAbstractSongsListTableWidget::musicSimilarQueryWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound(currentSongName());
}

void MusicAbstractSongsListTableWidget::musicSongSharedWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QVariantMap data;
    data["songName"] = currentSongName();

    MusicSongSharingWidget shareWidget;
    shareWidget.setData(MusicSongSharingWidget::Song, data);
    shareWidget.exec();
}

void MusicAbstractSongsListTableWidget::musicSongDownload()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget;
    download->setSongName(currentSongName(), MusicAbstractQueryRequest::MusicQuery);
    download->show();
}

void MusicAbstractSongsListTableWidget::musicSongPlayedMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched(songName(m_playRowIndex), QString());
}

void MusicAbstractSongsListTableWidget::musicPlayedSimilarQueryWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound(songName(m_playRowIndex));
}

void MusicAbstractSongsListTableWidget::musicSongPlayedSharedWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QVariantMap data;
    data["songName"] = songName(m_playRowIndex);

    MusicSongSharingWidget shareWidget;
    shareWidget.setData(MusicSongSharingWidget::Song, data);
    shareWidget.exec();
}

void MusicAbstractSongsListTableWidget::musicSongPlayedKMicroWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicLeftAreaWidget::instance()->createSoundKMicroWidget(songName(m_playRowIndex));
}

void MusicAbstractSongsListTableWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("Similar"), this, SLOT(musicSimilarQueryWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("Share"), this, SLOT(musicSongSharedWidget()));
}

QString MusicAbstractSongsListTableWidget::currentSongPath() const
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return QString();
    }

    return songPath(currentRow());
}

QString MusicAbstractSongsListTableWidget::songPath(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).musicPath().trimmed() : QString();
}

QString MusicAbstractSongsListTableWidget::currentSongName() const
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return QString();
    }

    return songName(currentRow());
}

QString MusicAbstractSongsListTableWidget::songName(int index) const
{
    if(rowCount() == 0 || index < 0)
    {
        return QString();
    }

    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).musicName().trimmed() : QString();
}
