#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicnumberutils.h"
#include "musicwidgetutils.h"

MusicSongsListItemInfoWidget::MusicSongsListItemInfoWidget(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::MusicSongsListItemInfoWidget)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

MusicSongsListItemInfoWidget::~MusicSongsListItemInfoWidget()
{
    delete m_ui;
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
        m_ui->artPicture->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}

void MusicSongsListItemInfoWidget::setMusicSongInformation(const MusicSong &info)
{
    QString musicArt = info.getMusicArtistFront();
    m_ui->songNameValue->setText( info.getMusicName().isEmpty() ? "-" :
                MusicUtils::Widget::elidedText(font(), info.getMusicName(), Qt::ElideRight, m_ui->songNameValue->width()) );
    m_ui->artlistValue->setText( musicArt.isEmpty() ? "-" :
                MusicUtils::Widget::elidedText(font(), musicArt, Qt::ElideRight, m_ui->artlistValue->width()) );
    m_ui->sizeValue->setText( MusicUtils::Widget::elidedText(font(), QString::number(MusicUtils::Number::sizeByte2MByte(
                            info.getMusicSize())).left(4) + "M", Qt::ElideRight, m_ui->sizeValue->width()) );
    m_ui->typeValue->setText( info.getMusicType().isEmpty() ? "-" :
                MusicUtils::Widget::elidedText(font(), info.getMusicType(), Qt::ElideRight, m_ui->typeValue->width()) );
    m_ui->timeValue->setText(
                MusicUtils::Widget::elidedText(font(), QString::number(info.getMusicPlayCount()), Qt::ElideRight, m_ui->timeValue->width()) );

    if(!showArtPicture(musicArt) && !showArtPicture(info.getMusicArtistBack()))
    {
        m_ui->artPicture->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}
