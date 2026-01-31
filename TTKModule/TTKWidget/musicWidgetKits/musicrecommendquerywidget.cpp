#include "musicrecommendquerywidget.h"
#include "musicsongrecommendrequest.h"
#include "musicitemquerytablewidget.h"
#include "musicimageutils.h"

MusicRecommendQueryWidget::MusicRecommendQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_tableWidget = new MusicItemQueryTableWidget(this);
    m_tableWidget->hide();
}

void MusicRecommendQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_tableWidget->setQueryInput(new MusicSongRecommendRequest(this));
    m_tableWidget->startToSearchByValue(TTK::generateSongTitle(value));
    createLabels();
}

void MusicRecommendQueryWidget::resizeGeometry()
{
    m_tableWidget->resizeGeometry();
}

void MusicRecommendQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    createFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWidget);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWidget);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> Recommend Music</font>"));
    grid->addWidget(firstLabel);
    QLabel *iconLabel = new QLabel(function);
    grid->addWidget(iconLabel);
    //
    grid->addWidget(m_container);

    QPixmap pix(":/image/lb_recmd_daily");
    const qint64 time = TTKDateTime::currentTimestamp();

    QPainter painter(&pix);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(54, 34, QPixmap(":/image/lb_recmd_date"));

    painter.setFont(QFont("Arial", 9));
    painter.setPen(QColor(0xFE, 0xD9, 0xD9));
    painter.drawText(QRect(54, 34, 115, 27), Qt::AlignCenter, TTKDateTime::format(time, TTK_WEEK_FORMAT));

    painter.setFont(QFont("Arial", 55, QFont::Bold));
    painter.setPen(QColor(0x20, 0x20, 0x20));
    painter.drawText(QRect(54, 27 + 34, 115, 115 - 27), Qt::AlignCenter, TTKDateTime::format(time, TTK_DAY_FORMAT));

    painter.drawPixmap(54, 34, QPixmap(":/image/lb_recmd_date_mask"));
    iconLabel->setPixmap(pix);

    m_mainWidget->layout()->addWidget(function);
}
