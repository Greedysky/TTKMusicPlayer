#include "musicsongslistplaywidget.h"
#include "musicsongtag.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicutils.h"
#include "musicconnectionpool.h"
#include "musicsongsharingwidget.h"
#include "musicsettingmanager.h"
#include "musicapplication.h"
#include "musicrightareawidget.h"
#include "musicleftareawidget.h"
#include "musicttkuiobject.h"

MusicSongsEnterPlayWidget::MusicSongsEnterPlayWidget(int index, QWidget *parent)
    : QWidget(parent), m_currentPlayIndex(index)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base,QBrush(QColor(0, 0, 0, 40)));
    setPalette(pal);
    setAutoFillBackground(true);
}

QString MusicSongsEnterPlayWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsEnterPlayWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    emit enterChanged(m_currentPlayIndex, -1);
}


MusicSongsListPlayWidget::MusicSongsListPlayWidget(int index, QWidget *parent)
    : MusicSongsEnterPlayWidget(index, parent), m_renameLine(nullptr)
{
    m_totalTime = "/00:00";

    m_artPictureLabel = new QLabel(this);
    m_artPictureLabel->setFixedSize(60, 60);
    m_artPictureLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_artPictureLabel->setGeometry(0, 0, 60, 60);

    m_songNameLabel = new QLabel(this);
    m_songNameLabel->setFixedSize(202, 25);
    m_songNameLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_songNameLabel->setStyleSheet(MusicUIObject::MCustomStyle11);
    m_songNameLabel->setGeometry(65, 5, 182, 25);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setFixedSize(100, 20);
    m_timeLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_timeLabel->setStyleSheet(MusicUIObject::MCustomStyle11);
    m_timeLabel->setGeometry(65, 37, 100, 20);

    m_columnOne = new MusicSongsEnterPlayWidget(index, this);
    m_columnThree = new MusicSongsEnterPlayWidget(index, this);

    m_loveButton = new QPushButton(this);
    m_loveButton->setGeometry(204, 40, 16, 16);
    m_loveButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_loveButton->setToolTip(tr("bestlove"));
    currentLoveStateClicked();

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(225, 40, 16, 16);
    m_deleteButton->setStyleSheet(MusicTTKUIObject::MKGTinyBtnDelete);
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("deleteMusic"));

    m_downloadButton = new QPushButton(this);
    m_downloadButton->setGeometry(180, 40, 16, 16);
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setToolTip(tr("songDownload"));
    currentDownloadStateClicked();

    QPushButton *addButton = new QPushButton(m_columnOne);
    addButton->setGeometry(2, 20, 16, 16);
    addButton->setStyleSheet(MusicTTKUIObject::MKGTinyBtnPlayLater);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    addButton->setToolTip(tr("playLater"));

    m_showMVButton = new QPushButton(m_columnThree);
    m_showMVButton->setGeometry(0, 39, 16, 16);
    m_showMVButton->setStyleSheet(MusicTTKUIObject::MKGTinyBtnMV);
    m_showMVButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_showMVButton->setToolTip(tr("showMV"));

    m_moreButton = new QPushButton(m_columnThree);
    m_moreButton->setGeometry(23, 39, 16, 16);
    m_moreButton->setStyleSheet(MusicTTKUIObject::MKGTinyBtnMore);
    m_moreButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_moreButton->setToolTip(tr("moreFunction"));

    QMenu *menu = new QMenu(m_columnThree);
    createMoreMenu(menu);
    m_moreButton->setMenu(menu);

    connect(m_loveButton, SIGNAL(clicked()), SIGNAL(currentLoveStateChanged()));
    connect(m_downloadButton, SIGNAL(clicked()), SIGNAL(currentDownloadStateChanged()));
    connect(m_deleteButton, SIGNAL(clicked()), parent, SLOT(setDeleteItemAt()));
    connect(this, SIGNAL(renameFinished(QString)), parent, SLOT(setItemRenameFinished(QString)));
    connect(this, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_columnOne, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_columnThree, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_showMVButton, SIGNAL(clicked()), SLOT(showMVButtonClicked()));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicRightAreaWidget::getClassName());
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicApplication::getClassName());
    M_CONNECTION_PTR->poolConnect(MusicLeftAreaWidget::getClassName(), getClassName());
}

