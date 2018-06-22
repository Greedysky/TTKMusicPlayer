#include "musiccloudtoolboxwidget.h"
#include "musiccloudtablewidget.h"

MusicCloudToolBoxWidgetItem::MusicCloudToolBoxWidgetItem(int index, const QString &text, QWidget *parent)
    : MusicFunctionToolBoxWidgetItem(parent)
{
    m_topWidget = new MusicFunctionToolBoxTopWidget(index, text, this);

    init();
}


MusicCloudToolBoxWidget::MusicCloudToolBoxWidget(QWidget *parent)
    : MusicFunctionToolBoxWidget(parent)
{
    m_uploadTable = new MusicCloudUploadTableWidget(this);
    addItem(m_uploadTable, tr("UploadFailed"));

    m_downloadTable = new MusicCloudDownloadTableWidget(this);
    addItem(m_downloadTable, tr("Download"));
}

MusicCloudToolBoxWidget::~MusicCloudToolBoxWidget()
{
    delete m_uploadTable;
    delete m_downloadTable;
}

MusicFunctionToolBoxWidgetItem *MusicCloudToolBoxWidget::createItem(QWidget *item, const QString &text)
{
    MusicFunctionToolBoxWidgetItem *it = new MusicCloudToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addItem(item);
    it->setItemExpand(true);
    return it;
}
