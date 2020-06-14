#include "musicclickedgroup.h"

MusicClickedGroup::MusicClickedGroup(QObject *parent)
    : QObject(parent)
{

}

void MusicClickedGroup::mapped(QWidget *widget)
{
    m_container << widget;
    connect(widget, SIGNAL(clicked()), this, SLOT(update()));
}

void MusicClickedGroup::update()
{
    QWidget *widget = TTKStatic_cast(QWidget*, sender());
    if(!widget)
    {
        return;
    }

    int index = -1;
    for(int i=0; i<m_container.count(); ++i)
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
