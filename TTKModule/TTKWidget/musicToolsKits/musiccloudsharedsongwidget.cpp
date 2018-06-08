#include "musiccloudsharedsongwidget.h"
#include "musicwidgetheaders.h"
#include "musicrightareawidget.h"
#include "musiccloudmanagerwidget.h"

MusicCloudSharedSongWidget::MusicCloudSharedSongWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_managerWidget = new MusicCloudManagerWidget(this);
    layout->addWidget(m_managerWidget);
    setLayout(layout);
}

MusicCloudSharedSongWidget::~MusicCloudSharedSongWidget()
{
    delete m_managerWidget;
}

void MusicCloudSharedSongWidget::showMainWindow()
{
    MusicRightAreaWidget::instance()->musicFunctionClicked(MusicRightAreaWidget::CloudManagerWidget, m_managerWidget);
}

void MusicCloudSharedSongWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
