#include "musicsongslistplaywidget.h"
#include "musicsongmeta.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicstringutils.h"
#include "musicwidgetutils.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"
#include "musictinyuiobject.h"
#include "musicsplititemclickedlabel.h"
#include "musicwidgetheaders.h"

#include <QTimer>

MusicSongsListPlayWidget::MusicSongsListPlayWidget(int index, QWidget *parent)
    : QWidget(parent), m_renameLine(nullptr)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(0, 0, 0, 40)));
    setPalette(pal);
    setAutoFillBackground(true);

    m_noCover = false;
    m_currentPlayIndex = index;

    QPushButton *addButton = new QPushButton(this);
    addButton->setGeometry(2, 25, 16, 16);
    addButton->setStyleSheet(MusicUIObject::MQSSTinyBtnPlayLater);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    addButton->setToolTip(tr("Play Later"));

    m_artistPictureLabel = new QLabel(this);
    m_artistPictureLabel->setFixedSize(60, 60);
    m_artistPictureLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_artistPictureLabel->setGeometry(20, 0, 60, 60);

    m_songNameLabel = new MusicSplitItemClickedLabel(this);
    m_songNameLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_songNameLabel->setStyleSheet(MusicUIObject::MQSSColorStyle01);
    m_songNameLabel->setGeometry(85, 5, 200, 25);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setFixedSize(100, 20);
    m_timeLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_timeLabel->setStyleSheet(MusicUIObject::MQSSColorStyle01);
    m_timeLabel->setGeometry(85, 37, 100, 20);

    m_downloadButton = new QPushButton(this);
    m_downloadButton->setGeometry(175, 40, 16, 16);
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setToolTip(tr("Download"));
    currentDownloadStateClicked();

    m_showMVButton = new QPushButton(this);
    m_showMVButton->setGeometry(211, 39, 16, 16);
    m_showMVButton->setStyleSheet(MusicUIObject::MQSSTinyBtnMV);
    m_showMVButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_showMVButton->setToolTip(tr("Show Movie"));

    m_loveButton = new QPushButton(this);
    m_loveButton->setGeometry(231, 40, 16, 16);
    m_loveButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_loveButton->setToolTip(tr("Add To Lovest"));
    currentLoveStateClicked();

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(251, 40, 16, 16);
    m_deleteButton->setStyleSheet(MusicUIObject::MQSSTinyBtnDelete);
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("Delete"));

    m_moreButton = new QPushButton(this);
    m_moreButton->setGeometry(271, 39, 16, 16);
    m_moreButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle11 + MusicUIObject::MQSSTinyBtnMore);
    m_moreButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_moreButton->setToolTip(tr("More"));

#ifdef Q_OS_UNIX
    addButton->setFocusPolicy(Qt::NoFocus);
    m_downloadButton->setFocusPolicy(Qt::NoFocus);
    m_showMVButton->setFocusPolicy(Qt::NoFocus);
    m_loveButton->setFocusPolicy(Qt::NoFocus);
    m_deleteButton->setFocusPolicy(Qt::NoFocus);
    m_moreButton->setFocusPolicy(Qt::NoFocus);
#endif

    QMenu *menu = new QMenu(this);
    createMoreMenu(menu);
    m_moreButton->setMenu(menu);

    connect(m_loveButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicAddSongToLovestList()));
    connect(m_downloadButton, SIGNAL(clicked()), MusicLeftAreaWidget::instance(), SLOT(musicDownloadSongToLocal()));
    connect(m_deleteButton, SIGNAL(clicked()), parent, SLOT(setDeleteItemAt()));
    connect(this, SIGNAL(renameFinished(QString)), parent, SLOT(setItemRenameFinished(QString)));
    connect(this, SIGNAL(enterChanged(int,int)), parent, SLOT(itemCellEntered(int,int)));
    connect(m_showMVButton, SIGNAL(clicked()), parent, SLOT(musicSongPlayedMovieFound()));
    connect(addButton, SIGNAL(clicked()), parent, SLOT(musicAddToPlayLater()));

    connect(MusicLeftAreaWidget::instance(), SIGNAL(currentLoveStateChanged()), SLOT(currentLoveStateClicked()));
    connect(MusicLeftAreaWidget::instance(), SIGNAL(currentDownloadStateChanged()), SLOT(currentDownloadStateClicked()));
}

