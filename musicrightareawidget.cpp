#include "musicrightareawidget.h"

MusicRightAreaWidget::MusicRightAreaWidget(QWidget *parent)
    : QWidget(parent)
{

}

void MusicRightAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;


    connect(ui->musicSearchRefreshButton,SIGNAL(clicked()), parent(), SLOT(musicSearchRefreshButtonRefreshed()));
}

