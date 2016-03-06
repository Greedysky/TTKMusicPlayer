#include "musicwebradiotoolwidget.h"
#include "musicwebentainradiolistview.h"
#include "musicwebmusicradiolistview.h"

#include <QBoxLayout>
#include <QButtonGroup>

MusicWebRadioToolWidget::MusicWebRadioToolWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *toolWidget = new QWidget(this);
    toolWidget->setFixedHeight(40);
    toolWidget->setStyleSheet("background:rgba(255,255,255,50);");

    QHBoxLayout *toolLayout = new QHBoxLayout(toolWidget);
    toolLayout->setSpacing(0);
    toolLayout->setContentsMargins(0, 0, 0, 0);
    m_netRadioButton1 = new QPushButton(tr("entertainmentRadio"), toolWidget);
    m_netRadioButton2 = new QPushButton(tr("musicRadio"), toolWidget);
    m_netRadioButton1->setStyleSheet("border:none; background:url(':/radio/toolOn');");
    m_netRadioButton2->setStyleSheet("border:none; background:url(':/radio/toolOff');");
    toolLayout->addWidget(m_netRadioButton1);
    toolLayout->addWidget(m_netRadioButton2);
    toolWidget->setLayout(toolLayout);
    m_netRadioButton1->setFixedHeight(40);
    m_netRadioButton2->setFixedHeight(40);

    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(toolWidget);
    mainLayout->addWidget(m_stackedWidget);
    setLayout(mainLayout);

    m_musicRadioListView = new MusicWebMusicRadioListView(this);
    m_entainRadioListView = new MusicWebEntainRadioListView(this);
    m_stackedWidget->addWidget(m_entainRadioListView);
    m_stackedWidget->addWidget(m_musicRadioListView);
    m_stackedWidget->setCurrentIndex(0);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_netRadioButton1, 0);
    buttonGroup->addButton(m_netRadioButton2, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

}

MusicWebRadioToolWidget::~MusicWebRadioToolWidget()
{
    delete m_netRadioButton1;
    delete m_netRadioButton2;
    delete m_musicRadioListView;
    delete m_entainRadioListView;
    delete m_stackedWidget;
}

void MusicWebRadioToolWidget::buttonClicked(int index)
{
    QString on = "border:none; background:url(':/radio/toolOn');";
    QString off = "border:none; background:url(':/radio/toolOff');";
    if(index == 0)
    {
        m_netRadioButton1->setStyleSheet(on);
        m_netRadioButton2->setStyleSheet(off);
    }
    else
    {
        m_netRadioButton1->setStyleSheet(off);
        m_netRadioButton2->setStyleSheet(on);
        m_musicRadioListView->initListItems();
    }
    m_stackedWidget->setCurrentIndex(index);
}
