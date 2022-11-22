#include "ttktabbutton.h"

#include <QHBoxLayout>

#define TAB_BUTTON_ON  "QPushButton{ border-radius:0px; background-color:#158FE1; color:white; }"
#define TAB_BUTTON_OFF "QPushButton{ border-radius:0px; background-color:#BFBFBF; color:white; }"

TTKTabButton::TTKTabButton(QWidget *parent)
    : QWidget(parent),
      m_currentIndex(0)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    m_buttonGroup = new QButtonGroup(this);
    QtButtonGroupConnect(m_buttonGroup, this, buttonClicked);
}

TTKTabButton::~TTKTabButton()
{
    delete m_buttonGroup;
}

void TTKTabButton::addButtons(const QStringList &titles)
{
    int index = count();
    QHBoxLayout *ly = TTKObject_cast(QHBoxLayout*, layout());
    if(ly->count() > 1)
    {
        ly->removeItem(ly->itemAt(ly->count() - 1));
    }

    for(const QString &v : qAsConst(titles))
    {
        QPushButton *button = new QPushButton(v, this);
        button->setFixedSize(100, 30);
        button->setStyleSheet(TAB_BUTTON_OFF);
        button->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
        button->setFocusPolicy(Qt::NoFocus);
#endif
        ly->addWidget(button);
        m_buttonGroup->addButton(button, index++);
    }

    ly->addStretch(1);
    buttonClicked(0);
}

void TTKTabButton::setButtonEnabled(bool enable)
{
    for(QAbstractButton *button : m_buttonGroup->buttons())
    {
        button->setEnabled(enable);
    }
}

void TTKTabButton::buttonClicked(int index)
{
    for(int i = 0; i < count(); ++i)
    {
        m_buttonGroup->button(i)->setStyleSheet(i == index ? TAB_BUTTON_ON : TAB_BUTTON_OFF);
    }

    m_currentIndex = index;
    Q_EMIT clicked(index);
}
