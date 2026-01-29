#include "musicwebdjradioquerywidget.h"
#include "musicdjradioprogramcategoryrequest.h"
#include "musictinyuiobject.h"
#include "musicpagequerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musiccoverrequest.h"
#include "musicwebdjradioinfowidget.h"

static constexpr int WIDTH_LABEL_SIZE = 150;
static constexpr int HEIGHT_LABEL_SIZE = 200;
static constexpr int LINE_SPACING_SIZE = 150;

MusicWebDJRadioQueryItemWidget::MusicWebDJRadioQueryItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(TTK::UI::TinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 150, WIDTH_LABEL_SIZE, 25);
    m_nameLabel->setText(TTK_DEFAULT_STR);

    m_creatorLabel = new QLabel(this);
    m_creatorLabel->setGeometry(0, 175, WIDTH_LABEL_SIZE, 25);
    m_creatorLabel->setText("by anonymous");
}

MusicWebDJRadioQueryItemWidget::~MusicWebDJRadioQueryItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_creatorLabel;
}

void MusicWebDJRadioQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(TTK::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_creatorLabel->setToolTip("by " + item.m_nickName);
    m_creatorLabel->setText(TTK::Widget::elidedText(m_creatorLabel->font(), m_creatorLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *req = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
        connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        req->startToRequest(item.m_coverUrl);
    }
}

void MusicWebDJRadioQueryItemWidget::downloadFinished(const QByteArray &bytes)
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

void MusicWebDJRadioQueryItemWidget::renderFinished(const QPixmap &data)
{
    m_iconLabel->setPixmap(data);
    m_playButton->raise();
}

void MusicWebDJRadioQueryItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}



MusicWebDJRadioQueryWidget::MusicWebDJRadioQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_initialized = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_pageQueryWidget = nullptr;

    m_networkRequest = new MusicDJRadioProgramCategoryRequest(this);
    connect(m_networkRequest, SIGNAL(createProgramItem(MusicResultDataItem)), SLOT(createProgramItem(MusicResultDataItem)));
}

MusicWebDJRadioQueryWidget::~MusicWebDJRadioQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_pageQueryWidget;
}

void MusicWebDJRadioQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_networkRequest->startToSearchByID(value);
}

void MusicWebDJRadioQueryWidget::setCurrentKey(const QString &id)
{
    MusicAbstractItemQueryWidget::setCurrentValue(id);
    MusicResultDataItem item;
    item.m_id = id;
    currentRadioClicked(item);
}

void MusicWebDJRadioQueryWidget::resizeWidget()
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

        const int lineNumber = (QUERY_WIDGET_WIDTH - LINE_SPACING_SIZE / 2) / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignLeft);
        }
    }
}

void MusicWebDJRadioQueryWidget::createProgramItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWindow);
        m_container->addWidget(scrollArea);

        m_initialized = true;
        QVBoxLayout *mainLayout = TTKObjectCast(QVBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QHBoxLayout *containTopLayout = new QHBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(10, 10, 0, 0);

        QPushButton *backButton = new QPushButton(tr("Back"), containTopWidget);
        backButton->setFixedSize(90, 30);
        backButton->setStyleSheet(TTK::UI::PushButtonStyle03);
        backButton->setCursor(QCursor(Qt::PointingHandCursor));
        connect(backButton, SIGNAL(clicked()), this, SIGNAL(backToMainMenu()));
        containTopLayout->addWidget(backButton);
        containTopLayout->addStretch(1);
#ifdef Q_OS_UNIX
        backButton->setFocusPolicy(Qt::NoFocus);
#endif
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(TTK::UI::ColorStyle12);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(35);
        containWidget->setLayout(m_gridLayout);

        mainLayout->addWidget(containTopWidget);
        mainLayout->addWidget(line);
        mainLayout->addWidget(containWidget);

        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWindow);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        mainLayout->addWidget(m_pageQueryWidget->createPageWidget(m_mainWindow, m_networkRequest->pageTotalSize()));
        mainLayout->addStretch(1);
    }

    if(m_pageQueryWidget)
    {
        m_pageQueryWidget->reset(m_networkRequest->pageTotalSize());
    }

    MusicWebDJRadioQueryItemWidget *label = new MusicWebDJRadioQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentRadioClicked(MusicResultDataItem)));
    label->setResultDataItem(item);

    const int lineNumber = (QUERY_WIDGET_WIDTH - LINE_SPACING_SIZE / 2) / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignLeft);

    m_resizeWidgets.append({label, label->font()});
}

void MusicWebDJRadioQueryWidget::currentRadioClicked(const MusicResultDataItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebDJRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setCurrentValue(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicWebDJRadioQueryWidget::backToMainMenuClicked()
{
    if(!m_initialized)
    {
        Q_EMIT backToMainMenu();
        return;
    }
    m_container->setCurrentIndex(0);
}

void MusicWebDJRadioQueryWidget::buttonClicked(int index)
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
