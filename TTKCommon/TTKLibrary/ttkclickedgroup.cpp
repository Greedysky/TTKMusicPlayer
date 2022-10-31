#include "ttkclickedgroup.h"

TTKClickedGroup::TTKClickedGroup(QObject *parent)
    : QObject(parent)
{

}

void TTKClickedGroup::mapped(QWidget *widget)
{
    m_container << widget;
    connect(widget, SIGNAL(clicked()), this, SLOT(update()));
}

void TTKClickedGroup::update()
{
    QWidget *widget = TTKObject_cast(QWidget*, sender());
    if(!widget)
    {
        return;
    }

    int index = -1;
    for(int i = 0; i < m_container.count(); ++i)
    {
        if(m_container[i] == widget)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        return;
    }

    Q_EMIT clicked(index);
}
