#include "musicsongslistiteminfowidget.h"
#include "ui_musicsongslistiteminfowidget.h"
#include "musicsettingmanager.h"
#include "musicwidgetutils.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"

MusicSongsListItemInfoWidget::MusicSongsListItemInfoWidget(QWidget *parent)
    : TTKAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongsListItemInfoWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    setWindowFlags(windowFlags() | Qt::Tool);

    m_ui->songNameValue->setStyleSheet(TTK::UI::ColorStyle03);
    m_ui->artlistValue->setStyleSheet(TTK::UI::ColorStyle03);
    m_ui->sizeValue->setStyleSheet(TTK::UI::ColorStyle03);
    m_ui->typeValue->setStyleSheet(TTK::UI::ColorStyle03);
    m_ui->timeValue->setStyleSheet(TTK::UI::ColorStyle03);
}

MusicSongsListItemInfoWidget::~MusicSongsListItemInfoWidget()
{
    delete m_ui;
}

bool MusicSongsListItemInfoWidget::showArtistPicture(const QString &name)
{
    const QPixmap originPath(QString(ART_DIR_FULL + name + SKN_FILE));
    if(!originPath.isNull())
    {
        m_ui->artPicture->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}

void MusicSongsListItemInfoWidget::setSongInformation(int index, const MusicSong &song)
{
    const QString &musicArtist = song.artist();
    m_ui->songNameValue->setText(song.name().isEmpty() ? TTK_DEFAULT_STR : TTK::Widget::elidedText(font(), song.name(), Qt::ElideRight, m_ui->songNameValue->width()));
    m_ui->artlistValue->setText(musicArtist.isEmpty() ? TTK_DEFAULT_STR : TTK::Widget::elidedText(font(), musicArtist, Qt::ElideRight, m_ui->artlistValue->width()));
    m_ui->typeValue->setText(song.format().isEmpty() ? TTK_DEFAULT_STR : TTK::Widget::elidedText(font(), song.format(), Qt::ElideRight, m_ui->typeValue->width()));
    m_ui->timeValue->setText(TTK::Widget::elidedText(font(), QString::number(song.playCount()), Qt::ElideRight, m_ui->timeValue->width()));

    QString path = song.path();
    QString fileSize = song.sizeStr();
    if(index == MUSIC_NETWORK_LIST)
    {
        path = TTK::generateNetworkSongPath(path);
        fileSize = TTK::Number::sizeByteToLabel(QFileInfo(path).size());
    }
    m_ui->sizeValue->setText(TTK::Widget::elidedText(font(), fileSize, Qt::ElideRight, m_ui->sizeValue->width()));

    if(G_SETTING_PTR->value(MusicSettingManager::OtherReadAlbumCover).toBool())
    {
        MusicSongMeta meta;
        if(meta.read(path))
        {
            const QPixmap &pix = meta.cover();
            if(!pix.isNull())
            {
                m_ui->artPicture->setPixmap(pix.scaled(60, 60));
                return;
            }
        }
    }

    if(!showArtistPicture(musicArtist) && !showArtistPicture(song.title()))
    {
        m_ui->artPicture->setPixmap(QPixmap(":/image/lb_default_art").scaled(60, 60));
    }
}
