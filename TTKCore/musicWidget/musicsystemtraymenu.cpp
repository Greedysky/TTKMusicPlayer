#include "musicsystemtraymenu.h"
#include "musicuiobject.h"
#include "musicutils.h"
#include "musickugouuiobject.h"

#include <QWidgetAction>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MusicSystemTrayMenu::MusicSystemTrayMenu(QWidget *parent)
    : QMenu(parent)
{
    setStyleSheet(MusicUIObject::MMenuStyle02);
    createPlayWidgetActions();
    addAction(m_widgetAction);

    m_showLrcAction = new QAction(QIcon(":/contextMenu/lrc"),tr("showDeskLrc"), this);
    connect(m_showLrcAction, SIGNAL(triggered()), SLOT(showDesktopLrc()));
    m_lockLrcAction = new QAction(QIcon(":/contextMenu/lock"), tr("lockLrc"), this);
    connect(m_lockLrcAction, SIGNAL(triggered()), SIGNAL(setWindowLockedChanged()));

    addSeparator();
    addAction(QIcon(":/contextMenu/window"), tr("showMainWindow"), parent, SLOT(showNormal()));
    addSeparator();
    addAction(QIcon(":/contextMenu/setting"), tr("showSetting"), parent, SLOT(musicSetting()));
    addAction(m_showLrcAction);
    addSeparator();
    addAction(m_lockLrcAction);
    addSeparator();
    addAction(QIcon(":/contextMenu/quit"), tr("appClose"), parent, SLOT(quitWindowClose()));
}

MusicSystemTrayMenu::~MusicSystemTrayMenu()
{
    delete m_showText;
    delete m_PlayOrStop;
    delete m_showLrcAction;
    delete m_lockLrcAction;
    delete m_widgetAction;
}

QString MusicSystemTrayMenu::getClassName()
{
    return staticMetaObject.className();
}

void MusicSystemTrayMenu::createPlayWidgetActions()
{
    m_widgetAction = new QWidgetAction(this);
    QWidget *widgetActionContainer = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout(widgetActionContainer);

    QWidget *widgetContainer = new QWidget(widgetActionContainer);
    QHBoxLayout *box = new QHBoxLayout(widgetContainer);
    box->setContentsMargins(9, 2, 9, 9);

    QToolButton *previousPlay = new QToolButton(widgetContainer);
    QToolButton *nextPlay = new QToolButton(widgetContainer);
    m_PlayOrStop = new QToolButton(widgetContainer);

    previousPlay->setFixedSize(32, 32);
    nextPlay->setFixedSize(32, 32);
    m_PlayOrStop->setFixedSize(32, 32);

    previousPlay->setStyleSheet(MusicKuGouUIObject::MKGContextPrevious);
    nextPlay->setStyleSheet(MusicKuGouUIObject::MKGContextNext);
    m_PlayOrStop->setStyleSheet(MusicKuGouUIObject::MKGContextPlay);

    previousPlay->setCursor(QCursor(Qt::PointingHandCursor));
    nextPlay->setCursor(QCursor(Qt::PointingHandCursor));
    m_PlayOrStop->setCursor(QCursor(Qt::PointingHandCursor));

    previousPlay->setToolTip(tr("Previous"));
    nextPlay->setToolTip(tr("Next"));
    m_PlayOrStop->setToolTip(tr("Play"));

    box->addWidget(previousPlay);
    box->addWidget(m_PlayOrStop);
    box->addWidget(nextPlay);

    m_showText = new QLabel(widgetActionContainer);
    m_showText->setAlignment(Qt::AlignCenter);
    m_showText->setStyleSheet(MusicUIObject::MCustomStyle12);
    vbox->addWidget(widgetContainer);
    vbox->addWidget(m_showText);
    widgetActionContainer->setLayout(vbox);
    m_widgetAction->setDefaultWidget(widgetActionContainer);

    connect(previousPlay, SIGNAL(clicked()), parent(), SLOT(musicPlayPrevious()));
    connect(nextPlay, SIGNAL(clicked()), parent(), SLOT(musicPlayNext()));
    connect(m_PlayOrStop, SIGNAL(clicked()), parent(), SLOT(musicStatePlay()));
}

void MusicSystemTrayMenu::setLabelText(const QString &text) const
{
    m_showText->setText(MusicUtils::UWidget::elidedText(font(), text, Qt::ElideRight, 160));
    m_showText->setToolTip(text);
}

void MusicSystemTrayMenu::showDesktopLrc(bool show) const
{
    m_showLrcAction->setText( show ? tr("hideDeskLrc") : tr("showDeskLrc"));
    m_lockLrcAction->setEnabled( show );
}

void MusicSystemTrayMenu::lockDesktopLrc(bool lock)
{
    m_lockLrcAction->setText(!lock ? tr("lockLrc") : tr("unlockLrc"));
}

void MusicSystemTrayMenu::showPlayStatus(bool status) const
{
    m_PlayOrStop->setStyleSheet(status ? MusicKuGouUIObject::MKGContextPlay :
                                         MusicKuGouUIObject::MKGContextPause);
}

void MusicSystemTrayMenu::showDesktopLrc()
{
    bool show = m_showLrcAction->text().trimmed() == tr("showDeskLrc").trimmed();
    m_showLrcAction->setText(show ? tr("hideDeskLrc") : tr("showDeskLrc"));
    m_lockLrcAction->setEnabled(show);
    emit setShowDesktopLrc(show);
}
