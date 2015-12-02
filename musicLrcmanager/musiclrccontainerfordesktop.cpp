#include "musiclrccontainerfordesktop.h"
#include "musiclrcmanagerfordesktop.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"

#include <QToolButton>
#include <QPushButton>

MusicLrcContainerForDesktop::MusicLrcContainerForDesktop(QWidget *parent)
    : MusicLrcContainer(parent)
{
    m_supperClass = parent;
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint );
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    m_containerType = "DESKTOP";
    //Move the QWidget in the appropriate location
    QSize windowSize = M_SETTING->value(MusicSettingManager::ScreenSize).toSize();
    m_geometry.setX(windowSize.width() - 300);
    m_geometry.setY(80);

    creatToolBarWidget();
    m_desktopWidget = new QWidget(this);
    m_desktopWidget->setObjectName("desktopWidget");
    m_musicLrcContainer << new MusicLRCManagerForDesktop(m_desktopWidget)
                        << new MusicLRCManagerForDesktop(m_desktopWidget);

    setGeometry(200,  windowSize.height() - height() - 150, m_geometry.x(), 2*m_geometry.y() + TOOLBAR_HEIGHT);
    m_desktopWidget->setGeometry(0, TOOLBAR_HEIGHT, m_geometry.x(), 2*m_geometry.y());
    setSelfGeometry();

    m_currentTime = -1;
    m_reverse = false;
    m_windowLocked = false;

    m_currentLrcFontSize = static_cast<MusicLRCManagerForDesktop*>(m_musicLrcContainer[0])->getFirstFontSize();
}

MusicLrcContainerForDesktop::~MusicLrcContainerForDesktop()
{
    clearAllMusicLRCManager();
    delete m_showMainWindow;
    delete m_toolPreSongButton;
    delete m_toolNextSongButton;
    delete m_toolPlayButton;
    delete m_toolCloseButton;
    delete m_toolSettingButton;
    delete m_toolWindowLockedButton;
    delete m_toolLrcBigerButton;
    delete m_toolLrcSmallerButton;
    delete m_toolUpdateLrcButton;
    delete m_toolBarWidget;
    delete m_desktopWidget;
}

void MusicLrcContainerForDesktop::creatToolBarWidget()
{
    m_toolBarWidget = new QWidget(this);
    m_toolBarWidget->setStyleSheet(MusicUIObject::MCustomStyle09);
    m_toolBarWidget->setGeometry((m_geometry.x() - TOOLBAR_WIDTH)/2, 0, TOOLBAR_WIDTH, TOOLBAR_HEIGHT);

    m_showMainWindow = new QPushButton(m_toolBarWidget);

    m_toolCloseButton = new QToolButton(m_toolBarWidget);
    m_toolCloseButton->setGeometry(290, 0, 30, TOOLBAR_HEIGHT);
    connect(m_toolCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_toolWindowLockedButton = new QToolButton(m_toolBarWidget);
    m_toolWindowLockedButton->setGeometry(260, 0, 30, TOOLBAR_HEIGHT);
    connect(m_toolWindowLockedButton, SIGNAL(clicked()), SLOT(setWindowLockedChanged()));

    m_toolLrcBigerButton = new QToolButton(m_toolBarWidget);
    m_toolLrcBigerButton->setGeometry(200, 0, 30, TOOLBAR_HEIGHT);
    connect(m_toolLrcBigerButton, SIGNAL(clicked()), SLOT(setLrcBigerChanged()));

    m_toolLrcSmallerButton = new QToolButton(m_toolBarWidget);
    m_toolLrcSmallerButton->setGeometry(230, 0, 30, TOOLBAR_HEIGHT);
    connect(m_toolLrcSmallerButton, SIGNAL(clicked()), SLOT(setLrcSmallerChanged()));

    m_toolUpdateLrcButton = new QToolButton(m_toolBarWidget);
    m_toolUpdateLrcButton->setGeometry(170, 0, 30, TOOLBAR_HEIGHT);
    connect(m_toolUpdateLrcButton, SIGNAL(clicked()),  SIGNAL(theCurrentLrcUpdated()));

    m_toolSettingButton = new QToolButton(m_toolBarWidget);
    m_toolSettingButton->setGeometry(140, 0, 30, TOOLBAR_HEIGHT);
    connect(m_toolSettingButton, SIGNAL(clicked()), SLOT(currentLrcCustom()));

    m_toolPreSongButton = new QToolButton(m_toolBarWidget);
    m_toolPreSongButton->setGeometry(50, 0, 30, TOOLBAR_HEIGHT);
    m_toolNextSongButton = new QToolButton(m_toolBarWidget);
    m_toolNextSongButton->setGeometry(110, 0, 30, TOOLBAR_HEIGHT);
    m_toolPlayButton = new QToolButton(m_toolBarWidget);
    m_toolPlayButton->setGeometry(80, 0, 30, TOOLBAR_HEIGHT);
    connect(m_toolPreSongButton, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayPrivious()));
    connect(m_toolNextSongButton, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayNext()));
    connect(m_toolPlayButton, SIGNAL(clicked()), m_supperClass, SLOT(musicKey()));
    connect(m_showMainWindow, SIGNAL(clicked()), m_supperClass, SLOT(showNormal()));

    setButtonIcon();
    setButtonCursor();
    setButtonTips();
    m_toolBarWidget->hide();
}

