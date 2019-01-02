#include "musicpagingwidgetobject.h"
#include "musicclickedlabel.h"
#include "musicuiobject.h"
#include "musicwidgetheaders.h"

#include <QSignalMapper>
#include <QFontMetrics>

#define PAGE_SIZE   10

MusicPagingWidgetObject::MusicPagingWidgetObject(QObject *parent)
    : QObject(parent)
{
    m_currentPage = 0;
    m_pagingWidget = nullptr;
}

MusicPagingWidgetObject::~MusicPagingWidgetObject()
{
    qDeleteAll(m_pagingItems);
    delete m_pagingWidget;
}

QWidget* MusicPagingWidgetObject::getCreatePagingWidget()
{
    return m_pagingWidget;
}

QWidget* MusicPagingWidgetObject::createPagingWidget(QWidget *parent, int total)
{
    m_pagingWidget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(m_pagingWidget);
    layout->setContentsMargins(0, 20, 0, 20);
    layout->setSpacing(12);

    for(int i=1; i<=PAGE_SIZE; ++i)
    {
        m_pagingItems << (new MusicClickedLabel(QString::number(i), m_pagingWidget));
    }
    m_pagingItems << (new MusicClickedLabel(tr("pre"), m_pagingWidget)) << (new MusicClickedLabel(tr("next"), m_pagingWidget));

    QSignalMapper *group = new QSignalMapper(m_pagingWidget);
    connect(group, SIGNAL(mapped(int)), SIGNAL(mapped(int)));

    int i=0;
    foreach(MusicClickedLabel *w, m_pagingItems)
    {
        QFont font(w->font());
        font.setPixelSize(17);
        w->setFont(font);
        w->setStyleSheet(MusicUIObject::MColorStyle04);
        w->setFixedWidth(QFontMetrics(font).width(w->text()));
        connect(w, SIGNAL(clicked()), group, SLOT(map()));
        group->setMapping(w, i++);
    }

    m_pagingItems[PAGE_SIZE]->hide();
    if(total <= PAGE_SIZE)
    {
        m_pagingItems[PAGE_SIZE + 1]->hide();
        for(int i=PAGE_SIZE - 1; i>=total; --i)
        {
            m_pagingItems[i]->hide();
        }
    }
    m_pagingItems[0]->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle01);

    layout->addStretch(1);
    if(total != 0)
    {
        layout->addWidget(m_pagingItems[PAGE_SIZE]);
        for(i=0; i<PAGE_SIZE; ++i)
        {
            layout->addWidget(m_pagingItems[i]);
        }
        layout->addWidget(m_pagingItems[PAGE_SIZE + 1]);
    }
    else
    {
        QWidget *func = new QWidget(m_pagingWidget);
        QVBoxLayout *funcLayout = new QVBoxLayout(func);
        QLabel *icon = new QLabel(func);
        icon->setPixmap(QPixmap(":/lrc/lb_no_results"));
        QLabel *text = new QLabel(tr("There Is Empty!"), func);
        text->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle05);
        text->setAlignment(Qt::AlignCenter);
        funcLayout->addWidget(icon, 0, Qt::AlignCenter);
        funcLayout->addWidget(text, 0, Qt::AlignCenter);
        func->setLayout(funcLayout);
        layout->addWidget(func);
    }
    layout->addStretch(1);

    m_pagingWidget->setLayout(layout);
    return m_pagingWidget;
}

void MusicPagingWidgetObject::reset(int total)
{
    if(m_pagingItems.isEmpty())
    {
        return;
    }

    foreach(MusicClickedLabel *label, m_pagingItems)
    {
        label->hide();
    }
    m_currentPage = 0;

    if(total > PAGE_SIZE)
    {
        total = PAGE_SIZE;
        m_pagingItems[PAGE_SIZE + 1]->show();
    }

    for(int i=0; i<total; ++i)
    {
        m_pagingItems[i]->setText(QString::number(i + 1));
        m_pagingItems[i]->show();
    }
}

void MusicPagingWidgetObject::paging(int index, int total)
{
    if(total <= 0)
    {
        return;
    }

    int page = m_pagingItems[0]->text().toInt();
    for(int i=0; i<m_pagingItems.count() - 2; ++i)
    {
        m_pagingItems[i]->setStyleSheet(MusicUIObject::MColorStyle04);
    }

    switch(index)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            m_currentPage = index;
            break;
        case 10:
            {
                page -= PAGE_SIZE;
                MusicClickedLabel *w = m_pagingItems[PAGE_SIZE];

                if(total <= PAGE_SIZE)
                {
                    w->hide();
                }
                else
                {
                    for(int i=0; i<PAGE_SIZE; ++i)
                    {
                        m_pagingItems[i]->setText(QString::number(page + i));
                        m_pagingItems[i]->show();
                    }
                    (m_pagingItems[0]->text().toInt() < PAGE_SIZE) ? w->hide() : w->show();
                }

                m_currentPage = 0;
                m_pagingItems[PAGE_SIZE + 1]->show();
                break;
            }
        case 11:
            {
                page += PAGE_SIZE;
                MusicClickedLabel *w = m_pagingItems[PAGE_SIZE + 1];
                int boundary = total - page + 1;
                    boundary = boundary < PAGE_SIZE ? boundary : PAGE_SIZE;

                for(int i=0; i<boundary; ++i)
                {
                    m_pagingItems[i]->setText(QString::number(page + i));
                }

                if(total - page >= PAGE_SIZE)
                {
                    w->show();
                }
                else
                {
                    w->hide();
                    for(int i=PAGE_SIZE - 1; i>(total - page); --i)
                    {
                        m_pagingItems[i]->hide();
                    }
                }

                m_currentPage = 0;
                m_pagingItems[PAGE_SIZE]->show();
                break;
            }
        default:
            break;
    }

    for(int i=0; i<m_pagingItems.count() - 2; ++i)
    {
        MusicClickedLabel *w = m_pagingItems[i];
        w->setFixedWidth(QFontMetrics(w->font()).width(w->text()));
    }

    MusicClickedLabel *w = m_pagingItems[m_currentPage];
    w->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle01);
    w->setFixedWidth(w->width() + 5);
}

int MusicPagingWidgetObject::currentIndex() const
{
    return m_pagingItems[m_currentPage]->text().toInt();
}
