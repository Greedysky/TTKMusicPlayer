#ifndef MUSICREMOTEWIDGET_H
#define MUSICREMOTEWIDGET_H

#include <QtWidgets>
#include "musicobject.h"
#include "musicmovewidgetabstract.h"

class MUSIC_EXPORT MusicRemoteWidget : public MusicMoveWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicRemoteWidget(QWidget *parent = 0);
    virtual ~MusicRemoteWidget();

    void showPlayStatus(bool);
    void setVolumeValue(int);
    virtual void setLabelText(const QString&){}

signals:
    void musicWindowSignal();
    void musicKeySignal();
    void musicPlayPriviousSignal();
    void musicPlayNextSignal();
    void musicVolumeSignal(int);
    void musicSettingSignal();

public slots:
    void musicVolumeChanged(int);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void paintEvent(QPaintEvent* event);

    QPushButton* m_showMainWindow;
    QPushButton* m_PreSongButton;
    QPushButton* m_NextSongButton;
    QPushButton* m_PlayButton;
    QPushButton* m_SettingButton;
    QWidget* m_mainWidget;
    QWidget* m_volumnWidget;
    QLabel* m_volumnLabel;
    QSlider* m_volumnSlider;

};

#endif // MUSICREMOTEWIDGET_H
