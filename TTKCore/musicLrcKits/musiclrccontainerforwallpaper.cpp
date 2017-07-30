#include "musiclrccontainerforwallpaper.h"
#include "musicdesktopwallpaperthread.h"
#include "musiclrcmanagerforinline.h"
#include "musiclayoutanimationwidget.h"
#include "musiclrcanalysis.h"
#include "musicstringutils.h"
#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif
#include <QBoxLayout>

MusicLrcContainerForWallpaper::MusicLrcContainerForWallpaper(QWidget *parent)
    : MusicLrcContainer(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowState(Qt::WindowNoState);
    setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    setLayout(vBoxLayout);

#ifdef Q_OS_WIN
    SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
                                 WS_EX_TRANSPARENT | WS_EX_LAYERED);
#endif

    m_containerType = "WALLPAPER";
    m_layoutWidget = new MusicLayoutAnimationWidget(this);
    m_layoutWidget->connectTo(this);
    vBoxLayout->addWidget(m_layoutWidget);

    m_animationFreshTime = 0;

    m_wallThread = new MusicDesktopWallpaperThread(this);
    MusicObject::MStriantMap para;
    para.insert("Mode", 2);
    para.insert("Path", "Play");
    para.insert("Type", 2);
    para.insert("Func", 1);
    para.insert("Time", 10);
    para.insert("Close", true);
    m_wallThread->setParamters(para);
}

MusicLrcContainerForWallpaper::~MusicLrcContainerForWallpaper()
{
    clearAllMusicLRCManager();
    m_wallThread->stopAndQuitThread();
    delete m_wallThread;
}

QString MusicLrcContainerForWallpaper::getClassName()
{
    return staticMetaObject.className();
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
    int width = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize().width() - LRC_PER_WIDTH;
    for(int i=0; i<MUSIC_LRC_INLINE_MAX_LINE; ++i)
    {
        MusicLRCManagerForInline *w = MStatic_cast(MusicLRCManagerForInline*, m_musicLrcContainer[i]);
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
       MusicLRCManager *w = new MusicLRCManagerForInline(this);
       m_layoutWidget->addWidget(w);
       m_musicLrcContainer.append(w);
    }
    m_layoutWidget->addStretch(1);

    initCurrentLrc(tr("Init Wallpaper Module"));
    if(!m_wallThread->isRunning())
    {
        m_wallThread->start();
    }
}

void MusicLrcContainerForWallpaper::updateCurrentLrc(qint64 time)
{
    if(m_lrcAnalysis->valid())
    {
        m_animationFreshTime = time;
        m_layoutWidget->start();
    }

    if(!m_wallThread->isRunning())
    {
        m_wallThread->start();
    }
}

void MusicLrcContainerForWallpaper::changeCurrentLrcColor()
{
    setSettingParameter();
}

void MusicLrcContainerForWallpaper::updateAnimationLrc()
{
    int length = (MUSIC_LRC_INLINE_MAX_LINE - m_lrcAnalysis->getLineMax())/2 + 1;
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
    MusicLRCManagerForInline *w = MStatic_cast(MusicLRCManagerForInline*, m_musicLrcContainer[index]);
    w->setCenterOnLrc(false);
    w->setFontSize(size);

    int value = 100 - transparent;
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(M_SETTING_PTR->value("LrcColorChoiced").toInt() != -1)
    {
        MusicLRCColor::LrcColorType index = MStatic_cast(MusicLRCColor::LrcColorType, M_SETTING_PTR->value("LrcColorChoiced").toInt());
        setLinearGradientColor(index);
    }
    else
    {
        MusicLRCColor cl(MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcFgColorChoiced").toString()),
                         MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcBgColorChoiced").toString()));
        setLinearGradientColor(cl);
    }
}
