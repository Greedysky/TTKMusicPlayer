#include "musicwebdjradiowidget.h"
#include "musicuiobject.h"
#include "musicwebdjradiocategorywidget.h"
#include "musicwebdjradiofoundwidget.h"
#include "musicdownloadsourcethread.h"
#include "musicsettingmanager.h"
#include "musicwidgetheaders.h"

MusicWebDJRadioProgramTableWidget::MusicWebDJRadioProgramTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setIconSize(QSize(ITEM_ROW_HEIGHT_L, ITEM_ROW_HEIGHT_L));
    setColumnCount(6);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 40);
    headerview->resizeSection(2, 315);
    headerview->resizeSection(3, 90);
    headerview->resizeSection(4, 105);
    headerview->resizeSection(5, 110);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    m_programThread = new MusicDJRadioProgramThread(this);
    connect(m_programThread, SIGNAL(createProgramItem(MusicResultsItem)), SLOT(createProgramItem(MusicResultsItem)));
}

MusicWebDJRadioProgramTableWidget::~MusicWebDJRadioProgramTableWidget()
{
    delete m_programThread;
}

void MusicWebDJRadioProgramTableWidget::init(MusicObject::Program type)
{
    m_programThread->startToDownload(type);
}

void MusicWebDJRadioProgramTableWidget::resizeWindow()
{
    const int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(2, width - WINDOW_WIDTH_MIN + 315);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 2);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    }
}

void MusicWebDJRadioProgramTableWidget::listCellEntered(int row, int column)
{
    if(column == 2 || column == 3)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }

    MusicAbstractTableWidget::listCellEntered(row, column);
}

void MusicWebDJRadioProgramTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    MusicResultsItems items(m_programThread->getSearchedItems());
    if(row < 0 || row >= items.count())
    {
        return;
    }

    switch(column)
    {
        case 2: emit programItemClicked(items[row].m_id, QString("-1")); break;
        case 3: emit programItemClicked(QString("-1"), items[row].m_nickName); break;
        default:break;
    }
}

void MusicWebDJRadioProgramTableWidget::createProgramItem(const MusicResultsItem &data)
{
    const int index = rowCount();
    setRowCount(index + 1);
    setRowHeight(index, ITEM_ROW_HEIGHT_L);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setIcon(QPixmap(":/image/lb_defaultArt"));
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(data.m_name);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 31));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 2, item);

                      item = new QTableWidgetItem(data.m_tags);
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(index, 3, item);

                      item = new QTableWidgetItem(tr("Played %1").arg(data.m_playCount));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 4, item);

                      item = new QTableWidgetItem(data.m_updateTime);
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(index, 5, item);

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadExtDataChanged(QByteArray,QVariantMap)), SLOT(downLoadFinished(QByteArray,QVariantMap)));
    if(!data.m_coverUrl.isEmpty() && data.m_coverUrl != COVER_URL_NULL)
    {
        QVariantMap map;
        map["id"] = index;
        download->setRawData(map);
        download->startToDownload(data.m_coverUrl);
    }
}

void MusicWebDJRadioProgramTableWidget::downLoadFinished(const QByteArray &data, const QVariantMap &ext)
{
    QTableWidgetItem *it = item(ext["id"].toInt(), 1);
    if(it)
    {
        QPixmap pix;
        pix.loadFromData(data);
        it->setIcon(pix);
    }
}



MusicWebDJRadioProgramWidget::MusicWebDJRadioProgramWidget(MusicObject::Program type, QWidget *parent)
    : QWidget(parent), m_type(type)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QWidget *top = new QWidget(this);
    layout->addWidget(top);
    QHBoxLayout *topLayout = new QHBoxLayout(top);
    topLayout->setContentsMargins(10, 20, 10, 0);
    top->setLayout(topLayout);

    QLabel *label = new QLabel(type == MusicObject::Recommed ? tr("Recommend") : tr("Program"), this);
    QFont font = label->font();
    font.setPixelSize(20);
    font.setBold(true);
    label->setFont(font);
    topLayout->addWidget(label);

    QPushButton *backButton = new QPushButton(tr("Back"), this);
    backButton->setFixedSize(90, 30);
    backButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    backButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(backButton, SIGNAL(clicked()), parent, SLOT(backToMainMenu()));
    topLayout->addWidget(backButton);
    layout->addWidget(top);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color:red");
    layout->addWidget(line);

    m_tableWidget = new MusicWebDJRadioProgramTableWidget(this);
    connect(m_tableWidget, SIGNAL(programItemClicked(QString,QString)), parent, SLOT(programItemClicked(QString,QString)));
    layout->addWidget(m_tableWidget);
}

MusicWebDJRadioProgramWidget::~MusicWebDJRadioProgramWidget()
{
    delete m_tableWidget;
}

void MusicWebDJRadioProgramWidget::init()
{
    m_tableWidget->init(m_type);
}

