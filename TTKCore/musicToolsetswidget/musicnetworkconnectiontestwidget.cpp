#include "musicnetworkconnectiontestwidget.h"
#include "ui_musicnetworkconnectiontestwidget.h"
#include "musicuiobject.h"

MusicNetworkConnectionItem::MusicNetworkConnectionItem(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *iconLabel = new QLabel(this);
    QLabel *nameText = new QLabel(this);
    QLabel *stateText = new QLabel(this);

    iconLabel->setFixedWidth(32);
    iconLabel->setPixmap(QPixmap(":/tiny/lb_question"));
    nameText->setFixedWidth(340);
    nameText->setText("fffff");
    stateText->setText("dfgdfg");

    layout->addWidget(iconLabel);
    layout->addWidget(nameText);
    layout->addWidget(stateText);

    setLayout(layout);
}

MusicNetworkConnectionItem::~MusicNetworkConnectionItem()
{

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

    ui->textLabel->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    ui->startButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    ui->verticalLayout->setSpacing(3);
    ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

    MusicNetworkConnectionItem *item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    ui->verticalLayout->addWidget(item);
    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    ui->verticalLayout->addWidget(item);
    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    ui->verticalLayout->addWidget(item);
    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    ui->verticalLayout->addWidget(item);
    item = new MusicNetworkConnectionItem(this);
    m_connectionItems << item;
    ui->verticalLayout->addWidget(item);

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
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveWidget::show();
}
