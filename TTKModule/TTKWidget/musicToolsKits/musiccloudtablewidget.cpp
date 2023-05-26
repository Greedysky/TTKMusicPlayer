#include "musiccloudtablewidget.h"
#include "musicitemdelegate.h"
#include "musicconnectionpool.h"
#include "musicnumberutils.h"
#include "musictoastlabel.h"

Q_DECLARE_METATYPE(MusicCloudDataItem)

MusicCloudDownloadTableWidget::MusicCloudDownloadTableWidget(QWidget *parent)
    : MusicAbstractDownloadTableWidget(parent)
{
    setColumnCount(4);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 168);
    headerview->resizeSection(2, 83);
    headerview->resizeSection(3, 50);

    m_type = TTK::Record::CloudDownload;
    setItemDelegateForColumn(2, m_progressBarDelegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    TTK::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    setFixedHeight(0);

    G_CONNECTION_PTR->setValue(className(), this);
}

MusicCloudDownloadTableWidget::~MusicCloudDownloadTableWidget()
{
    G_CONNECTION_PTR->removeValue(this);
}

void MusicCloudDownloadTableWidget::addCellItem(int index, const MusicSong &record)
{
    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(record.name());
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(TTK_PROGRESS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem(record.sizeStr());
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignRight | Qt::AlignVCenter);
    item->setData(TTK_TIME_ROLE, record.addTimeStr());
    setItem(index, 3, item);

    setFixedHeight(totalHeight());
}


MusicCloudUploadTableWidget::MusicCloudUploadTableWidget(QWidget *parent)
    : MusicAbstractDownloadTableWidget(parent)
{
    setColumnCount(3);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 251);
    headerview->resizeSection(2, 50);

    m_type = TTK::Record::CloudUpload;
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    TTK::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    setFixedHeight(0);

    G_CONNECTION_PTR->setValue(className(), this);
}

MusicCloudUploadTableWidget::~MusicCloudUploadTableWidget()
{
    G_CONNECTION_PTR->removeValue(this);
}

void MusicCloudUploadTableWidget::uploadFileError(const MusicCloudDataItem &item)
{
    const int count = rowCount() + 1;
    setRowCount(count);

    MusicSong record;
    record.setName(item.m_dataItem.m_name);
    record.setPath(item.m_path);
    record.setSizeStr(TTK::Number::sizeByteToLabel(item.m_dataItem.m_size));

    addCellItem(count - 1, record);
}

void MusicCloudUploadTableWidget::reuploadFile()
{
    if(!isValid())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    const QString data(it->data(TTK_DATA_ROLE).toString());
    if(!data.isEmpty())
    {
        Q_EMIT reuploadFilesToServer({data});
    }
}

void MusicCloudUploadTableWidget::reuploadFiles()
{
    if(!isValid())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    QStringList datas;
    for(const int index : multiSelectedIndexList())
    {
        QTableWidgetItem *it = item(index, 0);
        if(it == nullptr)
        {
            continue;
        }

        const QString data(it->data(TTK_DATA_ROLE).toString());
        if(!data.isEmpty())
        {
            datas << data;
        }
    }

    if(!datas.isEmpty())
    {
        Q_EMIT reuploadFilesToServer(datas);
    }
}

void MusicCloudUploadTableWidget::addCellItem(int index, const MusicSong &record)
{
    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(TTK_DATA_ROLE, record.path());
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(record.name());
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item);

                      item = new QTableWidgetItem(record.sizeStr());
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignRight | Qt::AlignVCenter);
    item->setData(TTK_TIME_ROLE, record.addTimeStr());
    setItem(index, 2, item);

    setFixedHeight(totalHeight());
}

void MusicCloudUploadTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    const bool status = !m_songs->isEmpty();
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(openFileDir()))->setEnabled(status);
    menu.addSeparator();

    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(removeItemAt()))->setEnabled(status);
    menu.addAction(tr("Delete All"), this, SLOT(removeItemAll()))->setEnabled(status);
    menu.addSeparator();

    menu.addAction(tr("Reupload"), this, SLOT(reuploadFile()))->setEnabled(status);
    menu.addAction(tr("Reupload All"), this, SLOT(reuploadFiles()))->setEnabled(status);
    menu.addSeparator();
    menu.exec(QCursor::pos());
}
