#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicnumberutils.h"
#include "musicwidgetutils.h"
#include "musicsettingmanager.h"
#include "musicsongtag.h"

MusicSongsListItemInfoWidget::MusicSongsListItemInfoWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongsListItemInfoWidget)
{
    m_ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::Tool);

    m_ui->songNameValue->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->artlistValue->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->sizeValue->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->typeValue->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->timeValue->setStyleSheet(MusicUIObject::MColorStyle03);
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

void MusicSongsListItemInfoWidget::setMusicSongInformation(const MusicSong &info)
{
    const QString &musicArtist = info.getMusicArtistFront();
    m_ui->songNameValue->setText( info.getMusicName().isEmpty() ? "-" : MusicUtils::Widget::elidedText(font(), info.getMusicName(), Qt::ElideRight, m_ui->songNameValue->width()) );
    m_ui->artlistValue->setText( musicArtist.isEmpty() ? "-" : MusicUtils::Widget::elidedText(font(), musicArtist, Qt::ElideRight, m_ui->artlistValue->width()) );
    m_ui->sizeValue->setText( MusicUtils::Widget::elidedText(font(), QString::number(MusicUtils::Number::sizeByte2MByte(info.getMusicSize())).left(4) + "M", Qt::ElideRight, m_ui->sizeValue->width()) );
    m_ui->typeValue->setText( info.getMusicType().isEmpty() ? "-" : MusicUtils::Widget::elidedText(font(), info.getMusicType(), Qt::ElideRight, m_ui->typeValue->width()) );
    m_ui->timeValue->setText( MusicUtils::Widget::elidedText(font(), QString::number(info.getMusicPlayCount()), Qt::ElideRight, m_ui->timeValue->width()) );

    if(M_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCoverChoiced).toBool())
    {
        MusicSongTag tag;
        if(tag.read(info.getMusicPath()))
        {
            QPixmap pix = tag.getCover();
            if(!pix.isNull())
            {
                m_ui->artPicture->setPixmap(pix.scaled(60, 60));
                return;
            }
        }
    }

    if(!showArtistPicture(musicArtist) && !showArtistPicture(info.getMusicArtistBack()))
    {
        m_ui->artPicture->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}
