#include "musicsystemtraymenu.h"
#include "musicuiobject.h"

#include <QWidgetAction>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

MusicSystemTrayMenu::MusicSystemTrayMenu(QWidget *parent)
    : QMenu(parent)
{
    setStyleSheet(MusicUIObject::MMenuStyle01);

    createPlayWidgetActions();
    createVolumeWidgetActions();
    addAction(m_widgetAction);
    addAction(m_volumeAction);
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
    delete m_showText;
    delete m_PlayOrStop;
    delete m_volumeSlider;
    delete m_showLrcAction;
    delete m_lockLrcAction;
    delete m_widgetAction;
    delete m_volumeAction;
}

void MusicSystemTrayMenu::createPlayWidgetActions()
{
    m_widgetAction = new QWidgetAction(this);
    QWidget *widgetActionContainer = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout(widgetActionContainer);
    vbox->setMargin(0);

    QWidget *widgetContainer = new QWidget(widgetActionContainer);
    QHBoxLayout *box = new QHBoxLayout(widgetContainer);
    box->setMargin(0);

    QToolButton *previousPlay = new QToolButton(widgetContainer);
    QToolButton *nextPlay = new QToolButton(widgetContainer);
    m_PlayOrStop = new QToolButton(widgetContainer);

    previousPlay->setIcon(QIcon(QString::fromUtf8(":/image/privious")));
    nextPlay->setIcon(QIcon(QString::fromUtf8(":/image/next")));
    m_PlayOrStop->setIcon(QIcon(QString::fromUtf8(":/image/play")));

    previousPlay->setIconSize(QSize(40,40));
    nextPlay->setIconSize(QSize(40,40));
    m_PlayOrStop->setIconSize(QSize(45,45));

    previousPlay->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    nextPlay->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_PlayOrStop->setStyleSheet(MusicUIObject::MToolButtonStyle04);

    previousPlay->setCursor(QCursor(Qt::PointingHandCursor));
    nextPlay->setCursor(QCursor(Qt::PointingHandCursor));
    m_PlayOrStop->setCursor(QCursor(Qt::PointingHandCursor));

    previousPlay->setToolTip(tr("Privious"));
    nextPlay->setToolTip(tr("Next"));
    m_PlayOrStop->setToolTip(tr("Play"));

    box->addWidget(previousPlay);
    box->addWidget(m_PlayOrStop);
    box->addWidget(nextPlay);

    m_showText = new QLabel(widgetActionContainer);
    m_showText->setAlignment(Qt::AlignCenter);
    m_showText->setStyleSheet(MusicUIObject::MCustomStyle11);
    vbox->addWidget(widgetContainer);
    vbox->addWidget(m_showText);
    widgetActionContainer->setLayout(vbox);
    m_widgetAction->setDefaultWidget(widgetActionContainer);

    connect(previousPlay, SIGNAL(clicked()), parent(), SLOT(musicPlayPrivious()));
    connect(nextPlay, SIGNAL(clicked()), parent(), SLOT(musicPlayNext()));
    connect(m_PlayOrStop, SIGNAL(clicked()), parent(), SLOT(musicKey()));
}

void MusicSystemTrayMenu::createVolumeWidgetActions()
{
    m_volumeAction = new QWidgetAction(this);
    QWidget *widgetContainer = new QWidget(this);
    QHBoxLayout *box = new QHBoxLayout(widgetContainer);

    m_volumeSlider = new QSlider(Qt::Horizontal, widgetContainer);
    m_volumeSlider->setRange(0,100);
    m_volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle04);
    box->addWidget(m_volumeSlider);
    widgetContainer->setLayout(box);

    m_volumeAction->setDefaultWidget(widgetContainer);
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), parent(), SLOT(musicVolumeChanged(int)));
}

void MusicSystemTrayMenu::setLabelText(const QString &text) const
{
    QFontMetrics str(font());
    m_showText->setText(str.elidedText(text, Qt::ElideRight,160));
    m_showText->setToolTip(text);
}

void MusicSystemTrayMenu::showDesktopLrc(const QString &show) const
{
    m_showLrcAction->setText((show == "true") ? tr("hideDeskLrc") : tr("showDeskLrc"));
    m_lockLrcAction->setEnabled((show == "true") ? true : false);
}

void MusicSystemTrayMenu::lockDesktopLrc(bool lock)
{
    m_lockLrcAction->setText(!lock ? tr("lockLrc") : tr("unlockLrc"));
}

void MusicSystemTrayMenu::showPlayStatus(bool status) const
{
    m_PlayOrStop->setIcon(QIcon(QString::fromUtf8(!status ? ":/image/stop"
                                                          : ":/image/play")) );
}

void MusicSystemTrayMenu::setVolume(int index)
{
    blockSignals(true);
    m_volumeSlider->setValue(index);
    blockSignals(false);
}

void MusicSystemTrayMenu::showDesktopLrc()
{
    bool show = m_showLrcAction->text().trimmed() == tr("showDeskLrc").trimmed();
    m_showLrcAction->setText(show ? tr("hideDeskLrc") : tr("showDeskLrc"));
    m_lockLrcAction->setEnabled(show);
    emit setShowDesktopLrc(show);
}
