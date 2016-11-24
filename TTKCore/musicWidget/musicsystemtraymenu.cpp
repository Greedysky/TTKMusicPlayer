#include "musicsystemtraymenu.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"
#include "musiccontextuiobject.h"
#include "musictinyuiobject.h"
#include "musicclickedslider.h"

#include <QLabel>
#include <QBoxLayout>
#include <QToolButton>
#include <QWidgetAction>

MusicSystemTrayMenu::MusicSystemTrayMenu(QWidget *parent)
    : QMenu(parent)
{
    setStyleSheet(MusicUIObject::MMenuStyle02);

    m_showLrcAction = new QAction(QIcon(":/contextMenu/btn_lrc"),tr("showDeskLrc"), this);
    connect(m_showLrcAction, SIGNAL(triggered()), SLOT(showDesktopLrc()));
    m_lockLrcAction = new QAction(QIcon(":/contextMenu/btn_lock"), tr("lockLrc"), this);
    connect(m_lockLrcAction, SIGNAL(triggered()), SIGNAL(setWindowLockedChanged()));

    createPlayWidgetActions();
    addSeparator();
    createVolumeWidgetActions();
    addAction(m_showLrcAction);
    addAction(m_lockLrcAction);
    addSeparator();
    addAction(tr("showMainWindow"), parent, SLOT(showNormal()));
    addSeparator();
    addAction(QIcon(":/contextMenu/btn_setting"), tr("showSetting"), parent, SLOT(musicSetting()));
    addSeparator();
    addAction(QIcon(":/contextMenu/btn_login"), tr("logout"));
    addAction(tr("appClose"), parent, SLOT(quitWindowClose()));
}

MusicSystemTrayMenu::~MusicSystemTrayMenu()
{
    delete m_showText;
    delete m_PlayOrStop;
    delete m_volumeButton;
    delete m_showLrcAction;
    delete m_lockLrcAction;
    delete m_volumeSlider;
}

QString MusicSystemTrayMenu::getClassName()
{
    return staticMetaObject.className();
}

void MusicSystemTrayMenu::setLabelText(const QString &text) const
{
    m_showText->setText(MusicUtils::Widget::elidedText(font(), text, Qt::ElideRight, 160));
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
    m_PlayOrStop->setStyleSheet(status ? MusicUIObject::MKGContextPlay : MusicUIObject::MKGContextPause);
}

void MusicSystemTrayMenu::setVolumeValue(int value) const
{
    m_volumeSlider->blockSignals(true);
    m_volumeSlider->setValue(value);
    m_volumeSlider->blockSignals(false);

    QString style = MusicUIObject::MKGTinyBtnSound;
    if(66 < value && value <=100)
    {
        style += "QToolButton{ margin-left:-48px; }";
    }
    else if(33 < value && value <=66)
    {
        style += "QToolButton{ margin-left:-32px; }";
    }
    else if(0 < value && value <=33)
    {
        style += "QToolButton{ margin-left:-16px; }";
    }
    else
    {
        style += "QToolButton{ margin-left:-64px; }";
    }
    m_volumeButton->setStyleSheet(style);
}

void MusicSystemTrayMenu::showDesktopLrc()
{
    bool show = m_showLrcAction->text().trimmed() == tr("showDeskLrc").trimmed();
    m_showLrcAction->setText(show ? tr("hideDeskLrc") : tr("showDeskLrc"));
    m_lockLrcAction->setEnabled(show);
    emit setShowDesktopLrc(show);
}

void MusicSystemTrayMenu::createPlayWidgetActions()
{
    QWidgetAction *widgetAction = new QWidgetAction(this);
    QWidget *widgetActionContainer = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout(widgetActionContainer);

    QWidget *widgetContainer = new QWidget(widgetActionContainer);
    widgetContainer->setFixedWidth(172);
    QHBoxLayout *box = new QHBoxLayout(widgetContainer);
    box->setContentsMargins(9, 2, 9, 9);

    QToolButton *previousPlay = new QToolButton(widgetContainer);
    QToolButton *nextPlay = new QToolButton(widgetContainer);
    m_PlayOrStop = new QToolButton(widgetContainer);

    previousPlay->setFixedSize(32, 32);
    nextPlay->setFixedSize(32, 32);
    m_PlayOrStop->setFixedSize(32, 32);

    previousPlay->setStyleSheet(MusicUIObject::MKGContextPrevious);
    nextPlay->setStyleSheet(MusicUIObject::MKGContextNext);
    m_PlayOrStop->setStyleSheet(MusicUIObject::MKGContextPlay);

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
    m_showText->setStyleSheet(MusicUIObject::MColorStyle03);
    vbox->addWidget(widgetContainer);
    vbox->addWidget(m_showText);
    widgetActionContainer->setLayout(vbox);
    widgetAction->setDefaultWidget(widgetActionContainer);
    addAction(widgetAction);

    connect(previousPlay, SIGNAL(clicked()), parent(), SLOT(musicPlayPrevious()));
    connect(nextPlay, SIGNAL(clicked()), parent(), SLOT(musicPlayNext()));
    connect(m_PlayOrStop, SIGNAL(clicked()), parent(), SLOT(musicStatePlay()));
}

void MusicSystemTrayMenu::createVolumeWidgetActions()
{
    QWidgetAction *widgetAction = new QWidgetAction(this);
    QWidget *widgetActionContainer = new QWidget(this);
    QHBoxLayout *vbox = new QHBoxLayout(widgetActionContainer);
    vbox->setContentsMargins(9, 2, 9, 9);

    m_volumeButton = new QToolButton(widgetActionContainer);
    m_volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumeButton->setFixedSize(16, 16);

    m_volumeSlider = new MusicClickedSlider(Qt::Horizontal, widgetActionContainer);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle06);

    vbox->addWidget(m_volumeButton);
    vbox->addWidget(m_volumeSlider);
    widgetActionContainer->setLayout(vbox);

    widgetAction->setDefaultWidget(widgetActionContainer);
    addAction(widgetAction);

    connect(m_volumeButton, SIGNAL(clicked()), parent(), SLOT(musicVolumeMute()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), parent(), SLOT(musicVolumeChanged(int)));
}
