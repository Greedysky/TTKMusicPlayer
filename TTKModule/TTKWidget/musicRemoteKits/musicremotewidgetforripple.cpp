#include "musicremotewidgetforripple.h"
#include "musictinyuiobject.h"
#include "musicmarqueewidget.h"
#include "musicqmmputils.h"
///
#include <qmmp/visual.h>

MusicRemoteWidgetForRipple::MusicRemoteWidgetForRipple(QWidget *parent)
    : MusicRemoteWidget(parent),
      m_mode(true),
      m_visual(nullptr)
{
    setGeometry(200, 200, 320, 100);
    adjustPosition(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);
    hbox->addWidget(m_mainWidget);
    setLayout(hbox);

    QWidget *bottomWidget = new QWidget(m_mainWidget);
    m_visualLayout = new QVBoxLayout(m_mainWidget);
    m_visualLayout->setContentsMargins(5, 0, 5, 0);
    m_visualLayout->setSpacing(0);
    m_visualLayout->addWidget(bottomWidget);
    m_mainWidget->setLayout(m_visualLayout);

    createVisualWidget();

    m_songNameLabel = new MusicMarqueeWidget(this);
    m_songNameLabel->setStyleSheet(TTK::UI::WidgetStyle01);

    m_preSongButton->hide();
    m_nextSongButton->hide();

    m_visualModeButton = new QPushButton(this);
#ifdef Q_OS_UNIX
    m_visualModeButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_visualModeButton->setToolTip(tr("Visual Mode"));
    m_visualModeButton->setFixedSize(26, 18);
    m_visualModeButton->setStyleSheet(TTK::UI::TinyBtnVisual);
    m_visualModeButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_visualModeButton, SIGNAL(clicked()), SLOT(visualModeChanged()));

    QHBoxLayout *bottomWidgetBox = new QHBoxLayout(bottomWidget);
    bottomWidgetBox->setContentsMargins(0, 0, 0, 0);
    bottomWidgetBox->setSpacing(1);
    bottomWidgetBox->addWidget(m_playButton);
    bottomWidgetBox->setSpacing(1);
    bottomWidgetBox->addWidget(m_songNameLabel);
    bottomWidgetBox->setSpacing(1);
    bottomWidgetBox->addWidget(m_volumeWidget);
    bottomWidgetBox->addWidget(m_visualModeButton);
    bottomWidgetBox->addWidget(m_settingButton);
    bottomWidgetBox->addWidget(m_showMainWindowButton);
    bottomWidget->setLayout(bottomWidgetBox);
}

MusicRemoteWidgetForRipple::~MusicRemoteWidgetForRipple()
{
    removeVisualWidget();
    delete m_visualModeButton;
    delete m_songNameLabel;
}

void MusicRemoteWidgetForRipple::setLabelText(const QString &value)
{
    m_songNameLabel->setText(TTK::Widget::elidedText(font(), value, Qt::ElideRight, width() - 20));
}

void MusicRemoteWidgetForRipple::visualModeChanged()
{
    removeVisualWidget();
    m_mode = !m_mode;
    createVisualWidget();
}

void MusicRemoteWidgetForRipple::createVisualWidget()
{
    TTK::TTKQmmp::setVisualEnabled(m_mode ? "outerripples" : "outerrayswave", true, this);

    const QList<Visual*> *vs = Visual::visuals();
    if(!vs->isEmpty() && vs->last())
    {
        m_visual = vs->last();
        m_visualLayout->insertWidget(0, m_visual);
    }
}

void MusicRemoteWidgetForRipple::removeVisualWidget()
{
    if(m_visualLayout->count() > 1)
    {
        m_visualLayout->removeWidget(m_visual);
        m_visual = nullptr;
    }

    TTK::TTKQmmp::setVisualEnabled(m_mode ? "outerripples" : "outerrayswave", false, this);
}
