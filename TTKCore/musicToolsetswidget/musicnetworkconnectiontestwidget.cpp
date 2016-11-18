#include "musicnetworkconnectiontestwidget.h"
#include "ui_musicnetworkconnectiontestwidget.h"
#include "musicuiobject.h"

MusicNetworkConnectionItem::MusicNetworkConnectionItem(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_iconLabel = new QLabel(this);
    m_nameText = new QLabel(this);
    m_stateText = new QLabel(this);

    m_iconLabel->setFixedWidth(32);
    m_nameText->setFixedWidth(340);

    layout->addWidget(m_iconLabel);
    layout->addWidget(m_nameText);
    layout->addWidget(m_stateText);

    stop();

    setLayout(layout);
}

QString MusicNetworkConnectionItem::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkConnectionItem::setText(const QString &text)
{
    m_nameText->setText(text);
}

void MusicNetworkConnectionItem::start()
{
    m_stateText->setText(tr("Detecting"));
    m_stateText->setStyleSheet(MusicUIObject::MColorStyle08);
}

void MusicNetworkConnectionItem::stop()
{
    m_iconLabel->setPixmap(QPixmap(":/tiny/lb_question"));

    m_stateText->setText(tr("Not Detected"));
    m_stateText->setStyleSheet(MusicUIObject::MColorStyle03);
}

void MusicNetworkConnectionItem::testFinshed()
{
    m_stateText->setText(tr("Detected"));
    m_stateText->setStyleSheet(MusicUIObject::MColorStyle07);
}

MusicNetworkConnectionItem::~MusicNetworkConnectionItem()
{
    delete m_iconLabel;
    delete m_nameText;
    delete m_stateText;
}


MusicNetworkConnectionTestWidget::MusicNetworkConnectionTestWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicNetworkConnectionTestWidget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->iconLabel->setType(MusicGifLabelWidget::Gif_Check_Blue);
    ui->textLabel->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    ui->startButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    ui->verticalLayout->setSpacing(3);
    ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

    MusicNetworkConnectionItem *item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("11111"));
    ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("22222"));
    ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("33333"));
    ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("44444"));
    ui->verticalLayout->addWidget(item);

    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    item->setText(tr("55555"));
    ui->verticalLayout->addWidget(item);

    connect(ui->startButton, SIGNAL(clicked()), SLOT(buttonStateChanged()));
}

MusicNetworkConnectionTestWidget::~MusicNetworkConnectionTestWidget()
{
    while(!m_connectionItems.isEmpty())
    {
        delete m_connectionItems.takeLast();
    }
    delete ui;
}

QString MusicNetworkConnectionTestWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkConnectionTestWidget::show()
{
    ui->iconLabel->timeout();
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveWidget::show();
}

void MusicNetworkConnectionTestWidget::buttonStateChanged()
{
    if(!ui->iconLabel->isRunning())
    {
        ui->iconLabel->start();
        ui->startButton->setText(tr("stopTest"));
        foreach(MusicNetworkConnectionItem *item, m_connectionItems)
        {
            item->start();
        }
    }
    else
    {
        ui->iconLabel->stop();
        ui->startButton->setText(tr("startTest"));
        foreach(MusicNetworkConnectionItem *item, m_connectionItems)
        {
            item->stop();
        }
    }
}