void MusicWebDJRadioProgramWidget::resizeWindow()
{
    m_tableWidget->resizeWindow();
}



MusicWebDJRadioWidget::MusicWebDJRadioWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    setObjectName("MainWindow");
    setStyleSheet(QString("#MainWindow{%1}").arg(MusicUIObject::MBackgroundStyle17));

    m_recommendWidget = nullptr;
    m_programWidget = nullptr;
    m_foundWidget = nullptr;

    initFirstWidget();
}

MusicWebDJRadioWidget::~MusicWebDJRadioWidget()
{
    delete m_recommendWidget;
    delete m_programWidget;
    delete m_categoryWidget;
    delete m_foundWidget;
}

void MusicWebDJRadioWidget::init()
{
    m_categoryWidget->init();
}

void MusicWebDJRadioWidget::resizeWindow()
{
    if(m_categoryWidget)
    {
        m_categoryWidget->resizeWindow();
    }

    if(m_foundWidget)
    {
        m_foundWidget->resizeWindow();
    }

    if(m_recommendWidget)
    {
        m_recommendWidget->resizeWindow();
    }

    if(m_programWidget)
    {
        m_programWidget->resizeWindow();
    }
}

void MusicWebDJRadioWidget::backToMainMenu()
{
    setCurrentIndex(0);
}

void MusicWebDJRadioWidget::createRecommendWidget()
{
    delete m_recommendWidget;
    m_recommendWidget = new MusicWebDJRadioProgramWidget(MusicObject::Recommed, this);
    m_recommendWidget->init();
    addWidget(m_recommendWidget);
    setCurrentWidget(m_recommendWidget);
}

void MusicWebDJRadioWidget::createProgramWidget()
{
    delete m_programWidget;
    m_programWidget = new MusicWebDJRadioProgramWidget(MusicObject::Rank, this);
    m_programWidget->init();
    addWidget(m_programWidget);
    setCurrentWidget(m_programWidget);
}

void MusicWebDJRadioWidget::programItemClicked(const QString &rid, const QString &cid)
{
    delete m_foundWidget;
    m_foundWidget = new MusicWebDJRadioFoundWidget(this);
    connect(m_foundWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenu()));

    if(rid == "-1" && cid != "-1")
    {
        m_foundWidget->setSongName(cid);
    }
    else
    {
        m_foundWidget->setSongNameById(rid);
    }

    addWidget(m_foundWidget);
    setCurrentWidget(m_foundWidget);
}

void MusicWebDJRadioWidget::currentCategoryClicked(const MusicResultsItem &item)
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
    MusicClickedLabel *recommendLabel = new MusicClickedLabel(tr("Recommend"), leftTop);
    QFont recommendFont = recommendLabel->font();
    recommendFont.setPixelSize(20);
    recommendFont.setBold(true);
    recommendLabel->setFont(recommendFont);
    leftTopLayout->addWidget(recommendLabel);
    leftTopLayout->addStretch(1);
    connect(recommendLabel, SIGNAL(clicked()), SLOT(createRecommendWidget()));

    MusicClickedLabel *recommendMoreLabel = new MusicClickedLabel(tr("More >"), leftTop);
    recommendMoreLabel->setStyleSheet(MusicUIObject::MColorStyle04);
    connect(recommendMoreLabel, SIGNAL(clicked()), SLOT(createRecommendWidget()));
    leftTopLayout->addWidget(recommendMoreLabel);
    topLayout->addWidget(leftTop);
    //
    QWidget *rightTop = new QWidget(top);
    QHBoxLayout *rightTopLayout = new QHBoxLayout(rightTop);
    rightTopLayout->setSpacing(0);
    rightTopLayout->setContentsMargins(0, 0, 0, 0);
    MusicClickedLabel *programLabel = new MusicClickedLabel(tr("Program"), rightTop);
    QFont programFont = programLabel->font();
    programFont.setPixelSize(20);
    programFont.setBold(true);
    programLabel->setFont(programFont);
    rightTopLayout->addWidget(programLabel);
    rightTopLayout->addStretch(1);
    connect(programLabel, SIGNAL(clicked()), SLOT(createProgramWidget()));

    MusicClickedLabel *programMoreLabel = new MusicClickedLabel(tr("More >"), rightTop);
    programMoreLabel->setStyleSheet(MusicUIObject::MColorStyle04);
    connect(programMoreLabel, SIGNAL(clicked()), SLOT(createProgramWidget()));
    rightTopLayout->addWidget(programMoreLabel);
    topLayout->addWidget(rightTop);
    //
    QFrame *line = new QFrame(w);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color:red");
    layout->addWidget(line);
    //
    m_categoryWidget = new MusicWebDJRadioCategoryWidget(this);
    connect(m_categoryWidget, SIGNAL(currentCategoryClicked(MusicResultsItem)), SLOT(currentCategoryClicked(MusicResultsItem)));
    layout->addWidget(m_categoryWidget);

    addWidget(w);
}

void MusicWebDJRadioWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
