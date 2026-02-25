#include "ttkeventfilter.h"

#include <QEvent>

TTKWheelEventFilter::TTKWheelEventFilter(QObject *parent)
    : QObject(parent)
{

}

bool TTKWheelEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    return (event->type() == QEvent::Wheel) ? true : QObject::eventFilter(watched, event);
}



TTKMouseEventFilter::TTKMouseEventFilter(QObject *parent)
    : QObject(parent)
{

}

bool TTKMouseEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    return (event->type() == QEvent::MouseButtonPress) ? true : QObject::eventFilter(watched, event);
}



TTKContextMenuEventFilter::TTKContextMenuEventFilter(QObject *parent)
    : QObject(parent)
{

}

bool TTKContextMenuEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    return (event->type() == QEvent::ContextMenu) ? true : QObject::eventFilter(watched, event);
}
