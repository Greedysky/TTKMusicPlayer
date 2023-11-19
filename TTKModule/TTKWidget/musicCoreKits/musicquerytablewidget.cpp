#include "musicquerytablewidget.h"

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent),
      m_networkRequest(nullptr)
{
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

MusicQueryTableWidget::~MusicQueryTableWidget()
{
    delete m_networkRequest;
}

void MusicQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    delete m_networkRequest;
    m_networkRequest = query;
    connect(m_networkRequest, SIGNAL(clearItems()), SLOT(removeItems()));
    connect(m_networkRequest, SIGNAL(createSearchedItem(MusicResultInfoItem)), SLOT(createSearchedItem(MusicResultInfoItem)));
}

MusicAbstractQueryRequest *MusicQueryTableWidget::queryInput() const
{
    return m_networkRequest;
}
