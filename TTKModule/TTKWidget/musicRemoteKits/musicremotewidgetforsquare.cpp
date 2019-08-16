#include "musicremotewidgetforsquare.h"

MusicRemoteWidgetForSquare::MusicRemoteWidgetForSquare(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 100, 100);
    adjustPostion(this);

    QGridLayout* grid = new QGridLayout(this);
    for(int i=0; i<3; i++)
    {
        grid->setRowMinimumHeight(i, 30);
        grid->setRowStretch(i, 1);
    }
    for(int j=0; j<3; j++)
    {
        grid->setColumnMinimumWidth(j, 30);
        grid->setColumnStretch(j, 1);
    }
    m_mainWidget->setFixedSize(70, 70);

    grid->addWidget(m_PlayButton, 0, 1, Qt::AlignCenter);
    grid->addWidget(m_PreSongButton, 1, 0, Qt::AlignCenter);
    grid->addWidget(m_NextSongButton, 1, 2, Qt::AlignCenter);
    grid->addWidget(m_SettingButton, 2, 1, Qt::AlignCenter);
    grid->addWidget(m_mainWidget, 1, 1, Qt::AlignCenter);

    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(m_mainWidget);
    mainWidgetLayout->setContentsMargins(0, 0, 0, 0);
    mainWidgetLayout->setSpacing(0);
    mainWidgetLayout->addWidget(m_showMainWindow);
    mainWidgetLayout->setAlignment(m_showMainWindow, Qt::AlignCenter);
    mainWidgetLayout->addWidget(m_volumeWidget);

    m_interval = 0.0f;
    for(int i=0; i<4; ++i)
    {
        m_effect[i] = new QGraphicsOpacityEffect(this);
        m_effect[i]->setOpacity(m_interval);
    }

    connect(&m_enterTimer, SIGNAL(timeout()), SLOT(enterTimeout()));
    connect(&m_leaveTimer, SIGNAL(timeout()), SLOT(leaveTimeout()));
}

MusicRemoteWidgetForSquare::~MusicRemoteWidgetForSquare()
{
    for(int i=0; i<4; ++i)
    {
        delete m_effect[i];
    }
}

void MusicRemoteWidgetForSquare::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_leaveTimer.stop();
    m_enterTimer.start(30);
}

void MusicRemoteWidgetForSquare::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_enterTimer.stop();
    m_leaveTimer.start(30);
}

void MusicRemoteWidgetForSquare::enterTimeout()
{
    m_interval += 0.1f;
    for(int i=0; i<4; ++i)
    {
        m_effect[i]->setOpacity(m_interval);
    }
    m_PreSongButton->setGraphicsEffect(m_effect[0]);
    m_NextSongButton->setGraphicsEffect(m_effect[1]);
    m_PlayButton->setGraphicsEffect(m_effect[2]);
    m_SettingButton->setGraphicsEffect(m_effect[3]);
}

void MusicRemoteWidgetForSquare::leaveTimeout()
{
    m_interval -= 0.1f;
    for(int i=0; i<4; ++i)
    {
        m_effect[i]->setOpacity(m_interval);
    }
    m_PreSongButton->setGraphicsEffect(m_effect[0]);
    m_NextSongButton->setGraphicsEffect(m_effect[1]);
    m_PlayButton->setGraphicsEffect(m_effect[2]);
    m_SettingButton->setGraphicsEffect(m_effect[3]);
}
