#include "musicquerytablewidget.h"

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    m_networkRequest = nullptr;
}

MusicQueryTableWidget::~MusicQueryTableWidget()
{
    delete m_networkRequest;
}

void MusicQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    delete m_networkRequest;
    m_networkRequest = query;
    connect(m_networkRequest, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_networkRequest, SIGNAL(createSearchedItem(MusicSearchedItem)), SLOT(createSearchedItem(MusicSearchedItem)));
}

MusicAbstractQueryRequest *MusicQueryTableWidget::getQueryInput()
{
    return m_networkRequest;
}
