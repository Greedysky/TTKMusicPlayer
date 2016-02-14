#include "musicremotewidgetforrectangle.h"
#include "musicsettingmanager.h"
#include "musicmarqueewidget.h"

MusicRemoteWidgetForRectangle::MusicRemoteWidgetForRectangle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 230, 70);
    setAttribute(Qt::WA_TranslucentBackground);

    QSize windowSize = M_SETTING->value(MusicSettingManager::ScreenSize).toSize();
    move( windowSize.width() - width() - 150, height() + 70);
    
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(5, 5, 5, 2);
    vbox->setSpacing(0);
    vbox->addWidget(m_mainWidget);

    m_PreSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_NextSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_PlayButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_SettingButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_mainWidget->setStyleSheet("#mainWidget{" + MusicUIObject::MCustomStyle09 + "}");

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

void MusicRemoteWidgetForRectangle::setLabelText(const QString &value)
{
    m_songNameLabel->setText(QFontMetrics(font()).elidedText(value,
                             Qt::ElideRight, 350));
}
