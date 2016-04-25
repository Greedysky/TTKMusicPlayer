#include "musicsongslistplaywidget.h"
#include "musicsongtag.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"
#include "musicsongsharingwidget.h"
#include "musicsettingmanager.h"

MusicSongsEnterPlayWidget::MusicSongsEnterPlayWidget(int index, QWidget *parent)
    : QWidget(parent), m_currentPlayIndex(index)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base,QBrush(QColor(0, 0, 0, 100)));
    setPalette(pal);
    setAutoFillBackground(true);
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
    m_loveButton->setGeometry(214, 35, 23, 23);
    m_loveButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_loveButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_loveButton->setToolTip(tr("bestlove"));
    currentLoveStateClicked();

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(235, 35, 23, 23);
    m_deleteButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_deleteButton->setIcon(QIcon(":/image/musicdelete"));
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("deleteMusic"));

    m_downloadButton = new QPushButton(this);
    m_downloadButton->setGeometry(170, 35, 23, 23);
    m_downloadButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_downloadButton->setIconSize(QSize(23, 23));
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setToolTip(tr("songDownload"));
    currentDownloadStateClicked();

    m_showMVButton = new QPushButton(m_columnThree);
    m_showMVButton->setGeometry(0, 35, 23, 23);
    m_showMVButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_showMVButton->setIcon(QIcon(":/share/showMV2"));
    m_showMVButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_showMVButton->setToolTip(tr("showMV"));

    m_songShareButton = new QPushButton(m_columnThree);
    m_songShareButton->setGeometry(0, 7, 23, 23);
    m_songShareButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_songShareButton->setIcon(QIcon(":/image/songShare"));
    m_songShareButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_songShareButton->setToolTip(tr("songShare"));

    connect(m_loveButton, SIGNAL(clicked()), SIGNAL(currentLoveStateChanged()));
    connect(m_downloadButton, SIGNAL(clicked()), SIGNAL(currentDownloadStateChanged()));
    connect(m_deleteButton, SIGNAL(clicked()), parent, SLOT(setDeleteItemAt()));
    connect(this, SIGNAL(renameFinished(QString)), parent, SLOT(setItemRenameFinished(QString)));
    connect(this, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_columnOne, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_columnThree, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_showMVButton, SIGNAL(clicked()), SLOT(showMVButtonClicked()));
    connect(m_songShareButton, SIGNAL(clicked()), SLOT(sharingButtonClicked()));

    M_CONNECTION->setValue("MusicSongsListPlayWidget", this);
    M_CONNECTION->poolConnect("MusicSongsListPlayWidget", "MusicRightAreaWidget");
    M_CONNECTION->poolConnect("MusicSongsListPlayWidget", "MusicApplication");
    M_CONNECTION->poolConnect("MusicLeftAreaWidget", "MusicSongsListPlayWidget");
}

MusicSongsListPlayWidget::~MusicSongsListPlayWidget()
{
    M_CONNECTION->poolDisConnect("MusicSongsListPlayWidget");
    delete m_renameLine;
    delete m_artPictureLabel;
    delete m_songNameLabel;
    delete m_timeLabel;
    delete m_loveButton;
    delete m_deleteButton;
    delete m_showMVButton;
    delete m_downloadButton;
    delete m_songShareButton;
    delete m_columnOne;
    delete m_columnThree;
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
        m_artPictureLabel->setPixmap(QPixmap(":/share/defaultArt").scaled(60, 60));
    }
}

bool MusicSongsListPlayWidget::showArtPicture(const QString &name) const
{
    QPixmap originPath(QString(ART_DOWNLOAD_AL + name + SKN_FILE));
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
    m_songNameLabel->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 180));
    m_songNameLabel->setToolTip(name);
    m_timeLabel->setText("00:00" + m_totalTime);

    if(!showArtPicture(name.split('-').front().trimmed()) &&
       !showArtPicture(name.split('-').back().trimmed()) )
    {
        m_artPictureLabel->setPixmap(QPixmap(":/share/defaultArt").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setItemRename()
{
    m_renameLine = new MusicSongsToolItemRenamedWidget(0, m_songNameLabel->toolTip(), this);
    connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(setChangItemName(QString)));
    m_renameLine->setFixedSize(182, 25);
    m_renameLine->setGeometry(65, 5, 182, 25);
    m_renameLine->show();
}

void MusicSongsListPlayWidget::setChangItemName(const QString &name)
{
    m_songNameLabel->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 180));
    m_songNameLabel->setToolTip(name);
    emit renameFinished(name);
    delete m_renameLine;
    m_renameLine = nullptr;
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
    bool state = M_SETTING->value(MusicSettingManager::MuiscSongLovedChoiced).toBool();
    m_loveButton->setIcon(QIcon(state ? ":/image/loveOn" : ":/image/loveOff"));
}

void MusicSongsListPlayWidget::currentDownloadStateClicked()
{
    bool state = M_SETTING->value(MusicSettingManager::DownloadMusicExistChoiced).toBool();
    m_downloadButton->setIcon(QIcon(state ? ":/appTools/buttonmydownfn" : ":/appTools/buttonmydownl"));
}
