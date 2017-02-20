#include "musiclrccommentswidget.h"
#include "musicsourcedownloadthread.h"
#include "musicemojilabelwidget.h"
#include "musicfunctionuiobject.h"
#include "musicinlinelrcuiobject.h"
#include "musicclickedlabel.h"
#include "musicuiobject.h"
#include "musicglobal.h"
#include "musicsemaphoreloop.h"

#include <qmath.h>
#include <QBoxLayout>
#include <QSignalMapper>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTextEdit>
#include <QFontMetrics>
#include <QDateTime>

MusicLrcCommentsItem::MusicLrcCommentsItem(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(60);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->addSpacing(2);
    //////////////////////////////////////////////////////////////////////
    QWidget *centerWidget = new QWidget(this);
    QHBoxLayout *centerWidgetLayout = new QHBoxLayout(centerWidget);
    centerWidgetLayout->setContentsMargins(0, 0, 0, 0);
    //////////////////////////////////////////////////////////////////////
    QWidget *textWidget = new QWidget(centerWidget);
    QVBoxLayout *textWidgetLayout = new QVBoxLayout(textWidget);
    textWidgetLayout->setContentsMargins(0, 5, 0, 6);
    textWidgetLayout->setSpacing(0);
    //////////////////////////////////////////////////////////////////////
    QWidget *userWidget = new QWidget(textWidget);
    QHBoxLayout *userWidgetLayout = new QHBoxLayout(userWidget);
    userWidgetLayout->setContentsMargins(0, 0, 0, 0);
    userWidgetLayout->setSpacing(2);
    m_userName = new QLabel(userWidget);
    m_userCommit = new QTextEdit(userWidget);
    m_userCommit->setReadOnly(true);
    m_userCommit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_userCommit->setStyleSheet(MusicUIObject::MBorderStyle01 + MusicUIObject::MBackgroundStyle01 +
                                MusicUIObject::MColorStyle04);
    m_userCommit->viewport()->setStyleSheet(MusicUIObject::MBackgroundStyle01 + MusicUIObject::MColorStyle04);
    m_userName->setStyleSheet(MusicUIObject::MColorStyle03);
    userWidgetLayout->addWidget(m_userName);
    userWidgetLayout->addWidget(m_userCommit);
    userWidget->setLayout(userWidgetLayout);
    //////////////////////////////////////////////////////////////////////
    QWidget *operWidget = new QWidget(textWidget);
    QHBoxLayout *operWidgetLayout = new QHBoxLayout(operWidget);
    operWidgetLayout->setContentsMargins(0, 0, 0, 0);
    operWidgetLayout->setSpacing(4);
    m_timerLabel = new QLabel(operWidget);
    m_timerLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    MusicClickedLabel *ngReportLabel = new MusicClickedLabel(tr("Report"), operWidget);
    ngReportLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    QFrame *spliteLine1 = new QFrame(operWidget);
    spliteLine1->setFixedWidth(1);
    spliteLine1->setFrameShape(QFrame::VLine);
    spliteLine1->setStyleSheet(MusicUIObject::MColorStyle03);
    QPushButton *starButton = new QPushButton(operWidget);
    starButton->setCursor(QCursor(Qt::PointingHandCursor));
    starButton->setFixedSize(12, 11);
    starButton->setStyleSheet(MusicUIObject::MKGInlineStar);
    m_starLabel = new QLabel(operWidget);
    m_starLabel->setText("(0)");
    m_starLabel->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MBackgroundStyle01);
    QFrame *spliteLine2 = new QFrame(operWidget);
    spliteLine2->setFixedWidth(1);
    spliteLine2->setFrameShape(QFrame::VLine);
    spliteLine2->setStyleSheet(MusicUIObject::MColorStyle03);
    MusicClickedLabel *ReplyLabel = new MusicClickedLabel(tr("Reply"), operWidget);
    ReplyLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    //////////////////////////////////////////////////////////////////////
    operWidgetLayout->addWidget(m_timerLabel);
    operWidgetLayout->addStretch(1);
    operWidgetLayout->addWidget(ngReportLabel);
    operWidgetLayout->addWidget(spliteLine1);
    operWidgetLayout->addWidget(starButton);
    operWidgetLayout->addWidget(m_starLabel);
    operWidgetLayout->addWidget(spliteLine2);
    operWidgetLayout->addWidget(ReplyLabel);
    operWidget->setLayout(operWidgetLayout);
    //////////////////////////////////////////////////////////////////////
    textWidgetLayout->addWidget(userWidget);
    textWidgetLayout->addWidget(operWidget);
    textWidget->setLayout(textWidgetLayout);
    //////////////////////////////////////////////////////////////////////
    m_iconLabel = new QLabel(centerWidget);
    m_iconLabel->setFixedSize(40, 40);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(m_iconLabel->size()));
    centerWidgetLayout->addWidget(m_iconLabel);
    centerWidgetLayout->addWidget(textWidget);
    centerWidget->setLayout(centerWidgetLayout);
    //////////////////////////////////////////////////////////////////////
    QFrame *solidLine = new QFrame(this);
    solidLine->setFixedHeight(1);
    solidLine->setStyleSheet(MusicUIObject::MBorderStyle06 + MusicUIObject::MColorStyle03);
    //////////////////////////////////////////////////////////////////////
    layout->addWidget(solidLine);
    layout->addWidget(centerWidget);
    setLayout(layout);
}

