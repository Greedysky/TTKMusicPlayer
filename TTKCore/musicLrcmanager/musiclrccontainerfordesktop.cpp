#include "musiclrccontainerfordesktop.h"
#include "musiclrcmanagerfordesktop.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"

#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>

MusicLrcContainerForDesktop::MusicLrcContainerForDesktop(QWidget *parent)
    : MusicLrcContainer(parent)
{
    m_supperClass = parent;
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    m_containerType = "DESKTOP";
    //Move the QWidget in the appropriate location
    QSize windowSize = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    m_geometry.setX(windowSize.width() - 300);
    m_geometry.setY(80);

    creatToolBarWidget();
    QWidget *desktopWidget = new QWidget(this);
    desktopWidget->setObjectName("desktopWidget");
    m_musicLrcContainer << new MusicLRCManagerForDesktop(desktopWidget)
                        << new MusicLRCManagerForDesktop(desktopWidget);

    setGeometry(200,  windowSize.height() - height() - 150, m_geometry.x(), 2*m_geometry.y() + TOOLBAR_HEIGHT);
    desktopWidget->setGeometry(0, TOOLBAR_HEIGHT, m_geometry.x(), 2*m_geometry.y());
    setSelfGeometry();

    m_reverse = false;
    m_windowLocked = false;

    m_currentLrcFontSize = MStatic_cast(MusicLRCManagerForDesktop*, m_musicLrcContainer[0])->getFirstFontSize();
}

MusicLrcContainerForDesktop::~MusicLrcContainerForDesktop()
{
    clearAllMusicLRCManager();
    delete m_toolPlayButton;
    delete m_toolBarWidget;
}

