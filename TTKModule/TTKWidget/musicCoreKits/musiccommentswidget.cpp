#include "musiccommentswidget.h"
#include "musicdownloadqueryfactory.h"
#include "musiccoverrequest.h"
#include "musicemojilabelwidget.h"
#include "musicfunctionuiobject.h"
#include "musicinteriorlrcuiobject.h"
#include "musicpagequerywidget.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "ttkclickedlabel.h"

#include <QTextEdit>

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
    //
    m_userName = new QLabel(userWidget);
    m_userCommit = new QTextEdit(userWidget);
    m_userCommit->setReadOnly(true);
    m_userCommit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_userCommit->setStyleSheet(TTK::UI::BorderStyle01 + TTK::UI::BackgroundStyle01 +
                                TTK::UI::ColorStyle04);
    m_userCommit->viewport()->setStyleSheet(TTK::UI::BackgroundStyle01 + TTK::UI::ColorStyle04);
    m_userName->setStyleSheet(TTK::UI::ColorStyle03);
    userWidgetLayout->addWidget(m_userName);
    userWidgetLayout->addWidget(m_userCommit);
    userWidget->setLayout(userWidgetLayout);
    //
    QWidget *operWidget = new QWidget(textWidget);
    QHBoxLayout *operWidgetLayout = new QHBoxLayout(operWidget);
    operWidgetLayout->setContentsMargins(0, 0, 0, 0);
    operWidgetLayout->setSpacing(4);

    m_timerLabel = new QLabel(operWidget);
    m_timerLabel->setStyleSheet(TTK::UI::ColorStyle03);

    TTKClickedLabel *ngReportLabel = new TTKClickedLabel(tr("Report"), operWidget);
    ngReportLabel->setStyleSheet(TTK::UI::ColorStyle03);

    QFrame *spliteLine1 = new QFrame(operWidget);
    spliteLine1->setFixedWidth(1);
    spliteLine1->setFrameShape(QFrame::VLine);
    spliteLine1->setStyleSheet(TTK::UI::ColorStyle03);

    QPushButton *starButton = new QPushButton(operWidget);
    starButton->setCursor(QCursor(Qt::PointingHandCursor));
    starButton->setFixedSize(12, 11);
    starButton->setStyleSheet(TTK::UI::InteriorStar);

    m_starLabel = new QLabel(operWidget);
    m_starLabel->setText("(0)");
    m_starLabel->setStyleSheet(TTK::UI::ColorStyle03 + TTK::UI::BackgroundStyle01);

    QFrame *spliteLine2 = new QFrame(operWidget);
    spliteLine2->setFixedWidth(1);
    spliteLine2->setFrameShape(QFrame::VLine);
    spliteLine2->setStyleSheet(TTK::UI::ColorStyle03);
    TTKClickedLabel *ReplyLabel = new TTKClickedLabel(tr("Reply"), operWidget);
    ReplyLabel->setStyleSheet(TTK::UI::ColorStyle03);
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
    solidLine->setStyleSheet(TTK::UI::BorderStyle04 + TTK::UI::ColorStyle03);
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

void MusicCommentsItem::addCellItem(const MusicResultDataItem &item)
{
    m_userName->setText(item.m_nickName + ":");
    m_userName->setFixedWidth(TTK::Widget::fontTextWidth(m_userName->font(), m_userName->text()));
    m_timerLabel->setText(TTKDateTime::format(item.m_time.toULongLong(), TTK_DATE_TIMES_FORMAT));
    m_timerLabel->setFixedWidth(TTK::Widget::fontTextWidth(m_timerLabel->font(), m_timerLabel->text()));
    m_starLabel->setText(QString("(%1)").arg(item.m_count));
    m_userCommit->setText(item.m_description);

    MusicCoverRequest *d = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startToRequest(item.m_coverUrl);
}

void MusicCommentsItem::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    QPixmap pix;
    pix.loadFromData(bytes);
    m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
}