MusicLrcCommentsItem::~MusicLrcCommentsItem()
{
    delete m_userName;
    delete m_userCommit;
    delete m_timerLabel;
    delete m_iconLabel;
    delete m_starLabel;
}

QString MusicLrcCommentsItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcCommentsItem::createSearchedItems(const MusicSongComment &comments)
{
    m_userName->setText(comments.m_nickName + ":");
    m_userName->setFixedWidth(QFontMetrics(m_userName->font()).width(m_userName->text()));
    m_timerLabel->setText(QDateTime::fromMSecsSinceEpoch(comments.m_time.toULongLong()).toString("yyyy-MM-dd hh:mm:ss"));
    m_timerLabel->setFixedWidth(QFontMetrics(m_timerLabel->font()).width(m_timerLabel->text()));
    m_starLabel->setText(QString("(%1)").arg(comments.m_likedCount));

    m_userCommit->setText(comments.m_content);
    int w = 60 + m_iconLabel->width() + m_userName->width();
    w = MStatic_cast(QWidget*, parent())->width() - w;
    int acWidth = QFontMetrics(m_userCommit->font()).width(comments.m_content);
    setFixedHeight(height() + QFontMetrics(m_userCommit->font()).height()*(acWidth/w));

    MusicSourceDownloadThread *thread = new MusicSourceDownloadThread(this);
    connect(thread, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(iconDataDownloadFinished(QByteArray)));
    thread->startToDownload(comments.m_avatarUrl);
}

void MusicLrcCommentsItem::iconDataDownloadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
}



