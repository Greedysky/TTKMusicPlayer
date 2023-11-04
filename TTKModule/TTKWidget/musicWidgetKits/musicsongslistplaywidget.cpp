#include "musicsongslistplaywidget.h"
#include "musicsongmeta.h"
#include "musicitemrenameedit.h"
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
    : QWidget(parent),
      m_noCover(false),
      m_currentPlayIndex(index),
      m_renameEdit(nullptr)
{
    QPalette plt(palette());
    plt.setBrush(QPalette::Base, QBrush(QColor(0, 0, 0, 40)));
    setPalette(plt);
    setAutoFillBackground(true);

    QPushButton *addButton = new QPushButton(this);
    addButton->setGeometry(2, 25, 16, 16);
    addButton->setStyleSheet(TTK::UI::TinyBtnPlayLater);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    addButton->setToolTip(tr("Play Later"));

    m_artistPictureLabel = new QLabel(this);
    m_artistPictureLabel->setFixedSize(60, 60);
    m_artistPictureLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_artistPictureLabel->setGeometry(20, 0, 60, 60);

    m_songNameLabel = new MusicSplitItemClickedLabel(this);
    m_songNameLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_songNameLabel->setStyleSheet(TTK::UI::LabelStyle02);
    m_songNameLabel->setGeometry(85, 5, 200, 25);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setFixedSize(100, 20);
    m_timeLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_timeLabel->setStyleSheet(TTK::UI::ColorStyle06);
    m_timeLabel->setGeometry(85, 37, 100, 20);

    m_downloadButton = new QPushButton(this);
    m_downloadButton->setGeometry(175, 40, 16, 16);
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setToolTip(tr("Download"));
    currentDownloadStateClicked();

    m_showMVButton = new QPushButton(this);
    m_showMVButton->setGeometry(211, 39, 16, 16);
    m_showMVButton->setStyleSheet(TTK::UI::TinyBtnMV);
    m_showMVButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_showMVButton->setToolTip(tr("Show Movie"));

    m_loveButton = new QPushButton(this);
    m_loveButton->setGeometry(231, 40, 16, 16);
    m_loveButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_loveButton->setToolTip(tr("Add To Lovest"));
    currentLoveStateClicked();

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(251, 40, 16, 16);
    m_deleteButton->setStyleSheet(TTK::UI::TinyBtnDelete);
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("Delete"));

    m_moreButton = new QPushButton(this);
    m_moreButton->setGeometry(271, 39, 16, 16);
    m_moreButton->setStyleSheet(TTK::UI::PushButtonStyle10 + TTK::UI::TinyBtnMore);
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

    connect(m_loveButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(addSongToLovestList()));
    connect(m_downloadButton, SIGNAL(clicked()), MusicLeftAreaWidget::instance(), SLOT(downloadSongToLocal()));
    connect(m_deleteButton, SIGNAL(clicked()), parent, SLOT(removeItemAt()));
    connect(this, SIGNAL(renameFinished(QString)), parent, SLOT(setItemRenameFinished(QString)));
    connect(this, SIGNAL(enterChanged(int,int)), parent, SLOT(itemCellEntered(int,int)));
    connect(m_showMVButton, SIGNAL(clicked()), parent, SLOT(showPlayedMovieQueryWidget()));
    connect(addButton, SIGNAL(clicked()), parent, SLOT(addToPlayLater()));

    connect(MusicLeftAreaWidget::instance(), SIGNAL(currentLoveStateChanged()), SLOT(currentLoveStateClicked()));
    connect(MusicLeftAreaWidget::instance(), SIGNAL(currentDownloadStateChanged()), SLOT(currentDownloadStateClicked()));
}

MusicSongsListPlayWidget::~MusicSongsListPlayWidget()
{
    delete m_renameEdit;
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
    if(!m_noCover && G_SETTING_PTR->value(MusicSettingManager::OtherReadAlbumCover).toBool())
    {
        return;
    }

    const QString &name = m_songNameLabel->toolTip().trimmed();
    if(!showArtistPicture(TTK::String::artistName(name)) && !showArtistPicture(TTK::String::songName(name)))
    {
        m_artistPictureLabel->setPixmap(QPixmap(":/image/lb_default_art").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setParameter(const QString &name, const QString &path, QString &time)
{
    MusicSongMeta meta;
    const bool state = meta.read(path);
    m_songNameLabel->setText(TTK::Widget::elidedText(font(), name, Qt::ElideRight, 198));
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

    if(state && G_SETTING_PTR->value(MusicSettingManager::OtherReadAlbumCover).toBool())
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

    if(!showArtistPicture(TTK::String::artistName(name)) && !showArtistPicture(TTK::String::songName(name)))
    {
        m_artistPictureLabel->setPixmap(QPixmap(":/image/lb_default_art").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setItemRename()
{
    m_renameEdit = new MusicItemRenameEidt(m_songNameLabel->toolTip(), this);
    connect(m_renameEdit, SIGNAL(renameFinished(QString)), SLOT(changItemName(QString)));
    m_renameEdit->setGeometry(85, 5, 200, 25);
    m_renameEdit->show();
}

void MusicSongsListPlayWidget::deleteRenameItem()
{
    delete m_renameEdit;
    m_renameEdit = nullptr;
}

void MusicSongsListPlayWidget::changItemName(const QString &name)
{
    m_songNameLabel->setText(TTK::Widget::elidedText(font(), name, Qt::ElideRight, 198));
    m_songNameLabel->setToolTip(name);
    Q_EMIT renameFinished(name);
    QTimer::singleShot(MT_MS, this, SLOT(deleteRenameItem()));
}

void MusicSongsListPlayWidget::currentLoveStateClicked()
{
    const bool state = MusicApplication::instance()->containsLovestItem();
    m_loveButton->setStyleSheet(state ? TTK::UI::TinyBtnLove : TTK::UI::TinyBtnUnLove);
}

void MusicSongsListPlayWidget::currentDownloadStateClicked()
{
    bool state = false;
    MusicApplication::instance()->containsDownloadItem(state);
    m_downloadButton->setStyleSheet(state ? TTK::UI::TinyBtnDownload : TTK::UI::TinyBtnUnDownload);
}

void MusicSongsListPlayWidget::enterEvent(QtEnterEvent *event)
{
    QWidget::enterEvent(event);
    Q_EMIT enterChanged(m_currentPlayIndex, -1);
}

void MusicSongsListPlayWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(TTK::UI::MenuStyle02);
    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("Similar"), parent(), SLOT(showPlayedSimilarQueryWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("Share"), parent(), SLOT(showPlayedSongSharedWidget()));
}

bool MusicSongsListPlayWidget::showArtistPicture(const QString &name) const
{
    const QPixmap originPath(QString(ART_DIR_FULL + name + SKN_FILE));
    if(!originPath.isNull())
    {
        m_artistPictureLabel->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}
