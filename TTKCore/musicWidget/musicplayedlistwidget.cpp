#include "musicplayedlistwidget.h"
#include "musicfunctionuiobject.h"
#include "musicsongsplayedlistwidget.h"
#include "musicuiobject.h"

#include <QLabel>
#include <QBoxLayout>

#define MAX_SIZE    3

MusicPlayedListWidget::MusicPlayedListWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    setToolTip(tr("playedList"));
    setStyleSheet(MusicUIObject::MKGBtnPlayedList);

    disconnect(this, SIGNAL(clicked()), this, SLOT(popupMenu()));
    connect(this, SIGNAL(clicked()), SLOT(popupMenu()));

    initWidget();
}

MusicPlayedListWidget::~MusicPlayedListWidget()
{
    delete m_musicPlayedListWidget;
    while(!m_labels.isEmpty())
    {
        delete m_labels.takeLast();
    }
}

QString MusicPlayedListWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlayedListWidget::setPlayListCount(int count)
{
    for(int i=MAX_SIZE-1; i>=0; --i)
    {
        m_labels[i]->setPixmap(QPixmap(QString(":/tiny/lb_number%1").arg(count%10)));
        count = count/10;
    }

    if(count > 1000)
    {
        for(int i=MAX_SIZE-1; i>=0; --i)
        {
            m_labels[i]->setPixmap(QPixmap(QString(":/tiny/lb_number%1").arg(9)));
        }
    }
}

void MusicPlayedListWidget::popupMenu()
{
    QPoint pos = mapToGlobal(QPoint(0, 0));
    pos.setY(pos.y() - m_containWidget->height() - 10);
    pos.setX(pos.x() - (m_containWidget->width() - width() - 3));

    m_menu->exec(pos);
}

void MusicPlayedListWidget::initWidget()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    layout->addStretch(2);
    for(int i=0; i<MAX_SIZE; ++i)
    {
        QLabel *label = new QLabel(this);
        label->setFixedWidth(9);
        label->setPixmap(QPixmap(":/tiny/lb_number0"));
        layout->addWidget(label);
        m_labels << label;
    }
    layout->addStretch(1);

    setLayout(layout);

    m_containWidget->setFixedSize(300, 400);
    QHBoxLayout *containLayout = new QHBoxLayout(m_containWidget);
    m_musicPlayedListWidget = new MusicSongsPlayedListWidget(m_containWidget);
    containLayout->addWidget(m_musicPlayedListWidget);
    m_containWidget->setLayout(containLayout);
}
