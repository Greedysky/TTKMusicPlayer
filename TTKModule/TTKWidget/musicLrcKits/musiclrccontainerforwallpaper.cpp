#include "musiclrccontainerforwallpaper.h"
#include "musicdesktopwallpaperthread.h"
#include "musiclrcmanagerforinterior.h"
#include "musiclayoutanimationwidget.h"
#include "musictransitionanimationlabel.h"
#include "musicbackgroundmanager.h"

MusicLrcContainerForWallpaper::MusicLrcContainerForWallpaper(QWidget *parent)
    : MusicLrcContainer(parent),
      m_animationFreshTime(0)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    setLayout(vBoxLayout);

    m_background = new MusicTransitionAnimationLabel(this);
    m_background->setScaledContents(true);
    vBoxLayout->addWidget(m_background);

    QVBoxLayout *bBoxLayout = new QVBoxLayout(m_background);
    bBoxLayout->setContentsMargins(0, 0, 0, 0);
    bBoxLayout->setSpacing(0);
    m_background->setLayout(bBoxLayout);

    m_containerType = LRC_WALLPAPER_TPYE;
    m_layoutWidget = new MusicVLayoutAnimationWidget(this);
    m_layoutWidget->connectTo(this);
    bBoxLayout->addWidget(m_layoutWidget);

    m_wallThread = new MusicDesktopWallpaperThread(this);
    connect(m_wallThread, SIGNAL(updateBackground(QPixmap)), SLOT(updateBackground(QPixmap)));

#ifdef Q_OS_WIN
    m_wallThread->sendMessageToDesktop();
    SetParent((HWND)winId(), m_wallThread->findDesktopIconWnd());
#endif
}

MusicLrcContainerForWallpaper::~MusicLrcContainerForWallpaper()
{
    clearAllMusicLRCManager();
    delete m_wallThread;
}

void MusicLrcContainerForWallpaper::startDrawLrc()
{
    m_lrcManagers[MUSIC_LRC_INTERIOR_MAX_LINE / 2]->startDrawLrc();
}

void MusicLrcContainerForWallpaper::stopDrawLrc()
{
    m_lrcManagers[MUSIC_LRC_INTERIOR_MAX_LINE / 2]->stopDrawLrc();
    m_layoutWidget->stop();
}

void MusicLrcContainerForWallpaper::applyParameter()
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize().width() - LRC_PER_WIDTH;
    for(int i = 0; i < MUSIC_LRC_INTERIOR_MAX_LINE; ++i)
    {
        MusicLrcManagerForInterior *w = TTKObjectCast(MusicLrcManagerForInterior*, m_lrcManagers[i]);
        w->setLrcPerWidth(width);
        w->setLrcFontSize(36);
        w->setY(35 + 36);
        w->setFixedHeight(35 + 36);
    }

    for(int i = 0; i < MUSIC_LRC_INTERIOR_MAX_LINE; ++i)
    {
        if(i == 0 || i == 10)
        {
            setItemStyleSheet(i, 25, 90);
        }
        else if(i == 1 || i == 9)
        {
            setItemStyleSheet(i, 20, 80);
        }
        else if(i == 2 || i == 8)
        {
            setItemStyleSheet(i, 15, 60);
        }
        else if(i == 3 || i == 7)
        {
            setItemStyleSheet(i, 10, 40);
        }
        else if(i == 4 || i == 6)
        {
            setItemStyleSheet(i, 5, 20);
        }
        else
        {
            setItemStyleSheet(i, 0, 0);
        }
    }
}

void MusicLrcContainerForWallpaper::setLrcAnalysisModel(MusicLrcAnalysis *analysis)
{
    MusicLrcContainer::setLrcAnalysisModel(analysis);
    m_layoutWidget->addStretch(1);
    for(int i = 0; i < MUSIC_LRC_INTERIOR_MAX_LINE; ++i)
    {
       MusicLrcManager *w = new MusicLrcManagerForInterior(this);
       m_layoutWidget->addWidget(w);
       m_lrcManagers.append(w);
    }
    m_layoutWidget->addStretch(1);

    initCurrentLrc(tr("Init wallpaper module now"));

    start(false);
}

void MusicLrcContainerForWallpaper::updateCurrentLrc(qint64 time)
{
    if(m_lrcAnalysis->isValid())
    {
        m_animationFreshTime = time;
        m_layoutWidget->start();
    }

    start(false);
}

void MusicLrcContainerForWallpaper::updateCurrentLrc(const QString &text)
{
    for(int i = 0; i < MUSIC_LRC_INTERIOR_MAX_LINE; ++i)
    {
        m_lrcManagers[i]->setText({});
    }
    m_lrcManagers[MUSIC_LRC_INTERIOR_MAX_LINE / 2]->setText(text);
}

void MusicLrcContainerForWallpaper::start(bool immediate)
{
    if(m_wallThread)
    {
        m_wallThread->setImagePath(G_BACKGROUND_PTR->artistPhotoList());

        if(!m_wallThread->isRunning())
        {
            m_wallThread->start();
        }

        if(immediate)
        {
            m_wallThread->timeout();
        }
    }
}

void MusicLrcContainerForWallpaper::changeCurrentLrcColor()
{
    applyParameter();
}

void MusicLrcContainerForWallpaper::updateBackground(const QPixmap &pix)
{
    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    m_background->setFixedSize(windowSize);
    m_background->setPixmap(pix.scaled(windowSize));
}

void MusicLrcContainerForWallpaper::updateAnimationLrc()
{
    const int length = (MUSIC_LRC_INTERIOR_MAX_LINE - m_lrcAnalysis->lineMax()) / 2 + 1;
    for(int i = 0; i < MUSIC_LRC_INTERIOR_MAX_LINE; ++i)
    {
        m_lrcManagers[i]->setText(m_lrcAnalysis->text(i - length));
    }
    m_lrcManagers[MUSIC_LRC_INTERIOR_MAX_LINE / 2]->startDrawLrcMask(m_animationFreshTime);
}

void MusicLrcContainerForWallpaper::initCurrentLrc(const QString &str)
{
    for(int i = 0; i < m_lrcAnalysis->lineMax(); ++i)
    {
        m_lrcManagers[i]->setText({});
    }
    m_lrcManagers[MUSIC_LRC_INTERIOR_MAX_LINE / 2]->setText(str);
}

void MusicLrcContainerForWallpaper::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLrcManagerForInterior *w = TTKObjectCast(MusicLrcManagerForInterior*, m_lrcManagers[index]);
    w->setFontSize(size);

    const int value = MV_MAX - transparent;
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(G_SETTING_PTR->value("LrcColor").toInt() != -1)
    {
        const MusicLrcColor::Color index = TTKStaticCast(MusicLrcColor::Color, G_SETTING_PTR->value("LrcColor").toInt());
        setLinearGradientColor(index);
    }
    else
    {
        const MusicLrcColor cl(MusicLrcColor::readColorConfig(G_SETTING_PTR->value("LrcFrontgroundColor").toString()),
                               MusicLrcColor::readColorConfig(G_SETTING_PTR->value("LrcBackgroundColor").toString()));
        setLinearGradientColor(cl);
    }
}
