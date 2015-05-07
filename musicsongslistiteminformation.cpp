#include "musicsongslistiteminformation.h"
#include "ui_musicsongslistiteminformation.h"
#include "core/musicobject.h"
#include <QTime>

MusicSongsListItemInformation::MusicSongsListItemInformation(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusicSongsListItemInformation)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color:rgba(255,255,255,125);");
}

MusicSongsListItemInformation::~MusicSongsListItemInformation()
{
    delete ui;
}

bool MusicSongsListItemInformation::showArtPicture(const QString& name)
{
    QPixmap originPath(QString(ART_DOWNLOAD + name + JPG_FILE));
    if(!originPath.isNull())
        ui->artPicture->setPixmap(originPath.scaled(60,60));
    else
        return false;
    return true;
}

void MusicSongsListItemInformation::setMusicSongInformation(QMusicSong& info)
{
    QString musicArt = info.getMusicArtistFront();
    QFontMetrics str(font());
    ui->songNameValue->setText( info.m_musicName.isEmpty() ? "-" :
       str.elidedText(info.m_musicName, Qt::ElideRight, ui->songNameValue->width()));
    ui->artlistValue->setText(musicArt.isEmpty() ? "-" : musicArt);
    ui->sizeValue->setText(QString::number(info.transSizeByte2MByte()).left(4)+"M");
    ui->typeValue->setText(info.m_musicType.isEmpty() ? "-" : info.m_musicType);
    ui->timeValue->setText(QTime::currentTime().toString(Qt::ISODate));

    if(!showArtPicture(musicArt) && !showArtPicture(info.getMusicArtistBack()))
    {
        ui->artPicture->setPixmap(QPixmap(":/share/defaultArt").scaled(60,60));
    }
}
