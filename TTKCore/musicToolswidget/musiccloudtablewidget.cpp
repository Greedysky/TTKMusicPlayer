#include "musiccloudtablewidget.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musiccoreutils.h"

#include <QScrollBar>

MusicCloudUploadTableWidget::MusicCloudUploadTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 300);
    headerview->resizeSection(2, 111);
    headerview->resizeSection(3, 50);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03.arg(50));

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_progressBarDelegate);

}

MusicCloudUploadTableWidget::~MusicCloudUploadTableWidget()
{
    delete m_progressBarDelegate;
    clear();
}

QString MusicCloudUploadTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudUploadTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}



MusicCloudDownloadTableWidget::MusicCloudDownloadTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 300);
    headerview->resizeSection(2, 111);
    headerview->resizeSection(3, 50);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03.arg(50));

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_progressBarDelegate);

    musicSongsFileName();

    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicCloudDownloadTableWidget::~MusicCloudDownloadTableWidget()
{
    delete m_progressBarDelegate;
    clear();

    MusicCloudDownloadRecordConfigManager xml;
    xml.writeDownloadConfig(m_musicRecords);
}

QString MusicCloudDownloadTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudDownloadTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudDownloadTableWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 3);
        if(it && it->data(MUSIC_TIMES_ROLE).toLongLong() == time)
        {
            item(i, 2)->setData(MUSIC_PROCS_ROLE, percent);
            item(i, 3)->setText( total );

            if(percent == 100)
            {
                m_musicRecords[i].m_size = total;
            }
            break;
        }
    }
}

void MusicCloudDownloadTableWidget::createDownloadItem(const QString &name, qint64 time)
{
    setRowCount( rowCount() + 1);
    QString musicName = name;
    musicName.remove(MusicUtils::Core::musicPrefix()).chop(4);

    MusicCloudDownloadRecord record;
    record.m_name = musicName;
    record.m_path = QFileInfo(name).absoluteFilePath();
    record.m_size = "0.0M";
    m_musicRecords << record;

    createItem(rowCount() - 1, musicName, "0.0M", time);
}

void MusicCloudDownloadTableWidget::musicSongsFileName()
{
    MusicCloudDownloadRecordConfigManager xml;
    if(!xml.readDownloadXMLConfig())
    {
        return;
    }
    xml.readDownloadConfig(m_musicRecords);

    setRowCount(m_musicRecords.count()); //reset row count
    for(int i=0; i<m_musicRecords.count(); i++)
    {
        createItem(i, m_musicRecords[i].m_name, m_musicRecords[i].m_size, 999);
    }
}

void MusicCloudDownloadTableWidget::createItem(int index, const QString &name,
                                               const QString &size, qint64 time)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 160));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip( name );
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem( size );
    item->setTextAlignment(Qt::AlignCenter);
    item->setData(MUSIC_TIMES_ROLE, time);
    setItem(index, 3, item);
}
