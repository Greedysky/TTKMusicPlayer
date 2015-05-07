#ifndef MUSICVIDEOCONTROL_H
#define MUSICVIDEOCONTROL_H

#include <QMenu>
#include <QWidget>
#include "../core/musicobject.h"
#include "../core/musiclibexportglobal.h"

class QSlider;
class QToolButton;
class QPushButton;
class QWidgetAction;

class MUSIC_EXPORT MusicVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MusicVideoControl(QWidget *parent = 0);
    ~MusicVideoControl();

    void setValue(qint64 position);
    void durationChanged(qint64 duration);
    void setButtonStyle(bool);

signals:
    void addMedia(const QStringList&);
    void playNext();
    void playPrivious();

public slots:
    void openFile();

protected:
    QMenu m_popupMenu;
    QMenu m_popupVolumn;
    QWidgetAction *m_widgetAction;
    QSlider *m_timeSlider;
    QSlider *m_volumnSlider;
    QToolButton *m_menuButton;
    QPushButton *m_playButton;
    QPushButton *m_playNext;
    QPushButton *m_playPrivious;
    QToolButton *m_volumnButton;

};

#endif // MUSICVIDEOCONTROL_H
