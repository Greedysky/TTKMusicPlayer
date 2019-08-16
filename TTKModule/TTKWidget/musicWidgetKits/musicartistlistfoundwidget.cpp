#include "musicartistlistfoundwidget.h"
#include "musicdownloadqueryplaylistthread.h"
#include "musicdownloadqueryfactory.h"
#include "musictinyuiobject.h"
#include "musicartistlistfoundcategorypopwidget.h"
#include "musicpagingwidgetobject.h"
#include "musicotherdefine.h"
#include "musicrightareawidget.h"

#include <qmath.h>
#include <QSignalMapper>

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  25
#define LINE_SPACING_SIZE  150

MusicArtistListFoundItemWidget::MusicArtistListFoundItemWidget(QWidget *parent)
    : MusicClickedLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setStyleSheet(MusicUIObject::MColorStyle09);
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    connect(this, SIGNAL(clicked()), SLOT(currentItemClicked()));
}

void MusicArtistListFoundItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    setToolTip(item.m_name);
    setText(MusicUtils::Widget::elidedText(font(), toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
}

void MusicArtistListFoundItemWidget::currentItemClicked()
{
    emit currentItemClicked(m_itemData);
}



MusicArtistListFoundWidget::MusicArtistListFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_firstInit = false;
    m_categoryChanged = false;
    m_gridLayout = nullptr;
    m_categoryButton = nullptr;
    m_pagingWidgetObject = nullptr;
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getArtistListThread(this);
    connect(m_downloadThread, SIGNAL(createArtistListItem(MusicResultsItem)), SLOT(createArtistListItem(MusicResultsItem)));
}

MusicArtistListFoundWidget::~MusicArtistListFoundWidget()
{
    delete m_gridLayout;
    delete m_categoryButton;
    delete m_downloadThread;
    delete m_pagingWidgetObject;
}

void MusicArtistListFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, QString());
}

void MusicArtistListFoundWidget::setSongNameById(const QString &id)
{
    setSongName(id);
}

void MusicArtistListFoundWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i]);
        }

        const int lineNumber = width()/LINE_SPACING_SIZE;
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicArtistListFoundWidget::createArtistListItem(const MusicResultsItem &item)
{
    if(!m_firstInit)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setAlignment(Qt::AlignLeft);
        scrollArea->setWidget(m_mainWindow);
        m_container->addWidget(scrollArea);

        m_firstInit = true;
        QHBoxLayout *mainlayout = MStatic_cast(QHBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QVBoxLayout *containTopLayout  = new QVBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);
        m_categoryButton = new MusicArtistListFoundCategoryPopWidget(m_mainWindow);
        m_categoryButton->setCategory(m_downloadThread->getQueryServer(), this);
        containTopLayout->addWidget(m_categoryButton);
        //
        QWidget *containNumberWidget = new QWidget(containTopWidget);
        QHBoxLayout *containNumberLayout  = new QHBoxLayout(containNumberWidget);
#ifdef Q_OS_WIN
        containNumberLayout->setSpacing(15);
#endif
        QSignalMapper *group = new QSignalMapper(this);
        connect(group, SIGNAL(mapped(int)), SLOT(numberButtonClicked(int)));
        for(int i=-1; i<27; ++i)
        {
            MusicClickedLabel *l = new MusicClickedLabel(QString(MStatic_cast(char, i + 65)), containNumberWidget);
            l->setStyleSheet(QString("QLabel::hover{%1} QLabel{%2}").arg(MusicUIObject::MColorStyle08).arg(MusicUIObject::MColorStyle11));
            connect(l, SIGNAL(clicked()), group, SLOT(map()));
            if(i == -1)
            {
                l->setText(tr("hot"));
            }
            else if(i == 26)
            {
                l->setText(tr("#"));
            }
            group->setMapping(l, i);
            containNumberLayout->addWidget(l);
        }
        containNumberWidget->setLayout(containNumberLayout);
        containTopLayout->addWidget(containNumberWidget);
        //
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(MusicUIObject::MColorStyle06);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(15);
        containWidget->setLayout(m_gridLayout);

        mainlayout->addWidget(containTopWidget);
        mainlayout->addWidget(line);
        mainlayout->addWidget(containWidget);

        m_pagingWidgetObject = new MusicPagingWidgetObject(m_mainWindow);
        connect(m_pagingWidgetObject, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

        const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
        mainlayout->addWidget(m_pagingWidgetObject->createPagingWidget(m_mainWindow, total));
        mainlayout->addStretch(1);
    }

    if(m_categoryChanged && m_pagingWidgetObject)
    {
        m_categoryChanged = false;
        const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
        m_pagingWidgetObject->reset(total);
    }

    MusicArtistListFoundItemWidget *label = new MusicArtistListFoundItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultsItem)), SLOT(currentArtistListClicked(MusicResultsItem)));
    label->setMusicResultsItem(item);

    const int lineNumber = width()/LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count()/lineNumber, m_resizeWidgets.count()%lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicArtistListFoundWidget::currentArtistListClicked(const MusicResultsItem &item)
{
    MusicRightAreaWidget::instance()->musicArtistSearch(item.m_id);
}

void MusicArtistListFoundWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_categoryId = category.m_id;
        m_songNameFull.clear();

        m_categoryButton->setText(category.m_name);
        m_categoryButton->closeMenu();

        numberButtonClicked(-1);
    }
}

void MusicArtistListFoundWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast();
        m_gridLayout->removeWidget(w);
        delete w;
    }

    const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
    m_pagingWidgetObject->paging(index, total);
    m_downloadThread->startToPage(m_pagingWidgetObject->currentIndex() - 1);
}

void MusicArtistListFoundWidget::numberButtonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast();
        m_gridLayout->removeWidget(w);
        delete w;
    }
    m_categoryChanged = true;

    const QString &v = QString("%1%2%3").arg(m_categoryId).arg(TTK_STR_SPLITER).arg(index);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, v);
}
