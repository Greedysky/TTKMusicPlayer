#include "musicquerytablewidget.h"

MusicQueryTableWidget::MusicQueryTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    m_downLoadManager = nullptr;
}

MusicQueryTableWidget::~MusicQueryTableWidget()
{
    delete m_downLoadManager;
}

void MusicQueryTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    delete m_downLoadManager;
    m_downLoadManager = query;
    connect(m_downLoadManager, SIGNAL(clearAllItems()), SLOT(clearAllItems()));
    connect(m_downLoadManager, SIGNAL(createSearchedItem(MusicSearchedItem)), SLOT(createSearchedItem(MusicSearchedItem)));
}

MusicAbstractQueryRequest *MusicQueryTableWidget::getQueryInput()
{
    return m_downLoadManager;
}
