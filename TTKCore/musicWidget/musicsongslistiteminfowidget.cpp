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

QString MusicSongsListItemInfoWidget::getClassName()
{
    return staticMetaObject.className();
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
                MusicUtils::UWidget::elidedText(font(), info.getMusicName(), Qt::ElideRight, ui->songNameValue->width()) );
    ui->artlistValue->setText( musicArt.isEmpty() ? "-" :
                MusicUtils::UWidget::elidedText(font(), musicArt, Qt::ElideRight, ui->artlistValue->width()) );
    ui->sizeValue->setText( MusicUtils::UWidget::elidedText(font(), QString::number(MusicUtils::UNumber::sizeByte2MByte(
                            info.getMusicSize())).left(4) + "M", Qt::ElideRight, ui->sizeValue->width()) );
    ui->typeValue->setText( info.getMusicType().isEmpty() ? "-" :
                MusicUtils::UWidget::elidedText(font(), info.getMusicType(), Qt::ElideRight, ui->typeValue->width()) );
    ui->timeValue->setText(
                MusicUtils::UWidget::elidedText(font(), QString::number(info.getMusicPlayCount()), Qt::ElideRight, ui->timeValue->width()) );

    if(!showArtPicture(musicArt) && !showArtPicture(info.getMusicArtistBack()))
    {
        ui->artPicture->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}
