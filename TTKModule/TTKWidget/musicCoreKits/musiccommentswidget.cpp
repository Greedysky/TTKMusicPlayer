#include "musiccommentswidget.h"
#include "musicdownloadsourcerequest.h"
#include "musicemojilabelwidget.h"
#include "musicfunctionuiobject.h"
#include "musicinteriorlrcuiobject.h"
#include "musicclickedlabel.h"
#include "musicpagequerywidget.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"

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
    m_userCommit->setStyleSheet(MusicUIObject::MQSSBorderStyle01 + MusicUIObject::MQSSBackgroundStyle01 +
                                MusicUIObject::MQSSColorStyle04);
    m_userCommit->viewport()->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01 + MusicUIObject::MQSSColorStyle04);
    m_userName->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    userWidgetLayout->addWidget(m_userName);
    userWidgetLayout->addWidget(m_userCommit);
    userWidget->setLayout(userWidgetLayout);
    //
    QWidget *operWidget = new QWidget(textWidget);
    QHBoxLayout *operWidgetLayout = new QHBoxLayout(operWidget);
    operWidgetLayout->setContentsMargins(0, 0, 0, 0);
    operWidgetLayout->setSpacing(4);
    m_timerLabel = new QLabel(operWidget);
    m_timerLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    MusicClickedLabel *ngReportLabel = new MusicClickedLabel(tr("Report"), operWidget);
    ngReportLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    QFrame *spliteLine1 = new QFrame(operWidget);
    spliteLine1->setFixedWidth(1);
    spliteLine1->setFrameShape(QFrame::VLine);
    spliteLine1->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    QPushButton *starButton = new QPushButton(operWidget);
    starButton->setCursor(QCursor(Qt::PointingHandCursor));
    starButton->setFixedSize(12, 11);
    starButton->setStyleSheet(MusicUIObject::MQSSInteriorStar);
    m_starLabel = new QLabel(operWidget);
    m_starLabel->setText("(0)");
    m_starLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03 + MusicUIObject::MQSSBackgroundStyle01);
    QFrame *spliteLine2 = new QFrame(operWidget);
    spliteLine2->setFixedWidth(1);
    spliteLine2->setFrameShape(QFrame::VLine);
    spliteLine2->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    MusicClickedLabel *ReplyLabel = new MusicClickedLabel(tr("Reply"), operWidget);
    ReplyLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03);
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
    m_iconLabel->setPixmap(QPixmap(":/image/lb_default_art").scaled(m_iconLabel->size()));
    centerWidgetLayout->addWidget(m_iconLabel);
    centerWidgetLayout->addWidget(textWidget);
    centerWidget->setLayout(centerWidgetLayout);
    //
    QFrame *solidLine = new QFrame(this);
    solidLine->setFixedHeight(1);
    solidLine->setStyleSheet(MusicUIObject::MQSSBorderStyle06 + MusicUIObject::MQSSColorStyle03);
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
    m_userName->setFixedWidth(MusicUtils::Widget::fontTextWidth(m_userName->font(), m_userName->text()));
    m_timerLabel->setText(QDateTime::fromMSecsSinceEpoch(comments.m_updateTime.toULongLong()).toString(MUSIC_YEAR_STIME_FORMAT));
    m_timerLabel->setFixedWidth(MusicUtils::Widget::fontTextWidth(m_timerLabel->font(), m_timerLabel->text()));
    m_starLabel->setText(QString("(%1)").arg(comments.m_playCount));
    m_userCommit->setText(comments.m_description);

    MusicDownloadSourceRequest *request = new MusicDownloadSourceRequest(this);
    connect(request, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    request->startToDownload(comments.m_coverUrl);
}

void MusicCommentsItem::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_LOGGER_ERROR("Input byte data is empty");
        return;
    }

    QPixmap pix;
    pix.loadFromData(bytes);
    m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
}



MusicCommentsWidget::MusicCommentsWidget(QWidget *parent)
    : QLabel(parent)
{
    m_isPain = true;
    m_topLabel = nullptr;
    m_commentsLabel = nullptr;
    m_messageEdit = nullptr;
    m_pageQueryWidget = nullptr;
    m_messageComments = nullptr;
    m_networkRequest = nullptr;
}

