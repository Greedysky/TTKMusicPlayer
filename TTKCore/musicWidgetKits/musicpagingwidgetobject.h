#ifndef MUSICPAGINGWIDGETOBJECT_H
#define MUSICPAGINGWIDGETOBJECT_H

#include <QObject>

class MusicPagingWidgetObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicPagingWidgetObject(QObject *parent = 0);

signals:

public slots:
};

#endif // MUSICPAGINGWIDGETOBJECT_H