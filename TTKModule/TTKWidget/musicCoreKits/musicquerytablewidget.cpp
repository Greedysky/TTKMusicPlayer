#include "musicquerytablewidget.h"
#include "musicgiflabelwidget.h"

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent),
      m_networkRequest(nullptr)
{
    m_loadingLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Module::CicleBlue, this);

    m_labelDelegate = new TTKLabelItemDelegate(this);
    m_labelDelegate->setStyleSheet(TTK::UI::BackgroundStyle13);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

MusicQueryTableWidget::~MusicQueryTableWidget()
{
    delete m_loadingLabel;
    delete m_labelDelegate;
    delete m_networkRequest;
}

void MusicQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    delete m_networkRequest;
    m_networkRequest = query;
    connect(m_networkRequest, SIGNAL(clearItems()), SLOT(removeItems()));
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    connect(m_networkRequest, SIGNAL(createResultItem(MusicResultInfoItem)), SLOT(createResultItem(MusicResultInfoItem)));
}

MusicAbstractQueryRequest *MusicQueryTableWidget::queryInput() const
{
    return m_networkRequest;
}

void MusicQueryTableWidget::itemCellClicked(int row, int column)
{
    MusicFillItemTableWidget::itemCellClicked(row, column);

    if(rowCount() > 0 && row == rowCount() - 1)
    {
        QTableWidgetItem *it = item(row, 0);
        if(it && it->data(TTK_DISPLAY_ROLE).toString() == tr("Query more data"))
        {
            setItemDelegateForRow(row, nullptr);
            clearSpans();
            removeRow(row);

            setLoadingStatus(true);
            m_networkRequest->startToPage(m_networkRequest->pageIndex() + 1);
        }
    }
}

void MusicQueryTableWidget::removeItems()
{
    if(rowCount() > 0)
    {
        setItemDelegateForRow(rowCount() - 1, nullptr);
    }

    MusicFillItemTableWidget::removeItems();
}

void MusicQueryTableWidget::createFinishedItem()
{
    setLoadingStatus(false);

    setRowCount(rowCount() + 1);
    const int count = rowCount() - 1;

    for(int i = 0; i < columnCount(); ++i)
    {
        setItem(count, i, new QTableWidgetItem);
    }
    setSpan(count, 0, 1, columnCount());

    QTableWidgetItem *it = item(count, 0);
    if(it)
    {
        const bool more = (m_networkRequest->pageTotalSize() > m_networkRequest->pageIndex() + 1);
        it->setData(TTK_DISPLAY_ROLE, more ? tr("Query more data") : tr("No more data"));
        setItemDelegateForRow(count, m_labelDelegate);
    }
}

void MusicQueryTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicFillItemTableWidget::resizeEvent(event);
    m_loadingLabel->move((width() - m_loadingLabel->width()) / 2, (height() - m_loadingLabel->height()) / 2);
}

void MusicQueryTableWidget::setLoadingStatus(bool status) const
{
    m_loadingLabel->run(status);
}