MusicCommentsWidget::~MusicCommentsWidget()
{
    deleteCommentsItems();
    delete m_topLabel;
    delete m_commentsLabel;
    delete m_messageEdit;
    delete m_pageQueryWidget;
    delete m_messageComments;
    delete m_networkRequest;
}

void MusicCommentsWidget::initialize(bool isPain)
{
    m_isPain = isPain;

    setObjectName("MainWidnow");
    setStyleSheet(QString("#MainWidnow{%1}").arg(MusicUIObject::MQSSBackgroundStyle19));

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
        closeButton->setStyleSheet(MusicUIObject::MQSSBtnPClose);
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
    m_messageEdit->verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
    m_messageEdit->setStyleSheet(MusicUIObject::MQSSBorderStyle02 + MusicUIObject::MQSSBackgroundStyle01 + MusicUIObject::MQSSColorStyle04);
    m_messageEdit->viewport()->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01 + MusicUIObject::MQSSColorStyle04);
    messageBox->setAttribute(Qt::WA_TranslucentBackground, true);
    m_commentsLabel = new QLabel(contentsWidget);
    m_commentsLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04);
    QFrame *solidLine = new QFrame(contentsWidget);
    solidLine->setFrameShape(QFrame::HLine);
    solidLine->setFixedHeight(1);
    solidLine->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    //
    QWidget *messageMiddle = new QWidget(messageBox);
    QHBoxLayout *messageMiddleLayout = new QHBoxLayout(messageMiddle);
    messageMiddleLayout->setContentsMargins(0, 10, 0, 0);
    QPushButton *emojiButton = new QPushButton(messageMiddle);
    QPushButton *sendButton = new QPushButton(tr("Send"), messageMiddle);
    emojiButton->setIcon(QIcon(":/lrc/lb_emoji"));
    emojiButton->setCursor(QCursor(Qt::PointingHandCursor));
    emojiButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    connect(emojiButton, SIGNAL(clicked()), SLOT(createEMOJILabelWidget()));
    sendButton->setFixedSize(65, 25);
    sendButton->setCursor(QCursor(Qt::PointingHandCursor));
    sendButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
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
        MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, contentsWidget, false);

        contentsWidget->setObjectName("contentsWidget");
        contentsWidget->setStyleSheet(QString("#contentsWidget{%1}").arg(MusicUIObject::MQSSBackgroundStyle01));
        QWidget *view = scrollArea->viewport();
        view->setObjectName("viewport");
        view->setStyleSheet(QString("#viewport{%1}").arg(MusicUIObject::MQSSBackgroundStyle01));
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
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    m_networkRequest->startToSearch(name);
    loop.exec();

    TTKStatic_cast(QVBoxLayout*, m_messageComments->layout())->addStretch(1);
    createPageWidget();

    initLabel(name, m_networkRequest->totalSize());
}

void MusicCommentsWidget::createSearchedItem(const MusicResultsItem &comments)
{
    MusicCommentsItem *item = new MusicCommentsItem(m_messageComments);
    item->createSearchedItem(comments);
    m_commentsItems << item;

    QVBoxLayout *layout = TTKStatic_cast(QVBoxLayout*, m_messageComments->layout());
    layout->insertWidget(layout->count() - 1, item);

    if(!m_isPain)
    {
        setFixedHeight(height() + item->height());
    }
}

void MusicCommentsWidget::buttonClicked(int index)
{
    deleteCommentsItems();

    const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
    m_pageQueryWidget->page(index, pageTotal);
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
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

void MusicCommentsWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicCommentsWidget::mouseMoveEvent(QMouseEvent *event)
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

void MusicCommentsWidget::createPageWidget()
{
    m_pageQueryWidget = new MusicPageQueryWidget(this);
    connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

    const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
    QWidget *w = m_pageQueryWidget->createPageWidget(m_messageComments, pageTotal);
    m_messageComments->layout()->addWidget(w);
}
