#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musictime.h"
#include <QTime>

MusicSongsListItemInfoWidget::MusicSongsListItemInfoWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MusicSongsListItemInfoWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(MusicUIObject::MCustomStyle01);
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

void MusicSongsListItemInfoWidget::setMusicSongInformation(QMusicSong &info)
{
    QString musicArt = info.getMusicArtistFront();
    QFontMetrics str(font());
    ui->songNameValue->setText( info.getMusicName().isEmpty() ? "-" :
                 str.elidedText(info.getMusicName(), Qt::ElideRight, ui->songNameValue->width()));
    ui->artlistValue->setText(musicArt.isEmpty() ? "-" : musicArt);
    ui->sizeValue->setText(QString::number(MusicTime::fileSzieByte2MByte(info.getMusicSize())).left(4) + "M");
    ui->typeValue->setText(info.getMusicType().isEmpty() ? "-" : info.getMusicType());
    ui->timeValue->setText(QTime::currentTime().toString(Qt::ISODate));

    if(!showArtPicture(musicArt) && !showArtPicture(info.getMusicArtistBack()))
    {
        ui->artPicture->setPixmap(QPixmap(":/share/defaultArt").scaled(60,60));
    }
}
