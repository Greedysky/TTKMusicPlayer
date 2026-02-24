#include "ttkeventfilter.h"

#include <QEvent>

TTKWheelEventFilter::TTKWheelEventFilter(QObject *parent)
    : QObject(parent)
{

}

bool TTKWheelEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::Wheel)
    {
        return true;
    }
    return QObject::eventFilter(watched, event);
}
