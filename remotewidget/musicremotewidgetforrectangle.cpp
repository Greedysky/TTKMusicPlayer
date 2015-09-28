#include "musicremotewidgetforrectangle.h"
#include "musicsettingmanager.h"

MusicRemoteWidgetForRectangle::MusicRemoteWidgetForRectangle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200,200,230,70);

    QSize windowSize = M_SETTING->value(MusicSettingManager::ScreenSize).toSize();
    move( windowSize.width() - width() - 150, height() + 70);

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(255,0,0,10));
    setPalette(pal);
    
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(5,5,5,2);
    vbox->setSpacing(0);
    vbox->addWidget(m_mainWidget);

    m_PreSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_NextSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_PlayButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_SettingButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_mainWidget->setStyleSheet("#mainWidget{" + MusicUIObject::MCustomStyle09 + "}");

    QHBoxLayout *mhbox = new QHBoxLayout(m_mainWidget);
    mhbox->setContentsMargins(0,0,0,0);
    mhbox->setSpacing(0);
    m_songNameLabel = new QLabel(this);
    mhbox->addWidget(m_songNameLabel);
    m_songNameLabel->setStyleSheet(MusicUIObject::MLabelStyle01);

    m_toolWidget = new QWidget(this);
    vbox->addWidget(m_toolWidget);
    QHBoxLayout *hbox = new QHBoxLayout(m_toolWidget);
    hbox->setContentsMargins(0,0,0,0);
    hbox->setSpacing(0);
    hbox->addWidget(m_showMainWindow);
    hbox->addWidget(m_PreSongButton);
    hbox->addWidget(m_PlayButton);
    hbox->addWidget(m_NextSongButton);
    hbox->addWidget(m_volumeWidget);
    hbox->addWidget(m_SettingButton);
}

MusicRemoteWidgetForRectangle::~MusicRemoteWidgetForRectangle()
{
    delete m_songNameLabel;
}

void MusicRemoteWidgetForRectangle::setLabelText(const QString &value)
{
    m_songNameLabel->setText(QFontMetrics(font()).elidedText(value,
                             Qt::ElideRight, 190));
}
