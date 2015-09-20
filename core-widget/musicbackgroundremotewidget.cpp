#include "musicbackgroundremotewidget.h"
#include "musicuiobject.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QBoxLayout>
#include <QListWidget>

MusicBackgroundRemoteWidget::MusicBackgroundRemoteWidget(QWidget *parent)
    : QWidget(parent)
{
    m_group = new QButtonGroup(this);
    initWidget();
}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{
    clearAllItems();
    delete m_listWidget;
    delete m_group;
}

void MusicBackgroundRemoteWidget::initWidget()
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setContentsMargins(0, 0, 0, 0);

    createButton();
    for(int i=0; i<4; ++i)
    {
        hbox->addWidget( m_group->button(i) );
    }
    vbox->addLayout(hbox);

    m_listWidget = new QListWidget(this);
    m_listWidget->setFrameShape(QFrame::NoFrame);
    m_listWidget->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_listWidget->setIconSize(QSize(100,80));
    m_listWidget->setViewMode(QListView::IconMode);
    m_listWidget->setMovement(QListView::Static);
    vbox->addWidget(m_listWidget);

    setLayout(vbox);
}

void MusicBackgroundRemoteWidget::createButton()
{
    for(int i=0; i<4; ++i)
    {
        QPushButton *button = new QPushButton(this);
        button->setFixedHeight(25);
        button->setText(tr("NetButton%1").arg(i));
        button->setStyleSheet(MusicUIObject::MPushButtonStyle01);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        m_group->addButton(button, i);
    }
    connect(m_group, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
}

void MusicBackgroundRemoteWidget::buttonClicked(int )
{
    clearAllItems();
    for(int i=0; i<100; i++)
    {
        createItem("sdfsdf", QIcon(":/image/noneImage"));
    }
}

void MusicBackgroundRemoteWidget::clearAllItems()
{
    m_listWidget->clear();
}

void MusicBackgroundRemoteWidget::createItem(const QString &name, const QIcon &icon)
{
    QListWidgetItem *item = new QListWidgetItem(m_listWidget);
    item->setIcon(icon);
    item->setToolTip(name);
    m_listWidget->addItem(item);
}
