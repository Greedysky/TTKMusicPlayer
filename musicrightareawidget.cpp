#include "musicrightareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent)
{

}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;

    ui->musicSearchRefreshButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchRefreshButton->setStyleSheet(MusicUIObject::MusicMainFunctionButtonForStackWidget);
    ui->musicSearchRefreshButton->setIconSize(QSize(25,25));
    ui->musicSearchRefreshButton->setIcon(QIcon(QString::fromUtf8(":/image/flash")));
    connect(ui->musicSearchRefreshButton,SIGNAL(clicked()), parent(),
            SLOT(musicSearchRefreshButtonRefreshed()));

    ui->musicIndexWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicIndexWidgetButton->setStyleSheet(MusicUIObject::MusicMainFunctionButtonForStackWidget);
    connect(ui->musicIndexWidgetButton,SIGNAL(clicked()), parent(),SLOT(musicIndexWidgetButtonSearched()));

    ui->musicSearchWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchWidgetButton->setStyleSheet(MusicUIObject::MusicMainFunctionButtonForStackWidget);
    connect(ui->musicSearchWidgetButton,SIGNAL(clicked()), parent(),SLOT(musicSearchWidgetButtonSearched()));

    ui->musicLrcWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicLrcWidgetButton->setStyleSheet(MusicUIObject::MusicMainFunctionButtonForStackWidget);
    connect(ui->musicLrcWidgetButton,SIGNAL(clicked()), parent(),SLOT(musicLrcWidgetButtonSearched()));

    ui->vedioWidgetButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->vedioWidgetButton->setStyleSheet(MusicUIObject::MusicMainFunctionButtonForStackWidget);
    connect(ui->vedioWidgetButton,SIGNAL(clicked()), parent(),SLOT(musicVedioWidgetButtonSearched()));
}