MusicCommentsWidget::MusicCommentsWidget(QWidget *parent)
    : QLabel(parent),
      m_isPain(true),
      m_messageEdit(nullptr),
      m_topLabel(nullptr),
      m_commentsLabel(nullptr),
      m_messageComments(nullptr),
      m_pageQueryWidget(nullptr),
      m_networkRequest(nullptr)
{

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

    setObjectName(className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), TTK::UI::BackgroundStyle07));

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
        closeButton->setStyleSheet(TTK::UI::BtnPClose);
        closeButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
        closeButton->setFocusPolicy(Qt::NoFocus);
#endif
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
    m_messageEdit->verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);
    m_messageEdit->setStyleSheet(TTK::UI::BorderStyle02 + TTK::UI::BackgroundStyle01 + TTK::UI::ColorStyle04);
    m_messageEdit->viewport()->setStyleSheet(TTK::UI::BackgroundStyle01 + TTK::UI::ColorStyle04);
    messageBox->setAttribute(Qt::WA_TranslucentBackground, true);
    m_commentsLabel = new QLabel(contentsWidget);
    m_commentsLabel->setStyleSheet(TTK::UI::ColorStyle04);
    QFrame *solidLine = new QFrame(contentsWidget);
    solidLine->setFrameShape(QFrame::HLine);
    solidLine->setFixedHeight(1);
    solidLine->setStyleSheet(TTK::UI::ColorStyle03);
    //
    QWidget *messageMiddle = new QWidget(messageBox);
    QHBoxLayout *messageMiddleLayout = new QHBoxLayout(messageMiddle);
    messageMiddleLayout->setContentsMargins(0, 10, 0, 0);
    QPushButton *emojiButton = new QPushButton(messageMiddle);
    QPushButton *sendButton = new QPushButton(tr("Send"), messageMiddle);
    emojiButton->setIcon(QIcon(":/lrc/lb_emoji"));
    emojiButton->setCursor(QCursor(Qt::PointingHandCursor));
    emojiButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    connect(emojiButton, SIGNAL(clicked()), SLOT(createEMOJILabelWidget()));
    sendButton->setFixedSize(65, 25);
    sendButton->setCursor(QCursor(Qt::PointingHandCursor));
    sendButton->setStyleSheet(TTK::UI::PushButtonStyle03);
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
        TTK::Widget::generateVScrollAreaFormat(scrollArea, contentsWidget, false);

        contentsWidget->setObjectName("ContentsWidget");
        contentsWidget->setStyleSheet(QString("#%1{ %2 }").arg(contentsWidget->objectName(), TTK::UI::BackgroundStyle01));
        QWidget *view = scrollArea->viewport();
        view->setObjectName("Viewport");
        view->setStyleSheet(QString("#%1{ %2 }").arg(view->objectName(), TTK::UI::BackgroundStyle01));
        //
        mainLayout->addWidget(topWidget);
        mainLayout->addWidget(scrollArea);
    }
    else
    {
        mainLayout->addWidget(contentsWidget);
        setFixedHeight(300);
    }

    initLabel({}, 0);
    setLayout(mainLayout);
}

void MusicCommentsWidget::setCurrentSongName(const QString &name)
{
    deleteCommentsItems();

    TTKSemaphoreLoop loop;
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    m_networkRequest->startToSearch(name);
    loop.exec();

    TTKObjectCast(QVBoxLayout*, m_messageComments->layout())->addStretch(1);
    createPageWidget();

    initLabel(name, m_networkRequest->totalSize());
}

void MusicCommentsWidget::createCommentItem(const MusicResultDataItem &item)
{
    MusicCommentsItem *comment = new MusicCommentsItem(m_messageComments);
    comment->addCellItem(item);
    m_commentsItems << comment;

    QVBoxLayout *layout = TTKObjectCast(QVBoxLayout*, m_messageComments->layout());
    layout->insertWidget(layout->count() - 1, comment);

    if(!m_isPain)
    {
        setFixedHeight(height() + comment->height());
    }
}

void MusicCommentsWidget::buttonClicked(int index)
{
    deleteCommentsItems();

    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
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

    QWidget *w = m_pageQueryWidget->createPageWidget(m_messageComments, m_networkRequest->pageTotalSize());
    m_messageComments->layout()->addWidget(w);
}
