#include "ladspaslider.h"

#include <QDoubleSpinBox>
#include <QSlider>
#include <QHBoxLayout>

LADSPASlider::LADSPASlider(double min, double max, double step, LADSPA_Data *value, QWidget *parent)
    : QWidget(parent),
      m_min(min),
      m_max(max),
      m_step(step),
      m_value(value)
{
    m_slider = new QSlider(Qt::Horizontal, this);
    m_spinBox = new QDoubleSpinBox(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_slider);
    layout->addWidget(m_spinBox);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_spinBox->setRange(min, max);
    m_spinBox->setSingleStep(step);
    m_spinBox->setValue(*value);

    m_slider->setRange(0, (max - min) / step);
    m_slider->setSingleStep(1);
    m_slider->setPageStep(10);
    m_slider->setValue((*value - min) / step);

    connect(m_spinBox, SIGNAL(valueChanged(double)), SLOT(setValue(double)));
    connect(m_slider, SIGNAL(valueChanged(int)), SLOT(setValue(int)));
}

void LADSPASlider::setValue(double v)
{
    *m_value = v;
    m_slider->blockSignals(true);
    m_slider->setValue((v-m_min)/m_step);
    m_slider->blockSignals(false);
}

void LADSPASlider::setValue(int v)
{
    m_spinBox->blockSignals(true);
    m_spinBox->setValue(v*m_step + m_min);
    *m_value = v*m_step + m_min;
    m_spinBox->blockSignals(false);
}
