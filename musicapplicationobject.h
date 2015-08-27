#ifndef MUSICAPPLICATIONOBJECT_H
#define MUSICAPPLICATIONOBJECT_H

#include <QObject>

class MusicApplication;
class MusicTimerAutoObject;
class QPropertyAnimation;

class MusicApplicationObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplicationObject(QObject *parent = 0);
    ~MusicApplicationObject();

    bool getWindowToTop() const {return m_setWindowToTop;}

signals:
    void getCurrentPlayList(QStringList &list);

public slots:
    void musicAboutUs();
    void musicAudioRecorder();
    void musicTimerWidget();
    void musicSetWindowToTop();
    void musicToolSetsParameter();

protected:
    bool m_setWindowToTop;
    QPropertyAnimation *m_animation;
    MusicApplication *m_supperClass;
    MusicTimerAutoObject *m_musicTimerAutoObj;

};

#endif // MUSICAPPLICATIONOBJECT_H
