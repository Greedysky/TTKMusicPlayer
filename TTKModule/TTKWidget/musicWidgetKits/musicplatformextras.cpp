#include "musicplatformextras.h"
#include "musicapplication.h"

#include <QStyle>

class MusicAbstractPlatformExtras
{
public:
    MusicAbstractPlatformExtras()
    {

    }

    virtual ~MusicAbstractPlatformExtras()
    {

    }

    /*!
     * Set current status.
     * Subclass should implement this function.
     */
    virtual void setStatus(bool status) = 0;
    /*!
     * Set current action.
     * Subclass should implement this function.
     */
    virtual void action() = 0;
};

#ifdef Q_OS_WIN
#if TTK_QT_VERSION_CHECK(5,2,0)
#  include <QtWinExtras/QtWinExtras>
#endif

class MusicWindowsExtras : public MusicAbstractPlatformExtras
{
public:
    MusicWindowsExtras()
        : MusicAbstractPlatformExtras()
    {
#if TTK_QT_VERSION_CHECK(5,2,0)
        m_playToolButton = nullptr;
        m_forwardToolButton = nullptr;
        m_backwardToolButton = nullptr;
        m_thumbnailToolBar = nullptr;

        action();
        createThumbnailToolBar();
#endif
    }

    ~MusicWindowsExtras()
    {
#if TTK_QT_VERSION_CHECK(5,2,0)
        delete m_playToolButton;
        delete m_forwardToolButton;
        delete m_backwardToolButton;
        delete m_thumbnailToolBar;
#endif
    }

    /*!
     * Set current status.
     */
    virtual void setStatus(bool status) override final
    {
#if TTK_QT_VERSION_CHECK(5,2,0)
        const QStyle::StandardPixmap pix = status ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay;
        m_playToolButton->setIcon(MusicApplication::instance()->style()->standardIcon(pix));
#else
        Q_UNUSED(status);
#endif
    }

    /*!
     * Set current action.
     */
    virtual void action() override final
    {
#if TTK_QT_VERSION_CHECK(5,2,0)
        QtWin::enableBlurBehindWindow(MusicApplication::instance());
        QtWin::disableBlurBehindWindow(MusicApplication::instance());
#endif
    }

private:
#if TTK_QT_VERSION_CHECK(5,2,0)
    /*!
     * Create thumbnail toolBar.
     */
    void createThumbnailToolBar()
    {
        MusicApplication *parent = MusicApplication::instance();
        m_thumbnailToolBar = new QWinThumbnailToolBar(parent);
        m_thumbnailToolBar->setWindow(parent->windowHandle());

        m_playToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
        m_playToolButton->setToolTip(QObject::tr("Play"));
        m_playToolButton->setIcon(parent->style()->standardIcon(QStyle::SP_MediaPlay));
        QObject::connect(m_playToolButton, SIGNAL(clicked()), parent, SLOT(switchPlayState()));

        m_forwardToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
        m_forwardToolButton->setToolTip(QObject::tr("Previous"));
        m_forwardToolButton->setIcon(parent->style()->standardIcon(QStyle::SP_MediaSeekBackward));
        QObject::connect(m_forwardToolButton, SIGNAL(clicked()), parent, SLOT(playPrevious()));

        m_backwardToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
        m_backwardToolButton->setToolTip(QObject::tr("Next"));
        m_backwardToolButton->setIcon(parent->style()->standardIcon(QStyle::SP_MediaSeekForward));
        QObject::connect(m_backwardToolButton, SIGNAL(clicked()), parent, SLOT(playNext()));

        m_thumbnailToolBar->addButton(m_forwardToolButton);
        m_thumbnailToolBar->addButton(m_playToolButton);
        m_thumbnailToolBar->addButton(m_backwardToolButton);
    }

    QWinThumbnailToolBar *m_thumbnailToolBar;
    QWinThumbnailToolButton *m_playToolButton;
    QWinThumbnailToolButton *m_forwardToolButton;
    QWinThumbnailToolButton *m_backwardToolButton;
#endif
};
#elif defined Q_OS_LINUX
class MusicLinuxExtras : public MusicAbstractPlatformExtras
{
public:
    MusicLinuxExtras()
        : MusicAbstractPlatformExtras()
    {

    }

    ~MusicLinuxExtras()
    {

    }

    /*!
     * Set current status.
     */
    virtual void setStatus(bool status) override final
    {
        Q_UNUSED(status);
    }

    /*!
     * Set current action.
     */
    virtual void action() override final
    {

    }
};
#else
class MusicMacExtras : public MusicAbstractPlatformExtras
{
public:
    MusicMacExtras()
        : MusicAbstractPlatformExtras()
    {

    }

    ~MusicMacExtras()
    {

    }

    /*!
     * Set current status.
     */
    virtual void setStatus(bool status) override final
    {
        Q_UNUSED(status);
    }

    /*!
     * Set current action.
     */
    virtual void action() override final
    {

    }
};
#endif


MusicPlatformExtras::MusicPlatformExtras(QObject *parent)
    : QObject(parent),
      m_disableBlurBehindWindow(true)
{
#ifdef Q_OS_WIN
    m_platformExtras = new MusicWindowsExtras;
#elif defined Q_OS_LINUX
    m_platformExtras = new MusicLinuxExtras;
#else
    m_platformExtras = new MusicMacExtras;
#endif
}

MusicPlatformExtras::~MusicPlatformExtras()
{
    delete m_platformExtras;
}

void MusicPlatformExtras::setCurrentPlayStatus(bool status) const
{
    m_platformExtras->setStatus(status);
}

void MusicPlatformExtras::disableBlurBehindWindow(bool enabled)
{
    m_disableBlurBehindWindow = enabled;
    m_platformExtras->action();
}
