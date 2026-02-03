#include "ttkclickedgroup.h"

TTKClickedGroup::TTKClickedGroup(QObject *parent)
    : QObject(parent)
{

}

void TTKClickedGroup::addWidget(QWidget *widget, int id)
{
    m_container.append({id, widget});
    connect(widget, SIGNAL(clicked()), this, SLOT(update()));
}

void TTKClickedGroup::update()
{
    QWidget *widget = TTKObjectCast(QWidget*, sender());
    if(!widget)
    {
        return;
    }

    int index = -1;
    for(int i = 0; i < m_container.count(); ++i)
    {
        const Data &data = m_container[i];
        if(data.m_widget == widget)
        {
            index = data.m_id == -1 ? i : data.m_id;
            break;
        }
    }

    if(index == -1)
    {
        return;
    }

    Q_EMIT clicked(index);
}
