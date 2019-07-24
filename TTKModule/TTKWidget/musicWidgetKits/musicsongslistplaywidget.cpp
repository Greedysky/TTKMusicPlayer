#include "musicsongslistplaywidget.h"
#include "musicsongtag.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musicwidgetutils.h"
#include "musicnumberdefine.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"
#include "musictinyuiobject.h"
#include "musicsplititemclickedlabel.h"
#include "musicwidgetheaders.h"
#include "musicotherdefine.h"

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
    m_totalTimeLabel = QString("/") + MUSIC_TIME_INIT;

    QPushButton *addButton = new QPushButton(this);
    addButton->setGeometry(2, 25, 16, 16);
    addButton->setStyleSheet(MusicUIObject::MKGTinyBtnPlayLater);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    addButton->setToolTip(tr("playLater"));

    m_artistPictureLabel = new QLabel(this);
    m_artistPictureLabel->setFixedSize(60, 60);
    m_artistPictureLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_artistPictureLabel->setGeometry(20, 0, 60, 60);

    m_songNameLabel = new MusicSplitItemClickedLabel(this);
    m_songNameLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_songNameLabel->setStyleSheet(MusicUIObject::MColorStyle01);
    m_songNameLabel->setGeometry(85, 5, 200, 25);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setFixedSize(100, 20);
    m_timeLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_timeLabel->setStyleSheet(MusicUIObject::MColorStyle01);
    m_timeLabel->setGeometry(85, 37, 100, 20);

    m_downloadButton = new QPushButton(this);
    m_downloadButton->setGeometry(175, 40, 16, 16);
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setToolTip(tr("songDownload"));
    currentDownloadStateClicked();

    m_showMVButton = new QPushButton(this);
    m_showMVButton->setGeometry(211, 39, 16, 16);
    m_showMVButton->setStyleSheet(MusicUIObject::MKGTinyBtnMV);
    m_showMVButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_showMVButton->setToolTip(tr("showMV"));

    m_loveButton = new QPushButton(this);
    m_loveButton->setGeometry(231, 40, 16, 16);
    m_loveButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_loveButton->setToolTip(tr("bestlove"));
    currentLoveStateClicked();

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(251, 40, 16, 16);
    m_deleteButton->setStyleSheet(MusicUIObject::MKGTinyBtnDelete);
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("deleteMusic"));

    m_moreButton = new QPushButton(this);
    m_moreButton->setGeometry(271, 39, 16, 16);
    m_moreButton->setStyleSheet(MusicUIObject::MPushButtonStyle13 + MusicUIObject::MKGTinyBtnMore);
    m_moreButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_moreButton->setToolTip(tr("moreFunction"));

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

    connect(m_loveButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicAddSongToLovestListAt()));
    connect(m_downloadButton, SIGNAL(clicked()), MusicLeftAreaWidget::instance(), SLOT(musicDownloadSongToLocal()));
    connect(m_deleteButton, SIGNAL(clicked()), parent, SLOT(setDeleteItemAt()));
    connect(this, SIGNAL(renameFinished(QString)), parent, SLOT(setItemRenameFinished(QString)));
    connect(this, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
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
    if(m_totalTimeLabel.contains(MUSIC_TIME_INIT))
    {
        m_totalTimeLabel = total;
    }
    m_timeLabel->setText(current + m_totalTimeLabel);
}

void MusicSongsListPlayWidget::updateCurrentArtist()
{
    if(!m_noCover && M_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCoverChoiced).toBool())
    {
        return;
    }

    const QString &name = m_songNameLabel->toolTip().trimmed();
    if(!showArtistPicture(MusicUtils::String::artistName(name)) && !showArtistPicture(MusicUtils::String::songName(name)))
    {
        m_artistPictureLabel->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setParameter(const QString &name, const QString &path, QString &time)
{
    MusicSongTag tag;
    const bool state = tag.read(path);
    m_songNameLabel->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 198));
    m_songNameLabel->setToolTip(name);

    if(state)
    {
        time = tag.getLengthString();
        m_totalTimeLabel = "/" + time;
    }
    m_timeLabel->setText(MUSIC_TIME_INIT + m_totalTimeLabel);

    if(state && M_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCoverChoiced).toBool())
    {
        QPixmap pix = tag.getCover();
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
        m_artistPictureLabel->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setItemRename()
{
    m_renameLine = new MusicSongsToolItemRenamedWidget(m_songNameLabel->toolTip(), this);
    connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(setChangItemName(QString)));
    m_renameLine->setGeometry(85, 5, 200, 25);
    m_renameLine->show();
}

void MusicSongsListPlayWidget::deleteRenameItem()
{
    delete m_renameLine;
    m_renameLine = nullptr;
}

void MusicSongsListPlayWidget::setChangItemName(const QString &name)
{
    m_songNameLabel->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 198));
    m_songNameLabel->setToolTip(name);
    emit renameFinished(name);
    QTimer::singleShot(MT_MS, this, SLOT(deleteRenameItem()));
}

void MusicSongsListPlayWidget::currentLoveStateClicked()
{
    const bool state = MusicApplication::instance()->musicLovestContains();
    m_loveButton->setStyleSheet(state ? MusicUIObject::MKGTinyBtnLove : MusicUIObject::MKGTinyBtnUnLove);
}

void MusicSongsListPlayWidget::currentDownloadStateClicked()
{
    bool state = false;
    MusicApplication::instance()->musicDownloadContains(state);
    m_downloadButton->setStyleSheet(state ? MusicUIObject::MKGTinyBtnDownload : MusicUIObject::MKGTinyBtnUnDownload);
}

void MusicSongsListPlayWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    emit enterChanged(m_currentPlayIndex, -1);
}

void MusicSongsListPlayWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QMenu *addMenu = menu->addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), parent(), SLOT(musicPlayedSimilarFoundWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), parent(), SLOT(musicSongPlayedSharedWidget()));
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