MusicLrcCommentsWidget::MusicLrcCommentsWidget(QWidget *parent)
    : QLabel(parent)
{
    setObjectName("MainWidnow");
    setStyleSheet(QString("#MainWidnow{%1}").arg(MusicUIObject::MBackgroundStyle19));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(40);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(0, 0, 10, 0);

    m_topLabel = new QLabel(topWidget);
    m_topLabel->setAlignment(Qt::AlignCenter);
    topWidgetLayout->addWidget(m_topLabel);
    QPushButton *closeButton = new QPushButton(this);
    closeButton->setFixedSize(14, 14);
    closeButton->setStyleSheet(MusicUIObject::MKGBtnPClose);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(closeButton, SIGNAL(clicked()), SLOT(close()));

    topWidgetLayout->addWidget(closeButton);
    topWidget->setLayout(topWidgetLayout);

    QWidget *contentsWidget = new QWidget(this);
    QVBoxLayout *contentsLayout = new QVBoxLayout(contentsWidget);
    contentsLayout->setContentsMargins(0, 0, 0 ,0);
    contentsLayout->setSpacing(0);
    //////////////////////////////////////////////////////////////////////
    QWidget *messageBox = new QWidget(contentsWidget);
    messageBox->setFixedHeight(150);
    QVBoxLayout *messageBoxLayout = new QVBoxLayout(messageBox);
    messageBoxLayout->setContentsMargins(10, 10, 10, 3);
    messageBox->setAttribute(Qt::WA_TranslucentBackground, false);
    m_messageEdit = new QTextEdit(messageBox);
    m_messageEdit->setFixedHeight(75);
    m_messageEdit->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_messageEdit->setStyleSheet(MusicUIObject::MBorderStyle02 + MusicUIObject::MBackgroundStyle01 +
                                 MusicUIObject::MColorStyle04);
    m_messageEdit->viewport()->setStyleSheet(MusicUIObject::MBackgroundStyle01 + MusicUIObject::MColorStyle04);
    messageBox->setAttribute(Qt::WA_TranslucentBackground, true);
    m_commentsLabel = new QLabel(contentsWidget);
    m_commentsLabel->setStyleSheet(MusicUIObject::MColorStyle04);
    QFrame *solidLine = new QFrame(contentsWidget);
    solidLine->setFrameShape(QFrame::HLine);
    solidLine->setFixedHeight(1);
    solidLine->setStyleSheet(MusicUIObject::MColorStyle03);
    //////////////////////////////////////////////////////////////////////
    QWidget *messageMiddle = new QWidget(messageBox);
    QHBoxLayout *messageMiddleLayout = new QHBoxLayout(messageMiddle);
    messageMiddleLayout->setContentsMargins(0, 10, 0, 0);
    QPushButton *emojiButton = new QPushButton(messageMiddle);
    QPushButton *sendButton = new QPushButton(tr("Send"), messageMiddle);
    emojiButton->setIcon(QIcon(":/lrc/lb_emoji"));
    emojiButton->setCursor(QCursor(Qt::PointingHandCursor));
    emojiButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    connect(emojiButton, SIGNAL(clicked()), SLOT(createEMOJILabelWidget()));
    sendButton->setFixedSize(65, 25);
    sendButton->setCursor(QCursor(Qt::PointingHandCursor));
    sendButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    messageMiddleLayout->addWidget(emojiButton);
    messageMiddleLayout->addStretch(1);
    messageMiddleLayout->addWidget(sendButton);
    messageMiddle->setLayout(messageMiddleLayout);
    //////////////////////////////////////////////////////////////////////
    messageBoxLayout->addWidget(m_messageEdit);
    messageBoxLayout->addWidget(messageMiddle);
    messageBoxLayout->addWidget(m_commentsLabel);
    messageBoxLayout->addWidget(solidLine);
    messageBox->setLayout(messageBoxLayout);
    //////////////////////////////////////////////////////////////////////
    m_messageComments = new QWidget(contentsWidget);
    QVBoxLayout *messageCommentsLayout = new QVBoxLayout(m_messageComments);
    messageCommentsLayout->setContentsMargins(0, 0, 0, 0);
    m_messageComments->setLayout(messageCommentsLayout);

    contentsLayout->addWidget(messageBox);
    contentsLayout->addWidget(m_messageComments);
    contentsWidget->setLayout(contentsLayout);
    //////////////////////////////////////////////////////////////////////
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(contentsWidget);

    contentsWidget->setObjectName("contentsWidget");
    contentsWidget->setStyleSheet(QString("#contentsWidget{%1}").arg(MusicUIObject::MBackgroundStyle01));
    QWidget *view = scrollArea->viewport();
    view->setObjectName("viewport");
    view->setStyleSheet(QString("#viewport{%1}").arg(MusicUIObject::MBackgroundStyle01));
    //////////////////////////////////////////////////////////////////////
    mainLayout->addWidget(topWidget);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);

    initLabel(QString(), 0);

    m_currentPage = 0;
    m_pagingWidget = nullptr;

    m_commentsThread = new MusicWYCommentsThread(this);
    connect(m_commentsThread, SIGNAL(createSearchedItems(MusicSongComment)), SLOT(createSearchedItems(MusicSongComment)));
}

MusicLrcCommentsWidget::~MusicLrcCommentsWidget()
{
    deletePagingItems();
    deleteCommentsItems();
    delete m_topLabel;
    delete m_commentsLabel;
    delete m_messageEdit;
    delete m_pagingWidget;
    delete m_messageComments;
    delete m_commentsThread;
}

