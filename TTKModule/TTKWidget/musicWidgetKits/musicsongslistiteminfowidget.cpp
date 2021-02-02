#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicnumberutils.h"
#include "musicwidgetutils.h"
#include "musicsettingmanager.h"
#include "musicsongmeta.h"

MusicSongsListItemInfoWidget::MusicSongsListItemInfoWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongsListItemInfoWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    setWindowFlags(windowFlags() | Qt::Tool);

    m_ui->songNameValue->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    m_ui->artlistValue->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    m_ui->sizeValue->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    m_ui->typeValue->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    m_ui->timeValue->setStyleSheet(MusicUIObject::MQSSColorStyle03);
}

MusicSongsListItemInfoWidget::~MusicSongsListItemInfoWidget()
{
    delete m_ui;
}

bool MusicSongsListItemInfoWidget::showArtistPicture(const QString &name)
{
    QPixmap originPath(QString(ART_DIR_FULL + name + SKN_FILE));
    if(!originPath.isNull())
    {
        m_ui->artPicture->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}

void MusicSongsListItemInfoWidget::setMusicSongInformation(const MusicSong &song)
{
    const QString &musicArtist = song.getMusicArtistFront();
    m_ui->songNameValue->setText(song.getMusicName().isEmpty() ? STRING_NULL : MusicUtils::Widget::elidedText(font(), song.getMusicName(), Qt::ElideRight, m_ui->songNameValue->width()));
    m_ui->artlistValue->setText(musicArtist.isEmpty() ? STRING_NULL : MusicUtils::Widget::elidedText(font(), musicArtist, Qt::ElideRight, m_ui->artlistValue->width()));
    m_ui->sizeValue->setText(MusicUtils::Widget::elidedText(font(), QString::number(MusicUtils::Number::sizeByte2MByte(song.getMusicSize())).left(4) + "M", Qt::ElideRight, m_ui->sizeValue->width()));
    m_ui->typeValue->setText(song.getMusicType().isEmpty() ? STRING_NULL : MusicUtils::Widget::elidedText(font(), song.getMusicType(), Qt::ElideRight, m_ui->typeValue->width()));
    m_ui->timeValue->setText(MusicUtils::Widget::elidedText(font(), QString::number(song.getMusicPlayCount()), Qt::ElideRight, m_ui->timeValue->width()));

    if(M_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCover).toBool())
    {
        MusicSongMeta meta;
        if(meta.read(song.getMusicPath()))
        {
            QPixmap pix = meta.getCover();
            if(!pix.isNull())
            {
                m_ui->artPicture->setPixmap(pix.scaled(60, 60));
                return;
            }
        }
    }

    if(!showArtistPicture(musicArtist) && !showArtistPicture(song.getMusicArtistBack()))
    {
        m_ui->artPicture->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}
