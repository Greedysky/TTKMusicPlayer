#include "musicremotewidgetforsimplestyle.h"
#include "musicmarqueewidget.h"
#include "musicwidgetutils.h"

MusicRemoteWidgetForSimpleStyle::MusicRemoteWidgetForSimpleStyle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 300, 40);
    adjustPostion(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(5, 5, 5, 2);
    vbox->addWidget(m_mainWidget);
    setLayout(vbox);

    m_songNameLabel = new MusicMarqueeWidget(this);
    m_songNameLabel->setStyleSheet(MusicUIObject::MQSSWidgetStyle01);
    m_preSongButton->hide();
    m_nextSongButton->hide();

    QHBoxLayout *mhbox = new QHBoxLayout(m_mainWidget);
    mhbox->setContentsMargins(0, 0, 3, 0);
    mhbox->setSpacing(3);
    mhbox->addWidget(m_playButton);
    mhbox->addWidget(m_songNameLabel);
    mhbox->addWidget(m_volumeWidget);
    mhbox->addWidget(m_settingButton);
    mhbox->addWidget(m_showMainWindowButton);
    m_mainWidget->setLayout(mhbox);
}

MusicRemoteWidgetForSimpleStyle::~MusicRemoteWidgetForSimpleStyle()
{
    delete m_songNameLabel;
}

void MusicRemoteWidgetForSimpleStyle::setLabelText(const QString &value)
{
    m_songNameLabel->setText(MusicUtils::Widget::elidedText(font(), value, Qt::ElideRight, width() - 20));
}
