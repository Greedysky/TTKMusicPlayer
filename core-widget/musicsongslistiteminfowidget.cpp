#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musictime.h"

MusicSongsListItemInfoWidget::MusicSongsListItemInfoWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MusicSongsListItemInfoWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(MusicUIObject::MCustomStyle02);
}

MusicSongsListItemInfoWidget::~MusicSongsListItemInfoWidget()
{
    delete ui;
}

bool MusicSongsListItemInfoWidget::showArtPicture(const QString &name)
{
    QPixmap originPath(QString(ART_DOWNLOAD + name + SKN_FILE));
    if(!originPath.isNull())
    {
        ui->artPicture->setPixmap(originPath.scaled(60,60));
        return true;
    }
    return false;
}

void MusicSongsListItemInfoWidget::setMusicSongInformation(MusicSong &info)
{
    QString musicArt = info.getMusicArtistFront();
    QFontMetrics str(font());
    ui->songNameValue->setText( info.getMusicName().isEmpty() ? "-" :
                str.elidedText( info.getMusicName(), Qt::ElideRight, ui->songNameValue->width()) );
    ui->artlistValue->setText( musicArt.isEmpty() ? "-" :
               str.elidedText( musicArt, Qt::ElideRight, ui->artlistValue->width()) );
    ui->sizeValue->setText( str.elidedText( QString::number(MusicTime::fileSzieByte2MByte(
                            info.getMusicSize())).left(4) + "M", Qt::ElideRight, ui->sizeValue->width()) );
    ui->typeValue->setText( info.getMusicType().isEmpty() ? "-" :
            str.elidedText( info.getMusicType(), Qt::ElideRight, ui->typeValue->width()) );
    ui->timeValue->setText(
            str.elidedText( QString::number(info.getMusicPlayCount()), Qt::ElideRight, ui->timeValue->width()) );


    if(!showArtPicture(musicArt) && !showArtPicture(info.getMusicArtistBack()))
    {
        ui->artPicture->setPixmap(QPixmap(":/share/defaultArt").scaled(60,60));
    }
}
