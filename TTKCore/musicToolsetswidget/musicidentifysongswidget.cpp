#include "musicidentifysongswidget.h"
#include "musictoolsetsuiobject.h"
#include "musicuiobject.h"

#include <QMovie>
#include <QTimer>
#include <QPainter>
#include <QBoxLayout>
#include <QStackedWidget>

MusicIdentifySongsWidget::MusicIdentifySongsWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    m_mainWindow = new QStackedWidget(this);
    m_mainWindow->setObjectName("MainWindow");
    m_mainWindow->setStyleSheet(QString("#MainWindow{%1}").arg(MusicUIObject::MBackgroundStyle17));
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    createDetectedWidget();
}

MusicIdentifySongsWidget::~MusicIdentifySongsWidget()
{
    delete m_detectedButton;
    delete m_detectedLabel;
    delete m_detectedMovie;
    delete m_mainWindow;
}

QString MusicIdentifySongsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicIdentifySongsWidget::detectedButtonClicked()
{
    if(m_detectedButton->styleSheet().contains(MusicUIObject::MKGSongsDetectStartBtn))
    {
        m_detectedMovie->start();
        m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStopBtn);
        m_detectedLabel->setText(tr("Recogniting the music being played"));
        QTimer::singleShot(3000, this, SLOT(detectedTimeOut()));
    }
    else
    {
        m_detectedMovie->stop();
        m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStartBtn);
        m_detectedLabel->setText(tr("Intelligent Recognition Of The Music Being Played"));
    }
}

void MusicIdentifySongsWidget::detectedTimeOut()
{
    m_detectedMovie->stop();
    createDetectedFailedWidget();
}

void MusicIdentifySongsWidget::createDetectedWidget()
{
    QWidget *widget = new QWidget(m_mainWindow);
    widget->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MCustomStyle04);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);

    m_detectedMovie = new QMovie(":/toolSets/lb_radar", QByteArray(), widget);
    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setFixedSize(200, 200);
    QVBoxLayout *iconLabelLayout = new QVBoxLayout(iconLabel);
    iconLabelLayout->setSpacing(0);
    iconLabelLayout->setContentsMargins(0, 0, 0, 0);
    m_detectedButton = new QPushButton(widget);
    m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStartBtn);
    m_detectedButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_detectedButton->setFixedSize(162, 162);
    iconLabelLayout->addWidget(m_detectedButton, 0, Qt::AlignCenter);
    iconLabel->setMovie(m_detectedMovie);
    iconLabel->setLayout(iconLabelLayout);
    connect(m_detectedButton, SIGNAL(clicked()), SLOT(detectedButtonClicked()));

    m_detectedLabel = new QLabel(widget);
    m_detectedLabel->setText(tr("Intelligent Recognition Of The Music Being Played"));

    QLabel *text = new QLabel(tr("ShotCut: AA + AA + A"), widget);
    text->setStyleSheet(MusicUIObject::MCustomStyle02);

    widgetLayout->addStretch(2);
    widgetLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(m_detectedLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(text, 0, Qt::AlignCenter);
    widget->setLayout(widgetLayout);

    m_mainWindow->addWidget(widget);
    m_mainWindow->setCurrentWidget(widget);
}

void MusicIdentifySongsWidget::createDetectedSuccessedWidget()
{

}

void MusicIdentifySongsWidget::createDetectedFailedWidget()
{
    QWidget *widget = new QWidget(m_mainWindow);
    widget->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MCustomStyle04);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);

    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setPixmap(QPixmap(":/toolSets/lb_detect_error"));
    QLabel *text1Label = new QLabel(tr("No Songs Identified"), widget);
    QLabel *text2Label = new QLabel(tr("Only The Music Being Played Can Be Recognized"), widget);
    QLabel *text3Label = new QLabel(tr("Redetect"), widget);

    QPushButton *reDetect = new QPushButton(widget);
    reDetect->setFixedSize(56, 56);
    reDetect->setStyleSheet(MusicUIObject::MKGSongsRedetectBtn);
    reDetect->setCursor(QCursor(Qt::PointingHandCursor));
    connect(reDetect, &QPushButton::clicked, this, [&]()
    {
        m_mainWindow->setCurrentIndex(0);
    });

    widgetLayout->addStretch(2);
    widgetLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(text1Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widgetLayout->addWidget(text2Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(2);
    widgetLayout->addWidget(reDetect, 0, Qt::AlignCenter);
    widgetLayout->addWidget(text3Label, 0, Qt::AlignCenter);
    widgetLayout->addStretch(1);
    widget->setLayout(widgetLayout);

    m_mainWindow->addWidget(widget);
    m_mainWindow->setCurrentWidget(widget);
}
