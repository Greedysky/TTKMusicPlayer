#ifndef MUSICAPPLICATIONOBJECT_H
#define MUSICAPPLICATIONOBJECT_H

#include <QMimeData>
#include <QMessageBox>
#include <QFileDialog>
#include "musiclibexportglobal.h"

class MusicTimerAutoObject;
class MusicMobileDevicesWidget;
class QPropertyAnimation;

class MUSIC_WIDGET_EXPORT MusicApplicationObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplicationObject(QObject *parent = 0);
    ~MusicApplicationObject();

    bool getWindowToTop() const {return m_setWindowToTop;}
    void nativeEvent(const QByteArray &eventType,
                     void *message, long *result);

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
    QWidget *m_supperClass;
    MusicTimerAutoObject *m_musicTimerAutoObj;
    MusicMobileDevicesWidget *m_mobileDevices;

};

#endif // MUSICAPPLICATIONOBJECT_H
