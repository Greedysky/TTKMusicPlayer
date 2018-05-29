#include "musicremotewidgetforrectangle.h"
#include "musicmarqueewidget.h"
#include "musicwidgetutils.h"

MusicRemoteWidgetForRectangle::MusicRemoteWidgetForRectangle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 230, 70);
    adjustPostion(this);

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
    m_toolWidget->setStyleSheet(QString("#toolWidget{%1}").arg(MusicUIObject::MBackgroundStyle04));
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
    m_songNameLabel->setText(MusicUtils::Widget::elidedText(font(), value, Qt::ElideRight, 350));
}
