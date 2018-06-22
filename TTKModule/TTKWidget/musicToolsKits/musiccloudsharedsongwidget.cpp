#include "musiccloudsharedsongwidget.h"
#include "musicwidgetheaders.h"
#include "musicrightareawidget.h"
#include "musiccloudmanagerwidget.h"
#include "musiccloudtoolboxwidget.h"

MusicCloudSharedSongWidget::MusicCloudSharedSongWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_toolBoxWidget = new MusicCloudToolBoxWidget(this);
    m_managerWidget = new MusicCloudManagerWidget(this);
    layout->addWidget(m_toolBoxWidget);
    setLayout(layout);
}

MusicCloudSharedSongWidget::~MusicCloudSharedSongWidget()
{
    delete m_toolBoxWidget;
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
