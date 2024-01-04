#include "musicplaylistquerywidget.h"
#include "musiccoverrequest.h"
#include "musicqueryplaylistrequest.h"
#include "musicplaylistqueryinfowidget.h"
#include "musicdownloadqueryfactory.h"
#include "musictinyuiobject.h"
#include "musicplaylistquerycategorypopwidget.h"
#include "musicpagequerywidget.h"

#include <QGridLayout>

static constexpr int WIDTH_LABEL_SIZE = 150;
static constexpr int HEIGHT_LABEL_SIZE = 200;
static constexpr int LINE_SPACING_SIZE = 200;

MusicPlaylistQueryItemWidget::MusicPlaylistQueryItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_topListenButton = new QPushButton(this);
    m_topListenButton->setGeometry(0, 0, WIDTH_LABEL_SIZE, 20);
    m_topListenButton->setIcon(QIcon(":/tiny/btn_listen_hover"));
    m_topListenButton->setText(" - ");
    m_topListenButton->setStyleSheet(TTK::UI::BorderStyle01 + TTK::UI::BackgroundStyle04 + TTK::UI::ColorStyle06);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(TTK::UI::TinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
    m_topListenButton->setFocusPolicy(Qt::NoFocus);
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 150, WIDTH_LABEL_SIZE, 25);
    m_nameLabel->setText(" - ");

    m_creatorLabel = new QLabel(this);
    m_creatorLabel->setGeometry(0, 175, WIDTH_LABEL_SIZE, 25);
    m_creatorLabel->setText("by anonymous");
}

MusicPlaylistQueryItemWidget::~MusicPlaylistQueryItemWidget()
{
    delete m_topListenButton;
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_creatorLabel;
}

void MusicPlaylistQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(TTK::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_creatorLabel->setToolTip("by " + item.m_nickName);
    m_creatorLabel->setText(TTK::Widget::elidedText(m_creatorLabel->font(), m_creatorLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    bool ok = false;
    const int count = item.m_count.toInt(&ok);
    if(ok)
    {
        if(count >= 10000)
        {
            m_topListenButton->setText(tr("%1W").arg(count / 10000));
        }
        else
        {
            m_topListenButton->setText(QString::number(count));
        }
    }
    else
    {
        m_topListenButton->setText(item.m_count);
    }

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *d = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->startToRequest(item.m_coverUrl);
    }
}

void MusicPlaylistQueryItemWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicImageRenderer *render = new MusicImageRenderer(sender());
    connect(render, SIGNAL(renderFinished(QPixmap)), SLOT(renderFinished(QPixmap)));
    render->setInputData(bytes, m_iconLabel->size());
    render->start();
}

void MusicPlaylistQueryItemWidget::renderFinished(const QPixmap &data)
{
    m_iconLabel->setPixmap(data);
    m_topListenButton->raise();
    m_playButton->raise();
}

void MusicPlaylistQueryItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}



MusicPlaylistQueryWidget::MusicPlaylistQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_initialized(false),
      m_categoryChanged(false),
      m_gridLayout(nullptr),
      m_pageQueryWidget(nullptr),
      m_infoWidget(nullptr),
      m_categoryButton(nullptr)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_networkRequest = G_DOWNLOAD_QUERY_PTR->makePlaylistRequest(this);
    connect(m_networkRequest, SIGNAL(createPlaylistItem(MusicResultDataItem)), SLOT(createPlaylistItem(MusicResultDataItem)));
}

MusicPlaylistQueryWidget::~MusicPlaylistQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_categoryButton;
    delete m_pageQueryWidget;
}

void MusicPlaylistQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSingleSearch({});
}

void MusicPlaylistQueryWidget::setCurrentID(const QString &id)
{
    setCurrentValue(id);

    MusicResultDataItem result;
    result.m_id = id;
    currentPlaylistClicked(result);
}

void MusicPlaylistQueryWidget::resizeWidget()
{
    if(m_infoWidget)
    {
        m_infoWidget->resizeWidget();
    }

    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(const TTKResizeWidget &widget : qAsConst(m_resizeWidgets))
        {
            m_gridLayout->removeWidget(widget.m_label);
        }

        const int lineNumber = QUERY_WIDGET_WIDTH / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicPlaylistQueryWidget::createPlaylistItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        TTK::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
        m_container->addWidget(scrollArea);

        m_initialized = true;
        QVBoxLayout *mainlayout = TTKObjectCast(QVBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QHBoxLayout *containTopLayout  = new QHBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);
        m_categoryButton = new MusicPlaylistFoundCategoryPopWidget(m_mainWindow);
        m_categoryButton->setCategory(m_networkRequest->queryServer(), this);
        containTopLayout->addWidget(m_categoryButton);
        containTopLayout->addStretch(1);

        const QStringList titles{tr("Recommend"), tr("Top"), tr("Hot"), tr("New")};
        for(const QString &data : qAsConst(titles))
        {
            QLabel *l = new QLabel(data, containTopWidget);
            l->setStyleSheet(QString("QLabel::hover{%1}").arg(TTK::UI::ColorStyle07));
            QFrame *hline = new QFrame(containTopWidget);
            hline->setFrameShape(QFrame::VLine);
            hline->setStyleSheet(TTK::UI::ColorStyle12);
            containTopLayout->addWidget(l);
            containTopLayout->addWidget(hline);
        }
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(35);
        containWidget->setLayout(m_gridLayout);

        mainlayout->addWidget(containTopWidget);
        mainlayout->addWidget(line);
        mainlayout->addWidget(containWidget);

        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWindow);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        mainlayout->addWidget(m_pageQueryWidget->createPageWidget(m_mainWindow, m_networkRequest->pageTotalSize()));
        mainlayout->addStretch(1);
    }

    if(m_categoryChanged && m_pageQueryWidget)
    {
        m_categoryChanged = false;
        m_pageQueryWidget->reset(m_networkRequest->pageTotalSize());
    }

    MusicPlaylistQueryItemWidget *label = new MusicPlaylistQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentPlaylistClicked(MusicResultDataItem)));
    label->setResultDataItem(item);

    const int lineNumber = QUERY_WIDGET_WIDTH / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);

    m_resizeWidgets.push_back({label, label->font()});
}

void MusicPlaylistQueryWidget::currentPlaylistClicked(const MusicResultDataItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicPlaylistQueryInfoWidget(this);

    MusicQueryPlaylistRequest *d = TTKObjectCast(MusicQueryPlaylistRequest*, G_DOWNLOAD_QUERY_PTR->makePlaylistRequest(this));
    MusicResultDataItem info(item);
    if(info.isEmpty())
    {
        d->startToQueryInfo(info);
    }

    m_infoWidget->setQueryInput(d);
    m_infoWidget->setResultDataItem(info, this);
    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicPlaylistQueryWidget::backToPlaylistMenu()
{
    m_container->setCurrentIndex(0);
}

void MusicPlaylistQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_value.clear();
        m_categoryChanged = true;
        m_categoryButton->setText(category.m_value);
        m_categoryButton->closeMenu();

        while(!m_resizeWidgets.isEmpty())
        {
            QWidget *w = m_resizeWidgets.takeLast().m_label;
            m_gridLayout->removeWidget(w);
            delete w;
        }
        m_networkRequest->startToSingleSearch(category.m_key);
    }
}

void MusicPlaylistQueryWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast().m_label;
        m_gridLayout->removeWidget(w);
        delete w;
    }

    m_pageQueryWidget->page(index, m_networkRequest->pageTotalSize());
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}
