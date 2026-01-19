#include "musicplatformextras.h"
#include "musicapplication.h"

#include <QStyle>

/*! @brief The class of the abstract platform extras.
 * @author Greedysky <greedysky@163.com>
 */
class AbstractPlatformExtras
{
public:
    AbstractPlatformExtras() = default;
    virtual ~AbstractPlatformExtras() = default;

    /*!
     * Set current status.
     * Subclass should implement this function.
     */
    virtual void setCurrentStatus(bool status) = 0;
    /*!
     * Set current action.
     * Subclass should implement this function.
     */
    virtual void action() = 0;
};

#ifdef Q_OS_WIN
#if TTK_QT_VERSION_CHECK(5,2,0) && !TTK_QT_VERSION_CHECK(6,0,0)
#  include <QtWinExtras/QtWinExtras>
#endif

/*! @brief The class of the windows platform extras.
 * @author Greedysky <greedysky@163.com>
 */
class WindowsPlatformExtras : public AbstractPlatformExtras
{
public:
    WindowsPlatformExtras()
        : AbstractPlatformExtras()
    {
#if TTK_QT_VERSION_CHECK(5,2,0) && !TTK_QT_VERSION_CHECK(6,0,0)
        m_playToolButton = nullptr;
        m_forwardToolButton = nullptr;
        m_backwardToolButton = nullptr;
        m_thumbnailToolBar = nullptr;

        action();
        createThumbnailToolBar();
#endif
    }

    ~WindowsPlatformExtras()
    {
#if TTK_QT_VERSION_CHECK(5,2,0) && !TTK_QT_VERSION_CHECK(6,0,0)
        delete m_playToolButton;
        delete m_forwardToolButton;
        delete m_backwardToolButton;
        delete m_thumbnailToolBar;
#endif
    }

    /*!
     * Set current status.
     */
    virtual void setCurrentStatus(bool status) override final
    {
#if TTK_QT_VERSION_CHECK(5,2,0) && !TTK_QT_VERSION_CHECK(6,0,0)
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
#if TTK_QT_VERSION_CHECK(5,2,0) && !TTK_QT_VERSION_CHECK(6,0,0)
        QtWin::enableBlurBehindWindow(MusicApplication::instance());
        QtWin::disableBlurBehindWindow(MusicApplication::instance());
#endif
    }

private:
#if TTK_QT_VERSION_CHECK(5,2,0) && !TTK_QT_VERSION_CHECK(6,0,0)
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
        QObject::connect(m_playToolButton, SIGNAL(clicked()), parent, SLOT(switchToPlayState()));

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
/*! @brief The class of the linux platform extras.
 * @author Greedysky <greedysky@163.com>
 */
class LinuxPlatformExtras : public AbstractPlatformExtras
{
public:
    LinuxPlatformExtras()
        : AbstractPlatformExtras()
    {

    }

    /*!
     * Set current status.
     */
    virtual void setCurrentStatus(bool status) override final
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
/*! @brief The class of the mac platform extras.
 * @author Greedysky <greedysky@163.com>
 */
class MacPlatformExtras : public AbstractPlatformExtras
{
public:
    MacPlatformExtras()
        : AbstractPlatformExtras()
    {

    }

    /*!
     * Set current status.
     */
    virtual void setCurrentStatus(bool status) override final
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
    : QObject(parent)
{
#ifdef Q_OS_WIN
    m_platformExtras = new WindowsPlatformExtras;
#elif defined Q_OS_LINUX
    m_platformExtras = new LinuxPlatformExtras;
#else
    m_platformExtras = new MacPlatformExtras;
#endif
}

MusicPlatformExtras::~MusicPlatformExtras()
{
    delete m_platformExtras;
}

void MusicPlatformExtras::setAction()
{
    m_platformExtras->action();
}

void MusicPlatformExtras::setCurrentPlayState(bool state) const
{
    m_platformExtras->setCurrentStatus(state);
}
