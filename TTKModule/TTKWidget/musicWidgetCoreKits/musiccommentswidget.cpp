#include "musiccommentswidget.h"
#include "musicdownloadsourcethread.h"
#include "musicemojilabelwidget.h"
#include "musicfunctionuiobject.h"
#include "musicinlinelrcuiobject.h"
#include "musicuiobject.h"
#include "ttkglobal.h"
#include "musicclickedlabel.h"
#include "musicsemaphoreloop.h"
#include "musicpagingwidgetobject.h"
#include "musicwidgetheaders.h"

#include <qmath.h>
#include <QTextEdit>
#include <QDateTime>

MusicCommentsItem::MusicCommentsItem(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(60);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->addSpacing(2);
    //
    QWidget *centerWidget = new QWidget(this);
    QHBoxLayout *centerWidgetLayout = new QHBoxLayout(centerWidget);
    centerWidgetLayout->setContentsMargins(0, 0, 0, 0);
    //
    QWidget *textWidget = new QWidget(centerWidget);
    QVBoxLayout *textWidgetLayout = new QVBoxLayout(textWidget);
    textWidgetLayout->setContentsMargins(0, 5, 0, 6);
    textWidgetLayout->setSpacing(0);
    //
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
    //
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
    //
#ifdef Q_OS_UNIX
    starButton->setFocusPolicy(Qt::NoFocus);
#endif
    //
    operWidgetLayout->addWidget(m_timerLabel);
    operWidgetLayout->addStretch(1);
    operWidgetLayout->addWidget(ngReportLabel);
    operWidgetLayout->addWidget(spliteLine1);
    operWidgetLayout->addWidget(starButton);
    operWidgetLayout->addWidget(m_starLabel);
    operWidgetLayout->addWidget(spliteLine2);
    operWidgetLayout->addWidget(ReplyLabel);
    operWidget->setLayout(operWidgetLayout);
    //
    textWidgetLayout->addWidget(userWidget);
    textWidgetLayout->addWidget(operWidget);
    textWidget->setLayout(textWidgetLayout);
    //
    m_iconLabel = new QLabel(centerWidget);
    m_iconLabel->setFixedSize(40, 40);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_defaultArt").scaled(m_iconLabel->size()));
    centerWidgetLayout->addWidget(m_iconLabel);
    centerWidgetLayout->addWidget(textWidget);
    centerWidget->setLayout(centerWidgetLayout);
    //
    QFrame *solidLine = new QFrame(this);
    solidLine->setFixedHeight(1);
    solidLine->setStyleSheet(MusicUIObject::MBorderStyle06 + MusicUIObject::MColorStyle03);
    //
    layout->addWidget(centerWidget);
    layout->addWidget(solidLine);
    setLayout(layout);
}

MusicCommentsItem::~MusicCommentsItem()
{
    delete m_userName;
    delete m_userCommit;
    delete m_timerLabel;
    delete m_iconLabel;
    delete m_starLabel;
}

void MusicCommentsItem::createSearchedItem(const MusicResultsItem &comments)
{
    m_userName->setText(comments.m_nickName + ":");
    m_userName->setFixedWidth(QFontMetrics(m_userName->font()).width(m_userName->text()));
    m_timerLabel->setText(QDateTime::fromMSecsSinceEpoch(comments.m_updateTime.toULongLong()).toString("yyyy-MM-dd hh:mm:ss"));
    m_timerLabel->setFixedWidth(QFontMetrics(m_timerLabel->font()).width(m_timerLabel->text()));
    m_starLabel->setText(QString("(%1)").arg(comments.m_playCount));
    m_userCommit->setText(comments.m_description);

    MusicDownloadSourceThread *thread = new MusicDownloadSourceThread(this);
    connect(thread, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(iconDataDownloadFinished(QByteArray)));
    thread->startToDownload(comments.m_coverUrl);
}

void MusicCommentsItem::iconDataDownloadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
}



MusicCommentsWidget::MusicCommentsWidget(QWidget *parent)
    : QLabel(parent)
{
    m_isPain = true;
    m_topLabel = nullptr;
    m_commentsLabel = nullptr;
    m_messageEdit = nullptr;
    m_pagingWidgetObject = nullptr;
    m_messageComments = nullptr;
    m_commentsThread = nullptr;
    m_pagingWidgetObject = nullptr;
}

MusicCommentsWidget::~MusicCommentsWidget()
{
    deleteCommentsItems();
    delete m_topLabel;
    delete m_commentsLabel;
    delete m_messageEdit;
    delete m_pagingWidgetObject;
    delete m_messageComments;
    delete m_commentsThread;
}