QString MusicLrcCommentsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcCommentsWidget::setCurrentSongName(const QString &name)
{
    deleteCommentsItems();

    MusicSemaphoreLoop loop;
    m_commentsThread->startSearchSong(name);
    m_commentsThread->startSearchSong(0);
    connect(m_commentsThread, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    MStatic_cast(QVBoxLayout*, m_messageComments->layout())->addStretch(1);
    createPagingWidget();

    initLabel(name, m_commentsThread->total());
}

void MusicLrcCommentsWidget::createSearchedItems(const MusicSongComment &comments)
{
    MusicLrcCommentsItem *item = new MusicLrcCommentsItem(this);
    item->createSearchedItems(comments);
    m_commentsItems << item;
    m_messageComments->layout()->addWidget(item);
}

void MusicLrcCommentsWidget::buttonClicked(int index)
{
    deleteCommentsItems();
    int total = ceil(m_commentsThread->total()*1.0/COMMIT_PAGE_SIZE);
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
            m_currentPage = index;
            break;
        case 5:
            {
                page -= 5;
                MusicClickedLabel *w = m_pagingItems[5];

                if(total <= 5)
                {
                    w->hide();
                }
                else
                {
                    for(int i=0; i<5; ++i)
                    {
                        m_pagingItems[i]->setText(QString::number(page + i));
                        m_pagingItems[i]->show();
                    }
                    (m_pagingItems[0]->text().toInt() < 5) ? w->hide() : w->show();
                }

                m_currentPage = 0;
                m_pagingItems[6]->show();
                break;
            }
        case 6:
            {
                page += 5;
                MusicClickedLabel *w = m_pagingItems[6];
                int boundary = total - page + 1;
                boundary = boundary < 5 ? boundary : 5;

                for(int i=0; i<boundary; ++i)
                {
                    m_pagingItems[i]->setText(QString::number(page + i));
                }

                if(total - page >= 5)
                {
                    w->show();
                }
                else
                {
                    w->hide();
                    for(int i=4; i>(total - page); --i)
                    {
                        m_pagingItems[i]->hide();
                    }
                }

                m_currentPage = 0;
                m_pagingItems[5]->show();
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
    m_pagingItems[m_currentPage]->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle01);
    m_commentsThread->startSearchSong(m_pagingItems[m_currentPage]->text().toInt() - 1);
}

void MusicLrcCommentsWidget::createEMOJILabelWidget()
{
    MusicEMOJILabelWidget *w = new MusicEMOJILabelWidget(this);
    connect(w, SIGNAL(dataChanged(QString)), SLOT(currentEMOJIchanged(QString)));
    w->move(15, 160);
    w->show();
}

void MusicLrcCommentsWidget::currentEMOJIchanged(const QString &data)
{
    m_messageEdit->insertPlainText(data);
}

void MusicLrcCommentsWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::initLabel(const QString &name, int total)
{
    m_topLabel->setText(tr("<span style='font-size:15pt;color:#AAAAAA;font-style:bold;'>\"%1\" Commit</span>"
                           "<span style='font-size:11pt;color:#666666;font-style:bold;'> (Just %2 Commits)</span>").arg(name).arg(total));

    m_commentsLabel->setText(tr("All Comments(%1)").arg(total));
}

void MusicLrcCommentsWidget::deleteCommentsItems()
{
    while(!m_commentsItems.isEmpty())
    {
        delete m_commentsItems.takeLast();
    }
}

void MusicLrcCommentsWidget::deletePagingItems()
{
    while(!m_pagingItems.isEmpty())
    {
        delete m_pagingItems.takeLast();
    }
}

void MusicLrcCommentsWidget::createPagingWidget()
{
    m_pagingWidget = new QWidget(m_messageComments);
    QHBoxLayout *layout = new QHBoxLayout(m_pagingWidget);
    layout->setContentsMargins(0, 20, 0, 20);
    layout->setSpacing(12);


    MusicClickedLabel *page1Button = new MusicClickedLabel("1", m_pagingWidget);
    MusicClickedLabel *page2Button = new MusicClickedLabel("2", m_pagingWidget);
    MusicClickedLabel *page3Button = new MusicClickedLabel("3", m_pagingWidget);
    MusicClickedLabel *page4Button = new MusicClickedLabel("4", m_pagingWidget);
    MusicClickedLabel *page5Button = new MusicClickedLabel("5", m_pagingWidget);
    MusicClickedLabel *previousButton = new MusicClickedLabel(tr("pre"), m_pagingWidget);
    MusicClickedLabel *nextButton = new MusicClickedLabel(tr("next"), m_pagingWidget);

    m_pagingItems << page1Button << page2Button << page3Button << page4Button
                  << page5Button << previousButton << nextButton;
    QSignalMapper *group = new QSignalMapper(m_pagingWidget);
    connect(group, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

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

    m_pagingItems[5]->hide();
    int count = ceil(m_commentsThread->total()*1.0/COMMIT_PAGE_SIZE);
    if(count <= 5)
    {
        m_pagingItems[6]->hide();
        for(int i=4; i>=count; --i)
        {
            m_pagingItems[i]->hide();
        }
    }
    page1Button->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle01);

    layout->addStretch(1);
    if(count != 0)
    {
        layout->addWidget(previousButton);
        layout->addWidget(page1Button);
        layout->addWidget(page2Button);
        layout->addWidget(page3Button);
        layout->addWidget(page4Button);
        layout->addWidget(page5Button);
        layout->addWidget(nextButton);
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
    MStatic_cast(QVBoxLayout*, m_messageComments->layout())->insertWidget(0, m_pagingWidget);
}
