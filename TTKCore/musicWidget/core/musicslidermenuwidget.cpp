#include "musicslidermenuwidget.h"
#include "musicuiobject.h"

#include <QLabel>
#include <QBoxLayout>

MusicSliderMenuWidget::MusicSliderMenuWidget(QWidget *parent)
    : MusicSliderMenuWidget(Qt::Vertical, parent)
{

}

MusicSliderMenuWidget::MusicSliderMenuWidget(Qt::Orientation orientation, QWidget *parent)
    : QMenu(parent)
{
    setStyleSheet(MusicUIObject::MMenuStyle02);
    m_widgetAction = new QWidgetAction(this);

    QWidget *widgetActionContainer = new QWidget(this);
    widgetActionContainer->setStyleSheet("background-color:white;");

    m_textLabel = new QLabel(widgetActionContainer);
    m_textLabel->setStyleSheet("color: rgb(230, 115, 0);");
    m_slider = new QSlider(orientation, widgetActionContainer);

    QBoxLayout *hbox = nullptr;
    if(orientation == Qt::Horizontal)
    {
        hbox = new QVBoxLayout(widgetActionContainer);
        m_textLabel->setText("0%      50%     100%");
        m_slider->setStyleSheet(MusicUIObject::MSliderStyle01);
    }
    else
    {
        hbox = new QHBoxLayout(widgetActionContainer);
        m_textLabel->setText("100%\n\n\n50%\n\n\n0%");
        m_slider->setStyleSheet(MusicUIObject::MSliderStyle02);
    }
    m_slider->setRange(0, 100);
    m_slider->setValue(100);
    m_slider->setCursor(QCursor(Qt::PointingHandCursor));
    hbox->setContentsMargins(0, 0, 0, 0);

    hbox->addWidget(m_textLabel);
    hbox->addWidget(m_slider);
    widgetActionContainer->setLayout(hbox);
    m_widgetAction->setDefaultWidget(widgetActionContainer);

    addAction(m_widgetAction);
    connect(m_slider, SIGNAL(valueChanged(int)), SIGNAL(valueChanged(int)));
}

MusicSliderMenuWidget::~MusicSliderMenuWidget()
{
    delete m_textLabel;
    delete m_slider;
    delete m_widgetAction;
}

QString MusicSliderMenuWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSliderMenuWidget::setValue(int value)
{
    m_slider->setValue(value);
}

int MusicSliderMenuWidget::getValue() const
{
    return m_slider->value();
}

void MusicSliderMenuWidget::setText(const QString &text)
{
    m_textLabel->setText(text);
}

QString MusicSliderMenuWidget::getText() const
{
    return m_textLabel->text();
}

void MusicSliderMenuWidget::showTextLabel(bool show)
{
    QHBoxLayout *ly = MStatic_cast(QHBoxLayout*, m_widgetAction->defaultWidget()->layout());
    if(!show)
    {
        ly->removeWidget(m_textLabel);
        m_textLabel->hide();
    }
    else
    {
        if(m_textLabel->isHidden())
        {
            ly->insertWidget(0, m_textLabel);
            m_textLabel->show();
        }
    }
}
