#ifndef MUSICAPPLICATIONOBJECT_H
#define MUSICAPPLICATIONOBJECT_H

#include <QObject>

class MusicApplication;
class QPropertyAnimation;

class MusicApplicationObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplicationObject(QObject *parent = 0);
    ~MusicApplicationObject();

signals:
public slots:

protected:
    MusicApplication *m_supperClass;
    QPropertyAnimation *m_animation;

};

#endif // MUSICAPPLICATIONOBJECT_H
