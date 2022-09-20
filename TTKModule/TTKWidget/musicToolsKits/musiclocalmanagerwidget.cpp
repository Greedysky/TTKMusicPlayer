#include "musiclocalmanagerwidget.h"
#include "musicsettingmanager.h"

MusicLocalManagerWidget::MusicLocalManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle12 + MusicUIObject::MQSSColorStyle09);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
}

MusicLocalManagerWidget::~MusicLocalManagerWidget()
{

}

void MusicLocalManagerWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty())
    {
        const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        m_resizeWidgets[0]->setFixedWidth((width - WINDOW_WIDTH_MIN) + 540);
    }
}

void MusicLocalManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}
