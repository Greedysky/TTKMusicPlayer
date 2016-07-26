#include "musicremotewidgetforsimplestyle.h"
#include "musicsettingmanager.h"
#include "musicmarqueewidget.h"
#include "musicutils.h"

MusicRemoteWidgetForSimpleStyle::MusicRemoteWidgetForSimpleStyle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 300, 40);
    setAttribute(Qt::WA_TranslucentBackground);

    QSize windowSize = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    move( windowSize.width() - width() - 150, height() + 70);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(5, 5, 5, 2);
    vbox->addWidget(m_mainWidget);
    setLayout(vbox);

    m_songNameLabel = new MusicMarqueeWidget(this);
    m_songNameLabel->setStyleSheet(MusicUIObject::MWidgetStyle01);
    m_PreSongButton->hide();
    m_NextSongButton->hide();

    QHBoxLayout *mhbox = new QHBoxLayout(m_mainWidget);
    mhbox->setContentsMargins(0, 0, 0, 0);
    mhbox->setSpacing(3);
    mhbox->addWidget(m_showMainWindow);
    mhbox->addWidget(m_PlayButton);
    mhbox->addWidget(m_songNameLabel);
    mhbox->addWidget(m_volumeWidget);
    mhbox->addWidget(m_SettingButton);
    m_mainWidget->setLayout(mhbox);
}

MusicRemoteWidgetForSimpleStyle::~MusicRemoteWidgetForSimpleStyle()
{
    delete m_songNameLabel;
}

QString MusicRemoteWidgetForSimpleStyle::getClassName()
{
    return staticMetaObject.className();
}

void MusicRemoteWidgetForSimpleStyle::setLabelText(const QString &value)
{
    m_songNameLabel->setText(MusicUtils::UWidget::elidedText(font(), value,
                             Qt::ElideRight, 350));
}