void MusicLrcContainerForDesktop::setButtonIcon() const
{
    m_toolCloseButton->setIcon(QIcon(QPixmap(":/image/close").scaled(30, 30)));
    m_toolWindowLockedButton->setIcon(QIcon(QPixmap(":/desktopTool/lockWindow").scaled(30, 30)));
    m_toolLrcBigerButton->setIcon(QIcon(QPixmap(":/desktopTool/lrcsizeUp").scaled(30, 30)));
    m_toolLrcSmallerButton->setIcon(QIcon(QPixmap(":/desktopTool/lrcsizeDown").scaled(30, 30)));
    m_toolUpdateLrcButton->setIcon(QIcon(QPixmap(":/desktopTool/updateLrc").scaled(30, 30)));
    m_toolPreSongButton->setIcon(QIcon(":/desktopTool/previous"));
    m_toolNextSongButton->setIcon(QIcon(":/desktopTool/next"));
    m_toolPlayButton->setIcon(QIcon(":/desktopTool/play"));
    m_toolSettingButton->setIcon(QIcon(":/desktopTool/setting"));
    m_showMainWindow->setIcon(QIcon(":/image/windowicon"));
}

void MusicLrcContainerForDesktop::setButtonCursor() const
{
    m_toolCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolWindowLockedButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolLrcBigerButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolLrcSmallerButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolUpdateLrcButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolPreSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolNextSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolPlayButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolSettingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_showMainWindow->setCursor(QCursor(Qt::PointingHandCursor));

    m_showMainWindow->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_toolCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolWindowLockedButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolLrcSmallerButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolUpdateLrcButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolLrcBigerButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolPreSongButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolNextSongButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolPlayButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolSettingButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
}

void MusicLrcContainerForDesktop::setButtonTips() const
{
    m_toolCloseButton->setToolTip(tr("hide"));
    m_toolWindowLockedButton->setToolTip(tr("lockLrc"));
    m_toolLrcBigerButton->setToolTip(tr("lrcBiger"));
    m_toolLrcSmallerButton->setToolTip(tr("lrcSmaller"));
    m_toolUpdateLrcButton->setToolTip(tr("updateLrc"));
    m_toolPreSongButton->setToolTip(tr("Privious"));
    m_toolNextSongButton->setToolTip(tr("Next"));
    m_toolPlayButton->setToolTip(tr("Play"));
    m_toolSettingButton->setToolTip(tr("Setting"));
    m_showMainWindow->setToolTip(tr("showMainWindow"));
}

void MusicLrcContainerForDesktop::startTimerClock()
{
    m_musicLrcContainer[!m_reverse]->startTimerClock();
}

void MusicLrcContainerForDesktop::initCurrentLrc() const
{
    if(m_currentTime == -1)
    {
        m_musicLrcContainer[0]->setText(tr("welcome use QMusicPlayer"));
        m_musicLrcContainer[0]->setGeometry(0, 20,
                                            static_cast<MusicLRCManagerForDesktop*>(m_musicLrcContainer[0])->x(),
                                            m_geometry.y());
        m_musicLrcContainer[1]->setGeometry(0, m_geometry.y() + 20, 0, 0);
    }
}

void MusicLrcContainerForDesktop::updateCurrentLrc(const QString &first,
                                                   const QString &second, qint64 time)
{
    m_currentTime = time;
    m_reverse = !m_reverse;
    static_cast<MusicLRCManagerForDesktop*>(m_musicLrcContainer[m_reverse])->resetOrigin();
    m_musicLrcContainer[ m_reverse]->stopLrcMask();
    m_musicLrcContainer[ m_reverse]->setText(second);
    m_musicLrcContainer[!m_reverse]->setText(first);
    m_musicLrcContainer[!m_reverse]->startLrcMask(time);

    resizeLrcSizeArea();
}

void MusicLrcContainerForDesktop::resizeLrcSizeArea()
{
    int width = static_cast<MusicLRCManagerForDesktop*>(m_musicLrcContainer[0])->x();
    m_musicLrcContainer[0]->setGeometry(0, 20, width, m_geometry.y());

    width = static_cast<MusicLRCManagerForDesktop*>(m_musicLrcContainer[1])->x();
    int pos = m_geometry.x() - width;
    if(pos < 0 )
    {
        pos = 0;
    }
    m_musicLrcContainer[1]->setGeometry(pos, m_geometry.y() + 20, width, m_geometry.y());
}

