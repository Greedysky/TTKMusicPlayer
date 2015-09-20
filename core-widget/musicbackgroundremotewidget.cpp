#include "musicbackgroundremotewidget.h"
#include "musicbackgroundlistwidget.h"
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
    m_listWidget->clearAllItems();
    for(int i=0; i<100; i++)
    {
        m_listWidget->createItem("sdfsdf", QIcon(":/image/noneImage"));
    }
}
