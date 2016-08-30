#include "musicidentifysongswidget.h"
#include "musictoolsetsuiobject.h"
#include "musicuiobject.h"

#include <QMovie>
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
    delete m_detectedLabel;
    delete m_detectedMovie;
    delete m_mainWindow;
}

void MusicIdentifySongsWidget::detectedButtonClicked()
{
    if(m_detectedButton->styleSheet().contains(MusicUIObject::MKGSongsDetectStartBtn))
    {
        m_detectedMovie->start();
        m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStopBtn);
    }
    else
    {
        m_detectedMovie->stop();
        m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStartBtn);
    }
}

void MusicIdentifySongsWidget::createDetectedWidget()
{
    QWidget *widget = new QWidget(m_mainWindow);
    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);

    m_detectedMovie = new QMovie(":/toolSets/lb_radar", QByteArray(), widget);
    QLabel *iconLabel = new QLabel(widget);
    iconLabel->setFixedSize(200, 200);
    QVBoxLayout *iconLabelLayout = new QVBoxLayout(iconLabel);
    iconLabelLayout->setSpacing(0);
    iconLabelLayout->setContentsMargins(0, 0, 0, 0);
    m_detectedButton = new QPushButton(widget);
    m_detectedButton->setStyleSheet(MusicUIObject::MKGSongsDetectStartBtn);
    m_detectedButton->setFixedSize(162, 162);
    iconLabelLayout->addWidget(m_detectedButton, 0, Qt::AlignCenter);
    iconLabel->setMovie(m_detectedMovie);
    iconLabel->setLayout(iconLabelLayout);
    connect(m_detectedButton, SIGNAL(clicked()), SLOT(detectedButtonClicked()));

    m_detectedLabel = new QLabel(widget);
    m_detectedLabel->setText(tr("detecting now!"));

    widgetLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    widgetLayout->addWidget(m_detectedLabel, 0, Qt::AlignCenter);
    widget->setLayout(widgetLayout);

    m_mainWindow->addWidget(widget);
    m_mainWindow->setCurrentWidget(widget);
}

void MusicIdentifySongsWidget::createDetectedSuccessedWidget()
{

}

void MusicIdentifySongsWidget::createDetectedFailedWidget()
{

}
