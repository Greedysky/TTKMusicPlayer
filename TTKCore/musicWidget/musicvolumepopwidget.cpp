#include "musicvolumepopwidget.h"
#include "musickugouuiobject.h"

#include <QSlider>
#include <QHBoxLayout>

MusicVolumePopWidget::MusicVolumePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();

    m_menuShown = false;

    disconnect(this, SIGNAL(clicked()), this, SLOT(popupMenu()));
}

MusicVolumePopWidget::~MusicVolumePopWidget()
{
    delete m_volumeSlider;
}

QString MusicVolumePopWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicVolumePopWidget::setValue(int value)
{
    m_volumeSlider->blockSignals(true);
    m_volumeSlider->setValue(value);
    m_volumeSlider->blockSignals(false);

    QString style = MusicKuGouUIObject::MKGBtnSound;
    if(66 < value && value <=100)
    {
        style += "QToolButton{ margin-left:-60px; }";
    }
    else if(33 < value && value <=66)
    {
        style += "QToolButton{ margin-left:-40px; }";
    }
    else if(0 < value && value <=33)
    {
        style += "QToolButton{ margin-left:-20px; }";
    }
    else
    {
        style += "QToolButton{ margin-left:-80px; }";
    }
    setStyleSheet(style);
}

int MusicVolumePopWidget::value() const
{
    return m_volumeSlider->value();
}

void MusicVolumePopWidget::enterEvent(QEvent *event)
{
    MusicToolMenuWidget::enterEvent(event);
    if(!m_menuShown)
    {
        m_menuShown = true;
        popupMenu();
    }
}

void MusicVolumePopWidget::leaveEvent(QEvent *event)
{
    MusicToolMenuWidget::leaveEvent(event);
    m_menuShown = false;
    m_menu->close();
}

void MusicVolumePopWidget::initWidget()
{
    m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint);
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    QString MMenuStyle = " \
            QMenu {color: rgb(150,150,150); border:none; padding: 5px; background:rgba(0,0,0,240);}\
            QMenu::item {padding: 4px 25px 4px 30px;border: 1px solid transparent; }\
            QMenu::item:disabled {color: #666666;}\
            QMenu::item:selected { color: white; background:rgba(0,0,0,200);}\
            QMenu::separator {height: 1px;background: #BBBBBB;margin-top: 5px; margin-bottom: 5px;}";

    m_menu->setStyleSheet(MMenuStyle);

    m_containWidget->setFixedSize(30, 100);
    QHBoxLayout *layout = new QHBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_volumeSlider = new QSlider(Qt::Vertical, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(100);
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SIGNAL(musicVolumeChanged(int)));

    layout->addWidget(m_volumeSlider);
    m_containWidget->setLayout(layout);

}
