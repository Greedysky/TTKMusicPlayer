#include "musicwebmvradioinfowidget.h"
#include "musicmvradioprogramrequest.h"
#include "musiccoversourcerequest.h"
#include "musicdownloadwidget.h"
#include "musicrightareawidget.h"

Q_DECLARE_METATYPE(TTK::MusicSongInformation)

MusicWebMVRadioInfoTableWidget::MusicWebMVRadioInfoTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicWebMVRadioInfoTableWidget::~MusicWebMVRadioInfoTableWidget()
{
    removeItems();
}

void MusicWebMVRadioInfoTableWidget::itemCellClicked(int row, int column)
{
    MusicQueryTableWidget::itemCellClicked(row, column);

    switch(column)
    {
        case 5:
        case 6:
        {
            const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
            if(!isValid(row) || row >= songInfos.count())
            {
                return;
            }

            MusicRightAreaWidget::instance()->showMovieRadioFound(QVariant::fromValue<TTK::MusicSongInformation>(songInfos[row]));
            break;
        }
        case 7: downloadQueryResult(row); break;
        default: break;
    }
}

void MusicWebMVRadioInfoTableWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    itemCellClicked(row, 5);
}

void MusicWebMVRadioInfoTableWidget::actionChanged(QAction *action)
{
    const int row = currentRow();
    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
    if(!isValid(row) || row >= songInfos.count())
    {
        return;
    }

    switch(action->data().toInt())
    {
        case 0: itemCellClicked(row, 6); break;
        case 1: itemCellClicked(row, 7); break;
        default: break;
    }
}

void MusicWebMVRadioInfoTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);

    QMenu menu;
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    const int row = currentRow();
    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
    if(!isValid(row) || row >= songInfos.count())
    {
        return;
    }

    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"))->setData(0);
    menu.addAction(tr("Download More..."))->setData(1);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(actionChanged(QAction*)));

    menu.exec(QCursor::pos());
}



MusicWebMVRadioInfoWidget::MusicWebMVRadioInfoWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_tableWidget = new MusicWebMVRadioInfoTableWidget(this);
    m_tableWidget->hide();

    MusicAbstractQueryRequest *req = new MusicMVRadioProgramRequest(this);
    m_tableWidget->setQueryInput(req);

    connect(req, SIGNAL(downloadDataChanged(QString)), SLOT(queryAllFinished()));
    connect(req, SIGNAL(createMVRadioItem(MusicResultDataItem)), SLOT(createMVRadioProgramItem(MusicResultDataItem)));
}

void MusicWebMVRadioInfoWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_tableWidget->startToSearchByValue(value);
}

void MusicWebMVRadioInfoWidget::resizeGeometry()
{
    m_tableWidget->resizeGeometry();

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 390;

    Data *data = &m_resizeWidgets[0];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[1];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
}

void MusicWebMVRadioInfoWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicWebMVRadioInfoWidget::createMVRadioProgramItem(const MusicResultDataItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *req = new MusicCoverSourceRequest(this);
        connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        req->startToRequest(item.m_coverUrl);
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 390;

    Data *data = &m_resizeWidgets[0];
    data->m_label->setToolTip(item.m_name);
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[1];
    data->m_label->setToolTip(tr("Type: %1").arg(item.m_nickName));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
}

void MusicWebMVRadioInfoWidget::downloadMVsButtonClicked()
{
    m_tableWidget->downloadBatchData();
}

void MusicWebMVRadioInfoWidget::createLabels()
{
    createThirdWidget();
    m_container->show();

    layout()->removeWidget(m_mainWidget);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWidget);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QWidget *firstTopFuncWidget = new QWidget(function);
    QHBoxLayout *firstTopFuncLayout = new QHBoxLayout(firstTopFuncWidget);
    firstTopFuncLayout->setContentsMargins(9, 9, 0, 9);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> MVRadio > %1 </font>").arg(m_currentPlaylistItem.m_name));

    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(90, 30);
    backButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    backButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(backButton, SIGNAL(clicked()), this, SIGNAL(backToMainMenu()));

    firstTopFuncLayout->addWidget(firstLabel);
    firstTopFuncLayout->addWidget(backButton);
    grid->addWidget(firstTopFuncWidget);
    //
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(210, 180);
    //
    QWidget *topLineWidget = new QWidget(topFuncWidget);
    QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
    topLineLayout->setContentsMargins(10, 5, 5, 0);

    QLabel *nameLabel = new QLabel(topLineWidget);
    QFont nameFont = nameLabel->font();
    nameFont.setPixelSize(20);
    nameLabel->setFont(nameFont);
    nameLabel->setStyleSheet(TTK::UI::FontStyle01);
    nameLabel->setText(TTK_DEFAULT_STR);

    QLabel *typeLabel = new QLabel(topLineWidget);
    typeLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    typeLabel->setText(TTK_DEFAULT_STR);

    topLineLayout->addWidget(nameLabel);
    topLineLayout->addStretch(1);
    topLineLayout->addWidget(typeLabel);
    topLineLayout->addStretch(5);
    topLineWidget->setLayout(topLineLayout);

    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    //
    QWidget *functionWidget = new QWidget(this);
    functionWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
    QHBoxLayout *hLayout = new QHBoxLayout(functionWidget);

    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("SongItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));

    hLayout->addWidget(m_songButton);
    hLayout->addStretch(1);
    functionWidget->setLayout(hLayout);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_songButton, 0);
    QtButtonGroupConnect(buttonGroup, m_container, setCurrentIndex, TTK_SLOT);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWidget->layout()->addWidget(function);

    m_resizeWidgets.append({nameLabel, nameLabel->font()});
    m_resizeWidgets.append({typeLabel, typeLabel->font()});
}

void MusicWebMVRadioInfoWidget::createThirdWidget()
{
    QWidget *songWidget = new QWidget(this);
    QVBoxLayout *vLayout = new QVBoxLayout(songWidget);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *middleFuncWidget = new QWidget(songWidget);
    middleFuncWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 5, 0, 5);

    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    QCheckBox *allCheckBox = new QCheckBox(TTK_SPACE + tr("All"), middleFuncWidget);
    QPushButton *downloadButton = new QPushButton(tr("Download"), middleFuncWidget);
    downloadButton->setFixedSize(55, 25);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    allCheckBox->setFocusPolicy(Qt::NoFocus);
    downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    middleFuncLayout->addWidget(marginLabel);
    middleFuncLayout->addWidget(allCheckBox);
    middleFuncLayout->addStretch(1);
    middleFuncLayout->addWidget(downloadButton);
    connect(allCheckBox, SIGNAL(clicked(bool)), m_tableWidget, SLOT(checkedItemsState(bool)));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadMVsButtonClicked()));

    vLayout->addWidget(middleFuncWidget);
    vLayout->addWidget(m_tableWidget);
    vLayout->addStretch(1);
    songWidget->setLayout(vLayout);

    m_tableWidget->show();
    m_container->addWidget(songWidget);
}
