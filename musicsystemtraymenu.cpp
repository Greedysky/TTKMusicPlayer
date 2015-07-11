#include "musicsystemtraymenu.h"
#include "core\musicobject.h"
#include <QWidgetAction>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MusicSystemTrayMenu::MusicSystemTrayMenu(QWidget *parent)
    : QMenu(parent)
{
    setStyleSheet(MusicObject::MusicSystemTrayMenu);
    createWidgetActions();
    addAction(m_widgetAction);
    m_showLrcAction = new QAction(QIcon(":/contextMenu/lrc"),tr("showDeskLrc"),this);
    connect(m_showLrcAction,SIGNAL(triggered()),SLOT(showDesktopLrc()));
    m_lockLrcAction = new QAction(QIcon(":/contextMenu/lock"),tr("lockLrc"),this);
    connect(m_lockLrcAction,SIGNAL(triggered()),SIGNAL(setWindowLockedChanged()));

    addSeparator();
    addAction(QIcon(":/contextMenu/window"),tr("showMainWindow"),parent,SLOT(showNormal()));
    addSeparator();
    addAction(QIcon(":/contextMenu/setting"),tr("showSetting"),parent,SLOT(musicSetting()));
    addAction(m_showLrcAction);
    addSeparator();
    addAction(m_lockLrcAction);
    addSeparator();
    addAction(QIcon(":/contextMenu/quit"),tr("appClose"),parent,SLOT(quitWindowClose()));
}

MusicSystemTrayMenu::~MusicSystemTrayMenu()
{
    delete m_lockLrcAction;
    delete m_showLrcAction;
    delete m_widgetAction;
}

void MusicSystemTrayMenu::createWidgetActions()
{
    m_widgetAction = new QWidgetAction(this);
    QWidget *widgetActionContainer = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout(widgetActionContainer);

    QWidget *widgetContainer = new QWidget(widgetActionContainer);
    QHBoxLayout *box = new QHBoxLayout(widgetContainer);

    m_previousPlay = new QToolButton(widgetContainer);
    m_nextPlay = new QToolButton(widgetContainer);
    m_PlayOrStop = new QToolButton(widgetContainer);

    m_previousPlay->setIcon(QIcon(QString::fromUtf8(":/image/privious")));
    m_nextPlay->setIcon(QIcon(QString::fromUtf8(":/image/next")));
    m_PlayOrStop->setIcon(QIcon(QString::fromUtf8(":/image/play")));

    m_previousPlay->setIconSize(QSize(40,40));
    m_nextPlay->setIconSize(QSize(40,40));
    m_PlayOrStop->setIconSize(QSize(45,45));

    m_previousPlay->setStyleSheet(MusicObject::MusicSystemTrayToolButtonStyle);
    m_nextPlay->setStyleSheet(MusicObject::MusicSystemTrayToolButtonStyle);
    m_PlayOrStop->setStyleSheet(MusicObject::MusicSystemTrayToolButtonStyle);

    m_previousPlay->setCursor(QCursor(Qt::PointingHandCursor));
    m_nextPlay->setCursor(QCursor(Qt::PointingHandCursor));
    m_PlayOrStop->setCursor(QCursor(Qt::PointingHandCursor));

    m_previousPlay->setToolTip(tr("Privious"));
    m_nextPlay->setToolTip(tr("Next"));
    m_PlayOrStop->setToolTip(tr("Play"));

    box->addWidget(m_previousPlay);
    box->addWidget(m_PlayOrStop);
    box->addWidget(m_nextPlay);

    m_showText = new QLabel(widgetActionContainer);
    m_showText->setAlignment(Qt::AlignCenter);
    m_showText->setStyleSheet("color:#888888");
    vbox->addWidget(widgetContainer);
    vbox->addWidget(m_showText);
    widgetActionContainer->setLayout(vbox);
    m_widgetAction->setDefaultWidget(widgetActionContainer);

    connect(m_previousPlay,SIGNAL(clicked()),parent(),SLOT(musicPlayPrivious()));
    connect(m_nextPlay,SIGNAL(clicked()),parent(),SLOT(musicPlayNext()));
//    connect(m_PlayOrStop,SIGNAL(clicked()),SLOT(musicKey()));
    connect(m_PlayOrStop,SIGNAL(clicked()),parent(),SLOT(musicKey()));
}

void MusicSystemTrayMenu::setLabelText(const QString& text)
{
    QFontMetrics str(font());
    m_showText->setText(str.elidedText(text, Qt::ElideRight,160));
    m_showText->setToolTip(text);
}

void MusicSystemTrayMenu::showDesktopLrc(const QString &show)
{
    (show == "true") ? m_showLrcAction->setText(tr("hideDeskLrc"))
                     : m_showLrcAction->setText(tr("showDeskLrc"));
    (show == "true") ? m_lockLrcAction->setEnabled(true)
                     : m_lockLrcAction->setEnabled(false);
}

void MusicSystemTrayMenu::lockDesktopLrc(bool lock)
{
    !lock ? m_lockLrcAction->setText(tr("lockLrc"))
          : m_lockLrcAction->setText(tr("unlockLrc"));
}

void MusicSystemTrayMenu::showPlayStatus(bool status)
{
    m_PlayOrStop->setIcon(!status ? QIcon(QString::fromUtf8(":/image/stop"))
                                  : QIcon(QString::fromUtf8(":/image/play")) );
}

void MusicSystemTrayMenu::showDesktopLrc()
{
    bool show = m_showLrcAction->text().trimmed() == tr("showDeskLrc").trimmed();
    show ? m_showLrcAction->setText(tr("hideDeskLrc"))
         : m_showLrcAction->setText(tr("showDeskLrc"));
    m_lockLrcAction->setEnabled(show);
    emit setShowDesktopLrc(show);
}
