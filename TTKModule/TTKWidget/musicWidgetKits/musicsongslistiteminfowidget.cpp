#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
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
    const QString &musicArtist = song.artistFront();
    m_ui->songNameValue->setText(song.name().isEmpty() ? TTK_DEFAULT_STR : MusicUtils::Widget::elidedText(font(), song.name(), Qt::ElideRight, m_ui->songNameValue->width()));
    m_ui->artlistValue->setText(musicArtist.isEmpty() ? TTK_DEFAULT_STR : MusicUtils::Widget::elidedText(font(), musicArtist, Qt::ElideRight, m_ui->artlistValue->width()));
    m_ui->sizeValue->setText(MusicUtils::Widget::elidedText(font(), song.sizeStr(), Qt::ElideRight, m_ui->sizeValue->width()));
    m_ui->typeValue->setText(song.type().isEmpty() ? TTK_DEFAULT_STR : MusicUtils::Widget::elidedText(font(), song.type(), Qt::ElideRight, m_ui->typeValue->width()));
    m_ui->timeValue->setText(MusicUtils::Widget::elidedText(font(), QString::number(song.playCount()), Qt::ElideRight, m_ui->timeValue->width()));

    if(G_SETTING_PTR->value(MusicSettingManager::OtherReadAlbumCover).toBool())
    {
        MusicSongMeta meta;
        if(meta.read(song.path()))
        {
            const QPixmap &pix = meta.cover();
            if(!pix.isNull())
            {
                m_ui->artPicture->setPixmap(pix.scaled(60, 60));
                return;
            }
        }
    }

    if(!showArtistPicture(musicArtist) && !showArtistPicture(song.artistBack()))
    {
        m_ui->artPicture->setPixmap(QPixmap(":/image/lb_default_art").scaled(60, 60));
    }
}
