#include "musicbackgroundpopwidget.h"
#include "musicwidgetheaders.h"
#include "ttkclickedslider.h"

MusicBackgroundPopWidget::MusicBackgroundPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
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
    Q_EMIT sliderStateChanged(true);
}

void MusicBackgroundPopWidget::sliderReleased()
{
    Q_EMIT sliderStateChanged(false);
}

void MusicBackgroundPopWidget::initialize()
{
    m_menu->setStyleSheet(TTK::UI::MenuStyle02);
    m_containWidget->setFixedSize(60, 160);

    QBoxLayout *hbox = new QHBoxLayout(m_containWidget);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    QLabel *textLabel = new QLabel(m_containWidget);
    textLabel->setStyleSheet(TTK::UI::ColorStyle10);
    textLabel->setText("100%\n\n\n\n\n50%\n\n\n\n\n0%");

    m_slider = new TTKClickedSlider(Qt::Vertical, m_containWidget);
    m_slider->setStyleSheet(TTK::UI::SliderStyle03);
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