void MusicLrcContainerForDesktop::creatToolBarWidget()
{
    m_toolBarWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(m_toolBarWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    m_toolBarWidget->setStyleSheet(MusicUIObject::MCustomStyle09);
    m_toolBarWidget->setGeometry((m_geometry.x() - TOOLBAR_WIDTH)/2, 0, TOOLBAR_WIDTH, TOOLBAR_HEIGHT);
    m_toolBarWidget->setLayout(layout);

    QPushButton *showMainWindow = new QPushButton(m_toolBarWidget);
    showMainWindow->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(showMainWindow);
    connect(showMainWindow, SIGNAL(clicked()), m_supperClass, SLOT(showNormal()));

    QToolButton *toolPreSongButton = new QToolButton(m_toolBarWidget);
    toolPreSongButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolPreSongButton);
    connect(toolPreSongButton, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayPrevious()));

    m_toolPlayButton = new QToolButton(m_toolBarWidget);
    m_toolPlayButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(m_toolPlayButton);
    connect(m_toolPlayButton, SIGNAL(clicked()), m_supperClass, SLOT(musicStatePlay()));

    QToolButton *toolNextSongButton = new QToolButton(m_toolBarWidget);
    toolNextSongButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolNextSongButton);
    connect(toolNextSongButton, SIGNAL(clicked()), m_supperClass, SLOT(musicPlayNext()));

    QToolButton *toolSettingButton = new QToolButton(m_toolBarWidget);
    toolSettingButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolSettingButton);
    connect(toolSettingButton, SIGNAL(clicked()), SLOT(currentLrcCustom()));

    QToolButton *toolUpdateLrcButton = new QToolButton(m_toolBarWidget);
    toolUpdateLrcButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolUpdateLrcButton);
    connect(toolUpdateLrcButton, SIGNAL(clicked()),  SIGNAL(theCurrentLrcUpdated()));

    QToolButton *toolLrcSmallerButton = new QToolButton(m_toolBarWidget);
    toolLrcSmallerButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolLrcSmallerButton);
    connect(toolLrcSmallerButton, SIGNAL(clicked()), SLOT(setLrcSmallerChanged()));

    QToolButton *toolLrcBigerButton = new QToolButton(m_toolBarWidget);
    toolLrcBigerButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolLrcBigerButton);
    connect(toolLrcBigerButton, SIGNAL(clicked()), SLOT(setLrcBiggerChanged()));

    QToolButton *toolMakeLrcTextButton = new QToolButton(m_toolBarWidget);
    toolMakeLrcTextButton->setFixedSize(TOOLBAR_TEXT_LENGTH, TOOLBAR_HEIGHT);
    layout->addWidget(toolMakeLrcTextButton);
    connect(toolMakeLrcTextButton, SIGNAL(clicked()), SLOT(theCurrentLrcMaked()));

    QToolButton *toolSearchLrcTextButton = new QToolButton(m_toolBarWidget);
    toolSearchLrcTextButton->setFixedSize(TOOLBAR_TEXT_LENGTH, TOOLBAR_HEIGHT);
    layout->addWidget(toolSearchLrcTextButton);
    connect(toolSearchLrcTextButton, SIGNAL(clicked()), SLOT(searchMusicLrcs()));

    QToolButton *toolUpdateLrcTextButton = new QToolButton(m_toolBarWidget);
    toolUpdateLrcTextButton->setFixedSize(TOOLBAR_TEXT_LENGTH, TOOLBAR_HEIGHT);
    layout->addWidget(toolUpdateLrcTextButton);
    connect(toolUpdateLrcTextButton, SIGNAL(clicked()), SIGNAL(theCurrentLrcUpdated()));

    QToolButton *toolErrorLrcTextButton = new QToolButton(m_toolBarWidget);
    toolErrorLrcTextButton->setFixedSize(TOOLBAR_TEXT_LENGTH, TOOLBAR_HEIGHT);
    layout->addWidget(toolErrorLrcTextButton);

    QToolButton *toolWindowLockedButton = new QToolButton(m_toolBarWidget);
    toolWindowLockedButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolWindowLockedButton);
    connect(toolWindowLockedButton, SIGNAL(clicked()), SLOT(setWindowLockedChanged()));

    QToolButton *toolCloseButton = new QToolButton(m_toolBarWidget);
    toolCloseButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    layout->addWidget(toolCloseButton);
    connect(toolCloseButton, SIGNAL(clicked()), SLOT(close()));

    showMainWindow->setIcon(QIcon(":/image/windowicon"));
    toolCloseButton->setIcon(QIcon(QPixmap(":/image/close").scaled(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT)));
    toolMakeLrcTextButton->setIcon(QIcon(QPixmap(":/desktopTool/makeLrcText")));
    toolSearchLrcTextButton->setIcon(QIcon(QPixmap(":/desktopTool/searchLrcText")));
    toolUpdateLrcTextButton->setIcon(QIcon(QPixmap(":/desktopTool/updateLrcText")));
    toolErrorLrcTextButton->setIcon(QIcon(QPixmap(":/desktopTool/errorLrcText")));
    toolWindowLockedButton->setIcon(QIcon(QPixmap(":/desktopTool/lockWindow").scaled(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT)));
    toolLrcBigerButton->setIcon(QIcon(QPixmap(":/desktopTool/lrcsizeUp").scaled(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT)));
    toolLrcSmallerButton->setIcon(QIcon(QPixmap(":/desktopTool/lrcsizeDown").scaled(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT)));
    toolUpdateLrcButton->setIcon(QIcon(QPixmap(":/desktopTool/updateLrc").scaled(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT)));
    toolPreSongButton->setIcon(QIcon(":/desktopTool/previous"));
    toolNextSongButton->setIcon(QIcon(":/desktopTool/next"));
    toolSettingButton->setIcon(QIcon(":/desktopTool/setting"));
    m_toolPlayButton->setIcon(QIcon(":/desktopTool/play"));

    toolMakeLrcTextButton->setIconSize(QSize(TOOLBAR_TEXT_LENGTH, 20));
    toolSearchLrcTextButton->setIconSize(QSize(TOOLBAR_TEXT_LENGTH, 20));
    toolUpdateLrcTextButton->setIconSize(QSize(TOOLBAR_TEXT_LENGTH, 20));
    toolErrorLrcTextButton->setIconSize(QSize(TOOLBAR_TEXT_LENGTH, 20));

    showMainWindow->setCursor(QCursor(Qt::PointingHandCursor));
    toolCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolMakeLrcTextButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolSearchLrcTextButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolUpdateLrcTextButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolErrorLrcTextButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolWindowLockedButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolLrcBigerButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolLrcSmallerButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolUpdateLrcButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolPreSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolNextSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolSettingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolPlayButton->setCursor(QCursor(Qt::PointingHandCursor));

    showMainWindow->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    toolCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolMakeLrcTextButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolSearchLrcTextButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolUpdateLrcTextButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolErrorLrcTextButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolWindowLockedButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolLrcSmallerButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolUpdateLrcButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolLrcBigerButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolPreSongButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolNextSongButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    toolSettingButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_toolPlayButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);

    showMainWindow->setToolTip(tr("showMainWindow"));
    toolCloseButton->setToolTip(tr("hide"));
    toolWindowLockedButton->setToolTip(tr("lockLrc"));
    toolLrcBigerButton->setToolTip(tr("lrcBiger"));
    toolLrcSmallerButton->setToolTip(tr("lrcSmaller"));
    toolUpdateLrcButton->setToolTip(tr("updateLrc"));
    toolPreSongButton->setToolTip(tr("Previous"));
    toolNextSongButton->setToolTip(tr("Next"));
    toolSettingButton->setToolTip(tr("Setting"));
    m_toolPlayButton->setToolTip(tr("Play"));

    m_toolBarWidget->hide();
}

