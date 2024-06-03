#include "musicpersonalradioquerywidget.h"
#include "musicwyqueryplaylistrequest.h"
#include "musicdownloadqueryfactory.h"
#include "musiccoverrequest.h"

MusicPersonalRadioQueryWidget::MusicPersonalRadioQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_queryTableWidget = new MusicItemQueryTableWidget(this);

    initFirstWidget();
}

void MusicPersonalRadioQueryWidget::setCurrentValue(const QString &value)
{
    MusicQueryPlaylistRequest *d = new MusicWYQueryPlaylistRequest(this);
    m_queryTableWidget->setQueryInput(d);

    MusicResultDataItem item;
    item.m_id = value;
    d->startToQueryInfo(item);
    setResultDataItem(item);

    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

void MusicPersonalRadioQueryWidget::resizeWidget()
{
    m_queryTableWidget->resizeSection();

    if(!m_resizeWidgets.isEmpty())
    {
        const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN;

        TTKResizeWidget *data = &m_resizeWidgets[0];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 200 + width));

        data = &m_resizeWidgets[1];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 180 + width));

        data = &m_resizeWidgets[2];
        data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, 320 + width));
    }
}

void MusicPersonalRadioQueryWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicPersonalRadioQueryWidget::setResultDataItem(const MusicResultDataItem &item)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_container->show();

    m_currentPlaylistItem = item;
    MusicAbstractItemQueryWidget::setCurrentValue(item.m_id);

    m_queryTableWidget->startToSearchByText(item.m_id);

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01);
    QVBoxLayout *grid = new QVBoxLayout(function);
    //
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(210, 180);

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *d = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->startToRequest(item.m_coverUrl);
    }

    QWidget *topLineWidget = new QWidget(topFuncWidget);
    QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
    topLineLayout->setContentsMargins(10, 5, 0, 0);

    QLabel *nameLabel = new QLabel(topLineWidget);
    QFont nameFont = nameLabel->font();
    nameFont.setPixelSize(20);
    nameLabel->setFont(nameFont);
    nameLabel->setStyleSheet(TTK::UI::FontStyle01);
    nameLabel->setToolTip(item.m_title);

    QLabel *creatorLabel = new QLabel(topLineWidget);
    creatorLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    creatorLabel->setToolTip(tr("PlayCount: %1").arg(item.m_count));

    QLabel *descriptionLabel = new QLabel(topLineWidget);
    descriptionLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    descriptionLabel->setToolTip(tr("Description: %1").arg(item.m_description));

    topLineLayout->addWidget(nameLabel);
    topLineLayout->addWidget(creatorLabel);
    topLineLayout->addWidget(descriptionLabel);
    topLineWidget->setLayout(topLineLayout);

    QWidget *topButtonWidget = new QWidget(topFuncWidget);
    topButtonWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
    QHBoxLayout *topButtonLayout = new QHBoxLayout(topButtonWidget);
    topButtonLayout->setContentsMargins(0, 0, 0, 0);

    topButtonLayout->addStretch(1);
    topButtonWidget->setLayout(topButtonLayout);
    topLineLayout->addWidget(topButtonWidget);
    //
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

#ifdef Q_OS_UNIX
    m_songButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets.push_back({nameLabel, nameLabel->font()});
    m_resizeWidgets.push_back({creatorLabel, creatorLabel->font()});
    m_resizeWidgets.push_back({descriptionLabel, descriptionLabel->font()});

    resizeWidget();
}
