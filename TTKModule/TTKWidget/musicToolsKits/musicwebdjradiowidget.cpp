#include "musicwebdjradiowidget.h"
#include "musicwebdjradiocategorywidget.h"
#include "musicwebdjradioquerywidget.h"
#include "musiccoversourcerequest.h"
#include "musicwidgetheaders.h"

MusicWebDJRadioProgramTableWidget::MusicWebDJRadioProgramTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setIconSize(QSize(TTK_ITEM_SIZE_L, TTK_ITEM_SIZE_L));
    setColumnCount(6);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 40);
    headerview->resizeSection(2, 315);
    headerview->resizeSection(3, 90);
    headerview->resizeSection(4, 105);
    headerview->resizeSection(5, 110);

    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    m_programThread = new MusicDJRadioProgramRequest(this);
    connect(m_programThread, SIGNAL(createProgramItem(MusicResultDataItem)), SLOT(createProgramItem(MusicResultDataItem)));
}

MusicWebDJRadioProgramTableWidget::~MusicWebDJRadioProgramTableWidget()
{
    delete m_programThread;
}

void MusicWebDJRadioProgramTableWidget::initialize(TTK::Program type)
{
    m_programThread->startRequest(type);
}

void MusicWebDJRadioProgramTableWidget::resizeSection()
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(2, 315 + (width - WINDOW_WIDTH_MIN));

    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 2);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    }
}

void MusicWebDJRadioProgramTableWidget::itemCellEntered(int row, int column)
{
    if(column == 2 || column == 3)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }

    MusicAbstractTableWidget::itemCellEntered(row, column);
}

void MusicWebDJRadioProgramTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    MusicResultDataItemList items(m_programThread->searchedItems());
    if(row < 0 || row >= items.count())
    {
        return;
    }

    switch(column)
    {
        case 2: Q_EMIT programItemClicked(items[row].m_id, QString("-1")); break;
        case 3: Q_EMIT programItemClicked(QString("-1"), items[row].m_nickName); break;
        default:break;
    }
}

void MusicWebDJRadioProgramTableWidget::createProgramItem(const MusicResultDataItem &data)
{
    const int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, TTK_ITEM_SIZE_2XL);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setIcon(QPixmap(":/image/lb_default_art"));
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(data.m_name);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 2, item);

                      item = new QTableWidgetItem(data.m_tags);
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(index, 3, item);

                      item = new QTableWidgetItem(tr("Played %1").arg(data.m_playCount));
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 4, item);

                      item = new QTableWidgetItem(data.m_updateTime);
    item->setForeground(QColor(TTK::UI::Color02));
    QtItemSetTextAlignment(item, Qt::AlignCenter);
    setItem(index, 5, item);

    if(TTK::isCoverValid(data.m_coverUrl))
    {
        MusicCoverSourceRequest *d = new MusicCoverSourceRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->setHeader("id", index);
        d->startRequest(data.m_coverUrl);
    }
}

void MusicWebDJRadioProgramTableWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicCoverSourceRequest *d(TTKObjectCast(MusicCoverSourceRequest*, sender()));
    if(!d)
    {
        return;
    }

    QTableWidgetItem *it = item(d->header("id").toInt(), 1);
    if(it)
    {
        QPixmap pix;
        pix.loadFromData(bytes);
        it->setIcon(pix);
    }
}



MusicWebDJRadioProgramWidget::MusicWebDJRadioProgramWidget(TTK::Program type, QWidget *parent)
    : QWidget(parent),
      m_type(type)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QWidget *top = new QWidget(this);
    layout->addWidget(top);
    QHBoxLayout *topLayout = new QHBoxLayout(top);
    topLayout->setContentsMargins(10, 20, 10, 0);
    top->setLayout(topLayout);

    QLabel *label = new QLabel(type == TTK::Program::Recommed ? tr("Recommend") : tr("Program"), this);
    QFont font = label->font();
    font.setPixelSize(20);
    font.setBold(true);
    label->setFont(font);
    topLayout->addWidget(label);

    QPushButton *backButton = new QPushButton(tr("Back"), this);
    backButton->setFixedSize(90, 30);
    backButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    backButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(backButton, SIGNAL(clicked()), parent, SLOT(backToMainMenu()));
    topLayout->addWidget(backButton);
    layout->addWidget(top);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
#endif

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet(TTK::UI::ColorStyle11);
    layout->addWidget(line);

    m_tableWidget = new MusicWebDJRadioProgramTableWidget(this);
    connect(m_tableWidget, SIGNAL(programItemClicked(QString,QString)), parent, SLOT(programItemClicked(QString,QString)));
    layout->addWidget(m_tableWidget);
}

MusicWebDJRadioProgramWidget::~MusicWebDJRadioProgramWidget()
{
    delete m_tableWidget;
}

void MusicWebDJRadioProgramWidget::initialize()
{
    m_tableWidget->initialize(m_type);
}

void MusicWebDJRadioProgramWidget::resizeWindow()
{
    m_tableWidget->resizeSection();
}



MusicWebDJRadioWidget::MusicWebDJRadioWidget(QWidget *parent)
    : QStackedWidget(parent),
      m_recommendWidget(nullptr),
      m_programWidget(nullptr),
      m_queryWidget(nullptr)
{
    setObjectName(className());
    setStyleSheet(QString("#%1{%2}").arg(className(), TTK::UI::BackgroundStyle10));

    initFirstWidget();
}

