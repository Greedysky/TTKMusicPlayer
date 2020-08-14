#include "ladspabutton.h"

LADSPAButton::LADSPAButton(LADSPA_Data *value, QWidget *parent)
    : QCheckBox(parent),
      m_value(value)
{
    setChecked(*value == 1.0);
    connect(this, SIGNAL(clicked (bool)), SLOT(enable(bool)));
}

void LADSPAButton::enable(bool yes)
{
    *m_value = yes ? 1.0 : 0.0;
}
