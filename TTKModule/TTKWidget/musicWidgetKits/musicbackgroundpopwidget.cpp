#include "musicbackgroundpopwidget.h"
#include "musicuiobject.h"
#include "musicclickedslider.h"
#include "musicwidgetheaders.h"

MusicBackgroundPopWidget::MusicBackgroundPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

MusicBackgroundPopWidget::~MusicBackgroundPopWidget()
{
    delete m_slider;
}

void MusicBackgroundPopWidget::setValue(int value)
{
    m_slider->setValue(value);
}

int MusicBackgroundPopWidget::value() const
{
    return m_slider->value();
}

void MusicBackgroundPopWidget::sliderPressed()
{
    emit sliderStateChanged(true);
}

void MusicBackgroundPopWidget::sliderReleased()
{
    emit sliderStateChanged(false);
}

void MusicBackgroundPopWidget::initWidget()
{
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    m_containWidget->setFixedSize(60, 160);

    QBoxLayout *hbox = new QHBoxLayout(m_containWidget);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    QLabel *textLabel = new QLabel(m_containWidget);
    textLabel->setStyleSheet(MusicUIObject::MColorStyle02);
    textLabel->setText("100%\n\n\n\n\n50%\n\n\n\n\n0%");

    m_slider = new MusicClickedSlider(Qt::Vertical, m_containWidget);
    m_slider->setStyleSheet(MusicUIObject::MSliderStyle03);
    connect(m_slider, SIGNAL(valueChanged(int)), SIGNAL(valueChanged(int)));
    connect(m_slider, SIGNAL(sliderPressed()), SLOT(sliderPressed()));
    connect(m_slider, SIGNAL(sliderReleased()), SLOT(sliderReleased()));

    m_slider->setRange(0, 100);
    m_slider->setValue(100);
    m_slider->setCursor(QCursor(Qt::PointingHandCursor));

    hbox->addWidget(textLabel);
    hbox->addWidget(m_slider);

    m_containWidget->setLayout(hbox);

}
