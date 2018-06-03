#include "musiccloudmanagerwidget.h"
#include "musicuiobject.h"

MusicCloudManagerWidget::MusicCloudManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MBackgroundStyle17 + MusicUIObject::MColorStyle09);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *topWidget = new QWidget(this);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *pLabel = new QLabel(tr("Personal Cloud"), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(MusicUIObject::MColorStyle13);

    QLabel *iLabel = new QLabel(tr("Sharing Of Cloud Disk Songs"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);
    QLabel *sLabel = new QLabel(tr("Size"), topWidget);

    m_sizeValueBar = new QProgressBar(topWidget);
    m_sizeValueBar->setRange(0, 100);
    m_sizeValueBar->setValue(60);
    m_sizeValueBar->setStyleSheet(MusicUIObject::MProgressBar01);

    m_sizeValueLabel = new QLabel("10.5M/10.0G", topWidget);

    topWidgetLayout->addWidget(pLabel);
    topWidgetLayout->addWidget(iLabel);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(sLabel);
    topWidgetLayout->addWidget(m_sizeValueBar);
    topWidgetLayout->addWidget(m_sizeValueLabel);
    topWidget->setLayout(topWidgetLayout);
    mainLayout->addWidget(topWidget);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *functionWidget = new QWidget(this);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 10, 10, 10);

    QPushButton *uploadButton = new QPushButton(tr("Upload"), functionWidget);
    uploadButton->setFixedSize(70, 30);
    uploadButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    uploadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *downloadButton = new QPushButton(tr("Download"), functionWidget);
    downloadButton->setFixedSize(70, 30);
    downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *deleteButton = new QPushButton(tr("Delete"), functionWidget);
    deleteButton->setFixedSize(70, 30);
    deleteButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    deleteButton->setCursor(QCursor(Qt::PointingHandCursor));

    functionWidgetLayout->addWidget(uploadButton);
    functionWidgetLayout->addWidget(downloadButton);
    functionWidgetLayout->addWidget(deleteButton);
    functionWidgetLayout->addStretch(1);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);
    mainLayout->addStretch(1);
    ////////////////////////////////////////////////////////////////////////////
}

MusicCloudManagerWidget::~MusicCloudManagerWidget()
{
    delete m_sizeValueLabel;
    delete m_sizeValueBar;
}
