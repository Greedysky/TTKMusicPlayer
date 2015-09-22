#include "musicbackgroundremotewidget.h"
#include "musicbackgroundlistwidget.h"
#include "musicobject.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QBoxLayout>

MusicBackgroundRemoteWidget::MusicBackgroundRemoteWidget(QWidget *parent)
    : QWidget(parent)
{
    m_currentIndex = -1;
    m_group = new QButtonGroup(this);
    initWidget();
}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{
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
    m_listWidget = new MusicBackgroundListWidget(this);
    connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                          SLOT(itemUserClicked(QListWidgetItem*)));
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

void MusicBackgroundRemoteWidget::buttonClicked(int index)
{
    int count = 0;
    switch(m_currentIndex = index)
    {
        case 0: count = 10; break;
        case 1: count = 20; break;
        case 2: count = 30; break;
        case 3: count = 40; break;
    }
    m_listWidget->clearAllItems();
    for(int i=0; i<count; i++)
    {
        m_listWidget->createItem(QString(), QIcon(":/image/noneImage"));
    }
}

void MusicBackgroundRemoteWidget::itemUserClicked(QListWidgetItem *item)
{
    if(!item->text().isEmpty())
    {
        emit showCustomSkin(QString("%1%2/%3%4").arg(THEME_CACHED)
                            .arg(m_currentIndex).arg(m_listWidget->currentRow())
                            .arg(JPG_FILE));
    }
}