void MusicCommentsWidget::initWidget(bool isPain)
{
    m_isPain = isPain;

    setObjectName("MainWidnow");
    setStyleSheet(QString("#MainWidnow{%1}").arg(MusicUIObject::MBackgroundStyle19));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *topWidget = nullptr;
    if(isPain)
    {
        topWidget = new QWidget(this);
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
    }
    else
    {
        m_topLabel = new QLabel(this);
        m_topLabel->hide();
    }

    QWidget *contentsWidget = new QWidget(this);
    QVBoxLayout *contentsLayout = new QVBoxLayout(contentsWidget);
    contentsLayout->setContentsMargins(0, 0, 0 ,0);
    contentsLayout->setSpacing(0);
    //
    QWidget *messageBox = new QWidget(contentsWidget);
    messageBox->setFixedHeight(150);
    QVBoxLayout *messageBoxLayout = new QVBoxLayout(messageBox);
    messageBoxLayout->setContentsMargins(10, 10, 10, 3);
    messageBox->setAttribute(Qt::WA_TranslucentBackground, false);
    m_messageEdit = new QTextEdit(messageBox);
    m_messageEdit->setFixedHeight(75);
    m_messageEdit->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_messageEdit->setStyleSheet(MusicUIObject::MBorderStyle02 + MusicUIObject::MBackgroundStyle01 + MusicUIObject::MColorStyle04);
    m_messageEdit->viewport()->setStyleSheet(MusicUIObject::MBackgroundStyle01 + MusicUIObject::MColorStyle04);
    messageBox->setAttribute(Qt::WA_TranslucentBackground, true);
    m_commentsLabel = new QLabel(contentsWidget);
    m_commentsLabel->setStyleSheet(MusicUIObject::MColorStyle04);
    QFrame *solidLine = new QFrame(contentsWidget);
    solidLine->setFrameShape(QFrame::HLine);
    solidLine->setFixedHeight(1);
    solidLine->setStyleSheet(MusicUIObject::MColorStyle03);
    //
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
#ifdef Q_OS_UNIX
    emojiButton->setFocusPolicy(Qt::NoFocus);
    sendButton->setFocusPolicy(Qt::NoFocus);
#endif
    //
    messageBoxLayout->addWidget(m_messageEdit);
    messageBoxLayout->addWidget(messageMiddle);
    messageBoxLayout->addWidget(m_commentsLabel);
    messageBoxLayout->addWidget(solidLine);
    messageBox->setLayout(messageBoxLayout);
    //
    m_messageComments = new QWidget(contentsWidget);
    QVBoxLayout *messageCommentsLayout = new QVBoxLayout(m_messageComments);
    messageCommentsLayout->setContentsMargins(0, 0, 0, 0);
    m_messageComments->setLayout(messageCommentsLayout);

    contentsLayout->addWidget(messageBox);
    contentsLayout->addWidget(m_messageComments);
    contentsWidget->setLayout(contentsLayout);
    //
    if(isPain)
    {
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setAlignment(Qt::AlignLeft);
        scrollArea->setWidget(contentsWidget);

        contentsWidget->setObjectName("contentsWidget");
        contentsWidget->setStyleSheet(QString("#contentsWidget{%1}").arg(MusicUIObject::MBackgroundStyle01));
        QWidget *view = scrollArea->viewport();
        view->setObjectName("viewport");
        view->setStyleSheet(QString("#viewport{%1}").arg(MusicUIObject::MBackgroundStyle01));
        //
        mainLayout->addWidget(topWidget);
        mainLayout->addWidget(scrollArea);
    }
    else
    {
        mainLayout->addWidget(contentsWidget);
        setFixedHeight(300);
    }
    setLayout(mainLayout);

    initLabel(QString(), 0);
}

void MusicCommentsWidget::setCurrentSongName(const QString &name)
{
    deleteCommentsItems();

    MusicSemaphoreLoop loop;
    connect(m_commentsThread, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    m_commentsThread->startToSearch(name);
    loop.exec();

    MStatic_cast(QVBoxLayout*, m_messageComments->layout())->addStretch(1);
    createPagingWidget();

    initLabel(name, m_commentsThread->getPageTotal());
}

void MusicCommentsWidget::createSearchedItem(const MusicResultsItem &comments)
{
    MusicCommentsItem *item = new MusicCommentsItem(m_messageComments);
    item->createSearchedItem(comments);
    m_commentsItems << item;

    QVBoxLayout *layout = MStatic_cast(QVBoxLayout*, m_messageComments->layout());
    layout->insertWidget(layout->count() - 1, item);

    if(!m_isPain)
    {
        setFixedHeight(height() + item->height());
    }
}

void MusicCommentsWidget::buttonClicked(int index)
{
    deleteCommentsItems();

    const int total = ceil(m_commentsThread->getPageTotal()*1.0/m_commentsThread->getPageSize());
    m_pagingWidgetObject->paging(index, total);
    m_commentsThread->startToPage(m_pagingWidgetObject->currentIndex() - 1);
}

void MusicCommentsWidget::createEMOJILabelWidget()
{
    MusicEMOJILabelWidget *w = new MusicEMOJILabelWidget(this);
    connect(w, SIGNAL(dataChanged(QString)), SLOT(currentEMOJIchanged(QString)));
    w->move(15, 160);
    w->show();
}

void MusicCommentsWidget::currentEMOJIchanged(const QString &data)
{
    m_messageEdit->insertPlainText(data);
}

void MusicCommentsWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicCommentsWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicCommentsWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicCommentsWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicCommentsWidget::initLabel(const QString &name, int total)
{
    m_topLabel->setText(tr("<span style='font-size:15pt;color:#AAAAAA;font-style:bold;'>\"%1\" Commit</span>"
                           "<span style='font-size:11pt;color:#666666;font-style:bold;'> (Just %2 Commits)</span>").arg(name).arg(total));

    m_commentsLabel->setText(tr("All Comments(%1)").arg(total));
}

void MusicCommentsWidget::deleteCommentsItems()
{
    qDeleteAll(m_commentsItems);
    m_commentsItems.clear();

    if(!m_isPain)
    {
        setFixedHeight(300);
    }
}

void MusicCommentsWidget::createPagingWidget()
{
    m_pagingWidgetObject = new MusicPagingWidgetObject(this);
    connect(m_pagingWidgetObject, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

    const int total = ceil(m_commentsThread->getPageTotal()*1.0/m_commentsThread->getPageSize());
    QWidget *w = m_pagingWidgetObject->createPagingWidget(m_messageComments, total);
    m_messageComments->layout()->addWidget(w);
}