MusicSongsListPlayWidget::~MusicSongsListPlayWidget()
{
    M_CONNECTION_PTR->poolDisConnect(getClassName());
    delete m_renameLine;
    delete m_artPictureLabel;
    delete m_songNameLabel;
    delete m_timeLabel;
    delete m_loveButton;
    delete m_deleteButton;
    delete m_showMVButton;
    delete m_downloadButton;
    delete m_moreButton;
    delete m_columnOne;
    delete m_columnThree;
}

QString MusicSongsListPlayWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsListPlayWidget::getWidget(QWidget *&one, QWidget *&two) const
{
    one = m_columnOne;
    two = m_columnThree;
}

void MusicSongsListPlayWidget::insertTimerLabel(const QString &t) const
{
    m_timeLabel->setText(t + m_totalTime);
}

void MusicSongsListPlayWidget::updateCurrentArtist()
{
    QString name = m_songNameLabel->toolTip();
    if(!showArtPicture(name.split('-').front().trimmed()) &&
       !showArtPicture(name.split('-').back().trimmed()) )
    {
        m_artPictureLabel->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QMenu *addMenu = menu->addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu->addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"));
    menu->addAction(QIcon(":/contextMenu/btn_ring"), tr("ringToMobile"));
    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(sharingButtonClicked()));
    menu->addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"));
}

bool MusicSongsListPlayWidget::showArtPicture(const QString &name) const
{
    QPixmap originPath(QString(ART_DIR_FULL + name + SKN_FILE));
    if(!originPath.isNull())
    {
        m_artPictureLabel->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}

void MusicSongsListPlayWidget::setParameter(const QString &name, const QString &path)
{
    MusicSongTag tag;
    if(tag.readFile(path))
    {
        m_totalTime = "/" + tag.getLengthString();
    }
    m_songNameLabel->setText(MusicUtils::UWidget::elidedText(font(), name, Qt::ElideRight, 180));
    m_songNameLabel->setToolTip(name);
    m_timeLabel->setText("00:00" + m_totalTime);

    if(!showArtPicture(name.split('-').front().trimmed()) &&
       !showArtPicture(name.split('-').back().trimmed()) )
    {
        m_artPictureLabel->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setItemRename()
{
    m_renameLine = new MusicSongsToolItemRenamedWidget(m_songNameLabel->toolTip(), this);
    connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(setChangItemName(QString)));
    m_renameLine->setFixedSize(182, 25);
    m_renameLine->setGeometry(65, 5, 182, 25);
    m_renameLine->show();
}

void MusicSongsListPlayWidget::deleteRenameItem()
{
    delete m_renameLine;
    m_renameLine = nullptr;
}

void MusicSongsListPlayWidget::setChangItemName(const QString &name)
{
    m_songNameLabel->setText(MusicUtils::UWidget::elidedText(font(), name, Qt::ElideRight, 180));
    m_songNameLabel->setToolTip(name);
    emit renameFinished(name);
    QTimer::singleShot(MT_MS, this, SLOT(deleteRenameItem()));
}

void MusicSongsListPlayWidget::showMVButtonClicked()
{
    emit videoButtonClicked(m_songNameLabel->toolTip());
}

void MusicSongsListPlayWidget::sharingButtonClicked()
{
    MusicSongSharingWidget shareWidget;
    shareWidget.setSongName(m_songNameLabel->toolTip());
    shareWidget.exec();
}

void MusicSongsListPlayWidget::currentLoveStateClicked()
{
    bool state = M_SETTING_PTR->value(MusicSettingManager::MuiscSongLovedChoiced).toBool();
    m_loveButton->setStyleSheet(state ? MusicTTKUIObject::MKGTinyBtnLove :
                                        MusicTTKUIObject::MKGTinyBtnUnLove);
}

void MusicSongsListPlayWidget::currentDownloadStateClicked()
{
    bool state = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicExistChoiced).toBool();
    m_downloadButton->setStyleSheet(state ? MusicTTKUIObject::MKGTinyBtnDownload :
                                            MusicTTKUIObject::MKGTinyBtnUnDownload);
}
