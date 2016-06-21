#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicutils.h"

MusicSongsListItemInfoWidget::MusicSongsListItemInfoWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MusicSongsListItemInfoWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

MusicSongsListItemInfoWidget::~MusicSongsListItemInfoWidget()
{
    delete ui;
}

bool MusicSongsListItemInfoWidget::showArtPicture(const QString &name)
{
    QPixmap originPath(QString(ART_DIR_FULL + name + SKN_FILE));
    if(!originPath.isNull())
    {
        ui->artPicture->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}

void MusicSongsListItemInfoWidget::setMusicSongInformation(const MusicSong &info)
{
    QString musicArt = info.getMusicArtistFront();
    ui->songNameValue->setText( info.getMusicName().isEmpty() ? "-" :
                MusicUtils::elidedText(font(), info.getMusicName(), Qt::ElideRight, ui->songNameValue->width()) );
    ui->artlistValue->setText( musicArt.isEmpty() ? "-" :
                MusicUtils::elidedText(font(), musicArt, Qt::ElideRight, ui->artlistValue->width()) );
    ui->sizeValue->setText( MusicUtils::elidedText(font(), QString::number(MusicUtils::sizeByte2MByte(
                            info.getMusicSize())).left(4) + "M", Qt::ElideRight, ui->sizeValue->width()) );
    ui->typeValue->setText( info.getMusicType().isEmpty() ? "-" :
                MusicUtils::elidedText(font(), info.getMusicType(), Qt::ElideRight, ui->typeValue->width()) );
    ui->timeValue->setText(
                MusicUtils::elidedText(font(), QString::number(info.getMusicPlayCount()), Qt::ElideRight, ui->timeValue->width()) );

    if(!showArtPicture(musicArt) && !showArtPicture(info.getMusicArtistBack()))
    {
        ui->artPicture->setPixmap(QPixmap(":/share/defaultArt").scaled(60, 60));
    }
}
