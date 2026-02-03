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

void TTKClickedGroup::removeWidget(QWidget *widget)
{
    m_container.removeOne({-1, widget});
}

QList<QWidget*> TTKClickedGroup::items() const
{
    QWidgetList widgets;
    for(const Data &data : qAsConst(m_container))
    {
        widgets.append(data.m_widget);
    }
    return widgets;
}

QWidget *TTKClickedGroup::widget(int id) const
{
    for(const Data &data : qAsConst(m_container))
    {
        if(data.m_id == id)
        {
            return data.m_widget;
        }
    }
    return nullptr;
}

void TTKClickedGroup::setId(QWidget *widget, int id)
{
    for(Data &data : m_container)
    {
        if(data.m_widget == widget)
        {
            data.m_id = id;
            break;
        }
    }
}

int TTKClickedGroup::id(QWidget *widget) const
{
    for(const Data &data : qAsConst(m_container))
    {
        if(data.m_widget == widget)
        {
            return data.m_id;
        }
    }
    return -1;
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
