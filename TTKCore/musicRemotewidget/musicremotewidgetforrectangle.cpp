#include "musicremotewidgetforrectangle.h"
#include "musicsettingmanager.h"
#include "musicmarqueewidget.h"
#include "musicutils.h"

MusicRemoteWidgetForRectangle::MusicRemoteWidgetForRectangle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 230, 70);
    setAttribute(Qt::WA_TranslucentBackground);

    QSize windowSize = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    move( windowSize.width() - width() - 150, height() + 70);
    
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(5, 5, 5, 2);
    vbox->setSpacing(0);
    vbox->addWidget(m_mainWidget);

    QHBoxLayout *mhbox = new QHBoxLayout(m_mainWidget);
    mhbox->setContentsMargins(5, 0, 5, 0);
    mhbox->setSpacing(0);
    m_songNameLabel = new MusicMarqueeWidget(this);
    mhbox->addWidget(m_songNameLabel);
    m_songNameLabel->setStyleSheet(MusicUIObject::MWidgetStyle01);

    m_toolWidget = new QWidget(this);
    m_toolWidget->setObjectName("toolWidget");
    m_toolWidget->setStyleSheet("#toolWidget{" + MusicUIObject::MCustomStyle09 + "}");
    vbox->addWidget(m_toolWidget);
    QHBoxLayout *hbox = new QHBoxLayout(m_toolWidget);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);
    hbox->addWidget(m_showMainWindow);
    hbox->addWidget(m_PreSongButton);
    hbox->addWidget(m_PlayButton);
    hbox->addWidget(m_NextSongButton);
    hbox->addWidget(m_volumeWidget);
    hbox->addWidget(m_SettingButton);
    m_toolWidget->setLayout(hbox);
}

MusicRemoteWidgetForRectangle::~MusicRemoteWidgetForRectangle()
{
    delete m_songNameLabel;
}

QString MusicRemoteWidgetForRectangle::getClassName()
{
    return staticMetaObject.className();
}

void MusicRemoteWidgetForRectangle::setLabelText(const QString &value)
{
    m_songNameLabel->setText(MusicUtils::UWidget::elidedText(font(), value,
                             Qt::ElideRight, 350));
}