MusicSongsListPlayWidget::~MusicSongsListPlayWidget()
{
    delete m_renameLine;
    delete m_artistPictureLabel;
    delete m_songNameLabel;
    delete m_timeLabel;
    delete m_loveButton;
    delete m_deleteButton;
    delete m_showMVButton;
    delete m_downloadButton;
    delete m_moreButton;
}

void MusicSongsListPlayWidget::updateTimeLabel(const QString &current, const QString &total)
{
    m_timeLabel->setText(current + total);
}

void MusicSongsListPlayWidget::updateCurrentArtist()
{
    if(!m_noCover && G_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCover).toBool())
    {
        return;
    }

    const QString &name = m_songNameLabel->toolTip().trimmed();
    if(!showArtistPicture(MusicUtils::String::artistName(name)) && !showArtistPicture(MusicUtils::String::songName(name)))
    {
        m_artistPictureLabel->setPixmap(QPixmap(":/image/lb_default_art").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setParameter(const QString &name, const QString &path, QString &time)
{
    MusicSongMeta meta;
    const bool state = meta.read(path);
    m_songNameLabel->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 198));
    m_songNameLabel->setToolTip(name);

    if(state)
    {
        time = meta.duration();
    }
    else
    {
        time = MUSIC_TIME_INIT;
        m_noCover = true;
    }

    m_timeLabel->setText(TTK_STRCAT(MUSIC_TIME_INIT, TTK_SEPARATOR) + time);

    if(state && G_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCover).toBool())
    {
        const QPixmap &pix = meta.cover();
        if(pix.isNull())
        {
            m_noCover = true;
        }
        else
        {
            m_noCover = false;
            m_artistPictureLabel->setPixmap(pix.scaled(60, 60));
            return;
        }
    }

    if(!showArtistPicture(MusicUtils::String::artistName(name)) && !showArtistPicture(MusicUtils::String::songName(name)))
    {
        m_artistPictureLabel->setPixmap(QPixmap(":/image/lb_default_art").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setItemRename()
{
    m_renameLine = new MusicSongsToolItemRenamedWidget(m_songNameLabel->toolTip(), this);
    connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(changItemName(QString)));
    m_renameLine->setGeometry(85, 5, 200, 25);
    m_renameLine->show();
}

void MusicSongsListPlayWidget::deleteRenameItem()
{
    delete m_renameLine;
    m_renameLine = nullptr;
}

void MusicSongsListPlayWidget::changItemName(const QString &name)
{
    m_songNameLabel->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 198));
    m_songNameLabel->setToolTip(name);
    Q_EMIT renameFinished(name);
    QTimer::singleShot(MT_MS, this, SLOT(deleteRenameItem()));
}

void MusicSongsListPlayWidget::currentLoveStateClicked()
{
    const bool state = MusicApplication::instance()->musicLovestContains();
    m_loveButton->setStyleSheet(state ? MusicUIObject::MQSSTinyBtnLove : MusicUIObject::MQSSTinyBtnUnLove);
}

void MusicSongsListPlayWidget::currentDownloadStateClicked()
{
    bool state = false;
    MusicApplication::instance()->musicDownloadContains(state);
    m_downloadButton->setStyleSheet(state ? MusicUIObject::MQSSTinyBtnDownload : MusicUIObject::MQSSTinyBtnUnDownload);
}

#if TTK_QT_VERSION_CHECK(6,0,0)
void MusicSongsListPlayWidget::enterEvent(QEnterEvent *event)
#else
void MusicSongsListPlayWidget::enterEvent(QEvent *event)
#endif
{
    QWidget::enterEvent(event);
    Q_EMIT enterChanged(m_currentPlayIndex, -1);
}

void MusicSongsListPlayWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("Similar"), parent(), SLOT(musicPlayedSimilarQueryWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("Share"), parent(), SLOT(musicSongPlayedSharedWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"), parent(), SLOT(musicSongPlayedKMicroWidget()));
}

bool MusicSongsListPlayWidget::showArtistPicture(const QString &name) const
{
    QPixmap originPath(QString(ART_DIR_FULL + name + SKN_FILE));
    if(!originPath.isNull())
    {
        m_artistPictureLabel->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}
