#include "musicremotewidgetforripples.h"
#include "musictinyuiobject.h"
#include "musicwidgetutils.h"
#include "musicmarqueewidget.h"
#include "musicqmmputils.h"
///qmmp incldue
#include "visual.h"
#include "visualfactory.h"

MusicRemoteWidgetForRipples::MusicRemoteWidgetForRipples(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 320, 100);
    adjustPostion(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);
    hbox->addWidget(m_mainWidget);
    setLayout(hbox);

    MusicUtils::QMMP::enableVisualPlugin("outerripples", true);

    QWidget *bottomWidget = new QWidget(m_mainWidget);
    QVBoxLayout *mhbox = new QVBoxLayout(m_mainWidget);
    mhbox->setContentsMargins(5, 0, 5, 0);
    mhbox->setSpacing(0);

    const QList<Visual*> *vs = Visual::visuals();
    if(!vs->isEmpty() && vs->last())
    {
        mhbox->addWidget(vs->last());
    }
    mhbox->addWidget(bottomWidget);
    m_mainWidget->setLayout(mhbox);

    m_songNameLabel = new MusicMarqueeWidget(this);
    m_songNameLabel->setStyleSheet(MusicUIObject::MWidgetStyle01);
    m_songNameLabel->setFixedWidth(160);

    m_PreSongButton->hide();
    m_NextSongButton->hide();

    QHBoxLayout *bottomWidgetBox = new QHBoxLayout(bottomWidget);
    bottomWidgetBox->setContentsMargins(0, 0, 0, 0);
    bottomWidgetBox->setSpacing(1);
    bottomWidgetBox->addWidget(m_PlayButton);
    bottomWidgetBox->setSpacing(1);
    bottomWidgetBox->addWidget(m_songNameLabel);
    bottomWidgetBox->setSpacing(1);
    bottomWidgetBox->addWidget(m_volumeWidget);
    bottomWidgetBox->addWidget(m_SettingButton);
    bottomWidgetBox->addWidget(m_showMainWindow);
    bottomWidget->setLayout(bottomWidgetBox);
}

MusicRemoteWidgetForRipples::~MusicRemoteWidgetForRipples()
{
    MusicUtils::QMMP::enableVisualPlugin("outerripples", false);
    delete m_songNameLabel;
}

void MusicRemoteWidgetForRipples::setLabelText(const QString &value)
{
    m_songNameLabel->setText(MusicUtils::Widget::elidedText(font(), value, Qt::ElideRight, 350));
}