void MusicLrcContainerForDesktop::resizeLrcSizeArea(bool bigger)
{
    m_geometry.setY(bigger ? m_geometry.y() + 1 : m_geometry.y() - 1);
    setSelfGeometry();
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        static_cast<MusicLRCManagerForDesktop*>(m_musicLrcContainer[i])->setLrcFontSize(bigger ? ++m_currentLrcFontSize
                                                                                               : --m_currentLrcFontSize);
    }
    m_musicLrcContainer[1]->setText(m_musicLrcContainer[1]->text());
    resizeLrcSizeArea();
    M_SETTING->setValue(MusicSettingManager::DLrcSizeChoiced, m_currentLrcFontSize);
}

void MusicLrcContainerForDesktop::stopLrcMask()
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        m_musicLrcContainer[i]->stopLrcMask();
    }
}

void MusicLrcContainerForDesktop::setMaskLinearGradientColor(QColor color) const
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        m_musicLrcContainer[i]->setMaskLinearGradientColor(color);
    }
}

void MusicLrcContainerForDesktop::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(!m_windowLocked && event->button() == Qt::LeftButton )
    {
        m_offset = event->globalPos() - frameGeometry().topLeft();
    }
}

void MusicLrcContainerForDesktop::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(!m_windowLocked && (event->buttons() & Qt::LeftButton) )
    {
       setCursor(Qt::CrossCursor);
       move(event->globalPos() - m_offset);
       M_SETTING->setValue(MusicSettingManager::DLrcGeometryChoiced, QWidget::geometry());
    }
}

void MusicLrcContainerForDesktop::enterEvent(QEvent *event)
{
    if(m_windowLocked)
    {
        return;
    }
    QWidget::enterEvent(event);
    m_toolBarWidget->show();
    setStyleSheet("#desktopWidget{" + MusicUIObject::MCustomStyle10 + "}");
}

void MusicLrcContainerForDesktop::leaveEvent(QEvent *event)
{
    if(m_windowLocked)
    {
        return;
    }
    QWidget::leaveEvent(event);
    m_toolBarWidget->hide();
    setStyleSheet("#desktopWidget{" + MusicUIObject::MCustomStyle01 + "}");
}

void MusicLrcContainerForDesktop::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    emit desktopLrcClosed();
}

void MusicLrcContainerForDesktop::setWindowLockedChanged()
{
    m_windowLocked = !m_windowLocked;
    if(m_windowLocked)
    {
       m_toolBarWidget->hide();
       setStyleSheet(MusicUIObject::MCustomStyle01);
    }
    M_SETTING->setValue(MusicSettingManager::DLrcLockedChoiced,  m_windowLocked ? 1 : 0);
    emit setWindowLockedChanged(m_windowLocked);
}

void MusicLrcContainerForDesktop::setSelfGeometry() const
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        static_cast<MusicLRCManagerForDesktop*>(m_musicLrcContainer[i])->setSelfGeometry(m_geometry.x(), m_geometry.y());
    }
}

void MusicLrcContainerForDesktop::setLrcBigerChanged()
{
    if(m_currentLrcFontSize > 35)
    {
        return;
    }
    resizeLrcSizeArea(true);
}

void MusicLrcContainerForDesktop::setLrcSmallerChanged()
{
    if(m_currentLrcFontSize < 25)
    {
        return;
    }
    resizeLrcSizeArea(false);
}

void MusicLrcContainerForDesktop::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu menu(this);
    QMenu changColorMenu(tr("changColorMenu"), this);
    changColorMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(tr("searchLrcs"), this, SLOT(searchMusicLrcs()));
    menu.addAction(tr("updateLrc"), this, SIGNAL(theCurrentLrcUpdated()));
    menu.addAction(tr("makeLrc"), this, SLOT(theCurrentLrcMaked()));
    menu.addSeparator();
    menu.addAction(tr("hide"), this, SLOT(close()));
    menu.addAction(QIcon(":/contextMenu/lock"), tr("lockLrc"), this, SLOT(setWindowLockedChanged()));
    menu.addMenu(&changColorMenu);
    menu.addSeparator();

    createColorMenu(changColorMenu);
    menu.addAction(tr("customSetting"), this, SLOT(currentLrcCustom()));

    menu.exec(QCursor::pos());
}

void MusicLrcContainerForDesktop::setSettingParameter()
{
    MusicLrcContainer::setSettingParameter();
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        m_musicLrcContainer[i]->setLrcFontSize((MusicLRCManager::LrcSizeTable)(
                                                m_currentLrcFontSize = M_SETTING->value(MusicSettingManager::DLrcSizeChoiced).toInt()));
    }
    M_SETTING->value(MusicSettingManager::DLrcLockedChoiced).toInt() == 1 ? m_windowLocked = true : m_windowLocked = false;
    QRect rect = M_SETTING->value(MusicSettingManager::DLrcGeometryChoiced).toRect();
    if(!rect.isEmpty())
    {
        setGeometry(rect);
    }
}

void MusicLrcContainerForDesktop::showPlayStatus(bool status) const
{
    m_toolPlayButton->setIcon(QIcon(QString::fromUtf8(!status ? ":/desktopTool/stop" : ":/desktopTool/play")) );
}