void MusicLrcContainerForDesktop::startTimerClock()
{
    m_musicLrcContainer[!m_reverse]->startTimerClock();
}

void MusicLrcContainerForDesktop::initCurrentLrc() const
{
    if(m_currentTime == 0)
    {
        m_musicLrcContainer[0]->setText(tr("welcome use TTKMusicPlayer"));
        m_musicLrcContainer[0]->setGeometry(0, 20,
                                            MStatic_cast(MusicLRCManagerForDesktop*, m_musicLrcContainer[0])->x(),
                                            m_geometry.y());
        m_musicLrcContainer[1]->setGeometry(0, m_geometry.y() + 20, 0, 0);
    }
}

void MusicLrcContainerForDesktop::updateCurrentLrc(const QString &first, const QString &second, qint64 time)
{
    m_reverse = !m_reverse;
    MStatic_cast(MusicLRCManagerForDesktop*, m_musicLrcContainer[m_reverse])->resetOrigin();
    m_musicLrcContainer[ m_reverse]->stopLrcMask();
    m_musicLrcContainer[ m_reverse]->setText(second);
    m_musicLrcContainer[!m_reverse]->setText(first);
    m_musicLrcContainer[!m_reverse]->startLrcMask(time);

    resizeLrcSizeArea();
}

void MusicLrcContainerForDesktop::resizeLrcSizeArea()
{
    int width = MStatic_cast(MusicLRCManagerForDesktop*, m_musicLrcContainer[0])->x();
    m_musicLrcContainer[0]->setGeometry(0, 20, width, m_geometry.y());

    width = MStatic_cast(MusicLRCManagerForDesktop*, m_musicLrcContainer[1])->x();
    int pos = m_geometry.x() - width;
    if(pos < 0 )
    {
        pos = 0;
    }
    m_musicLrcContainer[1]->setGeometry(pos, m_geometry.y() + 20, width, m_geometry.y());
}

void MusicLrcContainerForDesktop::resizeLrcSizeArea(bool resize)
{
    m_geometry.setY(resize ? m_geometry.y() + 1 : m_geometry.y() - 1);
    setSelfGeometry();

    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        MStatic_cast(MusicLRCManagerForDesktop*, manager)->setLrcFontSize(resize ? ++m_currentLrcFontSize
                                                                                 : --m_currentLrcFontSize);
    }
    m_musicLrcContainer[1]->setText(m_musicLrcContainer[1]->text());

    resizeLrcSizeArea();
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSizeChoiced, m_currentLrcFontSize);
}

void MusicLrcContainerForDesktop::stopLrcMask()
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        manager->stopLrcMask();
    }
}

void MusicLrcContainerForDesktop::setMaskLinearGradientColor(QColor color) const
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        manager->setMaskLinearGradientColor(color);
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
        M_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometryChoiced, QWidget::pos());
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
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcLockedChoiced,  m_windowLocked ? 1 : 0);
    emit setWindowLockedChanged(m_windowLocked);
}

void MusicLrcContainerForDesktop::setSelfGeometry() const
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        MStatic_cast(MusicLRCManagerForDesktop*, manager)->setSelfGeometry(m_geometry.x(), m_geometry.y());
    }
}

void MusicLrcContainerForDesktop::setLrcBiggerChanged()
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

    QAction *lrcLinkAc = menu.addAction(tr("localLinkOff"), this, SLOT(theLinkLrcChanged()));
    m_linkLocalLrc ? lrcLinkAc->setText(tr("localLinkOff")) : lrcLinkAc->setText(tr("localLinkOn"));
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
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        m_currentLrcFontSize = M_SETTING_PTR->value(MusicSettingManager::DLrcSizeChoiced).toInt();
        manager->setLrcFontSize(MStatic_cast(MusicLRCManager::LrcSizeTable, m_currentLrcFontSize));
    }
    m_windowLocked = M_SETTING_PTR->value(MusicSettingManager::DLrcLockedChoiced).toInt() == 1;
    QPoint point = M_SETTING_PTR->value(MusicSettingManager::DLrcGeometryChoiced).toPoint();
    if(!point.isNull())
    {
        move(point);
    }
}

void MusicLrcContainerForDesktop::showPlayStatus(bool status) const
{
    m_toolPlayButton->setIcon(QIcon(QString::fromUtf8(!status ? ":/desktopTool/stop" : ":/desktopTool/play")) );
}
