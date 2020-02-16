#include "musicscreensaverwidget.h"
#include "musicapplicationobject.h"
#include "musicsettingmanager.h"
#include "musicnumberdefine.h"
#include "musicuiobject.h"
#include "musictoolsetsuiobject.h"

#include <QEvent>
#include <QTimer>
#include <QBoxLayout>
#include <QApplication>

MusicScreenSaverWidget::MusicScreenSaverWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle17 + MusicUIObject::MQSSColorStyle09);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
    //
    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(50);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *pLabel = new QLabel(tr("Screen Saver "), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(MusicUIObject::MQSSColorStyle13);

    QLabel *iLabel = new QLabel(tr("Spend Your Leisure Time With You"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);

    QLabel *wLabel = new QLabel(tr("Wait"), topWidget);
    QLabel *mLabel = new QLabel(tr("Min"), topWidget);

    m_inputEdit = new QLineEdit(topWidget);
    m_inputEdit->setFixedWidth(50);
    m_inputEdit->setEnabled(false);
    m_inputEdit->setAlignment(Qt::AlignCenter);
    m_inputEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);

    m_caseButton = new QPushButton(topWidget);
    m_caseButton->setFixedSize(44, 20);
    m_caseButton->setCursor(Qt::PointingHandCursor);
    m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOff);

    topWidgetLayout->addWidget(pLabel);
    topWidgetLayout->addWidget(iLabel);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(wLabel);
    topWidgetLayout->addWidget(m_inputEdit);
    topWidgetLayout->addWidget(mLabel);
    topWidgetLayout->addWidget(m_caseButton);
    topWidget->setLayout(topWidgetLayout);
    mainLayout->addWidget(topWidget);
    //
    QFrame *frame = new QFrame(this);
    frame->setFixedHeight(1);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Plain);
    frame->setStyleSheet(MusicUIObject::MQSSColorStyle14);
    mainLayout->addWidget(frame);
    //
    QWidget *functionWidget = new QWidget(this);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(40, 10, 40, 10);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);

    connect(m_inputEdit, SIGNAL(textChanged(QString)), SLOT(inputDataChanged()));
    connect(m_caseButton, SIGNAL(clicked()), SLOT(caseButtonOnAndOff()));

    applySettingParameter();
}

MusicScreenSaverWidget::~MusicScreenSaverWidget()
{
    delete m_inputEdit;
    delete m_caseButton;
}

void MusicScreenSaverWidget::applySettingParameter()
{
    const bool state = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const int mins = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();

    m_inputEdit->setText(QString::number(mins));
    if(state)
    {
        caseButtonOnAndOff();
    }
}

void MusicScreenSaverWidget::inputDataChanged()
{
    const bool state = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    if(state)
    {
        M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
        MusicApplicationObject::instance()->applySettingParameter();
    }
}

void MusicScreenSaverWidget::caseButtonOnAndOff()
{
    const bool state = m_caseButton->styleSheet().contains(":/toolSets/btn_saver_off");
    if(state)
    {
        m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOn);
        M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
    }
    else
    {
        m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOff);
    }

    m_inputEdit->setEnabled(state);
    M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable, state);
    MusicApplicationObject::instance()->applySettingParameter();
}



MusicScreenSaverBackgroundWidget::MusicScreenSaverBackgroundWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    m_state = false;
    m_isRunning = false;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));

    setStyleSheet("background:blue");
    hide();

    qApp->installEventFilter(this);
}

MusicScreenSaverBackgroundWidget::~MusicScreenSaverBackgroundWidget()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    delete m_timer;
}

void MusicScreenSaverBackgroundWidget::applySettingParameter()
{
    m_state = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const QString &time = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toString();
    const int value = time.toInt();
    if(value != 0)
    {
        m_timer->setInterval(value * MT_M2MS);
    }
}

void MusicScreenSaverBackgroundWidget::timeout()
{
    m_isRunning = true;
    setParent(nullptr);
    showFullScreen();
}

bool MusicScreenSaverBackgroundWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()== QEvent::MouseButtonPress || event->type()== QEvent::MouseButtonRelease ||
       event->type()== QEvent::MouseButtonDblClick || event->type()== QEvent::MouseMove ||
       event->type()== QEvent::KeyPress || event->type()== QEvent::KeyRelease)
    {
        if(m_state)
        {
            if(m_isRunning)
            {
                m_isRunning = false;
                hide();
            }
            m_timer->stop();
            m_timer->start();
        }
        else
        {
            m_timer->stop();
        }
    }
    return QWidget::eventFilter(watched, event);
}
