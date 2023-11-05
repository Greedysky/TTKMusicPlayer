#include "musicsystemtraymenu.h"
#include "musiccontextuiobject.h"
#include "musictinyuiobject.h"
#include "musicrightareawidget.h"
#include "musicwidgetutils.h"
#include "ttkclickedslider.h"

#include <QWidgetAction>

MusicSystemTrayMenu::MusicSystemTrayMenu(QWidget *parent)
    : QMenu(parent)
{
    setStyleSheet(TTK::UI::MenuStyle02);

    m_showLrcAction = new QAction(QIcon(":/contextMenu/btn_lrc_label"),tr("Show Desktop Lrc"), this);
    connect(m_showLrcAction, SIGNAL(triggered()), SLOT(showDesktopLrc()));

    m_lockLrcAction = new QAction(QIcon(":/contextMenu/btn_lock"), tr("Lock Lrc"), this);
    connect(m_lockLrcAction, SIGNAL(triggered()), SLOT(setWindowLockedChanged()));
#ifdef Q_OS_UNIX
    addAction(QIcon(":/image/lb_app_logo"), tr("Show MainWindow"), parent, SLOT(showNormal()));
    addSeparator();
#else
    createPlayWidgetActions();
    addSeparator();
    createVolumeWidgetActions();
#endif
    addAction(m_showLrcAction);
    addAction(m_lockLrcAction);
    addSeparator();
    addAction(QIcon(":/contextMenu/btn_setting"), tr("Settings"), parent, SLOT(showSettingWidget()));
    addSeparator();
    addAction(tr("Quit"), parent, SLOT(quitWindow()));
}

MusicSystemTrayMenu::~MusicSystemTrayMenu()
{
#ifndef Q_OS_UNIX
    delete m_showText;
    delete m_playButton;
    delete m_volumeButton;
    delete m_volumeSlider;
#endif
    delete m_showLrcAction;
    delete m_lockLrcAction;
}

void MusicSystemTrayMenu::setLabelText(const QString &text) const
{
#ifndef Q_OS_UNIX
    m_showText->setText(TTK::Widget::elidedText(font(), text, Qt::ElideRight, 160));
    m_showText->setToolTip(text);
#else
    Q_UNUSED(text);
#endif
}

void MusicSystemTrayMenu::showDesktopLrc(bool show) const
{
    m_showLrcAction->setText(show ? tr("Hide Desktop Lrc") : tr("Show Desktop Lrc"));
    m_lockLrcAction->setEnabled(show);
}

void MusicSystemTrayMenu::lockDesktopLrc(bool lock)
{
    m_lockLrcAction->setText(!lock ? tr("Lock Lrc") : tr("Unlock Lrc"));
}

void MusicSystemTrayMenu::setWindowLockedChanged()
{
    MusicRightAreaWidget::instance()->setWindowLockedChanged();
}

void MusicSystemTrayMenu::setCurrentPlayStatus(bool status) const
{
#ifndef Q_OS_UNIX
    m_playButton->setStyleSheet(status ? TTK::UI::ContextPause : TTK::UI::ContextPlay);
#else
    Q_UNUSED(status);
#endif
}

void MusicSystemTrayMenu::setVolumeValue(int value) const
{
#ifndef Q_OS_UNIX
    m_volumeSlider->blockSignals(true);
    m_volumeSlider->setValue(value);
    m_volumeSlider->blockSignals(false);

    QString style = TTK::UI::TinyBtnSound;
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
#else
    Q_UNUSED(value);
#endif
}

void MusicSystemTrayMenu::showDesktopLrc()
{
    const bool show = m_showLrcAction->text().trimmed() == tr("Show Desktop Lrc").trimmed();
    m_showLrcAction->setText(show ? tr("Hide Desktop Lrc") : tr("Show Desktop Lrc"));
    m_lockLrcAction->setEnabled(show);
    MusicRightAreaWidget::instance()->setDestopLrcVisible(show);
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
    m_playButton = new QToolButton(widgetContainer);

    previousPlay->setFixedSize(32, 32);
    nextPlay->setFixedSize(32, 32);
    m_playButton->setFixedSize(32, 32);

    previousPlay->setStyleSheet(TTK::UI::ContextPrevious);
    nextPlay->setStyleSheet(TTK::UI::ContextNext);
    m_playButton->setStyleSheet(TTK::UI::ContextPlay);

    previousPlay->setCursor(QCursor(Qt::PointingHandCursor));
    nextPlay->setCursor(QCursor(Qt::PointingHandCursor));
    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));

    previousPlay->setToolTip(tr("Previous"));
    nextPlay->setToolTip(tr("Next"));
    m_playButton->setToolTip(tr("Play"));

    box->addWidget(previousPlay);
    box->addWidget(m_playButton);
    box->addWidget(nextPlay);

    m_showText = new QLabel(widgetActionContainer);
    m_showText->setAlignment(Qt::AlignCenter);
    m_showText->setStyleSheet(TTK::UI::ColorStyle03);

    vbox->addWidget(widgetContainer);
    vbox->addWidget(m_showText);
    widgetActionContainer->setLayout(vbox);
    widgetAction->setDefaultWidget(widgetActionContainer);
    addAction(widgetAction);

    connect(previousPlay, SIGNAL(clicked()), parent(), SLOT(playPrevious()));
    connect(nextPlay, SIGNAL(clicked()), parent(), SLOT(playNext()));
    connect(m_playButton, SIGNAL(clicked()), parent(), SLOT(switchToPlayState()));
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

    m_volumeSlider = new TTKClickedSlider(Qt::Horizontal, widgetActionContainer);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setStyleSheet(TTK::UI::SliderStyle08);

    vbox->addWidget(m_volumeButton);
    vbox->addWidget(m_volumeSlider);
    widgetActionContainer->setLayout(vbox);

    widgetAction->setDefaultWidget(widgetActionContainer);
    addAction(widgetAction);

    connect(m_volumeButton, SIGNAL(clicked()), parent(), SLOT(volumeMute()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), parent(), SLOT(volumeChanged(int)));
}
