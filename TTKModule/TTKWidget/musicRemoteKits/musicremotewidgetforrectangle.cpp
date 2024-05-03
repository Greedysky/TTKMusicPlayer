#include "musicremotewidgetforrectangle.h"
#include "musicmarqueewidget.h"

MusicRemoteWidgetForRectangle::MusicRemoteWidgetForRectangle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 230, 70);
    adjustPosition(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(5, 5, 5, 2);
    vbox->setSpacing(0);
    vbox->addWidget(m_mainWidget);

    QHBoxLayout *mhbox = new QHBoxLayout(m_mainWidget);
    mhbox->setContentsMargins(5, 0, 5, 0);
    mhbox->setSpacing(0);

    m_songNameLabel = new MusicMarqueeWidget(this);
    mhbox->addWidget(m_songNameLabel);
    m_songNameLabel->setStyleSheet(TTK::UI::WidgetStyle01);

    m_toolWidget = new QWidget(this);
    m_toolWidget->setObjectName("ToolWidget");
    m_toolWidget->setStyleSheet(QString("#%1{ %2 }").arg(m_toolWidget->objectName(), TTK::UI::BackgroundStyle04));
    vbox->addWidget(m_toolWidget);

    QHBoxLayout *hbox = new QHBoxLayout(m_toolWidget);
    hbox->setContentsMargins(0, 0, 3, 0);
    hbox->setSpacing(0);
    hbox->addWidget(m_preSongButton);
    hbox->addWidget(m_playButton);
    hbox->addWidget(m_nextSongButton);
    hbox->addWidget(m_volumeWidget);
    hbox->addWidget(m_settingButton);
    hbox->addWidget(m_showMainWindowButton);
    m_toolWidget->setLayout(hbox);
}

MusicRemoteWidgetForRectangle::~MusicRemoteWidgetForRectangle()
{
    delete m_songNameLabel;
}

void MusicRemoteWidgetForRectangle::setLabelText(const QString &value)
{
    m_songNameLabel->setText(TTK::Widget::elidedText(font(), value, Qt::ElideRight, width() - 20));
}
