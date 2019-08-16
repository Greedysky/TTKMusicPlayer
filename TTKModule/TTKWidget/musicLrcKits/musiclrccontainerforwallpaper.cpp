#include "musiclrccontainerforwallpaper.h"
#include "musicdesktopwallpaperthread.h"
#include "musiclrcmanagerforinline.h"
#include "musiclayoutanimationwidget.h"
#include "musictransitionanimationlabel.h"
#include "musicbackgroundmanager.h"
#include "musiclrcanalysis.h"
#include "musicstringutils.h"

MusicLrcContainerForWallpaper::MusicLrcContainerForWallpaper(QWidget *parent)
    : MusicLrcContainer(parent)
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

    m_animationFreshTime = 0;
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

void MusicLrcContainerForWallpaper::startTimerClock()
{
    m_musicLrcContainer[MUSIC_LRC_INLINE_MAX_LINE/2]->startTimerClock();
}

void MusicLrcContainerForWallpaper::stopLrcMask()
{
    m_musicLrcContainer[MUSIC_LRC_INLINE_MAX_LINE/2]->stopLrcMask();
    m_layoutWidget->stop();
}

void MusicLrcContainerForWallpaper::setSettingParameter()
{
    const int width = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize().width() - LRC_PER_WIDTH;
    for(int i=0; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
        MusicLrcManagerForInline *w = MStatic_cast(MusicLrcManagerForInline*, m_musicLrcContainer[i]);
        w->setLrcPerWidth(width);
        w->setLrcFontSize(36);
        w->setY(35 + 36);
        w->setFixedHeight(35 + 36);
    }

    for(int i=0; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
        if(i == 0 || i == 10) setItemStyleSheet(i, 25, 90);
        else if(i == 1 || i == 9) setItemStyleSheet(i, 20, 80);
        else if(i == 2 || i == 8) setItemStyleSheet(i, 15, 60);
        else if(i == 3 || i == 7) setItemStyleSheet(i, 10, 40);
        else if(i == 4 || i == 6) setItemStyleSheet(i, 5, 20);
        else setItemStyleSheet(i, 0, 0);
    }
}

void MusicLrcContainerForWallpaper::setLrcAnalysisModel(MusicLrcAnalysis *analysis)
{
    MusicLrcContainer::setLrcAnalysisModel(analysis);
    m_layoutWidget->addStretch(1);
    for(int i=0; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
       MusicLrcManager *w = new MusicLrcManagerForInline(this);
       m_layoutWidget->addWidget(w);
       m_musicLrcContainer.append(w);
    }
    m_layoutWidget->addStretch(1);

    initCurrentLrc(tr("Init Wallpaper Module"));

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
    for(int i=0; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
        m_musicLrcContainer[i]->setText(QString());
    }
    m_musicLrcContainer[MUSIC_LRC_INLINE_MAX_LINE/2]->setText(text);
}

void MusicLrcContainerForWallpaper::start(bool immediate)
{
    if(m_wallThread)
    {
        m_wallThread->setImagePath(M_BACKGROUND_PTR->getArtistPhotoPathList());

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
    setSettingParameter();
}

void MusicLrcContainerForWallpaper::updateBackground(const QPixmap &pix)
{
    m_background->setPixmap(pix);
}

void MusicLrcContainerForWallpaper::updateAnimationLrc()
{
    const int length = (MUSIC_LRC_INLINE_MAX_LINE - m_lrcAnalysis->getLineMax())/2 + 1;
    for(int i=0; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
        m_musicLrcContainer[i]->setText(m_lrcAnalysis->getText(i - length));
    }
    m_musicLrcContainer[MUSIC_LRC_INLINE_MAX_LINE/2]->startLrcMask(m_animationFreshTime);
}

void MusicLrcContainerForWallpaper::initCurrentLrc(const QString &str)
{
    for(int i=0; i<m_lrcAnalysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText( QString() );
    }
    m_musicLrcContainer[MUSIC_LRC_INLINE_MAX_LINE/2]->setText(str);
}

void MusicLrcContainerForWallpaper::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLrcManagerForInline *w = MStatic_cast(MusicLrcManagerForInline*, m_musicLrcContainer[index]);
    w->setFontSize(size);

    const int value = 100 - transparent;
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(M_SETTING_PTR->value("LrcColorChoiced").toInt() != -1)
    {
        const MusicLrcColor::LrcColorType index = MStatic_cast(MusicLrcColor::LrcColorType, M_SETTING_PTR->value("LrcColorChoiced").toInt());
        setLinearGradientColor(index);
    }
    else
    {
        const MusicLrcColor cl(MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcFrontgroundColorChoiced").toString()),
                               MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcBackgroundColorChoiced").toString()));
        setLinearGradientColor(cl);
    }
}