MusicWebDJRadioWidget::~MusicWebDJRadioWidget()
{
    delete m_recommendWidget;
    delete m_programWidget;
    delete m_categoryWidget;
    delete m_queryWidget;
}

void MusicWebDJRadioWidget::initialize()
{
    m_categoryWidget->initialize();
}

void MusicWebDJRadioWidget::resizeWidget()
{
    if(m_categoryWidget)
    {
        m_categoryWidget->resizeWindow();
    }

    if(m_recommendWidget)
    {
        m_recommendWidget->resizeWindow();
    }

    if(m_programWidget)
    {
        m_programWidget->resizeWindow();
    }

    if(m_queryWidget)
    {
        m_queryWidget->resizeWidget();
    }
}

void MusicWebDJRadioWidget::backToMainMenu()
{
    setCurrentIndex(0);
}

void MusicWebDJRadioWidget::createRecommendWidget()
{
    delete m_recommendWidget;
    m_recommendWidget = new MusicWebDJRadioProgramWidget(TTK::Program::Recommed, this);
    m_recommendWidget->initialize();
    addWidget(m_recommendWidget);
    setCurrentWidget(m_recommendWidget);
}

void MusicWebDJRadioWidget::createProgramWidget()
{
    delete m_programWidget;
    m_programWidget = new MusicWebDJRadioProgramWidget(TTK::Program::Rank, this);
    m_programWidget->initialize();
    addWidget(m_programWidget);
    setCurrentWidget(m_programWidget);
}

void MusicWebDJRadioWidget::programItemClicked(const QString &rid, const QString &cid)
{
    delete m_queryWidget;
    m_queryWidget = new MusicWebDJRadioQueryWidget(this);
    connect(m_queryWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenu()));

    if(rid == "-1" && cid != "-1")
    {
        m_queryWidget->setCurrentValue(cid);
    }
    else
    {
        m_queryWidget->setCurrentID(rid);
    }

    addWidget(m_queryWidget);
    setCurrentWidget(m_queryWidget);
}

void MusicWebDJRadioWidget::currentCategoryClicked(const MusicResultDataItem &item)
{
    programItemClicked("-1", item.m_id);
}

void MusicWebDJRadioWidget::initFirstWidget()
{
    QWidget *w = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setSpacing(15);
    layout->setContentsMargins(30, 30, 30, 0);
    w->setLayout(layout);
    //
    QWidget *top = new QWidget(w);
    layout->addWidget(top);
    QHBoxLayout *topLayout = new QHBoxLayout(top);
    topLayout->setSpacing(35);
    topLayout->setContentsMargins(0, 0, 0, 0);
    top->setLayout(topLayout);

    QWidget *leftTop = new QWidget(top);
    QHBoxLayout *leftTopLayout = new QHBoxLayout(leftTop);
    leftTopLayout->setSpacing(0);
    leftTopLayout->setContentsMargins(0, 0, 0, 0);
    TTKClickedLabel *recommendLabel = new TTKClickedLabel(tr("Recommend"), leftTop);
    QFont recommendFont = recommendLabel->font();
    recommendFont.setPixelSize(20);
    recommendFont.setBold(true);
    recommendLabel->setFont(recommendFont);
    leftTopLayout->addWidget(recommendLabel);
    leftTopLayout->addStretch(1);
    connect(recommendLabel, SIGNAL(clicked()), SLOT(createRecommendWidget()));

    TTKClickedLabel *recommendMoreLabel = new TTKClickedLabel(tr("More >"), leftTop);
    recommendMoreLabel->setStyleSheet(TTK::UI::ColorStyle04);
    connect(recommendMoreLabel, SIGNAL(clicked()), SLOT(createRecommendWidget()));
    leftTopLayout->addWidget(recommendMoreLabel);
    topLayout->addWidget(leftTop);
    //
    QWidget *rightTop = new QWidget(top);
    QHBoxLayout *rightTopLayout = new QHBoxLayout(rightTop);
    rightTopLayout->setSpacing(0);
    rightTopLayout->setContentsMargins(0, 0, 0, 0);
    TTKClickedLabel *programLabel = new TTKClickedLabel(tr("Program"), rightTop);
    QFont programFont = programLabel->font();
    programFont.setPixelSize(20);
    programFont.setBold(true);
    programLabel->setFont(programFont);
    rightTopLayout->addWidget(programLabel);
    rightTopLayout->addStretch(1);
    connect(programLabel, SIGNAL(clicked()), SLOT(createProgramWidget()));

    TTKClickedLabel *programMoreLabel = new TTKClickedLabel(tr("More >"), rightTop);
    programMoreLabel->setStyleSheet(TTK::UI::ColorStyle04);
    connect(programMoreLabel, SIGNAL(clicked()), SLOT(createProgramWidget()));
    rightTopLayout->addWidget(programMoreLabel);
    topLayout->addWidget(rightTop);
    //
    QFrame *line = new QFrame(w);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet(TTK::UI::ColorStyle11);
    layout->addWidget(line);
    //
    m_categoryWidget = new MusicWebDJRadioCategoryWidget(this);
    connect(m_categoryWidget, SIGNAL(currentCategoryClicked(MusicResultDataItem)), SLOT(currentCategoryClicked(MusicResultDataItem)));
    layout->addWidget(m_categoryWidget);

    addWidget(w);
}
