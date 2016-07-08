#include "musiccloudsharedsongwidget.h"
#include "musicnumberdefine.h"
#include "qnconf.h"

#include <QPainter>
#include <QBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QMenu>

MusicCloudSharedSongTableWidget::MusicCloudSharedSongTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 266);
    headerview->resizeSection(2, 35);

    QNConf::ACCESS_KEY = "L2GGQ-ttIlTScXVtXOdwPF2ftQAEiVK1qor5KCu3";
    QNConf::SECRET_KEY = "FXiQ8EWibo-9tIlWRS3UAJOqv94pM1QViU2Gw25y";

    m_networkManager = new QNetworkAccessManager(this);
    m_qnListData = new QNSimpleListData(m_networkManager, this);
    connect(m_qnListData, SIGNAL(receiveFinshed(QNDataItems)),
                          SLOT(receiveDataFinshed(QNDataItems)));

    QTimer::singleShot(MT_MS*100, this, SLOT(updateList()));
}

MusicCloudSharedSongTableWidget::~MusicCloudSharedSongTableWidget()
{
    delete m_qnListData;
    delete m_networkManager;
}

QString MusicCloudSharedSongTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudSharedSongTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudSharedSongTableWidget::updateList()
{
    emit updateLabelMessage(tr("List Updating"));
    m_qnListData->listDataToServer("music");
}

void MusicCloudSharedSongTableWidget::receiveDataFinshed(const QNDataItems &items)
{
    clear();
    setRowCount(items.count());
    for(int i=0; i<items.count(); ++i)
    {
        QNDataItem dataItem = items[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), dataItem.m_name, Qt::ElideRight, 260));
        item->setToolTip(dataItem.m_name);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), MusicUtils::UNumber::size2Number(dataItem.m_size),
                                                              Qt::ElideRight, 30));
        item->setToolTip(MusicUtils::UNumber::size2Label(dataItem.m_size));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
    emit updateLabelMessage(tr("List Update Finished!"));
}

void MusicCloudSharedSongTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    menu.addAction(QIcon(":/contextMenu/delete"), tr("deleteItem"));
    menu.addAction(QIcon(":/contextMenu/add"), tr("addNewItem"));
    menu.addAction(tr("changItemName"));
    menu.exec(QCursor::pos());
}


MusicCloudSharedSongWidget::MusicCloudSharedSongWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_tableWidget = new MusicCloudSharedSongTableWidget(this);
    connect(m_tableWidget, SIGNAL(updateLabelMessage(QString)), SLOT(updateLabelMessage(QString)));
    m_statusLabel = new QLabel(this);
    m_statusLabel->setFixedHeight(40);
    m_statusLabel->setStyleSheet("background:red");

    layout->addWidget(m_tableWidget);
    layout->addWidget(m_statusLabel);
    setLayout(layout);
}

MusicCloudSharedSongWidget::~MusicCloudSharedSongWidget()
{
    delete m_tableWidget;
    delete m_statusLabel;
}

QString MusicCloudSharedSongWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudSharedSongWidget::updateLabelMessage(const QString &text)
{
    m_statusLabel->setText(text);
}

void MusicCloudSharedSongWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255, 50));
    painter.end();

    QWidget::paintEvent(event);
}
